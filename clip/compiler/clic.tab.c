
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 7 "clic.y"

/*
 * $Log: clic.y,v $
 * Revision 1.1  2006/06/22 19:35:15  itk
 * uri: init sf.net repository
 *
 * Revision 1.84  2006/01/04 08:06:04  clip
 * uri: small fix
 *
 * Revision 1.82  2005/12/19 10:59:04  clip
 * uri: some fix about output compiler messages to stderr
 *
 * Revision 1.81  2004/07/19 13:41:31  clip
 * rust: DO proc WITH ... didn't respect () (pass by value)
 *
 * Revision 1.80  2003/12/17 09:46:42  clip
 * uri: "f->fname" as "alias->fname" not as "field->fname"
 *
 * Revision 1.79  2003/03/25 14:20:59  clip
 * uri: *gettext changed to _clic_*gettext
 *
 * Revision 1.78  2003/01/05 12:32:25  clip
 * possible fixes #95,#98
 * paul
 *
 * Revision 1.77  2003/01/05 10:34:22  clip
 * possible fixes #98
 * paul
 *
 * Revision 1.76  2003/01/05 07:45:44  clip
 * fix sigsegv on compile
 * closes #96
 * paul
 *
 * Revision 1.75  2002/12/05 09:41:01  clip
 * use '=' at operator level as assign
 * closes #68
 * paul
 *
 * Revision 1.74  2002/12/04 09:05:52  clip
 * possible profile cleanup in _clip_eval
 * paul
 *
 * Revision 1.73  2002/11/27 13:40:44  clip
 * initial _CGET_ pseudofunction(bug 62):
 * _CGET_(var[i1,i2,i3,...]) -> __CGET__(@var[i1,i2,i3],{i1,i2,i3},"var",...)
 * paul
 *
 * Revision 1.72  2002/11/06 12:03:41  clip
 * add plural locale construction:
 * [asdf] ^ num_expr == ngettext("asdf", "asdf", num_expr)
 * paul
 *
 * Revision 1.71  2002/10/25 11:54:33  clip
 * localized messages for clip itself
 * paul
 *
 * Revision 1.70  2002/10/11 10:27:11  clip
 * primary support for search of unresolved function names:
 * clip compler flag -N and/or envar CLIP_NAMES=yes will generate
 * files .ex (external refs) and .nm (defined names)
 * clip_makeslib will join .ex and .nm files if CLIP_NAMES=yes
 * new util clip_makelib also will join .ex .nm
 * new util clip_cp may be used to copy lib/obj files and correspond
 * .ex .nm files automatic
 * Closes: #17
 * paul
 *
 * Revision 1.69  2002/10/10 08:30:37  clip
 * &(expr) do not auto-codestr in codeblocks.
 * Closes #16
 * paul
 *
 * Revision 1.68  2002/08/15 12:18:34  clip
 * fix warnings in new bison
 * paul
 *
 * Revision 1.67  2002/08/15 07:46:52  clip
 * comm
 * paul
 *
 * Revision 1.66  2002/08/13 06:44:14  clip
 * fix for empty msgid in i18n strings
 * paul
 *
 * Revision 1.65  2002/08/08 09:33:24  clip
 * preproc fix for define/command priority
 * paul
 *
 * Revision 1.64  2002/06/30 08:08:31  clip
 * new preprocessor code
 * paul
 *
 * Revision 1.63  2002/05/16 07:05:22  clip
 * fix for _field->(a)->... syntax
 * paul
 *
 * Revision 1.62  2002/04/16 14:56:15  clip
 * bug in exprlist node - wrong stack fixup
 * paul
 *
 * Revision 1.61  2002/04/05 12:34:14  clip
 * "&()" preproc fix
 * "&a[1]" runtime fix
 * vardef place
 * paul
 *
 * Revision 1.60  2002/04/05 07:24:33  clip
 * wrong init in vardefs in func body
 * paul
 *
 * Revision 1.59  2002/04/03 13:32:53  clip
 * now possible use in init vars, declared in the same operator:
 * local a:=1, b:=a+1
 * paul
 *
 * Revision 1.58  2002/02/28 13:08:38  clip
 * split static func & static proc states
 * paul
 *
 * Revision 1.57  2002/02/05 08:30:57  clip
 * #translate directive improvements
 * paul
 *
 * Revision 1.56  2002/01/31 10:00:02  clip
 * &NAME.NUMBER fix
 * paul
 *
 * Revision 1.55  2002/01/14 12:39:59  clip
 * REF keyword in vardef
 * paul
 *
 * Revision 1.54  2002/01/07 07:26:35  clip
 * AS syntax cleanup
 * paul
 *
 * Revision 1.53  2002/01/05 12:50:48  clip
 * LOCAL a,b AS typename
 * STATIC a,b AS typename
 * func( a AS typename, b)
 * recognized, but yet not used
 * paul
 *
 * Revision 1.52  2002/01/04 09:12:46  clip
 * field +=
 * paul
 *
 * Revision 1.51  2001/12/24 13:51:01  clip
 * bad commands like
 * APEND BLANK
 * now will generate syntax error
 * paul
 *
 * Revision 1.50  2001/12/21 12:53:30  clip
 * +=, ... for fields
 * paul
 *
 * Revision 1.49  2001/12/21 11:48:20  clip
 *  (cAlias)->&(cf) := nSum    // ����� �� ����
 * paul
 *
 * Revision 1.48  2001/12/18 10:23:17  clip
 * field declaration
 * paul
 *
 * Revision 1.47  2001/12/10 13:40:31  clip
 * fox-compatible operators:
 * dimension a1(1),a2(1,2),a3(1,2,3)
 * a1(1) = "asdasd"
 * a2(1,1) = "32142"
 *
 * paul
 *
 * Revision 1.46  2001/12/10 12:09:30  clip
 * reduce '(' expr ')' to expr
 * paul
 *
 * Revision 1.45  2001/12/10 08:44:50  clip
 * {n.n.n} as DATE expression
 * paul
 *
 * Revision 1.44  2001/11/22 10:44:19  clip
 * expr: '(' expr_list ')' stack fix
 * paul
 *
 * Revision 1.43  2001/10/31 06:11:21  clip
 * DO func WITH x
 * take arguments by reference
 *
 * Revision 1.42  2001/10/29 06:51:21  clip
 * fix text fopen modes
 * paul
 *
 * Revision 1.41  2001/10/28 14:17:31  clip
 * (asdf)->fld := expr
 *
 * Revision 1.40  2001/10/15 11:12:28  clip
 * negative constants in CASE labels
 *
 * Revision 1.39  2001/09/05 05:30:26  clip
 * CODEBLOCK/CODESTR now can have paramters, f.e:
 * CODESTR(|a| f->first==a) will generate string
 * {|A|FIELD->FIRST==A}
 * paul
 *
 * Revision 1.38  2001/09/04 08:26:59  clip
 * CODEBLOCK pseudofunction
 * process codeblocks with macro calls as CODEBLOCK
 * paul
 *
 * Revision 1.37  2001/08/27 09:45:30  clip
 * switch operator
 * paul
 *
 * Revision 1.36  2001/08/24 14:38:46  clip
 * CODESTR pseudofunction
 * paul
 *
 * Revision 1.35  2001/08/23 13:52:06  clip
 * merge with NEWLOCALS
 * paul
 *
 * Revision 1.34.2.6  2001/08/21 13:20:11  clip
 * fix
 * paul
 *
 * Revision 1.34.2.5  2001/08/21 12:10:35  clip
 * local init in codeblock, f.e.:
 *
 * local a:={1,2,3}
 * cb:={|x|local(b:=a[2]),iif(x==NIL,b,b:=x)}
 * ? eval(cb)
 * ? a
 * ? eval(cb,'dddd')
 * ? a
 * ?
 *
 * paul
 *
 * Revision 1.34.2.4  2001/08/21 09:33:09  clip
 * runtime macro blocks {||&s}
 * paul
 *
 * Revision 1.34.2.3  2001/08/20 12:32:09  clip
 * macro in blocks
 * paul
 *
 * Revision 1.34.2.2  2001/08/20 10:43:09  clip
 * macro in blocks
 * paul
 *
 * Revision 1.34.2.1  2001/08/20 07:13:47  clip
 * block with macro
 * fields with macro
 * paul
 *
 * Revision 1.34  2001/07/25 13:47:26  clip
 * wrong /PROCNAME file procedure name
 * paul
 *
 * Revision 1.33  2001/06/22 08:29:50  clip
 * wrong op orger in 'NAME & mname'  macro
 * paul
 *
 * Revision 1.32  2001/06/09 08:13:28  clip
 * unary +
 * paul
 *
 * Revision 1.31  2001/05/30 09:45:43  clip
 * for .. in .. [keys]
 * paul
 *
 * Revision 1.30  2001/05/04 07:05:15  clip
 * procline filename
 * paul
 *
 * Revision 1.29  2001/04/16 07:29:39  clip
 * -> ( expr_list )
 * paul
 *
 * Revision 1.28  2001/04/03 09:17:00  clip
 * license errors
 * paul
 *
 * Revision 1.27  2001/03/30 11:51:02  clip
 * add copyright
 *
 * Revision 1.26  2001/02/22 14:09:11  clip
 * FOREACH take off
 * paul
 *
 * Revision 1.25  2001/02/21 12:52:58  clip
 * append HASHNAME(hash) - inverce to HASHSTR(str)
 *
 * append obj:modify() trigger:
 * modify(obj,hash,newval) -> real new val
 * MAPMODIFY(obj, enable) switch modify processing
 *
 * paul
 *
 * Revision 1.24  2000/12/09 12:13:28  clip
 * locale
 * paul
 *
 * Revision 1.23  2000/12/08 11:55:27  clip
 * start localisation
 * paul
 *
 * Revision 1.22  2000/12/07 07:19:38  clip
 * [ string_constants ] - prepare for i18n
 * linear sequence of func names in .po
 * paul
 *
 * Revision 1.21  2000/11/29 08:20:39  clip
 * x call
 * Paul Lasarev <paul@itk.ru>
 *
 * Revision 1.20  2000/11/20 13:46:50  clip
 * (asdf)->(expr)
 * paul
 *
 * Revision 1.19  2000/08/01 12:05:38  clip
 * append _thread.c
 * Paul Lasarev <paul@itk.ru>
 *
 * Revision 1.18  2000/05/27 00:15:16  clip
 * (asdf)=expr
 * Paul Lasarev <paul@itk.ru>
 *
 * Revision 1.17  2000/05/26 23:50:42  clip
 * (name)=expr
 * Paul Lasarev <paul@itk.ru>
 *
 * Revision 1.16  2000/05/25 16:00:38  clip
 * macro in codeblocks
 * break
 * Paul Lasarev <paul@itk.ru>
 *
 * Revision 1.15  2000/05/20 18:37:26  clip
 * change hash function
 * append HASH_xxxx extraction into hashcode.h
 * Paul Lasarev <paul@itk.ru>
 *
 * Revision 1.14  2000/05/12 20:57:02  clip
 * '(' expr_list ')'
 * Paul Lasarev <paul@itk.ru>
 *
 * Revision 1.13  2000/05/11 15:02:34  clip
 * {,,} now is 3 element-array
 * Paul Lasarev <paul@itk.ru>
 *
 * Revision 1.12  2000/04/29 20:13:56  clip
 * *** empty log message ***
 *
 * Revision 1.11  2000/04/29 20:08:48  clip
 * '(' expr ')' -> '(' expr ')'
 *
 * Revision 1.10  2000/04/29 19:49:32  clip
 * array elment object call
 *
 * Revision 1.9  2000/04/15 20:57:31  paul
 * mandrake fixes
 *
 * Revision 1.8  2000/03/30 16:25:10  paul
 * PARAM
 *
 * Revision 1.7  2000/03/20 18:59:45  paul
 * __field__ directive
 *
 * Revision 1.6  2000/03/18 21:58:09  paul
 * *** empty log message ***
 *
 * Revision 1.5  2000/03/16 19:29:11  paul
 * f->m-> , m->f-> fixed
 *
 * Revision 1.4  2000/03/03 21:37:33  paul
 * preprocessor bug fix, warnings removed
 *
 * Revision 1.3  1999/10/29 19:28:30  paul
 * bug in macroassign (stack was corrupted)
 *
 * Revision 1.2  1999/10/26 15:42:15  paul
 * change NOT priority
 *
 * Revision 1.1  1999/10/25 16:39:27  paul
 * first entry
 *
 */

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <float.h>
#include <math.h>
#include <stdarg.h>

#include "clic.h"
#include "node.h"
#include "nodepriv.h"

#define YYERROR_VERBOSE
static int if_depth=0, if_line, if_file, loop_line, loop_file, seq_line, seq_file;
int traplevel;
static void check_main();
#define CM check_main()
int ifexpr_flag=0;
int iif_expr=0;
int field_flag=0;
static void upstr(char *str);
static void push_operlist(Node *operlist);
static void pop_operlist(void);
Node *curr_operlist;
static Node *reduce_ExprListNode(Node *np);
static Node *assop_node( int assop, char *area, Node *areaExpr, char*name, Node *nameExpr, Node *expr);
static Node *(*def_node)(VarColl *cp) = 0;



/* Line 189 of yacc.c  */
#line 492 "clic.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     STRING = 258,
     NUMBER = 259,
     ISTRING = 260,
     NAME = 261,
     CTEXT = 262,
     DATE = 263,
     KEYS = 264,
     PREPROC = 265,
     RETURN = 266,
     FUNCTION = 267,
     PROCEDURE = 268,
     IPROCEDURE = 269,
     EPROCEDURE = 270,
     EXTERN = 271,
     PARAMETERS = 272,
     SFUNCTION = 273,
     SPROCEDURE = 274,
     TEXT = 275,
     PUBLIC = 276,
     PRIVATE = 277,
     STATIC = 278,
     LOCAL = 279,
     DIMENSION = 280,
     NIL = 281,
     END = 282,
     TRUE = 283,
     FALSE = 284,
     WHILE = 285,
     DO = 286,
     ENDDO = 287,
     EXIT = 288,
     LOOP = 289,
     WITH = 290,
     CALL = 291,
     IF = 292,
     IIF = 293,
     ENDIF = 294,
     ELSE = 295,
     ELSEIF = 296,
     DOCASE = 297,
     CASE = 298,
     OTHERWISE = 299,
     ENDCASE = 300,
     SWITCH = 301,
     ENDSWITCH = 302,
     FIELD = 303,
     MEMVAR = 304,
     IN = 305,
     MEMVARPTR = 306,
     FIELDPTR = 307,
     PCOUNT = 308,
     PARBEG = 309,
     PARAMBEG = 310,
     PSHIFT = 311,
     PALL = 312,
     CGET = 313,
     FOR = 314,
     STEP = 315,
     TO = 316,
     NEXT = 317,
     FOREACH = 318,
     BEGSEQ = 319,
     RECOVER = 320,
     USING = 321,
     BREAK = 322,
     EBREAK = 323,
     ANNOUNCE = 324,
     REQUEST = 325,
     IFNL = 326,
     IFEXPR = 327,
     CODESTR = 328,
     CODEBLOCK = 329,
     AS = 330,
     REF = 331,
     POWA = 332,
     MULA = 333,
     MODA = 334,
     DIVA = 335,
     SUBA = 336,
     ADDA = 337,
     ASSIGN = 338,
     POSTINCR = 339,
     OR = 340,
     AND = 341,
     NOT = 342,
     NE = 343,
     EQ = 344,
     LE = 345,
     GE = 346,
     POW = 347,
     MINUS = 348,
     DECR = 349,
     INCR = 350,
     REPTR = 351,
     RPTR = 352,
     DSEMICOLON = 353
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 429 "clic.y"

	char *string;
	void *ptr;
	Coll *coll;
	struct {
		Coll *coll;
		int haveRest;
	} arglist;
	VarColl *varcoll;
	Var *var;
	List *list;
	Node *node;
	Function *func;
	int i;
	struct
	{
		int curseq;
		Node *using;
		Node *operlist;
	}
	recover;
	SwitchEl *switchel;



