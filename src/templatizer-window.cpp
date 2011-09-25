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
#include <glibmm.h>
#include "main.h"
#include "templatizer-window.h"
#include "new-template-dialog.h"
#include "delete-template-confirmation-dialog.h"
#include "templatizer-template.h"
#include "template-list.h"

//placeholders for future gettextizing
#define _(X) (X)
#define N_(X) (X)

using namespace Templatizer;

TemplatizerWindow* TemplatizerWindow::create()
{
  TemplatizerWindow *window;
  Glib::RefPtr<Gtk::Builder> xml = 
    Gtk::Builder::create_from_file(Main::get_glade_path() + "/templatizer.gtk");

  xml->get_widget_derived("window", window);
  window->on_window_loaded();
  window->signal_delete_event().connect(
	sigc::mem_fun(*window, &TemplatizerWindow::on_delete_event));
  return window;
}

TemplatizerWindow::TemplatizerWindow(BaseObjectType* baseObject, 
                             const Glib::RefPtr<Gtk::Builder>& xml)
 : Gtk::Window(baseObject), name_column(N_("Templates"), name_renderer)
{
  nag_on_delete = true;
  signal_size_allocate().connect(sigc::mem_fun(*this, &TemplatizerWindow::on_window_allocated));

  xml->get_widget("contents", contents);
  xml->get_widget("template_contents", template_contents);
  xml->get_widget("scrolledwindow", scrolled_window);
  set_icon_from_file(Main::get_data_path() + "/templatizer.png");
  xml->get_widget("treeview", treeview);
  name_column.set_cell_data_func
    (name_renderer, sigc::mem_fun(*this, &TemplatizerWindow::cell_data_name));
  treeview->append_column(name_column);
  templates_list = Gtk::ListStore::create(templates_columns);
  sorted_templates_list = Gtk::TreeModelSort::create(templates_list);
  sorted_templates_list->set_sort_column(templates_columns.name, Gtk::SORT_ASCENDING);

  treeview->set_model(sorted_templates_list);
  treeview->get_selection()->set_mode(Gtk::SELECTION_SINGLE);
  treeview->set_headers_clickable(true);
  treeview->set_headers_visible(true);
  treeview->get_selection()->signal_changed().connect
    (sigc::mem_fun(*this, &TemplatizerWindow::on_template_selected));
  treeview->get_column(0)->set_sort_column(templates_columns.name);
  treeview->signal_button_press_event().connect_notify
          (sigc::mem_fun(*this, &TemplatizerWindow::on_treeview_clicked));


  xml->get_widget("new_menuitem", new_menuitem);
  new_menuitem->signal_activate().connect(sigc::mem_fun(*this,&TemplatizerWindow::on_new_clicked));
  xml->get_widget("quit_menuitem", quit_menuitem);
  quit_menuitem->signal_activate().connect(sigc::mem_fun(*this,&TemplatizerWindow::on_quit_clicked));
  xml->get_widget("undo_menuitem", undo_menuitem);
  undo_menuitem->signal_activate().connect(sigc::mem_fun(*this,&TemplatizerWindow::on_undo_clicked));
  xml->get_widget("cut_menuitem", cut_menuitem);
  cut_menuitem->signal_activate().connect(sigc::mem_fun(*this,&TemplatizerWindow::on_cut_clicked));
  xml->get_widget("copy_menuitem", copy_menuitem);
  copy_menuitem->signal_activate().connect(sigc::mem_fun(*this,&TemplatizerWindow::on_copy_clicked));
  xml->get_widget("paste_menuitem", paste_menuitem);
  paste_menuitem->signal_activate().connect(sigc::mem_fun(*this,&TemplatizerWindow::on_paste_clicked));
  xml->get_widget("select_all_menuitem", select_all_menuitem);
  select_all_menuitem->signal_activate().connect(sigc::mem_fun(*this,&TemplatizerWindow::on_select_all_clicked));
  xml->get_widget("deselect_menuitem", deselect_menuitem);
  deselect_menuitem->signal_activate().connect(sigc::mem_fun(*this,&TemplatizerWindow::on_deselect_clicked));
  xml->get_widget("delete_menuitem", delete_menuitem);
  delete_menuitem->signal_activate().connect(sigc::mem_fun(*this,&TemplatizerWindow::on_delete_clicked));
  xml->get_widget("about_menuitem", about_menuitem);
  about_menuitem->signal_activate().connect(sigc::mem_fun(*this,&TemplatizerWindow::on_about_clicked));


  xml->get_widget("template_entry", template_entry);
  template_entry->signal_changed().connect(sigc::mem_fun(*this, &TemplatizerWindow::on_template_entry_changed));
  template_entry->signal_focus_out_event().connect(sigc::mem_fun(*this, &TemplatizerWindow::on_leaving_template_entry));
  xml->get_widget("search_container", search_container);
  xml->get_widget("search_entry", search_entry);
  search_entry->set_text("");
  search_entry->signal_changed().connect(sigc::mem_fun(*this, &TemplatizerWindow::on_search_entry_changed));
  search_entry->signal_focus_in_event().connect(sigc::mem_fun(*this, &TemplatizerWindow::on_entering_search_entry));
  search_entry->signal_icon_press().connect(sigc::mem_fun(*this, &TemplatizerWindow::on_search_icon_clicked));
  xml->get_widget("use_button", use_button);
  use_button->signal_clicked().connect(sigc::mem_fun(*this, &TemplatizerWindow::on_use_clicked));
  xml->get_widget("eventbox", eventbox);
  eventbox->override_background_color(Gdk::RGBA("dim gray"));

  search_container->show_all();
  text_buffer = Gsv::Buffer::create();
  text_buffer->signal_changed().connect(sigc::mem_fun(*this, &TemplatizerWindow::on_textview_changed));
  textview = new Gsv::View(text_buffer);
  textview->signal_focus_out_event().connect(sigc::mem_fun(*this, &TemplatizerWindow::on_leaving_textview));
  scrolled_window->add(*manage(textview));
  scrolled_window->show_all();
  textview->set_wrap_mode(Gtk::WRAP_WORD);
    
  context_actions = Gtk::ActionGroup::create();
  context_actions->add(Gtk::Action::create("ContextNew", 
                                           N_("New Template ...")),
                       sigc::mem_fun(*this, 
                                     &TemplatizerWindow::on_new_clicked));
  context_actions->add(Gtk::Action::create("ContextDelete", N_("Delete")),
                       sigc::mem_fun(*this, 
                                     &TemplatizerWindow::on_delete_clicked));
  context_actions->add(Gtk::Action::create("ContextUse", 
                                           N_("Use in New Email ...")),
                       sigc::mem_fun(*this, 
                                     &TemplatizerWindow::on_use_clicked));
    ui_manager= Gtk::UIManager::create();
    ui_manager->insert_action_group(context_actions);

    ui_manager->add_ui_from_string("<ui>"
                                   "  <popup name='PopupMenu'>"
                                   "    <menuitem action='ContextNew'/>"
                                   "    <menuitem action='ContextDelete'/>"
                                   "    <menuitem action='ContextUse'/>"
                                   "  </popup>"
                                   "</ui>");
    treeview_context_menu = dynamic_cast<Gtk::Menu*>(ui_manager->get_widget("/PopupMenu")); 
}

