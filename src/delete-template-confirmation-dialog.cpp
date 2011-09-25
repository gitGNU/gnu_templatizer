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

#include "delete-template-confirmation-dialog.h"
#include "main.h"

using namespace Templatizer;

DeleteTemplateConfirmationDialog::DeleteTemplateConfirmationDialog()
{
  Glib::RefPtr<Gtk::Builder> xml
    = Gtk::Builder::create_from_file(Main::get_glade_path()
                                     + "/delete-confirmation-dialog.gtk");

  xml->get_widget("dialog", dialog);
  xml->get_widget("nag_checkbutton", nag_checkbutton);
}

DeleteTemplateConfirmationDialog::~DeleteTemplateConfirmationDialog()
{
  delete dialog;
}

void DeleteTemplateConfirmationDialog::set_parent_window(Gtk::Window &parent)
{
  dialog->set_transient_for(parent);
}

void DeleteTemplateConfirmationDialog::hide()
{
  dialog->hide();
}

int DeleteTemplateConfirmationDialog::run()
{
  dialog->show_all();
  return dialog->run();
}