/* Line 214 of yacc.c  */
#line 652 "clic.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 664 "clic.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   3295

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  122
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  80
/* YYNRULES -- Number of rules.  */
#define YYNRULES  318
/* YYNRULES -- Number of states.  */
#define YYNSTATES  638

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   353

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     112,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,   121,    92,   101,   116,     2,
     113,   114,    99,    97,   115,    98,   108,   100,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   106,     2,
      90,    91,    89,     2,    85,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   107,     2,   120,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   117,   119,   118,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      86,    87,    88,    93,    94,    95,    96,   102,   103,   104,
     105,   109,   110,   111
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     8,    11,    15,    17,    20,    22,
      25,    27,    29,    30,    34,    35,    39,    40,    45,    50,
      53,    54,    58,    59,    63,    64,    68,    69,    73,    74,
      78,    81,    85,    88,    90,    93,    96,    99,   101,   105,
     111,   117,   124,   131,   136,   142,   150,   158,   168,   177,
     186,   197,   204,   209,   215,   222,   226,   230,   234,   236,
     243,   249,   250,   257,   261,   265,   269,   273,   275,   277,
     278,   290,   291,   301,   302,   303,   311,   314,   323,   324,
     333,   339,   346,   347,   350,   353,   355,   360,   361,   364,
     367,   371,   373,   375,   377,   378,   380,   386,   392,   393,
     395,   396,   403,   404,   405,   408,   409,   412,   414,   416,
     418,   420,   421,   427,   428,   432,   434,   436,   437,   440,
     442,   448,   449,   453,   455,   457,   459,   461,   462,   465,
     467,   470,   473,   475,   478,   481,   484,   488,   492,   497,
     501,   506,   508,   510,   514,   516,   518,   520,   524,   527,
     532,   537,   539,   543,   547,   551,   558,   563,   569,   572,
     576,   579,   585,   590,   595,   601,   608,   612,   620,   626,
     632,   641,   648,   655,   659,   664,   670,   677,   681,   686,
     692,   699,   706,   711,   717,   725,   733,   743,   752,   761,
     772,   779,   785,   790,   796,   803,   807,   811,   815,   819,
     823,   827,   831,   835,   839,   843,   847,   851,   855,   859,
     863,   867,   870,   873,   876,   879,   882,   885,   888,   892,
     896,   903,   908,   910,   912,   916,   920,   921,   927,   928,
     934,   935,   942,   951,   955,   956,   960,   961,   963,   965,
     967,   969,   972,   975,   977,   980,   983,   985,   987,   989,
     991,   993,   995,   997,   999,  1000,  1003,  1008,  1013,  1017,
    1020,  1024,  1027,  1032,  1034,  1038,  1041,  1044,  1048,  1053,
    1055,  1058,  1062,  1066,  1068,  1071,  1075,  1079,  1083,  1088,
    1089,  1092,  1097,  1098,  1100,  1104,  1106,  1110,  1112,  1116,
    1118,  1119,  1122,  1127,  1130,  1132,  1136,  1140,  1146,  1148,
    1152,  1153,  1155,  1157,  1161,  1165,  1169,  1171,  1175,  1176,
    1178,  1180,  1184,  1186,  1188,  1190,  1192,  1194,  1196
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     123,     0,    -1,    -1,   123,   128,   112,    -1,   123,     7,
      -1,   123,    23,     7,    -1,    12,    -1,    23,    12,    -1,
      13,    -1,    23,    13,    -1,    14,    -1,    15,    -1,    -1,
     113,   187,   114,    -1,    -1,   127,   128,   112,    -1,    -1,
     124,     6,   126,   140,    -1,   125,     6,   126,   140,    -1,
      16,   188,    -1,    -1,    24,   129,   181,    -1,    -1,    23,
     130,   181,    -1,    -1,    21,   131,   181,    -1,    -1,    22,
     132,   181,    -1,    -1,    25,   133,   182,    -1,    49,   189,
      -1,    48,   189,   163,    -1,    17,   189,    -1,    11,    -1,
      11,   167,    -1,    70,   188,    -1,    69,     6,    -1,     1,
      -1,   167,    91,   167,    -1,     6,   110,     6,   177,   167,
      -1,     6,   110,   166,   177,   167,    -1,   174,     6,   110,
       6,   177,   167,    -1,   174,     6,   110,   166,   177,   167,
      -1,   174,     6,   177,   167,    -1,   166,   110,     6,   177,
     167,    -1,   113,   199,   114,   110,     6,   177,   167,    -1,
     113,   199,   114,   110,   166,   177,   167,    -1,   113,   199,
     114,   110,   113,   199,   114,   177,   167,    -1,   174,   113,
     199,   114,   110,     6,   177,   167,    -1,   174,   113,   199,
     114,   110,   166,   177,   167,    -1,   174,   113,   199,   114,
     110,   113,   199,   114,   177,   167,    -1,   174,   166,   110,
       6,   177,   167,    -1,   174,   166,   177,   167,    -1,   166,
     110,   166,   177,   167,    -1,   174,   166,   110,   166,   177,
     167,    -1,   164,    91,   167,    -1,   166,    91,   167,    -1,
     180,    91,   167,    -1,   167,    -1,     6,   113,   190,   114,
      91,   167,    -1,   113,   199,   114,    91,   167,    -1,    -1,
      30,   134,   167,   112,   127,   153,    -1,    31,     6,   191,
      -1,    31,   166,   191,    -1,    36,     6,   191,    -1,    36,
     166,   191,    -1,    33,    -1,    34,    -1,    -1,   144,   164,
     173,   167,    61,   167,   152,   112,   135,   127,   145,    -1,
      -1,   144,   164,    50,   167,   146,   112,   136,   127,   145,
      -1,    -1,    -1,    64,   150,   137,   127,   138,   148,    27,
      -1,    67,   198,    -1,    37,   113,   172,   115,   172,   115,
     172,   114,    -1,    -1,    37,   167,   112,   139,   127,   155,
     156,   157,    -1,    42,   112,   159,   160,   161,    -1,    46,
     167,   112,   141,   160,   162,    -1,    -1,    75,     6,    -1,
      76,     6,    -1,   142,    -1,   141,   143,   112,   127,    -1,
      -1,   142,   112,    -1,    43,   201,    -1,   143,   115,   201,
      -1,    59,    -1,    62,    -1,    27,    -1,    -1,     9,    -1,
     124,     6,   126,   140,   112,    -1,   125,     6,   126,   140,
     112,    -1,    -1,   147,    -1,    -1,    65,   150,   151,   112,
     149,   127,    -1,    -1,    -1,    66,   164,    -1,    -1,    60,
     167,    -1,    27,    -1,    32,    -1,    38,    -1,    37,    -1,
      -1,   155,    41,   167,   112,   127,    -1,    -1,    40,   112,
     127,    -1,    27,    -1,    39,    -1,    -1,   158,   112,    -1,
     158,    -1,   159,    43,   167,   112,   127,    -1,    -1,    44,
     112,   127,    -1,    27,    -1,    45,    -1,    27,    -1,    47,
      -1,    -1,    50,     6,    -1,     6,    -1,   175,     6,    -1,
     175,   166,    -1,     6,    -1,   175,     6,    -1,   174,     6,
      -1,   116,   165,    -1,   116,   165,   108,    -1,     6,   116,
     165,    -1,   116,   165,   108,     6,    -1,   116,   165,     4,
      -1,   116,   113,   167,   114,    -1,   200,    -1,     5,    -1,
       5,   102,   167,    -1,     8,    -1,   164,    -1,   166,    -1,
     113,   199,   114,    -1,    85,   167,    -1,     6,   113,   190,
     114,    -1,   166,   113,   190,   114,    -1,   180,    -1,   164,
     176,   167,    -1,   180,   176,   167,    -1,   166,   176,   167,
      -1,   167,   106,     6,   113,   190,   114,    -1,   180,   113,
     190,   114,    -1,   111,     6,   113,   190,   114,    -1,   174,
       6,    -1,     6,   110,     6,    -1,   174,   166,    -1,     6,
     109,   113,   167,   114,    -1,   174,     6,   110,     6,    -1,
     174,     6,   110,   166,    -1,     6,   110,   113,   199,   114,
      -1,   174,     6,   110,   113,   199,   114,    -1,     6,   110,
     166,    -1,   113,   199,   114,   110,   113,   199,   114,    -1,
     113,   199,   114,   110,   166,    -1,   113,   199,   114,   110,
       6,    -1,   174,   113,   199,   114,   110,   113,   199,   114,
      -1,   174,   113,   199,   114,   110,   166,    -1,   174,   113,
     199,   114,   110,     6,    -1,   166,   110,     6,    -1,   174,
     166,   110,     6,    -1,   166,   110,   113,   199,   114,    -1,
     174,   166,   110,   113,   199,   114,    -1,   166,   110,   166,
      -1,   174,   166,   110,   166,    -1,     6,   110,     6,   176,
     167,    -1,   174,     6,   110,     6,   176,   167,    -1,   174,
       6,   110,   166,   176,   167,    -1,   174,     6,   176,   167,
      -1,   166,   110,     6,   176,   167,    -1,   113,   199,   114,
     110,     6,   176,   167,    -1,   113,   199,   114,   110,   166,
     176,   167,    -1,   113,   199,   114,   110,   113,   199,   114,
     176,   167,    -1,   174,   113,   199,   114,   110,     6,   176,
     167,    -1,   174,   113,   199,   114,   110,   166,   176,   167,
      -1,   174,   113,   199,   114,   110,   113,   199,   114,   176,
     167,    -1,   174,   166,   110,     6,   176,   167,    -1,     6,
     110,   166,   176,   167,    -1,   174,   166,   176,   167,    -1,
     166,   110,   166,   176,   167,    -1,   174,   166,   110,   166,
     176,   167,    -1,   167,    91,   167,    -1,   167,    94,   167,
      -1,   167,    93,   167,    -1,   167,    90,   167,    -1,   167,
      89,   167,    -1,   167,    95,   167,    -1,   167,    96,   167,
      -1,   167,    97,   167,    -1,   167,    92,   167,    -1,   167,
      98,   167,    -1,   167,    99,   167,    -1,   167,   100,   167,
      -1,   167,   101,   167,    -1,   167,   102,   167,    -1,   167,
      87,   167,    -1,   167,    86,   167,    -1,    88,   167,    -1,
     105,   167,    -1,   167,   105,    -1,   104,   167,    -1,   167,
     104,    -1,    98,   167,    -1,    97,   167,    -1,   117,   197,
     118,    -1,   117,   195,   118,    -1,   117,   119,   178,   179,
     197,   118,    -1,    58,   113,   193,   114,    -1,    53,    -1,
      56,    -1,    54,   167,   120,    -1,    55,   167,   114,    -1,
      -1,    73,   171,   168,   197,   114,    -1,    -1,    74,   171,
     169,   197,   114,    -1,    -1,    24,   113,   178,   170,   181,
     114,    -1,   154,   113,   172,   115,   172,   115,   172,   114,
      -1,    68,   198,   114,    -1,    -1,   119,   187,   119,    -1,
      -1,   167,    -1,    83,    -1,    91,    -1,    52,    -1,   174,
      52,    -1,   175,    52,    -1,    51,    -1,   174,    51,    -1,
     175,    51,    -1,    83,    -1,    82,    -1,    81,    -1,    78,
      -1,    80,    -1,    79,    -1,    77,    -1,    91,    -1,    -1,
     187,   119,    -1,   167,   107,   199,   120,    -1,   180,   107,
     199,   120,    -1,   167,   106,     6,    -1,   111,     6,    -1,
     180,   106,     6,    -1,   185,   140,    -1,   181,   115,   185,
     140,    -1,   183,    -1,   182,   115,   183,    -1,     6,   184,
      -1,   166,   184,    -1,   113,   199,   114,    -1,   184,   113,
     199,   114,    -1,     6,    -1,     6,   186,    -1,     6,    83,
     167,    -1,   164,    91,   167,    -1,   166,    -1,   166,   186,
      -1,   166,    83,   167,    -1,   166,    91,   167,    -1,   107,
     199,   120,    -1,   186,   107,   199,   120,    -1,    -1,     6,
     140,    -1,   187,   115,     6,   140,    -1,    -1,     6,    -1,
     188,   115,     6,    -1,     6,    -1,   189,   115,     6,    -1,
     193,    -1,   193,   115,    57,    -1,    57,    -1,    -1,    35,
     192,    -1,    35,   192,   115,    57,    -1,    35,    57,    -1,
     194,    -1,   113,   194,   114,    -1,   192,   115,   194,    -1,
     192,   115,   113,   194,   114,    -1,   194,    -1,   193,   115,
     194,    -1,    -1,   167,    -1,   196,    -1,   195,   115,   196,
      -1,     6,   121,   167,    -1,     4,   121,   167,    -1,   198,
      -1,   197,   115,   198,    -1,    -1,   167,    -1,   167,    -1,
     199,   115,   167,    -1,     3,    -1,    28,    -1,    29,    -1,
      26,    -1,     4,    -1,   200,    -1,    98,   200,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   515,   515,   538,   547,   554,   559,   560,   563,   564,
     565,   566,   569,   570,   573,   574,   586,   587,   601,   615,
     636,   636,   648,   647,   660,   659,   671,   670,   682,   681,
     703,   704,   705,   706,   707,   709,   721,   731,   735,   736,
     737,   738,   739,   740,   741,   743,   744,   745,   746,   747,
     748,   750,   751,   752,   753,   755,   756,   757,   758,   759,
     773,   776,   775,   788,   790,   792,   794,   796,   803,   810,
     810,   817,   817,   824,   837,   824,   846,   849,   859,   858,
     882,   888,   894,   895,   896,   899,   900,   911,   914,   917,
     922,   929,   937,   938,   941,   942,   945,   946,   949,   956,
     964,   963,   977,   980,   981,   984,   985,   988,   989,   992,
     993,   996,   997,  1006,  1007,  1014,  1015,  1018,  1019,  1022,
    1023,  1032,  1033,  1040,  1041,  1044,  1045,  1048,  1049,  1052,
    1053,  1056,  1059,  1060,  1061,  1064,  1065,  1066,  1067,  1068,
    1070,  1073,  1074,  1096,  1101,  1106,  1107,  1108,  1112,  1114,
    1115,  1117,  1118,  1119,  1120,  1122,  1123,  1138,  1142,  1143,
    1144,  1146,  1149,  1150,  1151,  1152,  1153,  1155,  1156,  1157,
    1158,  1159,  1160,  1162,  1164,  1165,  1166,  1167,  1168,  1170,
    1171,  1172,  1173,  1174,  1176,  1177,  1178,  1179,  1180,  1181,
    1183,  1184,  1185,  1186,  1187,  1189,  1190,  1191,  1192,  1193,
    1194,  1195,  1196,  1197,  1199,  1200,  1201,  1202,  1203,  1204,
    1205,  1207,  1209,  1210,  1211,  1212,  1213,  1214,  1216,  1217,
    1218,  1239,  1240,  1241,  1242,  1243,  1245,  1245,  1247,  1247,
    1251,  1250,  1265,  1274,  1279,  1280,  1283,  1284,  1287,  1288,
    1291,  1292,  1293,  1296,  1297,  1298,  1301,  1302,  1303,  1304,
    1305,  1306,  1307,  1310,  1314,  1317,  1320,  1321,  1322,  1328,
    1335,  1342,  1350,  1363,  1367,  1374,  1375,  1378,  1379,  1382,
    1383,  1384,  1385,  1387,  1388,  1389,  1390,  1393,  1394,  1397,
    1398,  1406,  1417,  1418,  1422,  1436,  1440,  1447,  1448,  1449,
    1452,  1453,  1454,  1455,  1476,  1489,  1494,  1510,  1521,  1526,
    1537,  1538,  1543,  1544,  1547,  1548,  1552,  1564,  1579,  1580,
    1583,  1584,  1587,  1588,  1589,  1590,  1591,  1594,  1595
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "STRING", "NUMBER", "ISTRING", "NAME",
  "CTEXT", "DATE", "KEYS", "PREPROC", "RETURN", "FUNCTION", "PROCEDURE",
  "IPROCEDURE", "EPROCEDURE", "EXTERN", "PARAMETERS", "SFUNCTION",
  "SPROCEDURE", "TEXT", "PUBLIC", "PRIVATE", "STATIC", "LOCAL",
  "DIMENSION", "NIL", "END", "TRUE", "FALSE", "WHILE", "DO", "ENDDO",
  "EXIT", "LOOP", "WITH", "CALL", "IF", "IIF", "ENDIF", "ELSE", "ELSEIF",
  "DOCASE", "CASE", "OTHERWISE", "ENDCASE", "SWITCH", "ENDSWITCH", "FIELD",
  "MEMVAR", "IN", "MEMVARPTR", "FIELDPTR", "PCOUNT", "PARBEG", "PARAMBEG",
  "PSHIFT", "PALL", "CGET", "FOR", "STEP", "TO", "NEXT", "FOREACH",
  "BEGSEQ", "RECOVER", "USING", "BREAK", "EBREAK", "ANNOUNCE", "REQUEST",
  "IFNL", "IFEXPR", "CODESTR", "CODEBLOCK", "AS", "REF", "POWA", "MULA",
  "MODA", "DIVA", "SUBA", "ADDA", "ASSIGN", "POSTINCR", "'@'", "OR", "AND",
  "NOT", "'>'", "'<'", "'='", "'$'", "NE", "EQ", "LE", "GE", "'+'", "'-'",
  "'*'", "'/'", "'%'", "POW", "MINUS", "DECR", "INCR", "':'", "'['", "'.'",
  "REPTR", "RPTR", "DSEMICOLON", "'\\n'", "'('", "')'", "','", "'&'",
  "'{'", "'}'", "'|'", "']'", "'#'", "$accept", "file", "function",
  "procedure", "paramlist", "operlist", "oper", "$@1", "$@2", "$@3", "$@4",
  "$@5", "$@6", "$@7", "$@8", "$@9", "$@10", "$@11", "type_def",
  "switchlist", "switchbeg", "switchlabel", "for", "next", "keys",
  "recoverr", "recover", "$@12", "curseq", "using", "step", "enddo", "iif",
  "elseif", "else", "endif", "casebeg", "caselist", "otherwise", "endcase",
  "endswitch", "inalias", "name", "iname", "mname", "expr", "$@13", "$@14",
  "$@15", "bparam_list", "nilexpr", "eassign", "fieldptr", "memvarptr",
  "assign", "ass_op", "func", "code", "arr", "vardef", "dimdef", "dim",
  "dim_list", "var", "arr_list", "param_list", "namelist", "varlist",
  "arglist", "with_arglist", "with_arg_list", "arg_list", "arg",
  "pairlist", "pair", "exprlist", "nexpr", "expr_list", "constant",
  "mconstant", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,    64,   340,   341,   342,    62,
      60,    61,    36,   343,   344,   345,   346,    43,    45,    42,
      47,    37,   347,   348,   349,   350,    58,    91,    46,   351,
     352,   353,    10,    40,    41,    44,    38,   123,   125,   124,
      93,    35
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   122,   123,   123,   123,   123,   124,   124,   125,   125,
     125,   125,   126,   126,   127,   127,   128,   128,   128,   128,
     129,   128,   130,   128,   131,   128,   132,   128,   133,   128,
     128,   128,   128,   128,   128,   128,   128,   128,   128,   128,
     128,   128,   128,   128,   128,   128,   128,   128,   128,   128,
     128,   128,   128,   128,   128,   128,   128,   128,   128,   128,
     128,   134,   128,   128,   128,   128,   128,   128,   128,   135,
     128,   136,   128,   137,   138,   128,   128,   128,   139,   128,
     128,   128,   140,   140,   140,   141,   141,   142,   142,   143,
     143,   144,   145,   145,   146,   146,   147,   147,   148,   148,
     149,   148,   150,   151,   151,   152,   152,   153,   153,   154,
     154,   155,   155,   156,   156,   157,   157,   158,   158,   159,
     159,   160,   160,   161,   161,   162,   162,   163,   163,   164,
     164,   164,   165,   165,   165,   166,   166,   166,   166,   166,
     166,   167,   167,   167,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   167,   168,   167,   169,   167,
     170,   167,   167,   167,   171,   171,   172,   172,   173,   173,
     174,   174,   174,   175,   175,   175,   176,   176,   176,   176,
     176,   176,   176,   177,   178,   179,   180,   180,   180,   180,
     180,   181,   181,   182,   182,   183,   183,   184,   184,   185,
     185,   185,   185,   185,   185,   185,   185,   186,   186,   187,
     187,   187,   188,   188,   188,   189,   189,   190,   190,   190,
     191,   191,   191,   191,   192,   192,   192,   192,   193,   193,
     194,   194,   195,   195,   196,   196,   197,   197,   198,   198,
     199,   199,   200,   200,   200,   200,   200,   201,   201
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     3,     2,     3,     1,     2,     1,     2,
       1,     1,     0,     3,     0,     3,     0,     4,     4,     2,
       0,     3,     0,     3,     0,     3,     0,     3,     0,     3,
       2,     3,     2,     1,     2,     2,     2,     1,     3,     5,
       5,     6,     6,     4,     5,     7,     7,     9,     8,     8,
      10,     6,     4,     5,     6,     3,     3,     3,     1,     6,
       5,     0,     6,     3,     3,     3,     3,     1,     1,     0,
      11,     0,     9,     0,     0,     7,     2,     8,     0,     8,
       5,     6,     0,     2,     2,     1,     4,     0,     2,     2,
       3,     1,     1,     1,     0,     1,     5,     5,     0,     1,
       0,     6,     0,     0,     2,     0,     2,     1,     1,     1,
       1,     0,     5,     0,     3,     1,     1,     0,     2,     1,
       5,     0,     3,     1,     1,     1,     1,     0,     2,     1,
       2,     2,     1,     2,     2,     2,     3,     3,     4,     3,
       4,     1,     1,     3,     1,     1,     1,     3,     2,     4,
       4,     1,     3,     3,     3,     6,     4,     5,     2,     3,
       2,     5,     4,     4,     5,     6,     3,     7,     5,     5,
       8,     6,     6,     3,     4,     5,     6,     3,     4,     5,
       6,     6,     4,     5,     7,     7,     9,     8,     8,    10,
       6,     5,     4,     5,     6,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     2,     2,     2,     2,     2,     3,     3,
       6,     4,     1,     1,     3,     3,     0,     5,     0,     5,
       0,     6,     8,     3,     0,     3,     0,     1,     1,     1,
       1,     2,     2,     1,     2,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     2,     4,     4,     3,     2,
       3,     2,     4,     1,     3,     2,     2,     3,     4,     1,
       2,     3,     3,     1,     2,     3,     3,     3,     4,     0,
       2,     4,     0,     1,     3,     1,     3,     1,     3,     1,
       0,     2,     4,     2,     1,     3,     3,     5,     1,     3,
       0,     1,     1,     3,     3,     3,     1,     3,     0,     1,
       1,     3,     1,     1,     1,     1,     1,     1,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,     0,     1,    37,   312,   316,   142,   129,     4,   144,
      33,     6,     8,    10,    11,   282,     0,    24,    26,    22,
      20,    28,   315,   313,   314,    61,     0,    67,    68,     0,
       0,   109,     0,     0,     0,     0,   243,   240,   222,     0,
       0,   223,     0,    91,   102,   308,   308,     0,   282,   234,
     234,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     308,     0,     0,     0,     0,     0,   145,   146,    58,     0,
       0,   151,   141,     0,     0,     0,   300,     0,   129,     0,
     110,     0,   145,   146,    34,     0,   151,   283,    19,   285,
      32,     0,     0,     5,     7,     9,     0,   254,     0,     0,
       0,   290,   290,   290,   290,   236,     0,   117,     0,   127,
      30,     0,     0,   300,    73,   309,    76,     0,    36,    35,
     279,   226,   228,   148,   211,   217,   216,   214,   212,   259,
     310,     0,   132,     0,   135,     0,     0,   316,   129,   254,
       0,   302,     0,   306,    12,    12,     3,   129,     0,     0,
     236,   252,   249,   251,   250,   248,   247,   246,     0,     0,
       0,     0,   300,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     215,   213,     0,     0,   158,   244,   241,     0,   160,   130,
     245,   242,   131,     0,     0,     0,   300,     0,   143,     0,
     159,     0,   166,   289,   301,     0,   287,   298,   137,     0,
     300,     0,     0,     0,   158,     0,   160,     0,     0,   269,
       0,   273,    25,    82,    27,    23,   230,    21,     0,     0,
      29,   263,     0,   300,    63,    64,    65,    66,   237,     0,
      78,   119,   121,    87,     0,    31,   224,   225,     0,    14,
     233,    82,     0,   308,   308,   300,   147,     0,     0,   139,
     136,   134,   133,     0,     0,   279,     0,   219,   308,   218,
     279,    82,    82,     0,   238,   239,     0,   237,     0,    55,
     152,    56,   173,     0,   177,     0,   154,   210,   209,   199,
     198,   195,   203,   197,   196,   200,   201,   202,   204,   205,
     206,   207,   208,   258,     0,   253,     0,     0,     0,     0,
       0,     0,     0,    57,   260,     0,     0,   153,     0,     0,
       0,     0,     0,     0,   149,   300,   159,   166,     0,   147,
     173,   177,   195,     0,     0,     0,   284,   286,     0,     0,
     270,     0,     0,     0,   274,     0,     0,     0,   261,     0,
       0,   265,   266,     0,    14,   293,   300,   291,   294,   236,
      14,   118,     0,     0,     0,   121,    85,   128,   221,   300,
       0,   280,     0,   235,     0,     0,     0,     0,     0,   311,
     140,   138,   305,   304,   308,     0,     0,     0,   303,   307,
       0,    17,    18,    94,     0,   236,     0,     0,     0,     0,
       0,   150,   300,   256,   162,     0,   163,   182,    43,     0,
     174,     0,   178,   192,    52,   257,   156,   161,   179,    39,
     164,   191,    40,     0,   288,   299,   149,     0,   162,   163,
       0,   174,   178,   271,     0,     0,   272,   275,   276,    82,
      83,    84,     0,     0,     0,   264,     0,   301,     0,   300,
       0,     0,     0,    14,   123,   124,    80,     0,     0,     0,
      88,    22,     0,    98,    82,   227,   229,   157,    60,   169,
       0,   168,     0,   255,    13,    95,     0,     0,     0,   183,
      44,   175,   193,    53,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    59,   169,     0,   168,
       0,   277,     0,   262,   231,   267,     0,   107,   108,    62,
     295,   292,   300,   296,   236,   113,    14,     0,     0,   317,
      89,    14,     0,   125,   126,    81,    15,     0,   102,     0,
       0,    99,     0,   281,     0,     0,     0,     0,     0,   220,
      71,   105,   236,   155,   180,    41,   165,   181,    42,   172,
       0,   171,   190,    51,   176,   194,    54,     0,   172,     0,
     171,   278,   268,     0,     0,     0,     0,     0,     0,   318,
       0,    90,   103,    12,    12,    75,   184,    45,   167,   185,
      46,    14,     0,     0,     0,     0,     0,     0,     0,     0,
     167,     0,   297,    77,    14,     0,   115,   116,    79,     0,
       0,    82,    82,     0,     0,     0,   106,    69,   232,   187,
      48,   170,   188,    49,   170,     0,    14,   104,   100,     0,
       0,   186,    47,    93,    92,    72,    14,     0,     0,     0,
      14,    96,    97,     0,   189,    50,     0,    70
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    61,    62,   271,   370,   462,    98,    96,    91,
      92,    99,   100,   626,   581,   249,   463,   360,   348,   365,
     366,   458,    64,   625,   476,   531,   532,   630,   114,   600,
     583,   509,    65,   515,   567,   598,   241,   242,   364,   456,
     525,   245,    82,   134,    83,   130,   253,   254,   349,   121,
     239,   276,    85,    70,   159,   308,   226,   384,    86,   222,
     230,   231,   351,   223,   340,   252,    88,    90,   205,   234,
     357,   206,   207,   140,   141,   142,   143,   211,    72,   520
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -418
static const yytype_int16 yypact[] =
{
    -418,  1073,  -418,  -418,  -418,  -418,   -71,   148,  -418,  -418,
    2702,  -418,  -418,  -418,  -418,    52,    78,  -418,  -418,   302,
     -22,  -418,  -418,  -418,  -418,  -418,    30,  -418,  -418,    36,
    2788,  -418,   -14,  2702,    78,    78,  -418,  -418,  -418,  2702,
    2702,  -418,     6,  -418,  -418,  2702,  2702,   133,    52,     9,
       9,  2702,  2702,  2702,  2702,  2702,  2702,   187,  2702,    -4,
     949,   222,   231,   132,    51,   146,   577,   637,  3147,    27,
      19,   540,  -418,  2702,   152,     1,  2358,    55,   176,   -22,
    -418,  2702,   797,   497,  3169,    29,   653,  -418,   158,  -418,
     162,    31,    31,  -418,  -418,  -418,    31,  -418,    31,    40,
    2702,   -23,   246,   -23,   246,  2702,  3027,  -418,  3051,   -28,
     162,  2864,  2923,  2702,  -418,  3169,  -418,   174,  -418,   158,
     328,  -418,  -418,  3169,  3188,   225,   225,   225,   225,   239,
    3169,   -47,  -418,  2702,    12,   199,   228,   191,   261,  -418,
      -7,  -418,     0,  -418,   254,   254,  -418,  -418,   138,   297,
    2702,  -418,  -418,  -418,  -418,  -418,  -418,  -418,  2702,  2702,
    2702,    11,  2358,  2702,  2702,  2702,  2702,  2702,  2702,  2702,
    2702,  2702,  2702,  2702,  2702,  2702,  2702,  2702,  2702,  2702,
    -418,  -418,   353,  2702,   217,  -418,  -418,  2702,   421,   259,
    -418,  -418,  -418,  2702,   375,  2702,  2358,  2702,   225,  2702,
     381,  2702,   797,  -418,  3169,   224,   229,  -418,  -418,    13,
    2358,   236,    20,  2702,   473,  2702,   690,   378,   387,   210,
     309,    47,   282,   290,   282,   282,  -418,   282,    69,   289,
     308,  -418,  3075,  2444,  -418,  -418,  -418,  -418,  2949,   314,
    -418,   319,   329,  -418,   438,  -418,  -418,  -418,   265,  -418,
    -418,   290,   -85,  2702,  2702,  2358,   -25,  2702,  2975,  -418,
     443,  -418,  -418,  2702,  2702,   328,   341,  -418,  2702,  -418,
     328,   290,   290,  2702,  -418,  -418,  2702,  3169,   355,  3169,
    3169,  3169,   381,  2702,   797,   361,  3169,   748,  3188,   697,
     697,   494,   697,   697,   697,   697,   697,   757,   757,   141,
     141,   141,   225,   367,   111,  -418,    21,  2702,  2702,   272,
      35,  2702,  2702,  3169,  -418,   140,   368,  3169,  3001,  2702,
    2702,   275,  2702,  2702,   390,  2530,   481,   797,   369,   376,
     481,   797,   697,    37,   291,    67,  -418,  -418,  2702,  2702,
     382,  2702,  2702,  2702,   382,    31,   490,   500,  -418,    31,
    2702,   394,   394,    40,  -418,  -418,  2702,   393,  -418,  2702,
    -418,  -418,  2702,   397,    65,   364,   398,  -418,  -418,  2702,
    1775,  -418,   508,  -418,   296,   300,   401,  2702,    71,  3169,
    -418,  -418,  3169,  3169,  2702,   -59,   191,   396,  -418,  -418,
     306,  -418,  -418,   819,  2842,  2702,  2702,  2702,   313,  2702,
    2702,  -418,  2358,  -418,   381,  2702,   797,  3169,  3169,   408,
     381,  2702,   797,  3169,  3169,  -418,  -418,  -418,  3169,  3169,
    -418,  3169,  3169,  2702,  -418,  -418,  -418,    84,   481,   797,
     410,   481,   797,  3169,   205,  2702,  3169,  3169,  3169,   290,
    -418,  -418,   322,   324,  2702,  -418,  1892,  2896,   407,  2616,
     411,  1190,  3099,  -418,  -418,  -418,  -418,   207,   201,    22,
    -418,   404,   413,   226,   290,  -418,  -418,  -418,  3169,   381,
    2702,   797,   221,  -418,  -418,  -418,   417,  2702,   415,  3169,
    3169,  -418,  3169,  3169,   424,  2702,  2702,   326,  2702,  2702,
      88,  2702,  2702,   332,  2702,  2702,  3169,   481,  2702,   797,
      90,  -418,   208,  -418,  -418,  -418,   338,  -418,  -418,  -418,
    -418,  -418,  2702,  -418,  2702,   414,  -418,  1658,    60,  -418,
    -418,  -418,   207,  -418,  -418,  -418,  -418,   404,  -418,   516,
     529,  -418,   512,  -418,  2702,  2702,   342,  2702,  2702,  -418,
    -418,  2820,  2702,  -418,  3169,  3169,  -418,  3169,  3169,   381,
    2702,   797,  3169,  3169,  -418,  3169,  3169,   351,   481,  2702,
     797,  -418,  -418,   426,   428,   433,  2702,    86,  1307,  -418,
    1424,  -418,   483,   254,   254,  -418,  3169,  3169,   738,  3169,
    3169,  -418,  2702,   458,   457,  2702,  2702,   354,  2702,  2702,
     797,   359,  -418,  -418,  -418,  3123,  -418,  -418,  -418,    51,
     460,   290,   290,  2702,  2702,  2009,  3169,  -418,  -418,  3169,
    3169,   738,  3169,  3169,   797,  2126,  -418,  -418,  -418,   470,
     474,  3169,  3169,  -418,  -418,  -418,  -418,  2702,  2702,  1541,
    -418,  -418,  -418,  2009,  3169,  3169,  2243,  -418
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -418,  -418,   142,   145,  -139,  -238,   608,  -418,  -418,  -418,
    -418,  -418,  -418,  -418,  -418,  -418,  -418,  -418,  -248,  -418,
    -418,  -418,  -418,   -21,  -418,  -418,  -418,  -418,    83,  -418,
    -418,  -418,  -418,  -418,  -418,  -418,  -418,  -418,   248,  -418,
    -418,  -418,   178,   538,   334,    -1,  -418,  -418,  -418,   566,
    -129,  -418,   122,   130,   461,  -187,   486,  -418,    73,   -78,
    -418,   277,   405,   288,   416,  -110,   588,   444,  -134,   200,
    -418,   525,  -225,  -418,   384,  -243,   -41,     7,  -417,   120
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -311
static const yytype_int16 yytable[] =
{
      68,   312,   132,   371,   116,   117,   272,   200,   358,    84,
     374,   375,   233,   320,   224,   323,   259,   282,   225,   326,
     227,   278,   244,   391,   392,   189,   330,   404,   285,   106,
     372,    73,   108,   184,   373,   214,   101,   219,   111,   112,
     519,   410,   103,   428,   115,   115,   228,    36,    37,   523,
     123,   124,   125,   126,   127,   128,   372,   147,    87,   115,
     473,   132,   316,     4,     5,   131,   377,   256,   257,   524,
     190,   191,   198,   431,    71,   204,   328,   469,   185,   186,
     185,   186,    36,    37,    89,   378,    22,   218,    23,    24,
     497,    97,   454,    77,   549,   397,   558,   400,   107,   232,
     425,   569,    36,    37,   238,   519,    36,    37,   266,   133,
     455,   267,   204,   596,   201,   268,   446,    59,   269,   113,
     260,   376,   451,    69,   283,   597,   201,    59,   120,    59,
     342,   448,   258,   283,   405,    59,    59,    59,   343,   118,
     187,   472,   215,    59,   425,    59,    59,    59,   411,   277,
     405,    59,    59,    59,   339,   385,    59,   279,   280,   281,
     390,   204,   286,   287,   288,   289,   290,   291,   292,   293,
     294,   295,   296,   297,   298,   299,   300,   301,   302,    66,
     411,   135,   350,    59,   470,    77,   149,    59,   273,   136,
     304,   503,   313,   129,   309,   204,   317,   498,   318,   135,
      59,   550,   315,   559,    59,   261,    59,   136,   321,   204,
       4,     5,   332,   149,   149,   517,   533,   486,   149,   489,
     149,   274,   334,   492,   513,   495,   257,   389,   144,   275,
     450,   403,   204,    22,   262,    23,    24,   145,    11,    12,
      13,    14,   148,   179,   146,   180,   181,   182,   183,   527,
     185,   186,   115,   115,   204,   257,   379,    74,    75,   150,
     415,    76,   382,   383,    77,   199,   478,   115,   484,   220,
     220,   442,   393,   217,   220,   394,   220,   218,   568,   190,
     191,   233,   535,   570,   538,    74,   209,   563,   250,   210,
     398,   528,    77,   338,   151,   152,   153,   154,   155,   156,
     157,  -129,   235,   236,   237,   518,   407,   408,   305,    93,
     413,   414,   263,   521,    94,    95,   522,   339,   418,   419,
     257,   421,   422,   257,   204,   501,    77,   306,   561,   180,
     181,   182,   183,    77,   251,    67,   268,   433,   324,   539,
     436,   437,   438,   605,   325,   386,   434,   387,   185,   186,
     329,   257,   255,   619,   620,   447,   615,   443,   277,   303,
     102,   452,   586,   104,   589,   346,   347,   270,   204,    68,
      74,   209,   362,   363,   210,    77,   468,    77,   629,   368,
     369,   314,   264,   115,   336,   564,   409,   257,   633,   420,
     257,   604,   636,   337,   277,   479,   480,   345,   482,   483,
     341,   204,   350,   188,   192,   430,   257,   457,   363,   202,
     465,   268,   487,   584,   466,   268,    94,    95,   493,   216,
     474,   372,   496,   353,   628,   221,   221,   481,   257,   359,
     221,   361,   221,   229,   601,   602,   504,   345,   505,   257,
     546,   257,   502,    71,   367,    68,   554,   257,   204,   381,
      68,   506,   562,   257,   565,   566,   578,   257,   151,   152,
     153,   154,   155,   156,   157,   590,   257,   149,   611,   257,
     395,   149,   305,   614,   257,   401,   541,   536,   109,   110,
     402,   423,   416,   426,   544,   545,   427,   547,   548,   435,
     552,   553,    69,   555,   556,   284,   440,    77,   151,   152,
     153,   154,   155,   156,   157,   557,   441,   444,   449,   453,
     460,   447,   305,   277,   464,   467,    68,   264,   490,    71,
     500,   510,   573,   220,    71,   526,   514,   220,   163,   540,
     542,   310,   197,   576,   577,   574,   579,   580,   543,   575,
     592,   277,   593,   327,   163,   594,   331,   197,    66,   599,
     151,   152,   153,   154,   155,   156,   157,   587,   151,   152,
     153,   154,   155,   156,   157,   595,   591,    68,    69,    68,
     607,   608,   618,    69,   151,   152,   153,   154,   155,   156,
     157,   606,   631,   333,   609,   610,   632,   612,   613,    77,
      71,   174,   175,   176,   177,   178,   179,    77,   180,   181,
     182,   183,   621,   622,    68,   529,   -38,   212,   530,    63,
     162,   572,   637,   459,    68,   208,   122,   151,   152,   153,
     154,   155,   156,   157,    66,   265,   634,   635,    68,    66,
     445,   193,    68,   439,   352,    68,   119,   344,   248,    69,
     406,    71,   571,    71,   412,   307,   194,   195,     0,   311,
     388,     0,     0,   196,   151,   152,   153,   154,   155,   156,
     157,   319,     0,   322,     0,     0,     0,   429,   158,   432,
       0,     0,     0,     0,     0,   307,     0,   311,    71,   221,
       0,     0,     0,   221,     0,     0,     0,   229,    71,     0,
      69,     0,    69,     0,     0,    66,     0,     0,     0,     0,
       0,     0,    71,     0,    67,     0,    71,     0,     0,    71,
       0,     0,   471,     0,   151,   152,   153,   154,   155,   156,
     157,   149,     0,     0,     0,     0,     0,    69,   160,     0,
     151,   152,   153,   154,   155,   156,   157,    69,     0,     0,
       0,     0,     0,   396,     0,   399,    66,   161,    66,     0,
     162,    69,     0,     0,     0,    69,     0,     0,    69,   194,
     195,   499,     0,     0,     0,     0,   196,   151,   152,   153,
     154,   155,   156,   157,     0,     0,     0,   617,     0,     0,
      67,     0,     0,    66,     0,    67,     0,   319,   322,     0,
       0,   396,   399,    66,   174,   175,   176,   177,   178,   179,
     335,   180,   181,   182,   183,     0,     0,    66,     0,     0,
       0,    66,     0,     0,    66,   151,   152,   153,   154,   155,
     156,   157,     0,     0,   551,     0,     0,     0,   475,   305,
       0,     0,     0,     0,   560,   165,     0,   166,   167,   213,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,    67,   180,   181,   182,   183,   176,   177,   178,   179,
       0,   180,   181,   182,   183,   485,     0,   488,     0,     0,
       0,   491,     0,   494,   151,   152,   153,   154,   155,   156,
     157,     0,     0,     0,     0,     0,     0,     0,     0,   485,
     488,     0,   491,   494,     0,     0,     0,     0,     0,     0,
       0,     0,    67,     0,    67,   164,   165,     0,   166,   167,
     213,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,     0,   180,   181,   182,   183,     0,     0,     0,
     534,     0,   537,     0,     0,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    67,
       0,     0,     4,   137,     6,   138,     0,     9,   534,     0,
     537,     0,     0,    67,     0,     0,     0,    67,     0,     0,
      67,     0,     0,    79,     0,    22,     0,    23,    24,     0,
       0,     0,     0,     0,     0,     0,    80,    31,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      36,    37,    38,    39,    40,    41,     0,    42,     0,     0,
     585,     0,   588,     0,     0,     0,     0,    46,     0,   585,
       0,   588,    49,    50,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    51,     0,     0,    52,     0,   603,
       0,     0,     0,     0,     0,     0,    53,    54,     0,     0,
       0,   603,     0,    55,    56,     0,     0,     0,     0,     0,
      57,     0,    81,     0,     0,    59,    60,     0,   139,     0,
       0,     0,   627,     2,     3,   627,     4,     5,     6,     7,
       8,     9,     0,     0,    10,    11,    12,    13,    14,    15,
      16,     0,     0,     0,    17,    18,    19,    20,    21,    22,
       0,    23,    24,    25,    26,     0,    27,    28,     0,    29,
      30,    31,     0,     0,     0,    32,     0,     0,     0,    33,
       0,    34,    35,     0,    36,    37,    38,    39,    40,    41,
       0,    42,    43,     0,     0,     0,     0,    44,     0,     0,
      45,    46,    47,    48,     0,     0,    49,    50,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    51,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
      53,    54,     0,     0,     0,     0,     0,    55,    56,     0,
       0,     0,     0,     0,    57,   -16,    58,     0,     0,    59,
      60,     3,     0,     4,     5,     6,     7,     0,     9,     0,
       0,    10,    11,    12,    13,    14,    15,    16,     0,     0,
       0,    17,    18,   461,    20,    21,    22,  -111,    23,    24,
      25,    26,     0,    27,    28,     0,    29,    30,    31,  -111,
    -111,  -111,    32,     0,     0,     0,    33,     0,    34,    35,
       0,    36,    37,    38,    39,    40,    41,     0,    42,    43,
       0,     0,     0,     0,    44,     0,     0,    45,    46,    47,
      48,     0,     0,    49,    50,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    51,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,    53,    54,     0,
       0,     0,     0,     0,    55,    56,     0,     0,     0,     0,
       0,    57,   -16,    58,     0,     0,    59,    60,     3,     0,
       4,     5,     6,     7,     0,     9,     0,     0,    10,    11,
      12,    13,    14,    15,    16,     0,     0,     0,    17,    18,
     461,    20,    21,    22,  -120,    23,    24,    25,    26,     0,
      27,    28,     0,    29,    30,    31,     0,     0,     0,    32,
    -120,  -120,  -120,    33,     0,    34,    35,     0,    36,    37,
      38,    39,    40,    41,     0,    42,    43,     0,     0,     0,
       0,    44,     0,     0,    45,    46,    47,    48,     0,     0,
      49,    50,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    51,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     0,    53,    54,     0,     0,     0,     0,
       0,    55,    56,     0,     0,     0,     0,     0,    57,   -16,
      58,     0,     0,    59,    60,     3,     0,     4,     5,     6,
       7,     0,     9,     0,     0,    10,    11,    12,    13,    14,
      15,    16,     0,     0,     0,    17,    18,   461,    20,    21,
      22,   -86,    23,    24,    25,    26,     0,    27,    28,     0,
      29,    30,    31,     0,     0,     0,    32,   -86,   -86,     0,
      33,   -86,    34,    35,     0,    36,    37,    38,    39,    40,
      41,     0,    42,    43,     0,     0,     0,     0,    44,     0,
       0,    45,    46,    47,    48,     0,     0,    49,    50,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    51,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,     0,     0,     0,     0,     0,    55,    56,
       0,     0,     0,     0,     0,    57,   -16,    58,     0,     0,
      59,    60,     3,     0,     4,     5,     6,     7,     0,     9,
       0,     0,    10,    11,    12,    13,    14,    15,    16,     0,
       0,     0,    17,    18,   461,    20,    21,    22,  -112,    23,
      24,    25,    26,     0,    27,    28,     0,    29,    30,    31,
    -112,  -112,  -112,    32,     0,     0,     0,    33,     0,    34,
      35,     0,    36,    37,    38,    39,    40,    41,     0,    42,
      43,     0,     0,     0,     0,    44,     0,     0,    45,    46,
      47,    48,     0,     0,    49,    50,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    51,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,    53,    54,
       0,     0,     0,     0,     0,    55,    56,     0,     0,     0,
       0,     0,    57,   -16,    58,     0,     0,    59,    60,     3,
       0,     4,     5,     6,     7,     0,     9,     0,     0,    10,
      11,    12,    13,    14,    15,    16,     0,     0,     0,    17,
      18,   461,    20,    21,    22,  -122,    23,    24,    25,    26,
       0,    27,    28,     0,    29,    30,    31,     0,     0,     0,
      32,     0,     0,  -122,    33,  -122,    34,    35,     0,    36,
      37,    38,    39,    40,    41,     0,    42,    43,     0,     0,
       0,     0,    44,     0,     0,    45,    46,    47,    48,     0,
       0,    49,    50,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    51,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,    53,    54,     0,     0,     0,
       0,     0,    55,    56,     0,     0,     0,     0,     0,    57,
     -16,    58,     0,     0,    59,    60,     3,     0,     4,     5,
       6,     7,     0,     9,     0,     0,    10,    11,    12,    13,
      14,    15,    16,     0,     0,     0,    17,    18,   461,    20,
      21,    22,   -74,    23,    24,    25,    26,     0,    27,    28,
       0,    29,    30,    31,     0,     0,     0,    32,     0,     0,
       0,    33,     0,    34,    35,     0,    36,    37,    38,    39,
      40,    41,     0,    42,    43,     0,     0,     0,     0,    44,
     -74,     0,    45,    46,    47,    48,     0,     0,    49,    50,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      51,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     0,    53,    54,     0,     0,     0,     0,     0,    55,
      56,     0,     0,     0,     0,     0,    57,   -16,    58,     0,
       0,    59,    60,     3,     0,     4,     5,     6,     7,     0,
       9,     0,     0,    10,    11,    12,    13,    14,    15,    16,
       0,     0,     0,    17,    18,   461,    20,    21,    22,   507,
      23,    24,    25,    26,   508,    27,    28,     0,    29,    30,
      31,     0,     0,     0,    32,     0,     0,     0,    33,     0,
      34,    35,     0,    36,    37,    38,    39,    40,    41,     0,
      42,    43,     0,     0,     0,     0,    44,     0,     0,    45,
      46,    47,    48,     0,     0,    49,    50,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    51,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,    53,
      54,     0,     0,     0,     0,     0,    55,    56,     0,     0,
       0,     0,     0,    57,   -16,    58,     0,     0,    59,    60,
       3,     0,     4,     5,     6,     7,     0,     9,     0,     0,
      10,    11,    12,    13,    14,    15,    16,     0,     0,     0,
      17,    18,   461,    20,    21,    22,   623,    23,    24,    25,
      26,     0,    27,    28,     0,    29,    30,    31,     0,     0,
       0,    32,     0,     0,     0,    33,     0,    34,    35,     0,
      36,    37,    38,    39,    40,    41,     0,    42,    43,     0,
       0,   624,     0,    44,     0,     0,    45,    46,    47,    48,
       0,     0,    49,    50,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    51,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,    53,    54,     0,     0,
       0,     0,     0,    55,    56,     0,     0,     0,     0,     0,
      57,   -16,    58,     0,     0,    59,    60,     3,     0,     4,
       5,     6,     7,     0,     9,     0,     0,    10,    11,    12,
      13,    14,    15,    16,     0,     0,     0,    17,    18,   461,
      20,    21,    22,  -114,    23,    24,    25,    26,     0,    27,
      28,     0,    29,    30,    31,  -114,     0,     0,    32,     0,
       0,     0,    33,     0,    34,    35,     0,    36,    37,    38,
      39,    40,    41,     0,    42,    43,     0,     0,     0,     0,
      44,     0,     0,    45,    46,    47,    48,     0,     0,    49,
      50,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    51,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,    53,    54,     0,     0,     0,     0,     0,
      55,    56,     0,     0,     0,     0,     0,    57,   -16,    58,
       0,     0,    59,    60,     3,     0,     4,     5,     6,     7,
       0,     9,     0,     0,    10,    11,    12,    13,    14,    15,
      16,     0,     0,     0,    17,    18,   461,    20,    21,    22,
    -101,    23,    24,    25,    26,     0,    27,    28,     0,    29,
      30,    31,     0,     0,     0,    32,     0,     0,     0,    33,
       0,    34,    35,     0,    36,    37,    38,    39,    40,    41,
       0,    42,    43,     0,     0,     0,     0,    44,     0,     0,
      45,    46,    47,    48,     0,     0,    49,    50,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    51,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
      53,    54,     0,     0,     0,     0,     0,    55,    56,     0,
       0,     0,     0,     0,    57,   -16,    58,     0,     0,    59,
      60,     4,     5,     6,    78,     0,     9,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    79,     0,    22,     0,    23,    24,     0,     0,
       0,     0,     0,     0,     0,    80,    31,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    36,
      37,    38,    39,    40,    41,   203,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    46,     0,     0,     0,
       0,    49,    50,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    51,     0,     0,    52,     4,     5,     6,
      78,     0,     9,     0,     0,    53,    54,     0,     0,     0,
       0,     0,    55,    56,     0,     0,     0,     0,    79,    57,
      22,    81,    23,    24,    59,    60,     0,     0,     0,     0,
       0,    80,    31,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    36,    37,    38,    39,    40,
      41,   355,    42,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    46,     0,     0,     0,     0,    49,    50,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    51,
       0,     0,    52,     4,     5,     6,    78,     0,     9,     0,
       0,    53,    54,     0,     0,     0,     0,     0,    55,    56,
       0,     0,     0,     0,    79,    57,    22,   356,    23,    24,
      59,    60,     0,     0,     0,     0,     0,    80,    31,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    36,    37,    38,    39,    40,    41,   424,    42,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    46,     0,
       0,     0,     0,    49,    50,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    51,     0,     0,    52,     4,
       5,     6,    78,     0,     9,     0,     0,    53,    54,     0,
       0,     0,     0,     0,    55,    56,     0,     0,     0,     0,
      79,    57,    22,    81,    23,    24,    59,    60,     0,     0,
       0,     0,     0,    80,    31,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    36,    37,    38,
      39,    40,    41,   511,    42,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    46,     0,     0,     0,     0,    49,
      50,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    51,     0,     0,    52,     4,     5,     6,    78,     0,
       9,     0,     0,    53,    54,     0,     0,     0,     0,     0,
      55,    56,     0,     0,     0,     0,    79,    57,    22,   512,
      23,    24,    59,    60,     0,     0,     0,     0,     0,    80,
      31,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    36,    37,    38,    39,    40,    41,     0,
      42,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      46,     0,     0,     0,     0,    49,    50,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    51,     0,     0,
      52,     4,     5,     6,    78,     0,     9,     0,     0,    53,
      54,     0,     0,     0,     0,     0,    55,    56,     0,     0,
       0,     0,    79,    57,    22,    81,    23,    24,    59,    60,
       0,     0,     0,     0,     0,    80,    31,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    36,
      37,    38,    39,    40,    41,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    46,     0,     0,     0,
       0,    49,    50,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    51,     0,     0,    52,     0,     0,     0,
     582,     0,     0,     0,     0,    53,    54,     0,     0,     0,
       0,     0,    55,    56,     0,     0,     0,     0,     0,    57,
       0,   105,     0,   477,    59,    60,   164,   165,     0,   166,
     167,   213,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,     0,   180,   181,   182,   183,   164,   165,
       0,   166,   167,   213,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,     0,   180,   181,   182,   183,
     164,   165,     0,   166,   167,   213,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,     0,   180,   181,
     182,   183,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   164,   165,   246,   166,   167,   213,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,     0,
     180,   181,   182,   183,     0,     0,     0,     0,     0,   164,
     165,  -310,   166,   167,   213,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,     0,   180,   181,   182,
     183,     0,     0,     0,     0,   164,   165,   247,   166,   167,
     213,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,     0,   180,   181,   182,   183,     0,     0,     0,
       0,   164,   165,  -310,   166,   167,   213,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,     0,   180,
     181,   182,   183,     0,     0,     0,     0,   164,   165,   380,
     166,   167,   213,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,     0,   180,   181,   182,   183,     0,
       0,     0,     0,   164,   165,   417,   166,   167,   213,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
       0,   180,   181,   182,   183,     0,     0,   164,   165,   240,
     166,   167,   213,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,     0,   180,   181,   182,   183,     0,
       0,   164,   165,   243,   166,   167,   213,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,     0,   180,
     181,   182,   183,     0,     0,   164,   165,   354,   166,   167,
     213,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,     0,   180,   181,   182,   183,     0,     0,   164,
     165,   516,   166,   167,   213,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,     0,   180,   181,   182,
     183,     0,     0,   164,   165,   616,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
       0,   180,   181,   182,   183,   164,   165,     0,   166,   167,
     213,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,     0,   180,   181,   182,   183,   166,   167,   213,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,     0,   180,   181,   182,   183
};

static const yytype_int16 yycheck[] =
{
       1,   188,     6,   251,    45,    46,   145,     6,   233,    10,
     253,   254,    35,   200,    92,   202,     4,     6,    96,     6,
      98,   150,    50,   271,   272,     6,     6,     6,   162,    30,
     115,   102,    33,     6,   119,     6,     6,     6,    39,    40,
     457,     6,     6,     6,    45,    46,     6,    51,    52,    27,
      51,    52,    53,    54,    55,    56,   115,     6,     6,    60,
     119,     6,   196,     3,     4,    58,    91,   114,   115,    47,
      51,    52,    73,     6,     1,    76,   210,     6,    51,    52,
      51,    52,    51,    52,     6,   110,    26,   115,    28,    29,
       6,   113,    27,   116,     6,   282,     6,   284,   112,   100,
     325,   518,    51,    52,   105,   522,    51,    52,   115,   113,
      45,   118,   113,    27,   113,   115,   354,   116,   118,   113,
     108,   255,   360,     1,   113,    39,   113,   116,   119,   116,
      83,   356,   133,   113,   113,   116,   116,   116,    91,     6,
     113,   384,   113,   116,   369,   116,   116,   116,   113,   150,
     113,   116,   116,   116,   107,   265,   116,   158,   159,   160,
     270,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,     1,
     113,    59,   113,   116,   113,   116,    64,   116,    50,    59,
     183,   439,   193,     6,   187,   196,   197,   113,   199,    77,
     116,   113,   195,   113,   116,     6,   116,    77,   201,   210,
       3,     4,   213,    91,    92,   453,   464,   404,    96,   406,
      98,    83,   215,   410,   449,   412,   115,   268,     6,    91,
     359,   120,   233,    26,     6,    28,    29,     6,    12,    13,
      14,    15,    64,   102,   112,   104,   105,   106,   107,    23,
      51,    52,   253,   254,   255,   115,   257,   109,   110,   113,
     120,   113,   263,   264,   116,   113,   395,   268,   402,    91,
      92,   349,   273,   115,    96,   276,    98,   115,   516,    51,
      52,    35,   469,   521,   471,   109,   110,   512,   114,   113,
     283,    65,   116,    83,    77,    78,    79,    80,    81,    82,
      83,    91,   102,   103,   104,    98,   307,   308,    91,     7,
     311,   312,   121,   112,    12,    13,   115,   107,   319,   320,
     115,   322,   323,   115,   325,   120,   116,   110,   120,   104,
     105,   106,   107,   116,     6,     1,   115,   338,   114,   118,
     341,   342,   343,   581,   115,     4,   339,     6,    51,    52,
     114,   115,   113,   601,   602,   356,   594,   350,   359,     6,
      26,   362,   549,    29,   551,    75,    76,   113,   369,   370,
     109,   110,    43,    44,   113,   116,   377,   116,   616,   114,
     115,     6,   121,   384,     6,   514,   114,   115,   626,   114,
     115,   578,   630,     6,   395,   396,   397,   115,   399,   400,
      91,   402,   113,    69,    70,   114,   115,    43,    44,    75,
     114,   115,   405,   542,   114,   115,    12,    13,   411,    85,
     114,   115,   423,   115,   611,    91,    92,   114,   115,   115,
      96,   112,    98,    99,   573,   574,   114,   115,   114,   115,
     114,   115,   435,   370,     6,   446,   114,   115,   449,     6,
     451,   444,   114,   115,    40,    41,   114,   115,    77,    78,
      79,    80,    81,    82,    83,   114,   115,   345,   114,   115,
     115,   349,    91,   114,   115,   114,   477,   470,    34,    35,
     113,    91,   114,   114,   485,   486,   110,   488,   489,   107,
     491,   492,   370,   494,   495,   161,     6,   116,    77,    78,
      79,    80,    81,    82,    83,   498,     6,   113,   115,   112,
     112,   512,    91,   514,     6,   114,   517,   121,   110,   446,
     110,   114,     6,   345,   451,   112,   115,   349,    67,   112,
     115,   110,    71,   534,   535,     6,   537,   538,   114,    27,
     114,   542,   114,   209,    83,   112,   212,    86,   370,    66,
      77,    78,    79,    80,    81,    82,    83,   550,    77,    78,
      79,    80,    81,    82,    83,   566,   559,   568,   446,   570,
     112,   114,   112,   451,    77,    78,    79,    80,    81,    82,
      83,   582,   112,   110,   585,   586,   112,   588,   589,   116,
     517,    97,    98,    99,   100,   101,   102,   116,   104,   105,
     106,   107,   603,   604,   605,   463,   112,   110,   463,     1,
     113,   528,   633,   365,   615,    77,    50,    77,    78,    79,
      80,    81,    82,    83,   446,   139,   627,   628,   629,   451,
     353,    91,   633,   345,   229,   636,    48,   221,   113,   517,
     306,   568,   522,   570,   310,   184,   106,   107,    -1,   188,
     266,    -1,    -1,   113,    77,    78,    79,    80,    81,    82,
      83,   200,    -1,   202,    -1,    -1,    -1,   333,    91,   335,
      -1,    -1,    -1,    -1,    -1,   214,    -1,   216,   605,   345,
      -1,    -1,    -1,   349,    -1,    -1,    -1,   353,   615,    -1,
     568,    -1,   570,    -1,    -1,   517,    -1,    -1,    -1,    -1,
      -1,    -1,   629,    -1,   370,    -1,   633,    -1,    -1,   636,
      -1,    -1,   378,    -1,    77,    78,    79,    80,    81,    82,
      83,   599,    -1,    -1,    -1,    -1,    -1,   605,    91,    -1,
      77,    78,    79,    80,    81,    82,    83,   615,    -1,    -1,
      -1,    -1,    -1,   282,    -1,   284,   568,   110,   570,    -1,
     113,   629,    -1,    -1,    -1,   633,    -1,    -1,   636,   106,
     107,   427,    -1,    -1,    -1,    -1,   113,    77,    78,    79,
      80,    81,    82,    83,    -1,    -1,    -1,   599,    -1,    -1,
     446,    -1,    -1,   605,    -1,   451,    -1,   326,   327,    -1,
      -1,   330,   331,   615,    97,    98,    99,   100,   101,   102,
     110,   104,   105,   106,   107,    -1,    -1,   629,    -1,    -1,
      -1,   633,    -1,    -1,   636,    77,    78,    79,    80,    81,
      82,    83,    -1,    -1,   490,    -1,    -1,    -1,     9,    91,
      -1,    -1,    -1,    -1,   500,    87,    -1,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   517,   104,   105,   106,   107,    99,   100,   101,   102,
      -1,   104,   105,   106,   107,   404,    -1,   406,    -1,    -1,
      -1,   410,    -1,   412,    77,    78,    79,    80,    81,    82,
      83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   428,
     429,    -1,   431,   432,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   568,    -1,   570,    86,    87,    -1,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,    -1,   104,   105,   106,   107,    -1,    -1,    -1,
     469,    -1,   471,    -1,    -1,    -1,    -1,    -1,    -1,   605,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   615,
      -1,    -1,     3,     4,     5,     6,    -1,     8,   497,    -1,
     499,    -1,    -1,   629,    -1,    -1,    -1,   633,    -1,    -1,
     636,    -1,    -1,    24,    -1,    26,    -1,    28,    29,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    37,    38,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      51,    52,    53,    54,    55,    56,    -1,    58,    -1,    -1,
     549,    -1,   551,    -1,    -1,    -1,    -1,    68,    -1,   558,
      -1,   560,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    85,    -1,    -1,    88,    -1,   578,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    -1,    -1,
      -1,   590,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,
     111,    -1,   113,    -1,    -1,   116,   117,    -1,   119,    -1,
      -1,    -1,   611,     0,     1,   614,     3,     4,     5,     6,
       7,     8,    -1,    -1,    11,    12,    13,    14,    15,    16,
      17,    -1,    -1,    -1,    21,    22,    23,    24,    25,    26,
      -1,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      37,    38,    -1,    -1,    -1,    42,    -1,    -1,    -1,    46,
      -1,    48,    49,    -1,    51,    52,    53,    54,    55,    56,
      -1,    58,    59,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      67,    68,    69,    70,    -1,    -1,    73,    74,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,
      -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    98,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,
      -1,    -1,    -1,    -1,   111,   112,   113,    -1,    -1,   116,
     117,     1,    -1,     3,     4,     5,     6,    -1,     8,    -1,
      -1,    11,    12,    13,    14,    15,    16,    17,    -1,    -1,
      -1,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    33,    34,    -1,    36,    37,    38,    39,
      40,    41,    42,    -1,    -1,    -1,    46,    -1,    48,    49,
      -1,    51,    52,    53,    54,    55,    56,    -1,    58,    59,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,    69,
      70,    -1,    -1,    73,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    -1,
      -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    -1,    -1,
      -1,   111,   112,   113,    -1,    -1,   116,   117,     1,    -1,
       3,     4,     5,     6,    -1,     8,    -1,    -1,    11,    12,
      13,    14,    15,    16,    17,    -1,    -1,    -1,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
      33,    34,    -1,    36,    37,    38,    -1,    -1,    -1,    42,
      43,    44,    45,    46,    -1,    48,    49,    -1,    51,    52,
      53,    54,    55,    56,    -1,    58,    59,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    67,    68,    69,    70,    -1,    -1,
      73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    85,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    98,    -1,    -1,    -1,    -1,
      -1,   104,   105,    -1,    -1,    -1,    -1,    -1,   111,   112,
     113,    -1,    -1,   116,   117,     1,    -1,     3,     4,     5,
       6,    -1,     8,    -1,    -1,    11,    12,    13,    14,    15,
      16,    17,    -1,    -1,    -1,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    -1,    33,    34,    -1,
      36,    37,    38,    -1,    -1,    -1,    42,    43,    44,    -1,
      46,    47,    48,    49,    -1,    51,    52,    53,    54,    55,
      56,    -1,    58,    59,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    67,    68,    69,    70,    -1,    -1,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    97,    98,    -1,    -1,    -1,    -1,    -1,   104,   105,
      -1,    -1,    -1,    -1,    -1,   111,   112,   113,    -1,    -1,
     116,   117,     1,    -1,     3,     4,     5,     6,    -1,     8,
      -1,    -1,    11,    12,    13,    14,    15,    16,    17,    -1,
      -1,    -1,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    37,    38,
      39,    40,    41,    42,    -1,    -1,    -1,    46,    -1,    48,
      49,    -1,    51,    52,    53,    54,    55,    56,    -1,    58,
      59,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,
      69,    70,    -1,    -1,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,
      -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    -1,
      -1,    -1,   111,   112,   113,    -1,    -1,   116,   117,     1,
      -1,     3,     4,     5,     6,    -1,     8,    -1,    -1,    11,
      12,    13,    14,    15,    16,    17,    -1,    -1,    -1,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    37,    38,    -1,    -1,    -1,
      42,    -1,    -1,    45,    46,    47,    48,    49,    -1,    51,
      52,    53,    54,    55,    56,    -1,    58,    59,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    67,    68,    69,    70,    -1,
      -1,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    -1,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    98,    -1,    -1,    -1,
      -1,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,   111,
     112,   113,    -1,    -1,   116,   117,     1,    -1,     3,     4,
       5,     6,    -1,     8,    -1,    -1,    11,    12,    13,    14,
      15,    16,    17,    -1,    -1,    -1,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    -1,    33,    34,
      -1,    36,    37,    38,    -1,    -1,    -1,    42,    -1,    -1,
      -1,    46,    -1,    48,    49,    -1,    51,    52,    53,    54,
      55,    56,    -1,    58,    59,    -1,    -1,    -1,    -1,    64,
      65,    -1,    67,    68,    69,    70,    -1,    -1,    73,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      85,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    98,    -1,    -1,    -1,    -1,    -1,   104,
     105,    -1,    -1,    -1,    -1,    -1,   111,   112,   113,    -1,
      -1,   116,   117,     1,    -1,     3,     4,     5,     6,    -1,
       8,    -1,    -1,    11,    12,    13,    14,    15,    16,    17,
      -1,    -1,    -1,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    -1,    36,    37,
      38,    -1,    -1,    -1,    42,    -1,    -1,    -1,    46,    -1,
      48,    49,    -1,    51,    52,    53,    54,    55,    56,    -1,
      58,    59,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    -1,    -1,    73,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      98,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,
      -1,    -1,    -1,   111,   112,   113,    -1,    -1,   116,   117,
       1,    -1,     3,     4,     5,     6,    -1,     8,    -1,    -1,
      11,    12,    13,    14,    15,    16,    17,    -1,    -1,    -1,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,    -1,    36,    37,    38,    -1,    -1,
      -1,    42,    -1,    -1,    -1,    46,    -1,    48,    49,    -1,
      51,    52,    53,    54,    55,    56,    -1,    58,    59,    -1,
      -1,    62,    -1,    64,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    85,    -1,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    -1,    -1,
      -1,    -1,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,
     111,   112,   113,    -1,    -1,   116,   117,     1,    -1,     3,
       4,     5,     6,    -1,     8,    -1,    -1,    11,    12,    13,
      14,    15,    16,    17,    -1,    -1,    -1,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    37,    38,    39,    -1,    -1,    42,    -1,
      -1,    -1,    46,    -1,    48,    49,    -1,    51,    52,    53,
      54,    55,    56,    -1,    58,    59,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    67,    68,    69,    70,    -1,    -1,    73,
      74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    98,    -1,    -1,    -1,    -1,    -1,
     104,   105,    -1,    -1,    -1,    -1,    -1,   111,   112,   113,
      -1,    -1,   116,   117,     1,    -1,     3,     4,     5,     6,
      -1,     8,    -1,    -1,    11,    12,    13,    14,    15,    16,
      17,    -1,    -1,    -1,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      37,    38,    -1,    -1,    -1,    42,    -1,    -1,    -1,    46,
      -1,    48,    49,    -1,    51,    52,    53,    54,    55,    56,
      -1,    58,    59,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      67,    68,    69,    70,    -1,    -1,    73,    74,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,
      -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    98,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,
      -1,    -1,    -1,    -1,   111,   112,   113,    -1,    -1,   116,
     117,     3,     4,     5,     6,    -1,     8,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    26,    -1,    28,    29,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    37,    38,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    51,
      52,    53,    54,    55,    56,    57,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    68,    -1,    -1,    -1,
      -1,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    -1,    -1,    88,     3,     4,     5,
       6,    -1,     8,    -1,    -1,    97,    98,    -1,    -1,    -1,
      -1,    -1,   104,   105,    -1,    -1,    -1,    -1,    24,   111,
      26,   113,    28,    29,   116,   117,    -1,    -1,    -1,    -1,
      -1,    37,    38,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    51,    52,    53,    54,    55,
      56,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    68,    -1,    -1,    -1,    -1,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      -1,    -1,    88,     3,     4,     5,     6,    -1,     8,    -1,
      -1,    97,    98,    -1,    -1,    -1,    -1,    -1,   104,   105,
      -1,    -1,    -1,    -1,    24,   111,    26,   113,    28,    29,
     116,   117,    -1,    -1,    -1,    -1,    -1,    37,    38,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    51,    52,    53,    54,    55,    56,    57,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,    -1,
      -1,    -1,    -1,    73,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,    88,     3,
       4,     5,     6,    -1,     8,    -1,    -1,    97,    98,    -1,
      -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    -1,    -1,
      24,   111,    26,   113,    28,    29,   116,   117,    -1,    -1,
      -1,    -1,    -1,    37,    38,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    51,    52,    53,
      54,    55,    56,    57,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    68,    -1,    -1,    -1,    -1,    73,
      74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    -1,    -1,    88,     3,     4,     5,     6,    -1,
       8,    -1,    -1,    97,    98,    -1,    -1,    -1,    -1,    -1,
     104,   105,    -1,    -1,    -1,    -1,    24,   111,    26,   113,
      28,    29,   116,   117,    -1,    -1,    -1,    -1,    -1,    37,
      38,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    51,    52,    53,    54,    55,    56,    -1,
      58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      68,    -1,    -1,    -1,    -1,    73,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,
      88,     3,     4,     5,     6,    -1,     8,    -1,    -1,    97,
      98,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,
      -1,    -1,    24,   111,    26,   113,    28,    29,   116,   117,
      -1,    -1,    -1,    -1,    -1,    37,    38,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    51,
      52,    53,    54,    55,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    68,    -1,    -1,    -1,
      -1,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    -1,    -1,    88,    -1,    -1,    -1,
      60,    -1,    -1,    -1,    -1,    97,    98,    -1,    -1,    -1,
      -1,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,   111,
      -1,   113,    -1,    61,   116,   117,    86,    87,    -1,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,    -1,   104,   105,   106,   107,    86,    87,
      -1,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,    -1,   104,   105,   106,   107,
      86,    87,    -1,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,    -1,   104,   105,
     106,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    87,   120,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,    -1,
     104,   105,   106,   107,    -1,    -1,    -1,    -1,    -1,    86,
      87,   115,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,    -1,   104,   105,   106,
     107,    -1,    -1,    -1,    -1,    86,    87,   114,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,    -1,   104,   105,   106,   107,    -1,    -1,    -1,
      -1,    86,    87,   114,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,    -1,   104,
     105,   106,   107,    -1,    -1,    -1,    -1,    86,    87,   114,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,    -1,   104,   105,   106,   107,    -1,
      -1,    -1,    -1,    86,    87,   114,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
      -1,   104,   105,   106,   107,    -1,    -1,    86,    87,   112,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,    -1,   104,   105,   106,   107,    -1,
      -1,    86,    87,   112,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,    -1,   104,
     105,   106,   107,    -1,    -1,    86,    87,   112,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,    -1,   104,   105,   106,   107,    -1,    -1,    86,
      87,   112,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,    -1,   104,   105,   106,
     107,    -1,    -1,    86,    87,   112,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
      -1,   104,   105,   106,   107,    86,    87,    -1,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,    -1,   104,   105,   106,   107,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,    -1,   104,   105,   106,   107
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   123,     0,     1,     3,     4,     5,     6,     7,     8,
      11,    12,    13,    14,    15,    16,    17,    21,    22,    23,
      24,    25,    26,    28,    29,    30,    31,    33,    34,    36,
      37,    38,    42,    46,    48,    49,    51,    52,    53,    54,
      55,    56,    58,    59,    64,    67,    68,    69,    70,    73,
      74,    85,    88,    97,    98,   104,   105,   111,   113,   116,
     117,   124,   125,   128,   144,   154,   164,   166,   167,   174,
     175,   180,   200,   102,   109,   110,   113,   116,     6,    24,
      37,   113,   164,   166,   167,   174,   180,     6,   188,     6,
     189,   131,   132,     7,    12,    13,   130,   113,   129,   133,
     134,     6,   166,     6,   166,   113,   167,   112,   167,   189,
     189,   167,   167,   113,   150,   167,   198,   198,     6,   188,
     119,   171,   171,   167,   167,   167,   167,   167,   167,     6,
     167,   199,     6,   113,   165,   174,   175,     4,     6,   119,
     195,   196,   197,   198,     6,     6,   112,     6,   164,   174,
     113,    77,    78,    79,    80,    81,    82,    83,    91,   176,
      91,   110,   113,   176,    86,    87,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     104,   105,   106,   107,     6,    51,    52,   113,   166,     6,
      51,    52,   166,    91,   106,   107,   113,   176,   167,   113,
       6,   113,   166,    57,   167,   190,   193,   194,   165,   110,
     113,   199,   110,    91,     6,   113,   166,   115,   115,     6,
     164,   166,   181,   185,   181,   181,   178,   181,     6,   166,
     182,   183,   167,    35,   191,   191,   191,   191,   167,   172,
     112,   158,   159,   112,    50,   163,   120,   114,   193,   137,
     114,     6,   187,   168,   169,   113,   114,   115,   167,     4,
     108,     6,     6,   121,   121,   178,   115,   118,   115,   118,
     113,   126,   126,    50,    83,    91,   173,   167,   172,   167,
     167,   167,     6,   113,   166,   190,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   167,   167,   167,   167,   167,
     167,   167,   167,     6,   199,    91,   110,   176,   177,   199,
     110,   176,   177,   167,     6,   199,   190,   167,   167,   176,
     177,   199,   176,   177,   114,   115,     6,   166,   190,   114,
       6,   166,   167,   110,   199,   110,     6,     6,    83,   107,
     186,    91,    83,    91,   186,   115,    75,    76,   140,   170,
     113,   184,   184,   115,   112,    57,   113,   192,   194,   115,
     139,   112,    43,    44,   160,   141,   142,     6,   114,   115,
     127,   140,   115,   119,   197,   197,   190,    91,   110,   167,
     114,     6,   167,   167,   179,   187,     4,     6,   196,   198,
     187,   140,   140,   167,   167,   115,   176,   177,   199,   176,
     177,   114,   113,   120,     6,   113,   166,   167,   167,   114,
       6,   113,   166,   167,   167,   120,   114,   114,   167,   167,
     114,   167,   167,    91,    57,   194,   114,   110,     6,   166,
     114,     6,   166,   167,   199,   107,   167,   167,   167,   185,
       6,     6,   181,   199,   113,   183,   127,   167,   194,   115,
     172,   127,   167,   112,    27,    45,   161,    43,   143,   160,
     112,    23,   128,   138,     6,   114,   114,   114,   167,     6,
     113,   166,   197,   119,   114,     9,   146,    61,   172,   167,
     167,   114,   167,   167,   190,   176,   177,   199,   176,   177,
     110,   176,   177,   199,   176,   177,   167,     6,   113,   166,
     110,   120,   199,   140,   114,   114,   199,    27,    32,   153,
     114,    57,   113,   194,   115,   155,   112,   127,    98,   200,
     201,   112,   115,    27,    47,   162,   112,    23,    65,   124,
     125,   147,   148,   140,   176,   177,   199,   176,   177,   118,
     112,   167,   115,   114,   167,   167,   114,   167,   167,     6,
     113,   166,   167,   167,   114,   167,   167,   199,     6,   113,
     166,   120,   114,   194,   172,    40,    41,   156,   127,   200,
     127,   201,   150,     6,     6,    27,   167,   167,   114,   167,
     167,   136,    60,   152,   172,   176,   177,   199,   176,   177,
     114,   199,   114,   114,   112,   167,    27,    39,   157,    66,
     151,   126,   126,   176,   177,   127,   167,   112,   114,   167,
     167,   114,   167,   167,   114,   127,   112,   164,   112,   140,
     140,   167,   167,    27,    62,   145,   135,   176,   177,   127,
     149,   112,   112,   127,   167,   167,   127,   145
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1455 of yacc.c  */
#line 515 "clic.y"
    { ;}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 539 "clic.y"
    {
			if ((yyvsp[(2) - (3)].node))
			{
				if ( (yyvsp[(2) - (3)].node)->isExec )
					curFunction->lastExec=1;
				append_Node( curFunction->body, (yyvsp[(2) - (3)].node));
			}
		;}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 548 "clic.y"
    {
			if (curFunction==curFile->main)
				append_Node(curFile->ctext, new_LangNode((yyvsp[(2) - (2)].string)));
			else
				append_Node(curFunction->body, new_LangNode((yyvsp[(2) - (2)].string)));
		;}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 555 "clic.y"
    {
			append_Node(curFile->ctextStatic, new_LangNode((yyvsp[(3) - (3)].string)));
		;}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 559 "clic.y"
    {(yyval.i)=1; lex_vardef(1);;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 560 "clic.y"
    {(yyval.i)=0; lex_vardef(1);;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 563 "clic.y"
    {(yyval.i)=1;lex_vardef(1);;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 564 "clic.y"
    {(yyval.i)=0;lex_vardef(1);;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 565 "clic.y"
    {(yyval.i)=2;lex_vardef(1);;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 566 "clic.y"
    {(yyval.i)=3;lex_vardef(1);;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 569 "clic.y"
    {(yyval.varcoll)=new_VarColl();;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 570 "clic.y"
    { (yyval.varcoll)=(yyvsp[(2) - (3)].varcoll);;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 573 "clic.y"
    { (yyval.node)=new_OperListNode(); push_operlist((yyval.node)); ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 575 "clic.y"
    {
			(yyval.node)=(yyvsp[(1) - (3)].node);
			if ((yyvsp[(2) - (3)].node))
			{
				if ( (yyvsp[(2) - (3)].node)->isExec )
					curFunction->lastExec=1;
				append_Node( (yyval.node), (yyvsp[(2) - (3)].node));
			}
		;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 586 "clic.y"
    { (yyval.node)=NULL; ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 588 "clic.y"
    {
			eof_checks();
			curFunction=new_Function((yyvsp[(2) - (4)].string),(yyvsp[(3) - (4)].varcoll),(yyvsp[(1) - (4)].i),0,0,0);
			curFunction->typename = (yyvsp[(4) - (4)].string);
			lex_vardef(0);
			add_Function(curFile, curFunction);
			curStatics=curFunction->statics;
			fin_Function(curFunction);
			pop_operlist();
			push_operlist(curFunction->body);
			(yyval.node)=NULL;
		;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 602 "clic.y"
    {
			eof_checks();
			curFunction=new_Function((yyvsp[(2) - (4)].string),(yyvsp[(3) - (4)].varcoll),(yyvsp[(1) - (4)].i),0,0,0);
			curFunction->isProc=1;
			curFunction->typename = (yyvsp[(4) - (4)].string);
			lex_vardef(0);
			add_Function(curFile, curFunction);
			curStatics=curFunction->statics;
			fin_Function(curFunction);
			pop_operlist();
			push_operlist(curFunction->body);
			(yyval.node)=NULL;
		;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 615 "clic.y"
    {
			int i;
			char *s;
			for(i=0;i<(yyvsp[(2) - (2)].coll)->count;i++)
			{
				VAR(char, vp, (yyvsp[(2) - (2)].coll)->items[i]);
				if (insert_Coll( &curFile->externFunctions, vp)<0)
					free(vp);
				else
				{
					for(s=vp; *s; ++s)
						*s=toupper(*s);
					insert_Coll( &curFile->undeclExternFunctions, strdup(vp));
				}
			}
			removeAll_Coll((yyvsp[(2) - (2)].coll));
			delete_Coll((yyvsp[(2) - (2)].coll));
			(yyval.node)=NULL;
			if (curFunction->lastExec)
				yyerror("EXTERN after executable statements");
		;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 636 "clic.y"
    {
			lex_vardef(1);
			def_node=new_LocalDefNode1;
		;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 641 "clic.y"
    {
			/*$$=new_LocalDefNode($3, 1);*/
			(yyval.node)=NULL;
			lex_vardef(0); CM;
			def_node=0;
		;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 648 "clic.y"
    {
			lex_vardef(1);
			def_node = new_StaticDefNode;
		;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 653 "clic.y"
    {
			/*$$=new_StaticDefNode($3);*/
			(yyval.node)=NULL;
			lex_vardef(0);
			def_node=0;
		;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 660 "clic.y"
    {
			def_node = new_PublicDefNode;
		;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 664 "clic.y"
    {
			/*$$=new_PublicDefNode($2);*/
			(yyval.node)=NULL;
			CM;
			def_node=0;
		;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 671 "clic.y"
    {
			def_node = new_PrivateDefNode;
		;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 675 "clic.y"
    {
			/*$$=new_PrivateDefNode($2);*/
			(yyval.node)=NULL;
			CM;
			def_node=0;
		;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 682 "clic.y"
    {
			def_node = new_PrivateDefNode;
		;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 686 "clic.y"
    {
			/*$$=new_PrivateDefNode($2);*/
			(yyval.node)=NULL;
			CM;
			def_node=0;
		;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 703 "clic.y"
    { (yyval.node)=new_MemvarDefNode((yyvsp[(2) - (2)].varcoll)); ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 704 "clic.y"
    { (yyval.node)=new_FieldDefNode((yyvsp[(2) - (3)].varcoll), (yyvsp[(3) - (3)].string)); ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 705 "clic.y"
    { (yyval.node)=new_ParametersDefNode((yyvsp[(2) - (2)].varcoll)); CM;;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 706 "clic.y"
    { (yyval.node)=new_ReturnNode(NULL,0); CM;;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 707 "clic.y"
    { (yyval.node)=new_ReturnNode((yyvsp[(2) - (2)].node),0); CM;;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 710 "clic.y"
    {
				Coll *cp = (yyvsp[(2) - (2)].coll);
				int i;

				(yyval.node)=NULL;
				for(i=0; i< cp->count; ++i)
					append_Coll(&curFile->externModules, cp->items[i]);

				removeAll_Coll(cp);
				delete_Coll(cp);
			;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 721 "clic.y"
    {
				char *s = (yyvsp[(2) - (2)].string);

				(yyval.node)=NULL;
				free(curFile->main->name);
				curFile->main->name = s;
				for(; *s; ++s)
					*s = toupper(*s);
			;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 731 "clic.y"
    { (yyval.node)=NULL; ;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 735 "clic.y"
    { CM;(yyval.node)=new_OperExprNode(new_AssignNode((yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node),'=')); ;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 736 "clic.y"
    { CM;(yyval.node)=new_OperExprNode(assop_node((yyvsp[(4) - (5)].i),(yyvsp[(1) - (5)].string),NULL,(yyvsp[(3) - (5)].string),0,(yyvsp[(5) - (5)].node))); ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 737 "clic.y"
    { CM;(yyval.node)=new_OperExprNode(assop_node((yyvsp[(4) - (5)].i),(yyvsp[(1) - (5)].string),NULL,0,(yyvsp[(3) - (5)].node),(yyvsp[(5) - (5)].node))); ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 738 "clic.y"
    { CM; (yyval.node)=new_OperExprNode(assop_node((yyvsp[(5) - (6)].i), (yyvsp[(2) - (6)].string),NULL,(yyvsp[(4) - (6)].string),0,(yyvsp[(6) - (6)].node))); ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 739 "clic.y"
    { CM;(yyval.node)=new_OperExprNode(assop_node((yyvsp[(5) - (6)].i), (yyvsp[(2) - (6)].string),NULL,0,(yyvsp[(4) - (6)].node),(yyvsp[(6) - (6)].node))); ;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 740 "clic.y"
    {CM;(yyval.node)=new_OperExprNode(assop_node((yyvsp[(3) - (4)].i),NULL,NULL,(yyvsp[(2) - (4)].string),0,(yyvsp[(4) - (4)].node)));;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 741 "clic.y"
    { CM;(yyval.node)=new_OperExprNode(assop_node((yyvsp[(4) - (5)].i),NULL,(yyvsp[(1) - (5)].node),(yyvsp[(3) - (5)].string),0,(yyvsp[(5) - (5)].node))); ;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 743 "clic.y"
    { CM;(yyval.node)=new_OperExprNode(assop_node((yyvsp[(6) - (7)].i),NULL,(yyvsp[(2) - (7)].node),(yyvsp[(5) - (7)].string),0,(yyvsp[(7) - (7)].node))); ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 744 "clic.y"
    { CM;(yyval.node)=new_OperExprNode(assop_node((yyvsp[(6) - (7)].i),NULL,(yyvsp[(2) - (7)].node),0, (yyvsp[(5) - (7)].node),(yyvsp[(7) - (7)].node))); ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 745 "clic.y"
    { CM;(yyval.node)=new_OperExprNode(assop_node((yyvsp[(8) - (9)].i),NULL,(yyvsp[(2) - (9)].node),0,(yyvsp[(6) - (9)].node),(yyvsp[(9) - (9)].node))); ;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 746 "clic.y"
    { CM;(yyval.node)=new_OperExprNode(assop_node((yyvsp[(7) - (8)].i),NULL,(yyvsp[(3) - (8)].node),(yyvsp[(6) - (8)].string),0,(yyvsp[(8) - (8)].node))); ;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 747 "clic.y"
    { CM;(yyval.node)=new_OperExprNode(assop_node((yyvsp[(7) - (8)].i),NULL,(yyvsp[(3) - (8)].node),0, (yyvsp[(6) - (8)].node),(yyvsp[(8) - (8)].node))); ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 748 "clic.y"
    { CM;(yyval.node)=new_OperExprNode(assop_node((yyvsp[(9) - (10)].i),NULL,(yyvsp[(3) - (10)].node),0,(yyvsp[(7) - (10)].node),(yyvsp[(10) - (10)].node))); ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 750 "clic.y"
    { CM;(yyval.node)=new_OperExprNode(assop_node((yyvsp[(5) - (6)].i),NULL,(yyvsp[(2) - (6)].node),(yyvsp[(4) - (6)].string),0,(yyvsp[(6) - (6)].node))); ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 751 "clic.y"
    {CM;(yyval.node)=new_OperExprNode(assop_node((yyvsp[(3) - (4)].i),NULL,NULL,0,(yyvsp[(2) - (4)].node),(yyvsp[(4) - (4)].node)));;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 752 "clic.y"
    { CM;(yyval.node)=new_OperExprNode(assop_node((yyvsp[(4) - (5)].i),NULL,(yyvsp[(1) - (5)].node),0,(yyvsp[(3) - (5)].node),(yyvsp[(5) - (5)].node))); ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 753 "clic.y"
    { CM;(yyval.node)=new_OperExprNode(assop_node((yyvsp[(5) - (6)].i),NULL,(yyvsp[(2) - (6)].node),0,(yyvsp[(4) - (6)].node),(yyvsp[(6) - (6)].node))); ;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 755 "clic.y"
    { CM;(yyval.node)=new_OperExprNode(new_AssignNode((yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node),'=')); ;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 756 "clic.y"
    { CM;(yyval.node)=new_OperExprNode(new_AssignNode((yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node),'=')); ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 757 "clic.y"
    { CM;(yyval.node)=new_OperExprNode(new_ArrElNode((yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node),'=')); ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 758 "clic.y"
    { (yyval.node)=new_OperExprNode((yyvsp[(1) - (1)].node)); CM;;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 759 "clic.y"
    {
				Node *np;
				int i;
				CM;
				np = new_ExprListNode();
				append_Node(np, installName((yyvsp[(1) - (6)].string)));
				for(i=0; i< (yyvsp[(3) - (6)].arglist).coll->count; i++)
				{
					Node *p = (Node*) (yyvsp[(3) - (6)].arglist).coll->items[i];
					append_Node(np, p);
				}
				(yyval.node)=new_OperExprNode(new_ArrElNode(np, (yyvsp[(6) - (6)].node),'='));
			;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 773 "clic.y"
    { (yyval.node)=new_OperExprNode(new_AssignNode(new_MacroNode((yyvsp[(2) - (5)].node),0),(yyvsp[(5) - (5)].node),'=')); ;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 776 "clic.y"
    { CM;
			if (!curFunction->inLoop)
				{ loop_line = cl_line; loop_file = currentFile(); }
			curFunction->inLoop++;
		;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 782 "clic.y"
    {
			curFunction->inLoop--;
			(yyval.node)=new_WhileNode((yyvsp[(3) - (6)].node),(yyvsp[(5) - (6)].node));
			pop_operlist();
		;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 788 "clic.y"
    { CM; (yyval.node) = new_OperExprNode(new_CallNode((yyvsp[(2) - (3)].string), (yyvsp[(3) - (3)].arglist).coll, (yyvsp[(3) - (3)].arglist).haveRest)); ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 790 "clic.y"
    { CM; (yyval.node) = new_OperExprNode(new_CallNameNode((yyvsp[(2) - (3)].node), (yyvsp[(3) - (3)].arglist).coll)); ;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 792 "clic.y"
    { CM; (yyval.node) = new_OperExprNode(new_CallNode((yyvsp[(2) - (3)].string), (yyvsp[(3) - (3)].arglist).coll, (yyvsp[(3) - (3)].arglist).haveRest)); ;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 794 "clic.y"
    { CM; (yyval.node) = new_OperExprNode(new_CallNameNode((yyvsp[(2) - (3)].node), (yyvsp[(3) - (3)].arglist).coll)); ;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 796 "clic.y"
    {
			CM;
			if (!curFunction->inLoop)
				yyerror("EXIT statement out of loop");
			(yyval.node)=new_LoopExitNode(0);
		;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 803 "clic.y"
    {
			CM;
			if (!curFunction->inLoop)
				yyerror("LOOP statement out of loop");
			(yyval.node)=new_LoopExitNode(1);
		;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 810 "clic.y"
    { lex_initial(); ;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 811 "clic.y"
    {
			curFunction->inLoop--;
			(yyval.node)=new_ForNode((yyvsp[(2) - (11)].node),(yyvsp[(4) - (11)].node),(yyvsp[(6) - (11)].node),(yyvsp[(7) - (11)].node),(yyvsp[(10) - (11)].node));
			pop_operlist();
		;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 817 "clic.y"
    { lex_initial(); ;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 818 "clic.y"
    {
			curFunction->inLoop--;
			(yyval.node)=new_ForeachNode((yyvsp[(2) - (9)].node),(yyvsp[(4) - (9)].node),(yyvsp[(8) - (9)].node),(yyvsp[(5) - (9)].i));
			pop_operlist();
		;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 824 "clic.y"
    {
			CM;
			curFunction->seqNo = ++curFunction->allSeqNo;
			if (!curFunction->seqLevel)
			{
				seq_line = cl_line;
				seq_file = currentFile();
			}
			curFunction->seqLevel++;
			ARR_REALLOC(int, curFunction->seqStack, curFunction->seqLevel);
			curFunction->seqStack[curFunction->seqLevel-1]=(yyvsp[(2) - (2)].i);
		;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 837 "clic.y"
    {
			pop_operlist();
		;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 841 "clic.y"
    {
			CM;
			(yyval.node)=new_SeqNode((yyvsp[(4) - (7)].node),(yyvsp[(6) - (7)].recover).using,(yyvsp[(6) - (7)].recover).operlist,(yyvsp[(6) - (7)].recover).curseq);
		;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 847 "clic.y"
    {  CM; (yyval.node)=new_BreakNode((yyvsp[(2) - (2)].node), 1); ;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 850 "clic.y"
    {
			Coll *cp=new_Coll(NULL,NULL);
			append_Coll(cp,(yyvsp[(3) - (8)].node));
			append_Coll(cp,(yyvsp[(5) - (8)].node));
			(yyval.node)=new_OperExprNode(new_IfNode(cp,(yyvsp[(7) - (8)].node)));
			CM;
		;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 859 "clic.y"
    {
			if ((yyvsp[(2) - (3)].node)->isExprList && getCount_Node((yyvsp[(2) - (3)].node))>1 )
			{
				yyerror("please, use IIF function (not IF) on operator level to avoid conflicts");
				YYERROR;
			}
			if (!if_depth)
			{
				if_line = cl_line;
				if_file = currentFile();
			}
			if_depth++;
		;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 873 "clic.y"
    {
			CM;
			prepend_Coll((yyvsp[(6) - (8)].coll),(yyvsp[(5) - (8)].node));
			prepend_Coll((yyvsp[(6) - (8)].coll),(yyvsp[(2) - (8)].node));
			(yyval.node)=new_IfNode( (yyvsp[(6) - (8)].coll), (yyvsp[(7) - (8)].node));
			if_depth--;
			pop_operlist();
		;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 883 "clic.y"
    {
			CM;
			(yyval.node)=new_IfNode( (yyvsp[(3) - (5)].coll), (yyvsp[(4) - (5)].node));
		;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 889 "clic.y"
    {
			(yyval.node)=new_SwitchNode((yyvsp[(2) - (6)].node), (yyvsp[(4) - (6)].coll), (yyvsp[(5) - (6)].node));
		;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 894 "clic.y"
    { (yyval.string)=0; ;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 895 "clic.y"
    { (yyval.string)=(yyvsp[(2) - (2)].string); ;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 896 "clic.y"
    { (yyval.string)=(yyvsp[(2) - (2)].string); ;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 901 "clic.y"
    {
			SwitchEl *nl = NEW(SwitchEl);
			nl->labels = (yyvsp[(2) - (4)].coll);
			nl->operlist = (yyvsp[(4) - (4)].node);
			insert_Coll((yyvsp[(1) - (4)].coll), nl);
			(yyval.coll) = (yyvsp[(1) - (4)].coll);
			pop_operlist();
		;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 911 "clic.y"
    {
			(yyval.coll) = new_Coll(0, 0);
		;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 918 "clic.y"
    {
			(yyval.coll) = new_Coll(0,0);
			append_Coll((yyval.coll), (yyvsp[(2) - (2)].node));
		;}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 923 "clic.y"
    {
			(yyval.coll) = (yyvsp[(1) - (3)].coll);
			append_Coll((yyval.coll), (yyvsp[(3) - (3)].node));
		;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 930 "clic.y"
    {  CM;
			if (!curFunction->inLoop)
				{ loop_line = cl_line; loop_file = currentFile(); }
			curFunction->inLoop++;
		;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 941 "clic.y"
    { (yyval.i) = 0; ;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 942 "clic.y"
    { (yyval.i) = 1; ;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 945 "clic.y"
    {;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 946 "clic.y"
    {;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 949 "clic.y"
    {
			(yyval.recover).curseq = curFunction->seqNo;
			(yyval.recover).operlist = 0;
			(yyval.recover).using = 0;
			curFunction->seqNo=curFunction->seqStack[curFunction->seqLevel-1];
			curFunction->seqLevel--;
		;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 957 "clic.y"
    {
			yyerror("unclosed BEGIN SEQUENCE");
			curFunction->seqNo=curFunction->seqStack[curFunction->seqLevel-1];
			curFunction->seqLevel--;
			YYERROR;
		;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 964 "clic.y"
    {
			curFunction->seqNo=curFunction->seqStack[curFunction->seqLevel-1];
			curFunction->seqLevel--;
		;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 969 "clic.y"
    {
			(yyval.recover).operlist = (yyvsp[(6) - (6)].node);
			(yyval.recover).using = (yyvsp[(3) - (6)].node);
			(yyval.recover).curseq = (yyvsp[(2) - (6)].i);
			pop_operlist();
		;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 977 "clic.y"
    {(yyval.i) = curFunction->seqNo; ;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 980 "clic.y"
    { (yyval.node)=NULL; ;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 981 "clic.y"
    { (yyval.node)=(yyvsp[(2) - (2)].node); ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 984 "clic.y"
    { (yyval.node)=new_NumberConstNode(strdup("1"),0); ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 985 "clic.y"
    { (yyval.node)=(yyvsp[(2) - (2)].node); ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 996 "clic.y"
    { (yyval.coll)=new_Coll(NULL,NULL); ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 998 "clic.y"
    {
			(yyval.coll)=(yyvsp[(1) - (5)].coll);
			append_Coll((yyval.coll), (yyvsp[(3) - (5)].node));
			append_Coll((yyval.coll), (yyvsp[(5) - (5)].node));
			pop_operlist();
		;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 1006 "clic.y"
    { (yyval.node)=NULL; ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 1008 "clic.y"
    {
			(yyval.node)=(yyvsp[(3) - (3)].node);
			pop_operlist();
		;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 1018 "clic.y"
    { (yyval.coll)=new_Coll(NULL,NULL); ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 1024 "clic.y"
    {
			(yyval.coll)=(yyvsp[(1) - (5)].coll);
			append_Coll((yyval.coll), (yyvsp[(3) - (5)].node));
			append_Coll((yyval.coll), (yyvsp[(5) - (5)].node));
			pop_operlist();
		;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 1032 "clic.y"
    { (yyval.node)=NULL; ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 1034 "clic.y"
    {
			(yyval.node)=(yyvsp[(3) - (3)].node);
			pop_operlist();
		;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 1048 "clic.y"
    { (yyval.string)=NULL; ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 1049 "clic.y"
    { (yyval.string)=(yyvsp[(2) - (2)].string); ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 1052 "clic.y"
    { (yyval.node)=installName((yyvsp[(1) - (1)].string)); ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 1053 "clic.y"
    { (yyval.node)=new_MemvarNode(new_Var((yyvsp[(2) - (2)].string))); ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 1056 "clic.y"
    { (yyval.node)=(yyvsp[(2) - (2)].node); ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 1059 "clic.y"
    { (yyval.node)=installName((yyvsp[(1) - (1)].string)); ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 1060 "clic.y"
    { (yyval.node)=new_MemvarNode(new_Var((yyvsp[(2) - (2)].string))); ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 1061 "clic.y"
    { (yyval.node)=new2_FieldNode(NULL,NULL,(yyvsp[(2) - (2)].string),NULL); ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 1064 "clic.y"
    { (yyval.node)=new_MacroNode((yyvsp[(2) - (2)].node),0); ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 1065 "clic.y"
    { (yyval.node)=new_MacroNode((yyvsp[(2) - (3)].node),0); ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 1066 "clic.y"
    { (yyval.node)=new_MacroNode(new_OpNode(new_StringConstNode((yyvsp[(1) - (3)].string)),(yyvsp[(3) - (3)].node),'+'),0);  ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 1067 "clic.y"
    { (yyval.node)=new_MacroNode(new_OpNode((yyvsp[(2) - (4)].node),new_StringConstNode((yyvsp[(4) - (4)].string)),'+'),0); ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 1068 "clic.y"
    { char *s = (yyvsp[(3) - (3)].string)+strspn((yyvsp[(3) - (3)].string),",.-+");
			(yyval.node)=new_MacroNode(new_OpNode((yyvsp[(2) - (3)].node),new_StringConstNode(s),'+'),0); ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 1070 "clic.y"
    { (yyval.node)=new_MacroNode((yyvsp[(3) - (4)].node),1); ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 1074 "clic.y"
    {
				int i, l, bad;
				l = strlen((yyvsp[(1) - (1)].string));
				bad = 0;
				for(i=0; i<l; i++)
				{
					if ( (yyvsp[(1) - (1)].string)[i] & 0x80 )
					{
						bad = 1;
						break;
					}
				}
				if (l && !bad)
				{
					put_locale_string((yyvsp[(1) - (1)].string));
					(yyval.node)=new_LocaleStringNode((yyvsp[(1) - (1)].string));
				}
				else
				{
					(yyval.node)=new_StringConstNode((yyvsp[(1) - (1)].string));
				}
			;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 1096 "clic.y"
    {
				Node *e1 = new_StringConstNode((yyvsp[(1) - (3)].string));
				Node *e2 = new_StringConstNode(strdup((yyvsp[(1) - (3)].string)));
				(yyval.node) = new3_CallNode("_clic_ngettext", e1, e2, (yyvsp[(3) - (3)].node));
			;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 1101 "clic.y"
    {
				Coll *cp = new_Coll(0,0);
				append_Coll(cp, new_StringConstNode((yyvsp[(1) - (1)].string)));
				(yyval.node)=new_CallNode(strdup("CTOD"), cp, 0);
			;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 1108 "clic.y"
    {
		/*((ExprListNode*)$2)->last=1; ; */
		(yyval.node)=reduce_ExprListNode((yyvsp[(2) - (3)].node));
		;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 1112 "clic.y"
    { (yyval.node)=new_RefNode((yyvsp[(2) - (2)].node)); ;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 1114 "clic.y"
    { (yyval.node)=new_CallNode( (yyvsp[(1) - (4)].string), (yyvsp[(3) - (4)].arglist).coll, (yyvsp[(3) - (4)].arglist).haveRest); ;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 1115 "clic.y"
    { (yyval.node)=new_CallNameNode((yyvsp[(1) - (4)].node),(yyvsp[(3) - (4)].arglist).coll); ;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 1117 "clic.y"
    { (yyval.node)=new_ArrElNode((yyvsp[(1) - (1)].node),NULL,0); ;}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 1118 "clic.y"
    { (yyval.node)=new_AssignNode((yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node),(yyvsp[(2) - (3)].i)); ;}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 1119 "clic.y"
    { (yyval.node)=new_ArrElNode((yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node),(yyvsp[(2) - (3)].i)); ;}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 1120 "clic.y"
    { (yyval.node)=new_AssignNode((yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node),(yyvsp[(2) - (3)].i)); ;}
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 1122 "clic.y"
    { (yyval.node)=new_MethodNode((yyvsp[(1) - (6)].node),(yyvsp[(3) - (6)].string),(yyvsp[(5) - (6)].arglist).coll, (yyvsp[(5) - (6)].arglist).haveRest); ;}
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 1124 "clic.y"
    {
			Node *arr = (yyvsp[(1) - (4)].node);
			Node *last;
			char *name;
			last_List(&arr->list);
			last = (Node *) arr->list.current;
			if( !last || strcmp(last->name, "const") || !(name=((ConstNode*)last)->rval) )
			{
				yyerror("invalid array call");
				YYERROR;
			}
			remove_List(&arr->list);
			(yyval.node) = new_MethodNode(new_ArrElNode(arr,0,0), name, (yyvsp[(3) - (4)].arglist).coll, (yyvsp[(3) - (4)].arglist).haveRest);
		;}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 1138 "clic.y"
    {
		add_self();
		(yyval.node)=new_MethodNode(installName(strdup("__SELF__")),(yyvsp[(2) - (5)].string),(yyvsp[(4) - (5)].arglist).coll, (yyvsp[(4) - (5)].arglist).haveRest);
		;}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 1142 "clic.y"
    {(yyval.node)=new2_FieldNode(NULL,NULL,(yyvsp[(2) - (2)].string),NULL);;}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 1143 "clic.y"
    {(yyval.node)=new2_FieldNode((yyvsp[(1) - (3)].string),NULL,(yyvsp[(3) - (3)].string),NULL); ;}
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 1144 "clic.y"
    { (yyval.node)=new2_FieldNode(NULL,NULL,NULL,(yyvsp[(2) - (2)].node)); ;}
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 1146 "clic.y"
    { (yyval.node)=(yyvsp[(4) - (5)].node); ;}
    break;

  case 162:

/* Line 1455 of yacc.c  */
#line 1149 "clic.y"
    { (yyval.node)=new2_FieldNode((yyvsp[(2) - (4)].string),NULL,(yyvsp[(4) - (4)].string),NULL); ;}
    break;

  case 163:

/* Line 1455 of yacc.c  */
#line 1150 "clic.y"
    { (yyval.node)=new2_FieldNode((yyvsp[(2) - (4)].string),NULL,NULL,(yyvsp[(4) - (4)].node)); ;}
    break;

  case 164:

/* Line 1455 of yacc.c  */
#line 1151 "clic.y"
    { (yyval.node)=new2_FieldNode((yyvsp[(1) - (5)].string),NULL,NULL,reduce_ExprListNode((yyvsp[(4) - (5)].node))); ;}
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 1152 "clic.y"
    { (yyval.node)=new2_FieldNode((yyvsp[(2) - (6)].string),NULL,NULL,reduce_ExprListNode((yyvsp[(5) - (6)].node))); ;}
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 1153 "clic.y"
    { (yyval.node)=new2_FieldNode((yyvsp[(1) - (3)].string),NULL,NULL,(yyvsp[(3) - (3)].node)); ;}
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 1155 "clic.y"
    {(yyval.node)=new2_FieldNode(NULL,reduce_ExprListNode((yyvsp[(2) - (7)].node)),NULL,reduce_ExprListNode((yyvsp[(6) - (7)].node)));;}
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 1156 "clic.y"
    { (yyval.node)=new2_FieldNode(NULL,reduce_ExprListNode((yyvsp[(2) - (5)].node)),NULL,(yyvsp[(5) - (5)].node)); ;}
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 1157 "clic.y"
    { (yyval.node)=new2_FieldNode(NULL,reduce_ExprListNode((yyvsp[(2) - (5)].node)),(yyvsp[(5) - (5)].string),NULL); ;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 1158 "clic.y"
    {(yyval.node)=new2_FieldNode(NULL,reduce_ExprListNode((yyvsp[(3) - (8)].node)),NULL,reduce_ExprListNode((yyvsp[(7) - (8)].node)));;}
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 1159 "clic.y"
    { (yyval.node)=new2_FieldNode(NULL,reduce_ExprListNode((yyvsp[(3) - (6)].node)),NULL,(yyvsp[(6) - (6)].node)); ;}
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 1160 "clic.y"
    { (yyval.node)=new2_FieldNode(NULL,reduce_ExprListNode((yyvsp[(3) - (6)].node)),(yyvsp[(6) - (6)].string),NULL); ;}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 1162 "clic.y"
    {(yyval.node)=new2_FieldNode(NULL,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].string),NULL);;}
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 1164 "clic.y"
    {(yyval.node)=new2_FieldNode(NULL,(yyvsp[(2) - (4)].node),(yyvsp[(4) - (4)].string),NULL);;}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 1165 "clic.y"
    {(yyval.node)=new2_FieldNode(NULL,(yyvsp[(1) - (5)].node),NULL,reduce_ExprListNode((yyvsp[(4) - (5)].node)));;}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 1166 "clic.y"
    {(yyval.node)=new2_FieldNode(NULL,(yyvsp[(2) - (6)].node),NULL,reduce_ExprListNode((yyvsp[(5) - (6)].node)));;}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 1167 "clic.y"
    {(yyval.node)=new2_FieldNode(NULL,(yyvsp[(1) - (3)].node),NULL,(yyvsp[(3) - (3)].node));;}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 1168 "clic.y"
    {(yyval.node)=new2_FieldNode(NULL,(yyvsp[(2) - (4)].node),NULL,(yyvsp[(4) - (4)].node));;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 1170 "clic.y"
    { CM;(yyval.node)=(assop_node((yyvsp[(4) - (5)].i),(yyvsp[(1) - (5)].string),NULL,(yyvsp[(3) - (5)].string),0,(yyvsp[(5) - (5)].node))); ;}
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 1171 "clic.y"
    { CM; (yyval.node)=(assop_node((yyvsp[(5) - (6)].i), (yyvsp[(2) - (6)].string),NULL,(yyvsp[(4) - (6)].string),0,(yyvsp[(6) - (6)].node))); ;}
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 1172 "clic.y"
    { CM;(yyval.node)=(assop_node((yyvsp[(5) - (6)].i), (yyvsp[(2) - (6)].string),NULL,0,(yyvsp[(4) - (6)].node),(yyvsp[(6) - (6)].node))); ;}
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 1173 "clic.y"
    {CM;(yyval.node)=(assop_node((yyvsp[(3) - (4)].i),NULL,NULL,(yyvsp[(2) - (4)].string),0,(yyvsp[(4) - (4)].node)));;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 1174 "clic.y"
    { CM;(yyval.node)=(assop_node((yyvsp[(4) - (5)].i),NULL,(yyvsp[(1) - (5)].node),(yyvsp[(3) - (5)].string),0,(yyvsp[(5) - (5)].node))); ;}
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 1176 "clic.y"
    { CM;(yyval.node)=(assop_node((yyvsp[(6) - (7)].i),NULL,reduce_ExprListNode((yyvsp[(2) - (7)].node)),(yyvsp[(5) - (7)].string),0,(yyvsp[(7) - (7)].node))); ;}
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 1177 "clic.y"
    { CM;(yyval.node)=(assop_node((yyvsp[(6) - (7)].i),NULL,reduce_ExprListNode((yyvsp[(2) - (7)].node)),0, (yyvsp[(5) - (7)].node),(yyvsp[(7) - (7)].node))); ;}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 1178 "clic.y"
    { CM;(yyval.node)=(assop_node((yyvsp[(8) - (9)].i),NULL,reduce_ExprListNode((yyvsp[(2) - (9)].node)),0,reduce_ExprListNode((yyvsp[(6) - (9)].node)),(yyvsp[(9) - (9)].node))); ;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 1179 "clic.y"
    { CM;(yyval.node)=(assop_node((yyvsp[(7) - (8)].i),NULL,reduce_ExprListNode((yyvsp[(3) - (8)].node)),(yyvsp[(6) - (8)].string),0,(yyvsp[(8) - (8)].node))); ;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 1180 "clic.y"
    { CM;(yyval.node)=(assop_node((yyvsp[(7) - (8)].i),NULL,reduce_ExprListNode((yyvsp[(3) - (8)].node)),0, (yyvsp[(6) - (8)].node),(yyvsp[(8) - (8)].node))); ;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 1181 "clic.y"
    { CM;(yyval.node)=(assop_node((yyvsp[(9) - (10)].i),NULL,reduce_ExprListNode((yyvsp[(3) - (10)].node)),0,reduce_ExprListNode((yyvsp[(7) - (10)].node)),(yyvsp[(10) - (10)].node))); ;}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 1183 "clic.y"
    { CM;(yyval.node)=(assop_node((yyvsp[(5) - (6)].i),NULL,(yyvsp[(2) - (6)].node),(yyvsp[(4) - (6)].string),0,(yyvsp[(6) - (6)].node))); ;}
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 1184 "clic.y"
    { CM;(yyval.node)=(assop_node((yyvsp[(4) - (5)].i),(yyvsp[(1) - (5)].string),NULL,0,(yyvsp[(3) - (5)].node),(yyvsp[(5) - (5)].node))); ;}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 1185 "clic.y"
    {CM;(yyval.node)=(assop_node((yyvsp[(3) - (4)].i),NULL,NULL,0,(yyvsp[(2) - (4)].node),(yyvsp[(4) - (4)].node)));;}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 1186 "clic.y"
    { CM;(yyval.node)=(assop_node((yyvsp[(4) - (5)].i),NULL,(yyvsp[(1) - (5)].node),0,(yyvsp[(3) - (5)].node),(yyvsp[(5) - (5)].node))); ;}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 1187 "clic.y"
    { CM;(yyval.node)=(assop_node((yyvsp[(5) - (6)].i),NULL,(yyvsp[(2) - (6)].node),0,(yyvsp[(4) - (6)].node),(yyvsp[(6) - (6)].node))); ;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 1189 "clic.y"
    { (yyval.node)=new_OpNode((yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node),'e');  ;}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 1190 "clic.y"
    { (yyval.node)=new_OpNode((yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node),'E');  ;}
    break;

  case 197:

/* Line 1455 of yacc.c  */
#line 1191 "clic.y"
    { (yyval.node)=new_OpNode((yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node),'N');  ;}
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 1192 "clic.y"
    { (yyval.node)=new_OpNode((yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node),'<');  ;}
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 1193 "clic.y"
    { (yyval.node)=new_OpNode((yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node),'>');  ;}
    break;

  case 200:

/* Line 1455 of yacc.c  */
#line 1194 "clic.y"
    { (yyval.node)=new_OpNode((yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node),'L');  ;}
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 1195 "clic.y"
    { (yyval.node)=new_OpNode((yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node),'G');  ;}
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 1196 "clic.y"
    { (yyval.node)=new_OpNode((yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node),'+');  ;}
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 1197 "clic.y"
    { (yyval.node)=new_OpNode((yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node),'$');  ;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 1199 "clic.y"
    { (yyval.node)=new_OpNode((yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node),'-');  ;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 1200 "clic.y"
    { (yyval.node)=new_OpNode((yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node),'*');  ;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 1201 "clic.y"
    { (yyval.node)=new_OpNode((yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node),'/');  ;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 1202 "clic.y"
    { (yyval.node)=new_OpNode((yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node),'%');  ;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 1203 "clic.y"
    { (yyval.node)=new_OpNode((yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node),'^');  ;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 1204 "clic.y"
    { (yyval.node)=new_AndNode((yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));  ;}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 1205 "clic.y"
    { (yyval.node)=new_OrNode((yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));  ;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 1207 "clic.y"
    { (yyval.node)=new_NotNode((yyvsp[(2) - (2)].node));  ;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 1209 "clic.y"
    { (yyval.node)=new_IncrNode((yyvsp[(2) - (2)].node),1,0); ;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 1210 "clic.y"
    { (yyval.node)=new_IncrNode((yyvsp[(1) - (2)].node),1,1); ;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 1211 "clic.y"
    { (yyval.node)=new_IncrNode((yyvsp[(2) - (2)].node),0,0); ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1212 "clic.y"
    { (yyval.node)=new_IncrNode((yyvsp[(1) - (2)].node),0,1); ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 1213 "clic.y"
    { (yyval.node)=new_MinusNode((yyvsp[(2) - (2)].node)); ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1214 "clic.y"
    { (yyval.node)=(yyvsp[(2) - (2)].node); ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1216 "clic.y"
    {(yyval.node)=new_ArrayInitNode((yyvsp[(2) - (3)].node)); ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1217 "clic.y"
    {(yyval.node)=new_ArrayInitNode((yyvsp[(2) - (3)].node)); ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1218 "clic.y"
    {
			Node *np=(yyvsp[(4) - (6)].node);
			Node *el=(yyvsp[(5) - (6)].node);
			CodeNode *npp = (CodeNode*)np;
			int haveMacro = 0;
			fin_Function(curFunction);

			append_Node(curFunction->body,new_ReturnNode(el,1));

			npp->func->body->pass(npp->func->body, SearchMacro, 0, &haveMacro);

			if (haveMacro)
			{
				/* transform into CODEBLOCK(exprlist)" */
				remove_Coll(&curFile->codeblocks, npp->func);
				np = new_MacroNode(new_CodestrNode(el,1,curFunction->params),0);
			}
			curFunction = (yyvsp[(3) - (6)].func);

			(yyval.node)=np;
		;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1239 "clic.y"
    { (yyval.node)=new_CGetNode((yyvsp[(3) - (4)].coll)); ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1240 "clic.y"
    { (yyval.node)=new_PcountNode(); ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1241 "clic.y"
    { (yyval.node)=new_PshiftNode(); ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1242 "clic.y"
    { (yyval.node)=new_ParnNode((yyvsp[(2) - (3)].node)); ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1243 "clic.y"
    { (yyval.node)=new_ParnNode((yyvsp[(2) - (3)].node)); ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1245 "clic.y"
    {  curFunction->code_params=(yyvsp[(2) - (2)].varcoll); ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 1246 "clic.y"
    { (yyval.node)=new_CodestrNode((yyvsp[(4) - (5)].node),0,(yyvsp[(2) - (5)].varcoll)); curFunction->code_params=0; ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1247 "clic.y"
    {  curFunction->code_params=(yyvsp[(2) - (2)].varcoll); ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 1248 "clic.y"
    { (yyval.node)=new_MacroNode(new_CodestrNode((yyvsp[(4) - (5)].node),1,(yyvsp[(2) - (5)].varcoll)),0); curFunction->code_params=0; ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 1251 "clic.y"
    {
			if (curFunction->upfunc)
				curFunction = curFunction->upfunc;
		;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 1256 "clic.y"
    {
			Node *np;
			curFunction = (yyvsp[(3) - (6)].func);
			np = new_LocalDefNode((yyvsp[(5) - (6)].varcoll), 0, 1);
			prepend_Node(curFunction->body, np);
			(yyval.node) = new_NilConstNode();
			CM;
		;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 1266 "clic.y"
    {
			Coll *cp=new_Coll(NULL,NULL);
			append_Coll(cp,(yyvsp[(3) - (8)].node));
			append_Coll(cp,(yyvsp[(5) - (8)].node));
			(yyval.node)=new_IfNode(cp,(yyvsp[(7) - (8)].node));
		;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1275 "clic.y"
    {  (yyval.node)=new_BreakNode((yyvsp[(2) - (3)].node),0); ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1279 "clic.y"
    { (yyval.varcoll)=0; ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1280 "clic.y"
    { (yyval.varcoll)=(yyvsp[(2) - (3)].varcoll); ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1283 "clic.y"
    { (yyval.node)=new_NilConstNode(); ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1284 "clic.y"
    { (yyval.node)=(yyvsp[(1) - (1)].node); ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1301 "clic.y"
    { (yyval.i)='='; ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1302 "clic.y"
    { (yyval.i)='+'; ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1303 "clic.y"
    { (yyval.i)='-'; ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1304 "clic.y"
    { (yyval.i)='*'; ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1305 "clic.y"
    { (yyval.i)='/'; ;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1306 "clic.y"
    { (yyval.i)='%'; ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1307 "clic.y"
    { (yyval.i)='^'; ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1310 "clic.y"
    { (yyval.i)='='; ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 1314 "clic.y"
    { (yyval.func)=curFunction; ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1317 "clic.y"
    { (yyval.node)=new_CodeNode((yyvsp[(1) - (2)].varcoll)); ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1320 "clic.y"
    { (yyval.node)=(yyvsp[(3) - (4)].node); prepend_Node((yyvsp[(3) - (4)].node),(yyvsp[(1) - (4)].node)); ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1321 "clic.y"
    { (yyval.node)=(yyvsp[(1) - (4)].node); join_Node((yyvsp[(1) - (4)].node),(yyvsp[(3) - (4)].node)); ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1322 "clic.y"
    {
		(yyval.node)=new_ExprListNode();
		append_Node((yyval.node), (yyvsp[(1) - (3)].node));
		upstr((yyvsp[(3) - (3)].string));
		append_Node((yyval.node), new_HashConstNode((yyvsp[(3) - (3)].string)));
		;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1328 "clic.y"
    {
		(yyval.node)=new_ExprListNode();
		add_self();
		append_Node((yyval.node), installName(strdup("__SELF__")));
		upstr((yyvsp[(2) - (2)].string));
		append_Node((yyval.node), new_HashConstNode((yyvsp[(2) - (2)].string)));
		;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 1335 "clic.y"
    {
		(yyval.node)=(yyvsp[(1) - (3)].node);
		upstr((yyvsp[(3) - (3)].string));
		append_Node((yyval.node),new_HashConstNode((yyvsp[(3) - (3)].string)));
	;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1342 "clic.y"
    {
			(yyval.varcoll)=new_VarColl();
			(yyvsp[(1) - (2)].var)->type = (yyvsp[(2) - (2)].string);
			add_VarColl((yyval.varcoll), (yyvsp[(1) - (2)].var));
			if (def_node)
				append_Node( curr_operlist, def_node((yyval.varcoll)));

		;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1351 "clic.y"
    {
			if (def_node)
				(yyval.varcoll)=new_VarColl();
			else
				(yyval.varcoll)=(yyvsp[(1) - (4)].varcoll);
			(yyvsp[(3) - (4)].var)->type = (yyvsp[(4) - (4)].string);
			add_VarColl((yyval.varcoll), (yyvsp[(3) - (4)].var));
			if (def_node)
				append_Node( curr_operlist, def_node((yyval.varcoll)));
		;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1363 "clic.y"
    {
			(yyval.varcoll)=new_VarColl();
			add_VarColl((yyval.varcoll), (yyvsp[(1) - (1)].var));
		;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 1368 "clic.y"
    {
			(yyval.varcoll)=(yyvsp[(1) - (3)].varcoll);
			add_VarColl((yyval.varcoll), (yyvsp[(3) - (3)].var));
		;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1374 "clic.y"
    { (yyval.var)=newArrInit_Var((yyvsp[(1) - (2)].string),(yyvsp[(2) - (2)].node)); ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 1375 "clic.y"
    { (yyval.var)=mnewArrInit_Var((yyvsp[(1) - (2)].node),(yyvsp[(1) - (2)].node)); ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 1378 "clic.y"
    { (yyval.node)=(yyvsp[(2) - (3)].node); ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 1379 "clic.y"
    { (yyval.node)=(yyvsp[(1) - (4)].node); join_Node((yyvsp[(1) - (4)].node),(yyvsp[(3) - (4)].node)); ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1382 "clic.y"
    { (yyval.var)=new_Var((yyvsp[(1) - (1)].string)); ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1383 "clic.y"
    { (yyval.var)=newArrInit_Var((yyvsp[(1) - (2)].string),(yyvsp[(2) - (2)].node)); ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1384 "clic.y"
    { (yyval.var)=newInit_Var((yyvsp[(1) - (3)].string),(yyvsp[(3) - (3)].node)); ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1385 "clic.y"
    { yyerror("only inline assign := allowed in initialisation"); (yyval.var)=new_Var(strdup("")); ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1387 "clic.y"
    { (yyval.var)=mnew_Var((yyvsp[(1) - (1)].node)); ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 1388 "clic.y"
    { (yyval.var)=mnewArrInit_Var((yyvsp[(1) - (2)].node),(yyvsp[(1) - (2)].node)); ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 1389 "clic.y"
    { (yyval.var)=mnewInit_Var((yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));  ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1390 "clic.y"
    { yyerror("only inline assign := allowed in initialisation"); (yyval.var)=new_Var(strdup("")); ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 1393 "clic.y"
    { (yyval.node)=(yyvsp[(2) - (3)].node); ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1394 "clic.y"
    { (yyval.node)=(yyvsp[(1) - (4)].node); join_Node((yyvsp[(1) - (4)].node),(yyvsp[(3) - (4)].node)); ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1397 "clic.y"
    { (yyval.varcoll)=new_VarColl(); ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 1398 "clic.y"
    {
		Var *vp=new_Var((yyvsp[(1) - (2)].string));
		vp->type = (yyvsp[(2) - (2)].string);
		(yyval.varcoll)=new_VarColl();
		vp->no=0;
		insert_Coll( &(yyval.varcoll)->coll, vp);
		insert_Coll( &(yyval.varcoll)->unsorted, vp);
		;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1406 "clic.y"
    {
		Var *vp=new_Var((yyvsp[(3) - (4)].string));
		vp->type = (yyvsp[(4) - (4)].string);
		(yyval.varcoll)=(yyvsp[(1) - (4)].varcoll);
		vp->no = (yyval.varcoll)->unsorted.count;
		insert_Coll( &(yyval.varcoll)->coll, vp);
		insert_Coll( &(yyval.varcoll)->unsorted, vp);
		;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 1417 "clic.y"
    {(yyval.coll)=new_Coll(free,strcmp);;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1418 "clic.y"
    {
			(yyval.coll)=new_Coll(free,strcmp);
			insert_Coll((yyval.coll), (yyvsp[(1) - (1)].string));
		;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 1423 "clic.y"
    {
			int no;
			(yyval.coll)=(yyvsp[(1) - (3)].coll);
			if ( search_Coll( (yyval.coll), (yyvsp[(3) - (3)].string), &no) )
			{
				yyerror("name '%s' duplication", (yyvsp[(3) - (3)].string));
				free((yyvsp[(3) - (3)].string));
			}
			else
				insert_Coll( (yyval.coll), (yyvsp[(3) - (3)].string) );
		;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 1436 "clic.y"
    {
			(yyval.varcoll)=new_VarColl();
			add_VarColl((yyval.varcoll), new_Var((yyvsp[(1) - (1)].string)));
		;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 1441 "clic.y"
    {
			(yyval.varcoll)=(yyvsp[(1) - (3)].varcoll);
			add_VarColl((yyval.varcoll), new_Var((yyvsp[(3) - (3)].string)));
		;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 1447 "clic.y"
    { (yyval.arglist).coll=(yyvsp[(1) - (1)].coll); (yyval.arglist).haveRest=0; ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 1448 "clic.y"
    { (yyval.arglist).coll=(yyvsp[(1) - (3)].coll); (yyval.arglist).haveRest=1; ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 1449 "clic.y"
    { (yyval.arglist).coll=new_Coll(NULL,NULL); (yyval.arglist).haveRest=1;  ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 1452 "clic.y"
    { (yyval.arglist).coll=new_Coll(NULL,NULL); (yyval.arglist).haveRest=0; ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 1453 "clic.y"
    { (yyval.arglist).coll=(yyvsp[(2) - (2)].coll); (yyval.arglist).haveRest=0; ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 1454 "clic.y"
    { (yyval.arglist).coll=(yyvsp[(2) - (4)].coll); (yyval.arglist).haveRest=1; ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 1455 "clic.y"
    { (yyval.arglist).coll=new_Coll(NULL,NULL); (yyval.arglist).haveRest=1;  ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 1476 "clic.y"
    {
			(yyval.coll)=new_Coll(NULL,NULL);
			if ((yyvsp[(1) - (1)].node)) {
				Coll *cp;
				Node *np;

				insert_Coll((yyval.coll),(yyvsp[(1) - (1)].node));
				cp = (yyval.coll);
				np = (Node*) cp->items[0];
				if(!(np->isRef||np->isConst||(!np->isLval && !np->isField)))
					cp->items[0] = new_RefNode(np);
			}
		;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 1489 "clic.y"
    {
			(yyval.coll)=new_Coll(NULL,NULL);
			if ((yyvsp[(2) - (3)].node))
				insert_Coll((yyval.coll),(yyvsp[(2) - (3)].node));
	;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 1494 "clic.y"
    {
			(yyval.coll)=(yyvsp[(1) - (3)].coll);
			if (!(yyval.coll)->count)
				insert_Coll((yyval.coll),new_ArgNode(new_NilConstNode(),0));
			if ((yyvsp[(3) - (3)].node)) {
				Coll *cp;
				Node *np;
				insert_Coll((yyval.coll),(yyvsp[(3) - (3)].node));
				cp = (yyval.coll);
				np = (Node*) cp->items[cp->count-1];
				if(!(np->isRef||np->isConst||(!np->isLval && !np->isField)))
					cp->items[cp->count-1] = new_RefNode(np);
			}
			else
				insert_Coll((yyval.coll),new_ArgNode(new_NilConstNode(),0));
		;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 1510 "clic.y"
    {
			(yyval.coll)=(yyvsp[(1) - (5)].coll);
			if (!(yyval.coll)->count)
				insert_Coll((yyval.coll),new_ArgNode(new_NilConstNode(),0));
			if ((yyvsp[(4) - (5)].node))
				insert_Coll((yyval.coll),(yyvsp[(4) - (5)].node));
			else
				insert_Coll((yyval.coll),new_ArgNode(new_NilConstNode(),0));
		;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 1521 "clic.y"
    {
			(yyval.coll)=new_Coll(NULL,NULL);
			if ((yyvsp[(1) - (1)].node))
				insert_Coll((yyval.coll),(yyvsp[(1) - (1)].node));
		;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 1526 "clic.y"
    {
			(yyval.coll)=(yyvsp[(1) - (3)].coll);
			if (!(yyval.coll)->count)
				insert_Coll((yyval.coll),new_ArgNode(new_NilConstNode(),0));
			if ((yyvsp[(3) - (3)].node))
				insert_Coll((yyval.coll),(yyvsp[(3) - (3)].node));
			else
				insert_Coll((yyval.coll),new_ArgNode(new_NilConstNode(),0));
		;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 1537 "clic.y"
    { (yyval.node)=NULL; ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 1538 "clic.y"
    { (yyval.node)=(yyvsp[(1) - (1)].node); /*$$=new_ArgNode($1,0);*/ ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 1543 "clic.y"
    {;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 1544 "clic.y"
    {;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 1547 "clic.y"
    {;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 1548 "clic.y"
    {;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 1552 "clic.y"
    {
					(yyval.node)=new_ExprListNode();
					(yyval.node)->isConst=1;
					if ((yyvsp[(1) - (1)].node))
					{
						append_Node((yyval.node), (yyvsp[(1) - (1)].node));
						if (!(yyvsp[(1) - (1)].node)->isConst)
							(yyval.node)->isConst=0;
						else
							(yyval.node)->isConst=1;
					}
				;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 1564 "clic.y"
    {
					(yyval.node)=(yyvsp[(1) - (3)].node);
					if (getCount_Node((yyval.node)) == 0)
						append_Node((yyval.node),new_NilConstNode());
					if ((yyvsp[(3) - (3)].node))
					{
						append_Node((yyval.node), (yyvsp[(3) - (3)].node));
						if (!(yyvsp[(3) - (3)].node)->isConst)
							(yyval.node)->isConst=0;
					}
					else
						append_Node((yyval.node),new_NilConstNode());
				;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 1579 "clic.y"
    { (yyval.node)=NULL; ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 1583 "clic.y"
    { (yyval.node)=new_ExprListNode(); append_Node((yyval.node), (yyvsp[(1) - (1)].node)); ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 1584 "clic.y"
    { (yyval.node)=(yyvsp[(1) - (3)].node); append_Node((yyval.node), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 1587 "clic.y"
    { (yyval.node)=new_StringConstNode((yyvsp[(1) - (1)].string)); ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 1588 "clic.y"
    { (yyval.node)=new_LogicConstNode(1); ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 1589 "clic.y"
    { (yyval.node)=new_LogicConstNode(0); ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 1590 "clic.y"
    { (yyval.node)=new_NilConstNode(); ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 1591 "clic.y"
    { (yyval.node)=new_NumberConstNode((yyvsp[(1) - (1)].string),0); ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 1595 "clic.y"
    { (yyval.node)=(yyvsp[(2) - (2)].node); (yyval.node)->isMinus = 1; ;}
    break;



/* Line 1455 of yacc.c  */
#line 5495 "clic.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 1598 "clic.y"


/* ][ */

int clic_errorcount=0, clic_warncount;

int
clic_parse(const char *filename, FILE *stream)
{
  int ret=0;
  FILE *file;

  clic_errorcount=0;

  clic_warncount=0;

  curFile=new_File(strdup(filename));
  {
	char path[256], ch;
	const char *s;
	char *e;

#if 0
	*s=strrchr(filename, '/'); */
	if (s)
		s++;
	else
		s=filename;
#else
		s=filename;
#endif
	/*snprintf(path, sizeof(path), "_main_%s", s);*/
	snprintf(path, sizeof(path), "%s", s);
	e=strrchr(path, '.');
	if (e)
		*e=0;
	for (e = path; *e; ++e)
	{
		ch = *e;
		switch(ch)
		{
			case '.':
			case '/':
			case '\\':
			case '-':
			case '+':
				*e = '_';
		}
		*e = toupper(*e);
	}
	curFile->mname = strdup(path);
	if (nomain_flag)
		strncat(path,"_m",sizeof(path));

	curFunction=curFile->main=mainFunction=new_Function(strdup(path),new_VarColl(),nomain_flag?0:1,1,0,0);
	fin_Function(curFunction);
	fileStatics=curStatics=mainFunction->statics;
	push_operlist(curFunction->body);
	if (nomain_flag)
		mainFunction = 0;
	add_Function(curFile, curFunction);
  }

  if (stream)
	file=stream;
  else
	file=fopen(filename, "rt");

  if (file)
   {
	if (!lex_new_file(file, filename))
	{
	if (preproc_flag)
	{
		while( (ret=yylex())>0 )
			;
	}
	else
		ret=yyparse();
	}
	else
	{
		fclose(file);
		return -1;
	}
   }
  else
   {
	 yyerror("cannot open file '%s'", filename);
	 ret=-1;
   }

  if (ret==0)
   { /* test for all function definitions */
   }

  if (!stream && file)
	fclose(file);
  return ret;
}

static void
check_main()
{
	if (!mainFunction && curFunction==curFile->main)
		yyerror("executable operator outside of function (-n flag is active)");
}

static void
print_file()
{
	int index=-1, line, ret;
	char *filename;
	char *workingdirectory ;
	workingdirectory = getenv("PWD");

	do {
		ret = get_include (&index, &line, &filename);
		if (index>=0)
			fprintf(stderr, _clic_gettext("in file '%s',\n\tincluded at line %d "), filename, line);/*angelo*/
		else
			fprintf(stderr, _clic_gettext("in file '%s'\n"), filename);
	} while (ret!=0);
	fprintf(stderr, _clic_gettext("  working directory : '%s'\n"), workingdirectory);   /*angelo*/
}

int
yyerror(const char *s, ... )
{
	if (!clic_warncount && !clic_errorcount)
		fprintf(stderr, "\n");
	clic_errorcount++;
	if (s)
	{
		va_list ap;
		va_start(ap, s);
		fprintf(stderr, _clic_gettext("error %d: "), clic_errorcount);/*angelo*/
		vfprintf(stderr, _clic_gettext(s), ap);
		va_end(ap);

		fprintf(stderr,_clic_gettext("\n\tline %ld pos %ld (yychar=%d, '%c') "), cl_line+corr_line, cl_pos, yychar, (yychar>32&&yychar<256)?yychar:' ');/*angelo*/
	}
	else
		fprintf(stderr, _clic_gettext("error %d, (yychar=%d) "), clic_errorcount, yychar);/*angelo*/

	print_file();
	fflush(stdout);
	return 0;
}

int
yywarning(const char *s, ... )
{
	if (!clic_warncount && !clic_errorcount)
		fprintf(stderr, "\n");
	clic_warncount++;
	if (s)
	{
		va_list ap;
		va_start(ap, s);
		fprintf(stderr, _clic_gettext("warning %d: "), clic_warncount);/*angelo*/
		vfprintf(stderr, _clic_gettext(s), ap);
		va_end(ap);

		fprintf(stderr, _clic_gettext("\n\tline %ld, pos %ld, "), cl_line+corr_line, cl_pos);/*angelo*/
	}
	else
		fprintf(stderr, _clic_gettext("warning %d, "), clic_warncount);/*angelo*/
	print_file();
	fflush(stdout);
	return 0;
}

void
eof_checks(void)
{
	if (curFunction->inLoop>0)
	{
		yyerror("unclosed loop statement (level %d, begin at line %d file %s)",
			curFunction->inLoop, loop_line, fileName(loop_file) );
		/*curFunction->inLoop = 0;*/
	}
	if (if_depth)
	{
		yyerror("unclosed IF statement (level %d, begin at line %d file %s)",
			if_depth, if_line, fileName(if_file) );
		if_depth = 0;
	}
	if (curFunction->seqLevel>0)
	{
		yyerror("unclosed SEQUENCE statement (level %d, begin at line %d file %s)",
			curFunction->seqLevel, seq_line, fileName(seq_file) );
		/*curFunction->seqLevel = 0;*/
	}
}

static void upstr(char *str)
{
	char *s;
	for(s=str; *s; ++s)
		*s=toupper(*s);
}

Node *curr_operlist;
static Coll *operlist_stack=0;

static void
init_operlist_stack(void)
{
	if (!operlist_stack)
		operlist_stack = new_Coll(0,0);
}

static void
push_operlist(Node *operlist)
{
	init_operlist_stack();
	append_Coll(operlist_stack, operlist);
	curr_operlist = operlist;
}

static void
pop_operlist(void)
{
/*	init_operlist_stack();*/
	if (operlist_stack->count>0)
	{
		atRemove_Coll (operlist_stack, operlist_stack->count-1);
		curr_operlist = (Node*) operlist_stack->items[operlist_stack->count-1];
	}
}

static Node *
reduce_ExprListNode(Node *np)
{
	if (strcmp(np->name, "exprlist"))
		return np;
	if (getCount_Node(np) == 1)
	{
		Node *p = (Node*) np->list.head;
		return p;
	}
	else
	{
		((ExprListNode*)np)->last = 1;
		return np;
	}
}

static Node *
assop_node( int assop, char *area, Node *areaExpr, char*name, Node *nameExpr, Node *expr)
{
	switch(assop)
	{
	case '=':
	default:
		return new_AssignFieldNode(area, areaExpr, name, nameExpr, expr);
	case '+':
	case '-':
	case '*':
	case '/':
	case '^':
	case '%':
		return new_AssignFieldNode(area, areaExpr, name, nameExpr,
			new_OpNode(new2_FieldNode(area, areaExpr, name, nameExpr), expr, assop));
	}
}