void TemplatizerWindow::on_treeview_clicked(GdkEventButton* event)
{
  // this is how we display the context menu on the treeview when
  // we right-click on it.
  if (event->type == GDK_BUTTON_PRESS && event->button == 3)
    treeview_context_menu->popup(event->button, event->time);
}


void TemplatizerWindow::on_window_loaded()
{
  load_saved_templates();
}

TemplatizerWindow::~TemplatizerWindow()
{
  delete templates;
}

bool TemplatizerWindow::quit()
{
  hide();
  return true;
}

bool TemplatizerWindow::on_delete_event(GdkEventAny *event)
{
  return quit();
}

void TemplatizerWindow::on_exit_clicked()
{
  quit();
}

void TemplatizerWindow::on_template_selected()
{
  TemplatizerTemplate *t = get_selected_template();
  if (t)
    {
      template_entry->set_text(t->get_name());
      text_buffer->set_text(t->get_text());
      update_buttons();
    }
}

void TemplatizerWindow::on_template_activated(const Gtk::TreeModel::Path& path,
                                              Gtk::TreeViewColumn *col)
{
}

void TemplatizerWindow::on_about_clicked()
{
  Gtk::AboutDialog* dialog;

  Glib::RefPtr<Gtk::Builder> xml
    = Gtk::Builder::create_from_file(Main::get_glade_path() + "/about-dialog.gtk");

  xml->get_widget("dialog", dialog);
  dialog->set_icon_from_file(Main::get_data_path() + "/templatizer.png");

  std::string name = PACKAGE_NAME;
  std::transform(name.begin(), ++name.begin(), name.begin(), toupper);
  dialog->set_program_name(name);
  dialog->set_version(PACKAGE_VERSION);
  dialog->set_logo(Gdk::Pixbuf::create_from_file(Main::get_data_path() + "/templatizer.png"));
  dialog->show_all();
  dialog->set_transient_for(*this);
  dialog->run();
  delete dialog;
  return;
}

