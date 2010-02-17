/*
	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI
 */

#ifndef CI_CONST_H
#define CI_CONST_H

typedef struct
{
   int type;
   char *str;
}
ConstStruct;

ConstStruct *new_ConstStruct(int type);
void delete_ConstStruct(void *cp);

void print_ConstStruct(ConstStruct * cp, FILE * file);
void printString(FILE * stream, const char *str);

#endif
