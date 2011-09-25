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

#ifndef TEMPLATE_LIST_H
#define TEMPLATE_LIST_H

#include <gtkmm.h>
#include <string>
#include <vector>
#include <list>
#include <sqlite3.h>


#include "templatizer-template.h"
namespace Templatizer
{
class TemplateList
{
    public:

	//! Copy constructor.
        TemplateList(const TemplateList& template_list);

	//! Destructor.
        virtual ~TemplateList();
        
        typedef std::list<TemplatizerTemplate*>::iterator iterator;
        typedef std::list<TemplatizerTemplate*>::const_iterator const_iterator;

        void add(const TemplateList *g);
        
        size_t size() const {return _templates.size();}
        TemplatizerTemplate* back() {return _templates.back();}
        bool remove_template(TemplatizerTemplate *t);
        bool push_back(TemplatizerTemplate *t);

        std::list<TemplatizerTemplate*>& get_templates(){return _templates;}
        const std::list<TemplatizerTemplate*>& get_templates()const {return _templates;}
        int get_free_id() const;
        bool save(TemplatizerTemplate *t);


        static TemplateList* load_from_database();

    protected:
        //Constructor
        TemplateList();
        //Loading constructor
        TemplateList(std::string file);
    private:

        std::list<TemplatizerTemplate*> _templates;
        sqlite3 *db;

        //helpers
        bool create_empty_database();
        void remove_all_templates();

};
}

#endif // TEMPLATE_LIST_H

