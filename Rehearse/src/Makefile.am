# Copyright (C) 2006 International Business Machines and others.
# All Rights Reserved.
# This file is distributed under the Common Public License.

## $Id$

# Author:  Andreas Waechter           IBM    2006-04-13

AUTOMAKE_OPTIONS = foreign

########################################################################
#                               librehearse                             #
########################################################################

# Name of the library compiled in this directory.  We want it to be installed
# in the 'lib' directory
lib_LTLIBRARIES = librehearse.la

# List all source files for this library, including headers
librehearse_la_SOURCES = \
	CelModel.cpp CelModel.h \
	CelVariable.cpp CelVariable.h \
	CelNumVar.cpp CelNumVar.h \
	CelIntVar.cpp CelIntVar.h \
	CelExpression.cpp CelExpression.h \
	CelBoolVar.cpp CelBoolVar.h

# List all additionally required libraries
if DEPENDENCY_LINKING
librehearse_la_LIBADD = $(REHEARSE_LIBS)
endif

# This is for libtool
librehearse_la_LDFLAGS = $(LT_LDFLAGS)

########################################################################
#                            Additional flags                          #
########################################################################

# Here list all include flags, relative to this "srcdir" directory.  This
# "cygpath" stuff is necessary to compile with native compilers on Windows.
# "top_srcdir" refers to the basic directory for the main package that is
# being compiled.
AM_CPPFLAGS = $(REHEARSE_CFLAGS)

# This line is necessary to allow VPATH compilation
DEFAULT_INCLUDES = -I. -I`$(CYGPATH_W) $(srcdir)`

########################################################################
#                Headers that need to be installed                     #
########################################################################

# Here list all the header files that are required by a user of the library,
# and that therefore should be installed in 'include/coin'
includecoindir = $(includedir)/coin

includecoin_HEADERS = \
	CelModel.h CelVariable.h CelNumVar.h CelIntVar.h CelExpression.h CelBoolVar.h CelBoolVarArray.h CelCoefficients.fwd.h CelNumVarArray.h CelVariableArray.h

install-exec-local:
	$(install_sh_DATA) config_rehearse.h $(DESTDIR)$(includecoindir)/RehearseConfig.h

uninstall-local:
	rm -f $(DESTDIR)$(includecoindir)/RehearseConfig.h
