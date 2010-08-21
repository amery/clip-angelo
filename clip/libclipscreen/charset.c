/*
	Copyright (C) 2001  ITK
	Author   : Paul Lasarev <paul@itk.ru>
	License : (GPL) http://www.itk.ru/clipper/license.html

*/
/*
	$Log: charset.c,v $
	Revision 1.2  2007/10/17 13:30:03  itk
	uri: fix warnings for gcc 4.2

	Revision 1.1  2006/06/22 19:35:31  itk
	uri: init sf.net repository

	Revision 1.10  2004/05/21 11:22:19  clip
	rust: minor fix for 'configure -m'

	Revision 1.9  2004/03/01 13:01:57  clip
	uri: add some support UTF-8 to screen output

	Revision 1.8  2002/04/09 12:14:04  clip
	screen patches by Przemyslaw Czerpak
	paul

	Revision 1.7  2002/02/01 09:02:15  clip
	pgchars
	paul

	Revision 1.6  2002/01/28 07:31:49  clip
	C/unknown charset cleanup
	paul

	Revision 1.5  2002/01/16 15:29:08  clip
	chinese patch
	preproc bugs
	some pseudographic chars < 32
	paul

*/
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include <ci_clip.h>

#include <ci_charset.h>
#include <ci_screen.h>

extern char *CLIPROOT;

#include <charset/static_getunicode.c>
#include <charset/static_addpair.c>
#include <charset/load_charset.c>
#include <charset/make_translation.c>
#include <charset/make_uniTable.c>
#include <charset/u32toutf8.c>
#include <charset/load_charset_name.c>
#include <charset/static_get_str.c>
#include <charset/load_charset_tables.c>
#include <charset/charset_have_pg.c>
