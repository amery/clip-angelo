/*
	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI
*/
#ifndef CN_PLURAL_H
#define CN_PLURAL_H

typedef struct PluralData PluralData;

PluralData *plural_new(char *expr, int elen);

void      plural_delete(PluralData * plural);

int       plural_eval(PluralData * plural, unsigned long int n);

#endif
