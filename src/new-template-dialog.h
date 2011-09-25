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

#ifndef NEW_TEMPLATE_DIALOG_H
#define NEW_TEMPLATE_DIALOG_H

#include <memory>
#include <gtkmm.h>

namespace Templatizer
{
class NewTemplateDialog
{
 public:
    NewTemplateDialog();
    ~NewTemplateDialog();

    void set_parent_window(Gtk::Window &parent);

    int run();

    Glib::ustring get_template_name() {return entry->get_text();};
    
 private:
    Gtk::Dialog* dialog;
    Gtk::Entry *entry;
    Gtk::Button *accept_button;

    //callbacks
    void on_entry_changed();

    //helpers
    void update_buttons();
};
}

#endif
