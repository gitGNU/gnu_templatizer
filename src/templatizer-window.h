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

#ifndef TEMPLATIZER_WINDOW_H
#define TEMPLATIZER_WINDOW_H

#include <sigc++/sigc++.h>
#include <memory>
#include <gtkmm.h>
#include <gtksourceviewmm.h>

namespace Templatizer
{
class TemplatizerTemplate;
class TemplateList;


class TemplatizerWindow: public Gtk::Window
{
 public:
    static TemplatizerWindow* create();

    TemplatizerWindow(BaseObjectType* baseObject, 
               const Glib::RefPtr<Gtk::Builder>& xml);

    virtual ~TemplatizerWindow();
 private:

    //for the treeview
    Gtk::TreeView *treeview;
    class TemplatesColumns: public Gtk::TreeModelColumnRecord {
  public:
      TemplatesColumns()
	{add(name);add(template_data);}
      Gtk::TreeModelColumn<Glib::ustring> name;
      Gtk::TreeModelColumn<TemplatizerTemplate*> template_data;
    };
    const TemplatesColumns templates_columns;
    Glib::RefPtr<Gtk::ListStore> templates_list;
    Glib::RefPtr<Gtk::TreeModelSort> sorted_templates_list;
    Gtk::CellRendererText name_renderer;
    Gtk::TreeViewColumn name_column;
    void cell_data_name(Gtk::CellRenderer *renderer, const Gtk::TreeIter &i);
    
    //widgets
    Gtk::Box *contents;
    Gtk::MenuItem *new_menuitem;
    Gtk::ImageMenuItem *quit_menuitem;
    Gtk::ImageMenuItem *undo_menuitem;
    Gtk::ImageMenuItem *cut_menuitem;
    Gtk::ImageMenuItem *copy_menuitem;
    Gtk::ImageMenuItem *paste_menuitem;
    Gtk::ImageMenuItem *select_all_menuitem;
    Gtk::MenuItem *deselect_menuitem;
    Gtk::ImageMenuItem *delete_menuitem;
    Gtk::MenuItem *about_menuitem;
    Gtk::Entry *template_entry;
    Gtk::Box *search_container;
    Gtk::Button *use_button;
    Gtk::Entry *search_entry;
    Gtk::Box *template_contents;
    Gtk::ScrolledWindow *scrolled_window;
    Gsv::View *textview;
    Glib::RefPtr<Gsv::Buffer> text_buffer;
    Gtk::EventBox *eventbox;
    Gtk::Menu *treeview_context_menu;
    Glib::RefPtr<Gtk::UIManager> ui_manager;
    Glib::RefPtr<Gtk::ActionGroup> context_actions;

    //callbacks
    void on_window_loaded();
    void on_exit_clicked();
    void on_template_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn *col);
    void on_template_selected();
    bool on_delete_event(GdkEventAny *event);
    void on_about_clicked();
    void on_new_clicked();
    void on_quit_clicked();
    void on_undo_clicked();
    void on_cut_clicked();
    void on_copy_clicked();
    void on_paste_clicked();
    void on_select_all_clicked();
    void on_deselect_clicked();
    void on_delete_clicked();

    void on_search_entry_changed();
    void on_textview_changed();
    void on_template_entry_changed();
    void on_use_clicked();
    void on_search_icon_clicked(Gtk::EntryIconPosition pos, const GdkEventButton *button);
    void on_treeview_clicked(GdkEventButton* event);
    bool on_leaving_template_entry(GdkEventFocus *event);
    bool on_leaving_textview(GdkEventFocus *event);
    bool on_entering_search_entry(GdkEventFocus *event);
    void on_window_allocated(Gtk::Allocation & allocation);


    //helpers
    bool quit();
    void update_buttons();
    TemplatizerTemplate* get_selected_template();
    void load_saved_templates();
    void add_template_row(TemplatizerTemplate *t);
    void fill_templates();
    void draw_search_indicator();
    int nag_about_delete_template();
    void search_templates_in_the_background(TemplateList *t, Glib::ustring match);

    //DATA
    TemplateList *templates;
    bool nag_on_delete; // e.g. do you want to delete this template?
};
}

#endif // TEMPLATIZER_WINDOW_H
