#   Copyright (C) 2011 Ben Asselstine
# 
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 3 of the License, or
#   (at your option) any later version.
# 
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU Library General Public License for more details.
# 
#   You should have received a copy of the GNU General Public License
#   along with this program; if not, write to the Free Software
#   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 
#   02110-1301, USA.
MAINTAINERCLEANFILES= Makefile.in

bin_PROGRAMS = templatizer
noinst_LIBRARIES = libgui.a libmodel.a

templatizer_SOURCES = main.cpp main.h templatizer-main.cpp
templatizer_LDADD = libgui.a libmodel.a $(GTKMM_LIBS) $(LIBSIGC_LIBS) $(GTKSOURCEVIEWMM_LIBS) $(SQLITE_LIBS)

libgui_a_SOURCES = \
    templatizer-window.cpp templatizer-window.h  \
    new-template-dialog.cpp new-template-dialog.h \
    delete-template-confirmation-dialog.cpp \
    delete-template-confirmation-dialog.h

libmodel_a_SOURCES = \
    templatizer-template.cpp templatizer-template.h  \
    template-list.cpp template-list.h  


INCLUDES = \
   $(GTKMM_CFLAGS) \
   $(LIBSIGC_CFLAGS) \
   $(GTKSOURCEVIEWMM_CFLAGS) \
   $(SQLITE_CFLAGS) \
   -DTEMPLATIZER_DATADIR='"$(TEMPLATIZER_DATADIR)"' 

AM_CXXFLAGS = -Wall
