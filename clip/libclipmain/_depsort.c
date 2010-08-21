/*
	Copyright (C) 2002  ITK
	Author  :Paul Lasarev <paul@itk.ru>
	License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

*/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <errno.h>

#include <ci_clip.h>
#include <ci_error.ch>
#include <ci_coll.h>

extern char *CLIPROOT;

#define NEW(type) ((type*)calloc(sizeof(type),1))
#define NEWVECT(type,len) ((type*)calloc(sizeof(type),(len)))

/*
m:={;
    {"a",{"b","c"},'<commands>'},;
    {"b",{"c","d"},'<commands>'},;
    {"c",{"y"},'<commands>'},;
    {"y",{},'<commands>'};
   }

x := depsort(m [,target])

  ? x
    {;
     {"y",{},'<commands>'},;
     {"c",{"y"},'<commands>'},;
     {"b",{"c","d"},'<commands>'},;
     {"a",{"b","c"},'<commands>'};
    }
*/

#include <_depsort/typedef_struct_DepNode.h>

static DepNode *new_DepNode(ClipMachine * ClipMachineMemory, ClipVar * vp);

static void delete_DepNode(void *Np);

static int cmp_DepNode(void *p1, void *p2);

#include <_depsort/static_delete_DepNode.c>
#include <_depsort/static_cmp_DepNode.c>
#include <_depsort/static_new_DepNode.c>
#include <_depsort/static_find_node.c>
#include <_depsort/set_cycle.c>
#include <_depsort/static_depth_search.c>
#include <_depsort/clip_DEPSORT.c>
