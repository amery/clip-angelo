/*
	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI
 */

#ifndef CN_CONST_H
#define CN_CONST_H
#include "ci_const/typedef_struct_ConstStruct.h"

ConstStruct *new_ConstStruct(int type);

void      delete_ConstStruct(void *cp);

void      print_ConstStruct(ConstStruct * cp, FILE * file);

void      printString(FILE * stream, const char *str);

#endif
