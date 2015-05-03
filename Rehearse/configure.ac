## Copyright (C) 2015 Nereo SARL.
# All Rights Reserved.
# This file is distributed under the MIT License.

## $Id: configure.ac,v 1.0 2015/04/13 13:37:00 onurcel Exp $

# Author:  Onur Çelebi                   NEREO    2015-04-13

#############################################################################
#                       Names and other basic things                        #
#############################################################################

AC_PREREQ(2.59)

AC_INIT([librehearse],[1.0],[onur.celebi@nereo.fr])

AC_COPYRIGHT([
Copyright 2015 Nereo.
All Rights Reserved.
This file is part of the open source package COIN-OR which is distributed
under the MIT License.])

# List one file in the package so that the configure script can test
# whether the package is actually there
AC_CONFIG_SRCDIR(src/CelBoolVar.cpp)

# Where should everything be installed by default?  Here, we want it
# to be installed directly in 'bin', 'lib', and 'include' subdirectories
# of the directory where configure is run.  The default would be
# /usr/local.
AC_PREFIX_DEFAULT([`pwd`])



#############################################################################
#                         Standard build tool stuff                         #
#############################################################################

# Get the system type
AC_CANONICAL_BUILD

# If this project depends on external projects, the Externals file in
# the source root directory contains definition of where to find those
# externals.  The following macro ensures that those externals are
# retrieved by svn if they are not there yet.
AC_COIN_PROJECTDIR_INIT(Rehearse)

# Check if user wants to produce debugging code
AC_COIN_DEBUG_COMPILE(Rehearse)

# Get the name of the C++ compiler and appropriate compiler options
AC_COIN_PROG_CXX

# Initialize automake and libtool
AC_COIN_INIT_AUTO_TOOLS

#############################################################################
#                              COIN components                              #
#############################################################################

AC_COIN_CHECK_PACKAGE(Osi, [osi], [Rehearse])
if test $coin_has_osi != yes ; then
  AC_MSG_ERROR([Required package Osi not available.])
fi
AC_COIN_CHECK_PACKAGE(Cbc, [osi-cbc])
AC_COIN_CHECK_PACKAGE(Clp, [osi-clp])



##############################################################################
#                  GNU libraries                                             #
##############################################################################
AC_COIN_CHECK_GNU_ZLIB
AC_COIN_CHECK_GNU_BZLIB


##############################################################################
#                   Finishing up by writing all the output                   #
##############################################################################

# Here list all the files that configure should create (except for the
# configuration header file)
AC_CONFIG_FILES([Makefile
                 src/Makefile
                 tests/Makefile
                 rehearse.pc])

# Here put the location and name of the configuration header file
AC_CONFIG_HEADER([src/config.h src/config_rehearse.h])

# Finally, we let configure write all the output...
AC_COIN_FINALIZE















