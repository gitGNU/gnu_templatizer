//  Copyright (C) 2011 Ben Asselstine
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

#include <gtkmm.h>
#include <sigc++/functors/mem_fun.h>

#include "new-template-dialog.h"
#include "main.h"

using namespace Templatizer;

NewTemplateDialog::NewTemplateDialog()
{
    Glib::RefPtr<Gtk::Builder> xml
	= Gtk::Builder::create_from_file(Main::get_glade_path()
				    + "/new-template-dialog.gtk");

    xml->get_widget("dialog", dialog);
    xml->get_widget("accept_button", accept_button);
    xml->get_widget("entry", entry);
    entry->signal_changed().connect
      (sigc::mem_fun(this, &NewTemplateDialog::on_entry_changed));
    
    update_buttons();
}


void NewTemplateDialog::on_entry_changed()
{
  update_buttons();
}

NewTemplateDialog::~NewTemplateDialog()
{
  delete dialog;
}
void NewTemplateDialog::set_parent_window(Gtk::Window &parent)
{
  dialog->set_transient_for(parent);
}

int NewTemplateDialog::run()
{
  dialog->show_all();
  return dialog->run();
}

void NewTemplateDialog::update_buttons()
{
  if (entry->get_text() == "")
    accept_button->set_sensitive(false);
  else
    accept_button->set_sensitive(true);
}