void TemplatizerWindow::on_new_clicked()
{
  NewTemplateDialog d;
  d.set_parent_window(*this);
  int response = d.run();
  if (response == Gtk::RESPONSE_ACCEPT)
    {
      // stop the active search.
      on_search_icon_clicked(Gtk::ENTRY_ICON_SECONDARY, NULL);

      // make a new template.
      Glib::ustring name = d.get_template_name();
      TemplatizerTemplate *t = new TemplatizerTemplate(templates->get_free_id(), name, "");
      // add it to our list, which saves it to the database.
      templates->push_back(t);

      // display and re-sort our list of displayed template names.
      add_template_row(t);
      sorted_templates_list = Gtk::TreeModelSort::create(templates_list);
      sorted_templates_list->set_sort_column(templates_columns.name, Gtk::SORT_ASCENDING);
      treeview->set_model(sorted_templates_list);
      update_buttons();
      //here we go about selecting the new template
      for (Gtk::TreeIter i = sorted_templates_list->children().begin(),
           end = sorted_templates_list->children().end(); i != end; ++i) 
        if ((*i)[templates_columns.template_data] == t)
          {
            treeview->get_selection()->select(i);
            break;
          }
    }
}

void TemplatizerWindow::on_quit_clicked()
{
  quit();
}

void TemplatizerWindow::on_undo_clicked()
{
  if (template_entry->property_is_focus())
    {
      ;  // FIXME: wouldn't it be nice if the entry had undo support.
    }
  if (textview->property_is_focus())
    text_buffer->get_undo_manager()->undo();
}

void TemplatizerWindow::on_cut_clicked()
{
  if (template_entry->property_is_focus())
    template_entry->cut_clipboard();
  if (textview->property_is_focus())
    text_buffer->cut_clipboard(textview->get_clipboard("CLIPBOARD"));
}

void TemplatizerWindow::on_copy_clicked()
{
  if (template_entry->property_is_focus())
    template_entry->copy_clipboard();
  if (textview->property_is_focus())
    text_buffer->copy_clipboard(textview->get_clipboard("CLIPBOARD"));
}

void TemplatizerWindow::on_paste_clicked()
{
  if (template_entry->property_is_focus())
    template_entry->paste_clipboard();
  if (textview->property_is_focus())
    text_buffer->paste_clipboard(textview->get_clipboard("CLIPBOARD"));
}

void TemplatizerWindow::on_select_all_clicked()
{
  if (template_entry->property_is_focus())
    template_entry->select_region(0, template_entry->get_text().length());
  if (textview->property_is_focus())
    text_buffer->select_range(text_buffer->begin(), text_buffer->end());
}
void TemplatizerWindow::on_deselect_clicked()
{
  if (template_entry->property_is_focus())
    {
      int len = template_entry->get_text().length();
      template_entry->select_region(len, len);
    }
  if (textview->property_is_focus())
    text_buffer->select_range(text_buffer->end(), text_buffer->end());
}

int TemplatizerWindow::nag_about_delete_template()
{
  DeleteTemplateConfirmationDialog d;
  d.set_parent_window(*this);
  int response = d.run();
  if (d.get_nag_state() == true)
    nag_on_delete = false;
  d.hide();
  return response;
}

