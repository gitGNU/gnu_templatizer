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
#include <assert.h>
#include <memory>
#include <iostream>
#include <assert.h>
#include <glib.h>
#include <gtkmm/main.h>
#include <gtksourceviewmm/init.h>

#include "main.h"
#include "templatizer-window.h"

using namespace Templatizer;
struct Main::Impl
{
    std::auto_ptr<Gtk::Main> gtk_main;
};


static Main *singleton;

Main::Main(int &argc, char **&argv)
    : impl(new Impl)
{
    version_appears_on_the_command_line = false;
    singleton = this;

    try
      {
        Glib::OptionContext context("");
        Glib::OptionEntry entry;
        Glib::OptionGroup options (PACKAGE_NAME, "Templatizer Options", "Command-line options for Templatizer");
        entry.set_long_name ("version");
        entry.set_short_name ('V');
        entry.set_description ("Show version and exit.");
        options.add_entry (entry, version_appears_on_the_command_line);
        context.add_group(options);
        context.set_summary("This program assists in sending out form mails.");

        try
          {
            context.parse(argc, argv);
          }
        catch(const Glib::Error& ex)
          {
            std::cout << ex.what() << std::endl;
            std::cout << "Try `" PACKAGE_NAME 
              " --help' for more information." << std::endl;
            exit(0);
          }

        if (version_appears_on_the_command_line)
          {
            std::cout << PACKAGE_NAME << " " << PACKAGE_VERSION << std::endl;
            std::cout << 
              "This is free software: " <<
              "you are free to change and redistribute it." << std::endl;
            std::cout << 
              "There is NO WARRANTY, to the extent permitted by law." << 
              std::endl;
            exit(0);
          }

	impl->gtk_main.reset(new Gtk::Main(argc, argv, context));

    }
    catch (const Glib::Error &ex) {
	std::cerr << ex.what() << std::endl;
    }
}

Main::~Main()
{
    delete impl;
    singleton = 0;
}

Main &Main::instance()
{
    assert(singleton != 0);
    return *singleton;
}

bool Main::iterate_main_loop()
{
    try
    {
	impl->gtk_main->iteration(false);
    }
    catch (const Glib::Error &ex) {
	std::cerr << ex.what() << std::endl;
    }

    return true;
}

namespace Gsv {  class UndoManager_Class { public: static Glib::ObjectBase* wrap_new(GObject*); };  }
#include "templatizer-window.h"
void Main::start_main_loop()
{
    Gsv::init();
    try
    {
      window = TemplatizerWindow::create();
      impl->gtk_main->run(*window);
      window->hide();
      delete window;
    }
    catch (const Glib::Error &ex) {
	std::cerr << ex.what() << std::endl;
    }
}

void Main::stop_main_loop()
{
    try
    {
	impl->gtk_main->quit();
    }
    catch (const Glib::Error &ex) {
	std::cerr << ex.what() << std::endl;
    }
}
std::string Main::get_glade_path()
{
  return Glib::ustring(TEMPLATIZER_DATADIR)  + "/glade";
}

std::string Main::get_data_path()
{
  return Glib::ustring(TEMPLATIZER_DATADIR) + "/data";
}

TemplatizerWindow* Main::get_window()
{
  return window;
}
