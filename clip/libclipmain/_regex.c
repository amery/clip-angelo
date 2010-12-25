/*
    Copyright (C) 2001  ITK
    Authors  : Uri Hnykin <uri@itk.ru>, Paul Lasarev <paul@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

*/

#include <string.h>
#include <sys/types.h>
#include <regex.h>

#include <ci_clip.h>
#include <ci_error.ch>

#define NEW(type)	((type*)calloc(sizeof(type),1))
#ifndef RE_NREGS
#define RE_NREGS	30
#endif

#include <regex/clip_SEARCH.c>
#include <regex/destroy_c_regex.c>
#include <regex/clip_RGCOMP.c>
#include <regex/clip_RGEXEC.c>
#include <regex/clip_RGCANCEL.c>
#include <regex/clip_SPLIT.c>
