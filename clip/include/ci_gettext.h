/* Internal header for GNU gettext internationalization functions.
   Copyright (C) 1995, 1997 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI
	*/

#ifndef CN__GETTEXT_H
#define CN__GETTEXT_H 1

#include <stdio.h>

#if HAVE_LIMITS_H || _LIBC
#include <limits.h>
#endif

/* The magic number of the GNU message catalog format.  */
#define _MAGIC 0x950412de
#define _MAGIC_SWAPPED 0xde120495

/* Revision number of the currently used .mo (binary) file format.  */
#define MO_REVISION_NUMBER 0

/* The following contortions are an attempt to use the C preprocessor
   to determine an unsigned integral type that is 32 bits wide.  An
   alternative approach is to use autoconf's AC_CHECK_SIZEOF macro, but
   doing that would require that the configure script compile and *run*
   the resulting executable.  Locally running cross-compiled executables
   is usually not possible.  */

#if __STDC__
#define UINT_MAX_32_BITS 4294967295U
#else
#define UINT_MAX_32_BITS 0xFFFFFFFF
#endif

/* If UINT_MAX isn't defined, assume it's a 32-bit type.
   This should be valid for all systems GNU cares about because
   that doesn't include 16-bit systems, and only modern systems
   (that certainly have <limits.h>) have 64+-bit integral types.  */

#ifndef UINT_MAX
#define UINT_MAX UINT_MAX_32_BITS
#endif

#if UINT_MAX == UINT_MAX_32_BITS
typedef unsigned nls_uint32;

#else
#if USHRT_MAX == UINT_MAX_32_BITS
typedef unsigned short nls_uint32;

#else
#if ULONG_MAX == UINT_MAX_32_BITS
typedef unsigned long nls_uint32;

#else
  /* The following line is intended to throw an error.  Using #error is
     not portable enough.  */
"Cannot determine unsigned 32-bit data type."
#endif
#endif
#endif
/* Header for binary .mo file format.  */
#include "ci_gettext/struct_mo_file_header.h"
#include "ci_gettext/struct_string_des.h"


#ifndef W
#define W(flag, data) ((flag) ? SWAP (data) : (data))
#endif
#include "ci_gettext/struct_loaded_domain.h"


#endif
