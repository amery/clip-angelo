/*
   Copyright (C) 2001  ITK
   Authors  : Uri Hnykin <uri@itk.ru>, Paul Lasarev <paul@itk.ru>
   License : (GPL) http://www.itk.ru/clipper/license.html
 */

/*
  $Log: bzip2.c,v $
  Revision 1.1  2006/06/22 20:12:02  itk
  uri: init sf.net

  Revision 1.5  2004/11/03 13:00:59  clip
  rust: a few bugs fixed

  Revision 1.4  2003/07/03 07:15:56  clip
  fix a lot of warnings
  paul

  Revision 1.3  2002/01/30 08:20:07  clip
  cigwin PG_CHARS fixes
  bzip2 func prefix auto-configuration
  paul

  Revision 1.2  2001/11/15 13:16:49  clip
  fix
  paul

 */

#include <errno.h>
#include <limits.h>
#include <string.h>

#include <ci_hashcode.h>
#include <ci_clip.h>
#include <ci_error.ch>

#include <bzlib.h>
#include <ci_bzpref.h>

#include <bzip2/static_destroy_bzip_file.c>
#include <bzip2/clip_BZIP2OPEN.c>
#include <bzip2/clip_BZIP2READ.c>
#include <bzip2/clip_BZIP2WRITE.c>
#include <bzip2/clip_BZIP2CLOSE.c>
#include <bzip2/static_write_ulong.c>
#include <bzip2/static_read_ulong.c>
#include <bzip2/clip_BZIP2.c>
#include <bzip2/clip_BUNZIP2.c>