void TemplatizerWindow::on_delete_clicked()
{
  int response = Gtk::RESPONSE_ACCEPT;
  if (nag_on_delete == true)
    response = nag_about_delete_template();
  if (response == Gtk::RESPONSE_ACCEPT)
    {
      Glib::RefPtr<Gtk::TreeSelection> selection = treeview->get_selection();
      Gtk::TreeModel::iterator iterrow = selection->get_selected();
      if (iterrow) 
        {
          Gtk::TreeModel::Row row = *iterrow;
          TemplatizerTemplate *t = row[templates_columns.template_data];
          template_entry->set_text("");
          text_buffer->set_text("");
          //go find the one i want to erase
          //the iterator points to an element in the sorted list
          //instead of our actual list
          for (Gtk::TreeIter i = templates_list->children().begin(),
               end = templates_list->children().end(); i != end; ++i) 
            if ((*i)[templates_columns.template_data] == t)
              {
                templates_list->erase(i);
                break;
              }

          templates->remove_template(t);
          sorted_templates_list = Gtk::TreeModelSort::create(templates_list);
          sorted_templates_list->set_sort_column(templates_columns.name, Gtk::SORT_ASCENDING);
          treeview->set_model(sorted_templates_list);
          update_buttons();
        }
    }
}

void TemplatizerWindow::on_textview_changed()
{
  // when the template text changes, we update our in-memory template object.
  // we save the template to the database later when the focus leaves the 
  // template textview.
  TemplatizerTemplate *t = get_selected_template();
  t->set_text(text_buffer->get_text());
}

void TemplatizerWindow::on_template_entry_changed()
{
  // when the template name changes, we also change the name in our displayed 
  // list of template names.
  // we save the template to the database later when the focus leaves the 
  // template entry.
  Glib::RefPtr<Gtk::TreeSelection> selection = treeview->get_selection();
  Gtk::TreeModel::iterator iterrow = selection->get_selected();

  if (iterrow) 
    {
      Gtk::TreeModel::Row row = *iterrow;
      TemplatizerTemplate *t = row[templates_columns.template_data];
      t->set_name(template_entry->get_text());
      row[templates_columns.name] = template_entry->get_text();
    }
}

void TemplatizerWindow::on_use_clicked()
{
  Glib::ustring uri = "mailto:?subject=" +
    Glib::uri_escape_string(template_entry->get_text()) +"&body=" + 
    Glib::uri_escape_string(text_buffer->get_text());
  GError *error = NULL;
  gtk_show_uri(gdk_screen_get_default(), uri.data(),
               gtk_get_current_event_time(), &error); 
  if (error)
    g_free(error);
}

void TemplatizerWindow::on_search_entry_changed()
{
  if (search_entry->get_text() == "")
    {
      fill_templates();
      update_buttons();
      return;
    }
  templates_list->clear();
  Glib::signal_idle().connect_once(sigc::bind(sigc::mem_fun(*this, &TemplatizerWindow::search_templates_in_the_background), templates, search_entry->get_text()));

}

void TemplatizerWindow::search_templates_in_the_background(TemplateList *t, Glib::ustring match)
{
  for (TemplateList::iterator i = t->get_templates().begin(); i != t->get_templates().end();i++)
    {
      if ((*i)->contains(match))
        add_template_row(*i);
    }
  sorted_templates_list = Gtk::TreeModelSort::create(templates_list);
  sorted_templates_list->set_sort_column(templates_columns.name, Gtk::SORT_ASCENDING);
  treeview->set_model(sorted_templates_list);
  update_buttons();
}

void TemplatizerWindow::on_search_icon_clicked(Gtk::EntryIconPosition pos, const GdkEventButton *button)
{
  TemplatizerTemplate *t = get_selected_template();
  search_entry->set_text("");
  update_buttons();
  if (t)
    {
      // here we are re-selecting the same template that was already
      // selected before we cleared the search.
      // we get the full list of templates again, but with our previous 
      // selected template reselected.
      for (Gtk::TreeIter i = sorted_templates_list->children().begin(),
           end = sorted_templates_list->children().end(); i != end; ++i) 
        if ((*i)[templates_columns.template_data] == t)
          {
            treeview->get_selection()->select(i);
            break;
          }
    }
}

