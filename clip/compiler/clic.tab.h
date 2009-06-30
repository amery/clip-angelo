
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
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

/* Line 1676 of yacc.c  */
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



/* Line 1676 of yacc.c  */
#line 176 "clic.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


