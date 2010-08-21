/*
    Copyright (C) 2001  ITK
    Author   : Paul Lasarev <paul@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI
*/
#ifndef CN_YUVM_H
#define CN_YUVM_H

#include <stdio.h>
#include <ctype.h>

#include "ci_clitypes.h"
#include "ci_coll.h"
#include "ci_node.h"
#include "ci_cliphash.h"

extern int clic_errorcount, clic_warncount;

extern int preproc_flag, pcode_flag, pc_flag, asm_flag;

extern int command_flag, main_flag;

extern const char *std_ch_filename;

extern Coll include_files;

extern int namespace_warning;

extern int nomain_flag;

extern int ifexpr_flag;

extern int iif_expr;

extern int field_flag;

extern int auto_memvar;

extern char *clic_charset;

extern int rmc_flag;

extern char *outdir;

int clic_parse(const char *filename, FILE * stream);

int yyerror(const char *s, ...);

int yywarning(const char *s, ...);

int yyparse();

int yylex();

int v_printf(int level, const char *fmt, ...);

int vr_printf(int level, const char *fmt, ...);	/* raw version */

extern int v_neednl;

void printVersion(FILE * file);

#define hashstr(x) _clip_hashstr(x)

/* lex analizer part */

int lex_new_file(FILE * file, const char *filename);

void init_lex();

void resume_lex();

void clear_lex();

int get_include(int *index, int *line, char **filename);

extern Coll includePaths;

extern Coll predefines;

extern Coll poName;

extern Coll paName;

extern Coll lib_dirs;

extern Coll reffuncs;

 /* return 0 if no more files; on first call index must be -1, then will keep
    state */
int currentFile();

char *fileName(int file);

void print_Var(Var * vp, int level);

void fprintfOffs(FILE * out, int offs, const char *format, ...);

extern long clic_line, corr_line, all_lines, clic_pos, cl_line, cl_pos;

extern int clic_notexpand_macro;

extern Coll fileList;

extern VarColl *curStatics, *fileStatics;

 /* keep incremental file name list */

void printString(FILE * stream, const char *str);

void assign_String(char **oldstr, char *newstr);

void assign_constString(char **oldstr, const char *newstr);

#include "ci_clic/typedef_struct_Function.h"
#include "ci_clic/typedef_struct_SwitchEl.h"
#include "ci_clic/typedef_struct_StrBuf.h"

#include "ci_clic/typedef_struct_File.h"

extern int genDebug;

/*extern int optLevel;*/
extern int vmLongJmps;

extern int verbose;

File *new_File(char *name);

void delete_File(File * file);

void add_Function(File * file, Function * func);

void print_File(File * file);

void codegen_File(File * file);

void write_File(File * file);

void write_OFile(File * file, long *len);

void compile_File(const char *name);

void share_File(const char *name);

void test_File(File * ClipMachineMemory);

void write_Cfunc(const char *fname, int num, char **vect, Coll * ex, Coll * nm);

void add_HashName(long hash, const char *name);

const char *get_HashName(long hash);

Function *new_Function(char *name, VarColl * params, int isPublic, int isMain, Function * upfunc, int isCodeblock);

void delete_Function(void *f);

int equal_Function(Function * f1, Function * f2);

extern char *clic_outpath;

extern File *curFile;

extern Function *curFunction, *mainFunction;

extern Coll *fileIncludes;

void init_parser();

void resume_parser();

Node *installName(char *name);

int undeclaredName(const char *name, int memvar);

void add_self(void);

Node *genAssign(char *name, Node * expr, int in_line);

Node *genCall(const char *name, Node * exprlist);

char *getName(const char *name);

char *setName(const char *name);

char *GetName(char *name);

char *SetName(char *name);

extern int traplevel;

void printCString(FILE * file, char *str);

/* recode str into appropiate charset */
void recodeString(char *str);

extern char *sourceCharset;

extern char *targetCharset;

extern int changeCharset;

extern char *out_charset;

extern char *CC, *COMPILE_FLAG, *CFLAGS, *ADDCFLAGS, *COPT, *CDBG, *OUT_FLAG, *INCLUDE_FLAG, *OBJSUF, *LIBSUF, *SLIBSUF, *SOBJSUF, *SFLAGS;

extern char *CLIPLIB, *CLIPSLIB;

extern char *CLIPROOT;

extern char *CLIP_MODULE;

extern char *CLIP_LOCALE_ROOT;

int put_locale_string(char *name);

int put_locale_string_plural(char *singular, char *plural);

int set_locale_name(char *filename);

int resume_locale(void);

int preprocCFile(char *iname, char *oname);

void compile_CFile(char *name);

int haveMacro(char *s);

StrBuf *new_StrBuf();

void delete_StrBuf(StrBuf * bp);

void putByte_StrBuf(StrBuf * bp, unsigned char b);

void putInt_StrBuf(StrBuf * bp, int i);

void putShort_StrBuf(StrBuf * bp, short s);

void putLong_StrBuf(StrBuf * bp, long l);

void putDouble_StrBuf(StrBuf * bp, double d);

void putStr_StrBuf(StrBuf * bp, const char *str);

void putString_StrBuf(StrBuf * bp, const char *str);

void write_StrBuf(StrBuf * bp, void *p, int len);

extern StrBuf bracket_buf;

#ifdef MEMDBG

#define malloc m_malloc
#define free m_free
#define calloc m_calloc
#define realloc m_realloc
#define strdup m_strdup
extern int c_sum, s_sum, m_sum, m_num, s_num, c_num;

void *m_calloc(size_t nmemb, size_t size);

void *m_malloc(size_t size);

void m_free(void *ptr);

void *m_realloc(void *ptr, size_t size);

char *m_strdup(const char *s);

#endif

void eof_checks(void);

Node *curr_operlist;

void fin_Function(Function * fpp);

int choose_quot(char *str, int *lquot, int *rquot, int simple);

char *quot_str(char *str, int simple);

extern int shared_flag, eshared_flag;

char *strsuff(const char *str, const char *suff);

void get_opt(int argc, char **argv);

void lex_vardef(int set);

void lex_initial(void);

extern int compat_as_word;

extern int names_flag;

extern char *_clic_gettext(const char *msg);

extern char *_clic_ngettext(const char *msg, const char *nmsg, int n);

#endif
