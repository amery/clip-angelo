/*
Start total new system v. 0.0
with hard coded long name variables to have clear system
Angelo GIRARDI
*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <ci_clipcfg.h>
#include <ci_charset.h>

char *CLIPROOT = CLIP_ROOT;

#include <clip_conv/static_make_utbl.c>
#include <clip_conv/static_put_utf8.c>
#include <clip_conv/static_put_ucs2.c>
#include <clip_conv/main.c>
