/*
	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

*/
#ifndef CN_PO_UTIL_H
#define CN_PO_UTIL_H

#include <stdio.h>
#include "ci_po_util/typedef_struct_PoEntry.h"

PoEntry  *new_PoEntry(char *msg, char *file, int line, PoEntry * first);

void      delete_PoEntry(void *entry);

int       cmp_PoEntry(void *p1, void *p2);

int       po_write_header(FILE * out);

int       po_write_entry(FILE * out, PoEntry * entry, int dupflag);

PoEntry  *po_read_entry(FILE * in);

int       po_write_entry_compat(FILE * out, PoEntry * entry, int dupflag);

PoEntry  *po_read_entry_compat(FILE * in);

int       po_parse_template(FILE * in, char *filename, char *start, char *stop,
			    void *par,
			    int (*entry_handler) (void *par, char *filename,
						  int line, char *txt), int (*norm_handler) (void *par, char *txt, int len));

#endif
