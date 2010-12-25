/*
	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI
 */

#ifndef CN_YTYPES_H
#define CN_YTYPES_H

typedef unsigned char Uchar;

typedef unsigned int Uint;

typedef unsigned short Ushort;

typedef unsigned long Ulong;

#if defined(__GNUC__)
typedef long long llong;

typedef unsigned long long Ullong;
#else
typedef __int64 llong;

typedef unsigned __int64 Ullong;
#endif
#include "ci_clitypes/typedef_struct_OffsEntry.h"

#define NEW(type) ((type*)calloc(sizeof(type),1))
#define NEWVECT(type,len) ((type*)calloc(sizeof(type),(len)))
#define VAR(type,var,ini) type *var=(type*)(ini)
#define NEWVAR(type,var) type *var=(type*)calloc(sizeof(type),1)

#endif
