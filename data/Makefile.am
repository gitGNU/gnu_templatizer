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
SUBDIRS=icons
MAINTAINERCLEANFILES= Makefile.in

EXTRA_DIST = README templatizer.desktop.in

DISTCLEANFILES = templatizer.desktop
desktopdir= $(datadir)/applications
desktop_DATA= templatizer.desktop

# also need to do a  xdg-icon-resource forceupdate
install-data-hook:
		xdg-desktop-icon install --novendor $(builddir)/templatizer.desktop
