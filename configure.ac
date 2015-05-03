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
AC_CONFIG_SRCDIR(configure.ac)

# Where should everything be installed by default?  Here, we want it
# to be installed directly in 'bin', 'lib', and 'include' subdirectories
# of the directory where configure is run.  The default would be
# /usr/local.
AC_PREFIX_DEFAULT([`pwd`])





# Initialize automake
AC_COIN_CREATE_LIBTOOL


#############################################################################
#                  Check which subprojects are there                        #
#############################################################################
AC_COIN_MAIN_PACKAGEDIR(CoinUtils)
AC_COIN_MAIN_PACKAGEDIR(Osi)
AC_COIN_MAIN_PACKAGEDIR(Clp)
AC_COIN_MAIN_PACKAGEDIR(Cgl)
AC_COIN_MAIN_PACKAGEDIR(Cbc)
AC_COIN_MAIN_PACKAGEDIR(Rehearse)


##############################################################################
#                   Finishing up by writing all the output                   #
##############################################################################

# Here list all the files that configure should create (except for the
# configuration header file)
AC_CONFIG_FILES([Makefile])

# Finally, we let configure write all the output...
AC_COIN_FINALIZE