TemplatizerTemplate* TemplatizerWindow::get_selected_template()
{
  Glib::RefPtr<Gtk::TreeSelection> selection = treeview->get_selection();
  Gtk::TreeModel::iterator iterrow = selection->get_selected();
  if (iterrow) 
    {
      Gtk::TreeModel::Row row = *iterrow;
      return row[templates_columns.template_data];
    }
  return NULL;
}

void TemplatizerWindow::update_buttons()
{
  TemplatizerTemplate *t = get_selected_template();
  if (search_entry->get_text() != "" && t == NULL)
    return;

  template_contents->set_sensitive(t != NULL);
  template_entry->set_visible(t != NULL); 
  textview->set_visible(t != NULL); 
  //the set-visible line avoids a bug where the text in the textview appears 
  //grey instead of black.
  treeview->set_sensitive(templates_list->children().size() > 0);

  undo_menuitem->set_sensitive(t != NULL);
  cut_menuitem->set_sensitive(t != NULL);
  copy_menuitem->set_sensitive(t != NULL);
  paste_menuitem->set_sensitive(t != NULL);
  select_all_menuitem->set_sensitive(t != NULL);
  deselect_menuitem->set_sensitive(t != NULL);
  delete_menuitem->set_sensitive(t != NULL);
      
  draw_search_indicator();
  search_entry->set_sensitive(templates_list->children().size() > 0);
}

void TemplatizerWindow::draw_search_indicator()
{
  // this is how we put the Search text on the search entry when we are not
  // searching.
  if (search_entry->get_text() != "" || search_entry->has_focus())
    return;
  Pango::AttrList attrs;
  Pango::Attribute weight =  
    Pango::Attribute::create_attr_weight(Pango::WEIGHT_BOLD);
  Pango::Attribute fg =  Pango::Attribute::create_attr_foreground(0.4*65535.0, 
                                                                  0.4*65535.0, 
                                                                  0.4*65535.0);
  Pango::Attribute italic =  
    Pango::Attribute::create_attr_style(Pango::STYLE_ITALIC);
  attrs.insert(weight);
  attrs.insert(fg);
  attrs.insert(italic);
  Glib::RefPtr<Pango::Layout> layout = search_entry->get_layout();
  layout->set_attributes(attrs);
  layout->set_text(N_("Search"));
}

void TemplatizerWindow::fill_templates()
{
  templates_list->clear();
  for (TemplateList::iterator i = templates->get_templates().begin(); 
       i != templates->get_templates().end(); i++)
    add_template_row(*i);
}

void TemplatizerWindow::load_saved_templates()
{
  templates = TemplateList::load_from_database();
  fill_templates();
}

void TemplatizerWindow::add_template_row(TemplatizerTemplate *t)
{
  // put a template name into our displayed list of templates 
  Gtk::TreeIter i = templates_list->append();
  (*i)[templates_columns.name] = t->get_name();
  (*i)[templates_columns.template_data] = t;
}

bool TemplatizerWindow::on_leaving_template_entry(GdkEventFocus *event)
{
  // when we're done editing the template name, we flush to disk. 
  TemplatizerTemplate *t = get_selected_template();
  if (t && templates)
    templates->save(t);
  return true;
}

bool TemplatizerWindow::on_leaving_textview(GdkEventFocus *event)
{
  // when we're done editing the template text, we flush to disk. 
  TemplatizerTemplate *t = get_selected_template();
  if (t && templates)
    templates->save(t);
  return true;
}
  
bool TemplatizerWindow::on_entering_search_entry(GdkEventFocus *event)
{
  // when we click on the search entry, we clear the active template.
  treeview->get_selection()->unselect_all();
  update_buttons();
  return true;
}
  
void TemplatizerWindow::on_window_allocated(Gtk::Allocation & allocation)
{
  // this is a hack to make the "Search" text appear on the search entry.
  update_buttons();
}

void TemplatizerWindow::cell_data_name(Gtk::CellRenderer *renderer, const Gtk::TreeIter &i)
{
  // we specially render the textview column, so that it is ellipsized,
  // according to the controlling pane.
  Gtk::CellRendererText*cell = dynamic_cast<Gtk::CellRendererText*>(renderer);
  cell->property_ellipsize() = Pango::ELLIPSIZE_END;
  cell->property_text() =  (*i)[templates_columns.name];
}
