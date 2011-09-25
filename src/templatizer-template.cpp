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

#include "templatizer-template.h"

using namespace Templatizer;

TemplatizerTemplate::TemplatizerTemplate(int i, Glib::ustring n, Glib::ustring t)
  :id(i), name(n), text(t)
{
}

TemplatizerTemplate::TemplatizerTemplate(const TemplatizerTemplate& t)
  :id(t.id), name(t.name), text(t.text)
{
}

TemplatizerTemplate::~TemplatizerTemplate()
{
}
        
bool TemplatizerTemplate::contains(Glib::ustring t)
{
  if (name.casefold().find(t.casefold()) != Glib::ustring::npos)
    return true;
  if (text.casefold().find(t.casefold()) != Glib::ustring::npos)
    return true;
  return false;
}
        
std::string TemplatizerTemplate::get_id_as_str() const
{
  return Glib::ustring::compose("%1", id);
}
