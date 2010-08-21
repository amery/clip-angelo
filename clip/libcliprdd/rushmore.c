/*
	Copyright (C) 2001  ITK
	Author  : Rushad Nabiullin <rust@itk.ru>
	License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI
*/
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <ci_error.ch>
#include <ci_rdd.h>

#define ADDLEX(lex) if(rd->rmfilter) astrcat(&rd->rmfilter,&rd->rmflen,(lex))

#include <rushmore/static_astrcat.c>
#include <rushmore/rm_yylex.c>
#include <rushmore/static_rm_checkpar.c>
#include <rushmore/rm_init.c>
#include <rushmore/static_rm_and.c>
#include <rushmore/static_rm_or.c>
#include <rushmore/static_rm_not.c>
#include <rushmore/static_rm_invoper.c>
#include <rushmore/static_rm_setscope.c>
#include <rushmore/static_rm_cmp.c>
#include <rushmore/static_rm_checkscope.c>
#include <rushmore/static_rm_intersectscope.c>
#include <rushmore/static_rm_alias2name.c>
#include <rushmore/static_rm_prim.c>
#include <rushmore/static_rm_term.c>
#include <rushmore/rm_expr.c>
#include <rushmore/rm_checkjoin.c>
#include <rushmore/rm_union.c>
#include <rushmore/rm_intersect.c>
#include <rushmore/rm_difference.c>
#include <rushmore/rm_evalpartial.c>
#include <rushmore/rm_inverse.c>
#include <rushmore/rm_copyfilter.c>
#include <rushmore/rm_isfiltrec.c>
