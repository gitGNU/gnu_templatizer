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

#ifndef TEMPLATIZER_TEMPLATE_H
#define TEMPLATIZER_TEMPLATE_H
#include <gtkmm.h>
namespace Templatizer
{
class TemplatizerTemplate
{
    public:

        TemplatizerTemplate(int id, Glib::ustring name, Glib::ustring text);

	//! Copy constructor.
        TemplatizerTemplate(const TemplatizerTemplate&);

        int get_id() const {return id;};
        std::string get_id_as_str() const;
        Glib::ustring get_name() const {return name;};
        Glib::ustring get_text() const {return text;};
        void set_name(Glib::ustring n) {name = n;};
        void set_text(Glib::ustring t) {text = t;};
        bool contains(Glib::ustring t);

	//! Destructor.
        ~TemplatizerTemplate();

    protected:
    private:
        int id;
        Glib::ustring name;
        Glib::ustring text;

};
}

#endif // TEMPLATIZER_TEMPLATE_H
