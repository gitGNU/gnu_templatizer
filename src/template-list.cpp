// Copyright (C) 2011 Ben Asselstine
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Library General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 
//  02110-1301, USA.

#include <config.h>
#include "template-list.h"
#include "templatizer-template.h"

using namespace Templatizer;

TemplateList::TemplateList()
{
  create_empty_database();
}

TemplateList::TemplateList(std::string file)
{
  if (sqlite3_open(file.c_str(), &db) != SQLITE_OK)
    {
      db = NULL;
      return;
    }
  const char *sql ="select * from templates;";
  const char *fail = NULL;
  sqlite3_stmt *res = NULL;
  if (sqlite3_prepare(db, sql, -1, &res, &fail) == SQLITE_OK)
    {
      if (res)
        {
          while (sqlite3_step(res) != SQLITE_DONE)
            {
              std::string name = (const char*) sqlite3_column_text(res, 1);
              std::string text = (const char*) sqlite3_column_text(res, 2);
              int id = sqlite3_column_int(res, 0);
              _templates.push_back(new TemplatizerTemplate (id, name, text));
            }
        }
      sqlite3_finalize(res);
    }
}

TemplateList::TemplateList(const TemplateList& g)
{
  for (const_iterator it = g.get_templates().begin(); 
       it != g.get_templates().end(); it++)
    _templates.push_back(new TemplatizerTemplate(*(*it)));
}

void TemplateList::add(const TemplateList *g)
{
  if (g)
    {
      for (const_iterator it = g->get_templates().begin();
           it != g->get_templates().end(); it++)
        _templates.push_back(new TemplatizerTemplate(*(*it)));
    }
}

TemplateList::~TemplateList()
{
  //remove_all_templates();
  sqlite3_close(db);
}

void TemplateList::remove_all_templates()
{
  for (iterator it = _templates.begin(); it != _templates.end(); it++)
    delete (*it);
  _templates.clear();
}

int TemplateList::get_free_id() const
{
  std::list<int> ids;
  for (const_iterator it = _templates.begin(); it != _templates.end(); it++)
    ids.push_back((*it)->get_id());
  ids.sort();
  int count = 0;
  for (std::list<int>::iterator i = ids.begin(); i != ids.end(); i++)
    {
      if (count != (*i))
        return count;
      count++;
    }
  return ids.size();
}
        
TemplateList* TemplateList::load_from_database()
{
  std::string file;
  file += Glib::get_user_data_dir() + "/" + PACKAGE;
  //dir exists?
  if (Glib::file_test(file, Glib::FILE_TEST_EXISTS | Glib::FILE_TEST_IS_DIR))
    {
      file += "/templates-db";
  
      if (Glib::file_test(file, Glib::FILE_TEST_EXISTS | Glib::FILE_TEST_IS_REGULAR))
        return new TemplateList(file);
      else
        return new TemplateList();
    }
  else
    return new TemplateList();
}

bool TemplateList::create_empty_database()
{
  std::string file = Glib::get_user_data_dir() + "/" + PACKAGE;
  if (!Glib::file_test(file, Glib::FILE_TEST_EXISTS | Glib::FILE_TEST_IS_DIR))
    Gio::File::create_for_path(file)->make_directory();
  file += "/templates-db";
  if (sqlite3_open_v2 (file.c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL) != SQLITE_OK)
    return false;
  //now we send it the commands to make a new database.
  gchar   *err = NULL;
  std::string cmd = 
    "create table templates"
    "(id int, "
    "name varchar(1024), "
    "txt varchar(32768));";
  if (sqlite3_exec (db, cmd.c_str(), NULL, NULL, &err) != SQLITE_OK)
    return false;
  sqlite3_free (err);
  return true;
}
        
bool TemplateList::remove_template(TemplatizerTemplate *t)
{
  gchar   *err = NULL;
  _templates.remove(t);
  std::string cmd = 
    "delete from templates where id = " + t->get_id_as_str() + ";";
  if ( sqlite3_exec (db, cmd.c_str(), NULL, NULL, &err) != SQLITE_OK)
    return false;
  sqlite3_free (err);
  return true;
}

bool TemplateList::push_back(TemplatizerTemplate *t)
{
  _templates.push_back(t);
  char *sql = NULL;
  if (asprintf(&sql, "insert into templates (id, name, txt) values (?1, ?2, ?3)") != -1)
    {
      const char *fail = NULL;
      sqlite3_stmt *res = NULL;
      if (sqlite3_prepare(db, sql, strlen (sql) + 1, &res, &fail) == SQLITE_OK)
        {
          std::string n = t->get_name();
          std::string txt = t->get_text();
          if (sqlite3_bind_int(res, 1, t->get_id()) == SQLITE_OK &&
              sqlite3_bind_text(res, 2, 
                                g_strdup(n.c_str()), -1, g_free) == SQLITE_OK &&
              sqlite3_bind_text(res, 3, 
                                g_strdup(txt.c_str()), -1, g_free) == SQLITE_OK)
            {
              if (sqlite3_step(res) == SQLITE_DONE)
                return true;
            }
          else
            return false;
        }
      else
        return false;
    }
  else
    return false;
  return false;
}

bool TemplateList::save(TemplatizerTemplate *t)
{
  char *sql = NULL;
  if (asprintf(&sql, 
               "update templates set name=?1, txt=?2 where id = ?3;)") != -1)
    {
      const char *fail = NULL;
      sqlite3_stmt *res = NULL;
      if (sqlite3_prepare(db, sql, strlen (sql) + 1, &res, &fail) == SQLITE_OK)
        {
          std::string n = t->get_name();
          std::string txt = t->get_text();
          if (sqlite3_bind_text(res, 1, g_strdup(n.c_str()), -1, 
                                g_free) == SQLITE_OK &&
              sqlite3_bind_text(res, 2, g_strdup(txt.c_str()), -1, 
                                g_free) == SQLITE_OK &&
              sqlite3_bind_int(res, 3, t->get_id()) == SQLITE_OK)
            {
              if (sqlite3_step(res) == SQLITE_DONE)
                return true;
            }
          else
            return false;

        }
      else
        return false;
    }
  else
    return false;
  return false;
}
