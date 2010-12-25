/*
	Copyright (C) 2001  ITK
	Author   : Paul Lasarev <paul@itk.ru>
	License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI
*/
/* [ */
%{
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <float.h>
#include <math.h>
#include <stdarg.h>

#include "ci_clic.h"
#include "ci_node.h"
#include "ci_nodepriv.h"

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

%}

/* ][ grammar section */


%union
{
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
}

/* ISTRING is in18n string - as [ str] */

%token <string> STRING NUMBER ISTRING
%token <string>	NAME CTEXT
%token <string>	DATE
%token <i>	KEYS

%token PREPROC

%token RETURN FUNCTION PROCEDURE IPROCEDURE EPROCEDURE EXTERN PARAMETERS
%token SFUNCTION SPROCEDURE TEXT
%token PUBLIC PRIVATE STATIC LOCAL DIMENSION
%token NIL END TRUE FALSE
%token WHILE DO ENDDO EXIT LOOP WITH CALL
%token IF IIF ENDIF ELSE ELSEIF
%token DOCASE CASE OTHERWISE ENDCASE SWITCH ENDSWITCH
%token FIELD MEMVAR IN MEMVARPTR FIELDPTR
%token PCOUNT PARBEG PARAMBEG PSHIFT PALL CGET
%token FOR STEP TO NEXT FOREACH
%token BEGSEQ RECOVER USING BREAK EBREAK
%token ANNOUNCE REQUEST
%token IFNL IFEXPR CODESTR CODEBLOCK
%token AS REF

%type <i>	function procedure assign curseq keys ass_op
/*
%type <i>	assop eassop
*/
%type <node>	oper operlist name mname else step using otherwise iname
%type <node>	constant expr nexpr nilexpr exprlist arg expr_list code mconstant
%type <coll>	namelist arg_list with_arg_list elseif caselist casebeg
%type <arglist> arglist with_arglist
%type <var>	var dim
%type <varcoll> paramlist param_list vardef varlist bparam_list dimdef
%type <string>	inalias
%type <func>	func
%type <recover> recover
%type <node>	arr arr_list dim_list
%type <node>	pairlist pair
%type <coll>	switchlist switchlabel switchbeg
%type <string>	type_def

%right ASSIGN ADDA SUBA DIVA MODA MULA POWA
%right POSTINCR
%left '@'
%left OR
%left AND
%right NOT
%left '>' GE '<' LE EQ NE '=' '$'
%left '+' '-'
%left '*' '/' '%'
%left POW
%right MINUS
%right INCR DECR
%left ':' '[' '.'
%left RPTR REPTR
%right DSEMICOLON


%%


file:		{ }
/*
	| file function NAME paramlist type_def '\n'
		{
			curFunction=new_Function($3,$4,$2,0,0,0);
			curFunction->typename = $5;
			lex_vardef(0);
			add_Function(curFile, curFunction);
			curStatics=curFunction->statics;
			fin_Function(curFunction);
		}

	| file procedure NAME paramlist type_def '\n'
		{
			curFunction=new_Function($3,$4,$2,0,0,0);
			curFunction->isProc=1;
			curFunction->typename = $5;
			lex_vardef(0);
			add_Function(curFile, curFunction);
			curStatics=curFunction->statics;
			fin_Function(curFunction);
		}
*/
	| file oper '\n'
		{
			if ($2)
			{
				if ( $2->isExec )
					curFunction->lastExec_of_Function=1;
				append_Node( curFunction->Node_body_of_Function, $2);
			}
		}
	| file CTEXT
		{
			if (curFunction==curFile->Function_main_of_File)
				append_Node(curFile->Node_ctext_of_File, new_LangNode($2));
			else
				append_Node(curFunction->Node_body_of_Function, new_LangNode($2));
		}
	| file STATIC CTEXT
		{
			append_Node(curFile->Node_ctextStatic_of_File, new_LangNode($3));
		};

function:	FUNCTION	{$$=1; lex_vardef(1);}
	| STATIC FUNCTION	{$$=0; lex_vardef(1);}
		;

procedure:	PROCEDURE	{$$=1;lex_vardef(1);}
	| STATIC PROCEDURE	{$$=0;lex_vardef(1);}
	| IPROCEDURE	{$$=2;lex_vardef(1);}
	| EPROCEDURE	{$$=3;lex_vardef(1);}
		;

paramlist:                  {$$=new_VarColl();}
	| '('  param_list ')' { $$=$2;}
		;

operlist:	{ $$=new_OperListNode(); push_operlist($$); }
	| operlist oper '\n'
		{
			$$=$1;
			if ($2)
			{
				if ( $2->isExec )
					curFunction->lastExec_of_Function=1;
				append_Node( $$, $2);
			}
		}
		;

oper:                	{ $$=NULL; }
	| function NAME paramlist type_def
		{
			eof_checks();
			curFunction=new_Function($2,$3,$1,0,0,0);
			curFunction->typename_of_Function = $4;
			lex_vardef(0);
			add_Function(curFile, curFunction);
			curStatics=curFunction->VarColl_statics_of_Function;
			fin_Function(curFunction);
			pop_operlist();
			push_operlist(curFunction->Node_body_of_Function);
			$$=NULL;
		}

	| procedure NAME paramlist type_def
		{
			eof_checks();
			curFunction=new_Function($2,$3,$1,0,0,0);
			curFunction->isProc_of_Function=1;
			curFunction->typename_of_Function = $4;
			lex_vardef(0);
			add_Function(curFile, curFunction);
			curStatics=curFunction->VarColl_statics_of_Function;
			fin_Function(curFunction);
			pop_operlist();
			push_operlist(curFunction->Node_body_of_Function);
			$$=NULL;
		}
	| EXTERN namelist	{
			int i;
			char *s;
			for(i=0;i<$2->count_of_Coll;i++)
			{
				VAR(char, vp, $2->items_of_Coll[i]);
				if (insert_Coll( &curFile->Coll_externFunctions_of_File, vp)<0)
					free(vp);
				else
				{
					for(s=vp; *s; ++s)
						*s=toupper(*s);
					insert_Coll( &curFile->Coll_undeclExternFunctions_of_File, strdup(vp));
				}
			}
			removeAll_Coll($2);
			delete_Coll($2);
			$$=NULL;
			if (curFunction->lastExec_of_Function)
				yyerror("EXTERN after executable statements");
		}
	| LOCAL {
			lex_vardef(1);
			def_node=new_LocalDefNode1;
		}
	 vardef
		{
			/*$$=new_LocalDefNode($3, 1);*/
			$$=NULL;
			lex_vardef(0); CM;
			def_node=0;
		}
	| STATIC
		{
			lex_vardef(1);
			def_node = new_StaticDefNode;
		}
	vardef
		{
			/*$$=new_StaticDefNode($3);*/
			$$=NULL;
			lex_vardef(0);
			def_node=0;
		}
	| PUBLIC
		{
			def_node = new_PublicDefNode;
		}
	vardef
		{
			/*$$=new_PublicDefNode($2);*/
			$$=NULL;
			CM;
			def_node=0;
		}
	| PRIVATE
		{
			def_node = new_PrivateDefNode;
		}
	 vardef
		{
			/*$$=new_PrivateDefNode($2);*/
			$$=NULL;
			CM;
			def_node=0;
		}
	| DIMENSION
		{
			def_node = new_PrivateDefNode;
		}
	 dimdef
		{
			/*$$=new_PrivateDefNode($2);*/
			$$=NULL;
			CM;
			def_node=0;
		}
/*
	| NAME varlist	{
				if (!strcasecmp($1, "field"))
					$$=new_FieldDefNode($2, 0);
				else if (!strcasecmp($1, "memvar"))
					$$=new_MemvarDefNode($2);
				else
					$$=new_NamespaceDefNode($1,$2);
				CM;
			}
*/
	| MEMVAR varlist	{ $$=new_MemvarDefNode($2); }
	| FIELD varlist inalias { $$=new_FieldDefNode($2, $3); }
	| PARAMETERS varlist	{ $$=new_ParametersDefNode($2); CM;}
	| RETURN		{ $$=new_ReturnNode(NULL,0); CM;}
	| RETURN  expr		{ $$=new_ReturnNode($2,0); CM;}

	| REQUEST namelist
			{
				Coll *cp = $2;
				int i;

				$$=NULL;
				for(i=0; i< cp->count_of_Coll; ++i)
					append_Coll(&curFile->Coll_externModules_of_File, cp->items_of_Coll[i]);

				removeAll_Coll(cp);
				delete_Coll(cp);
			}
	| ANNOUNCE NAME	{
				char *s = $2;

				$$=NULL;
				free(curFile->Function_main_of_File->name_of_Function);
				curFile->Function_main_of_File->name_of_Function = s;
				for(; *s; ++s)
					*s = toupper(*s);
			}

	| error 		{ $$=NULL; }

	/*| '(' expr_list ')' '=' expr %prec ASSIGN { $$=new_OperExprNode(new_AssignNode($2,$5,'=')); }*/

	| expr '=' expr { CM;$$=new_OperExprNode(new_AssignNode($1,$3,'=')); }
	| NAME RPTR NAME ass_op expr %prec ASSIGN { CM;$$=new_OperExprNode(assop_node($4,$1,NULL,$3,0,$5)); }
	| NAME RPTR mname ass_op expr %prec ASSIGN { CM;$$=new_OperExprNode(assop_node($4,$1,NULL,0,$3,$5)); }
	| fieldptr NAME RPTR NAME ass_op expr  %prec ASSIGN { CM; $$=new_OperExprNode(assop_node($5, $2,NULL,$4,0,$6)); }
	| fieldptr NAME RPTR mname ass_op expr  %prec ASSIGN { CM;$$=new_OperExprNode(assop_node($5, $2,NULL,0,$4,$6)); }
	| fieldptr NAME ass_op expr  %prec ASSIGN {CM;$$=new_OperExprNode(assop_node($3,NULL,NULL,$2,0,$4));}
	| mname RPTR NAME ass_op expr  %prec ASSIGN { CM;$$=new_OperExprNode(assop_node($4,NULL,$1,$3,0,$5)); }

	| '(' expr_list ')' RPTR NAME ass_op expr  %prec ASSIGN { CM;$$=new_OperExprNode(assop_node($6,NULL,$2,$5,0,$7)); }
	| '(' expr_list ')' RPTR mname ass_op expr  %prec ASSIGN { CM;$$=new_OperExprNode(assop_node($6,NULL,$2,0, $5,$7)); }
	| '(' expr_list ')' RPTR '(' expr_list ')' ass_op expr  %prec ASSIGN { CM;$$=new_OperExprNode(assop_node($8,NULL,$2,0,$6,$9)); }
	| fieldptr '(' expr_list ')' RPTR NAME ass_op expr  %prec ASSIGN { CM;$$=new_OperExprNode(assop_node($7,NULL,$3,$6,0,$8)); }
	| fieldptr '(' expr_list ')' RPTR mname ass_op expr  %prec ASSIGN { CM;$$=new_OperExprNode(assop_node($7,NULL,$3,0, $6,$8)); }
	| fieldptr '(' expr_list ')' RPTR '(' expr_list ')' ass_op expr  %prec ASSIGN { CM;$$=new_OperExprNode(assop_node($9,NULL,$3,0,$7,$10)); }

	| fieldptr mname RPTR NAME ass_op expr  %prec ASSIGN { CM;$$=new_OperExprNode(assop_node($5,NULL,$2,$4,0,$6)); }
	| fieldptr mname ass_op expr  %prec ASSIGN {CM;$$=new_OperExprNode(assop_node($3,NULL,NULL,0,$2,$4));}
	| mname RPTR mname ass_op expr  %prec ASSIGN { CM;$$=new_OperExprNode(assop_node($4,NULL,$1,0,$3,$5)); }
	| fieldptr mname RPTR mname ass_op expr %prec ASSIGN  { CM;$$=new_OperExprNode(assop_node($5,NULL,$2,0,$4,$6)); }

	| name '=' expr 	{ CM;$$=new_OperExprNode(new_AssignNode($1,$3,'=')); }
	| mname '=' expr  	{ CM;$$=new_OperExprNode(new_AssignNode($1,$3,'=')); }
	| arr '=' expr 		{ CM;$$=new_OperExprNode(new_ArrElNode($1,$3,'=')); }
	| expr  		{ $$=new_OperExprNode($1); CM;}
	| NAME '(' arglist ')' '=' expr {
				Node *np;
				int i;
				CM;
				np = new_ExprListNode();
				append_Node(np, installName($1));
				for(i=0; i< $3.coll->count_of_Coll; i++)
				{
					Node *p = (Node*) $3.coll->items_of_Coll[i];
					append_Node(np, p);
				}
				$$=new_OperExprNode(new_ArrElNode(np, $6,'='));
			}

	| '(' expr_list ')' '=' expr %prec ASSIGN { $$=new_OperExprNode(new_AssignNode(new_MacroNode($2,0),$5,'=')); }

	| WHILE
		{ CM;
			if (!curFunction->inLoop_of_Function)
				{ loop_line = cl_line; loop_file = currentFile(); }
			curFunction->inLoop_of_Function++;
		}
	  expr '\n' operlist enddo
		{
			curFunction->inLoop_of_Function--;
			$$=new_WhileNode($3,$5);
			pop_operlist();
		}

	| DO NAME with_arglist { CM; $$ = new_OperExprNode(new_CallNode($2, $3.coll, $3.haveRest)); }

	| DO mname with_arglist	{ CM; $$ = new_OperExprNode(new_CallNameNode($2, $3.coll)); }

	| CALL NAME with_arglist { CM; $$ = new_OperExprNode(new_CallNode($2, $3.coll, $3.haveRest)); }

	| CALL mname with_arglist	{ CM; $$ = new_OperExprNode(new_CallNameNode($2, $3.coll)); }

	| EXIT  {
			CM;
			if (!curFunction->inLoop_of_Function)
				yyerror("EXIT statement out of loop");
			$$=new_LoopExitNode(0);
		}

	| LOOP  {
			CM;
			if (!curFunction->inLoop_of_Function)
				yyerror("LOOP statement out of loop");
			$$=new_LoopExitNode(1);
		}

	|  for name eassign expr TO expr step '\n' { lex_initial(); } operlist next
		{
			curFunction->inLoop_of_Function--;
			$$=new_ForNode($2,$4,$6,$7,$10);
			pop_operlist();
		}

	| for name IN expr keys '\n' { lex_initial(); } operlist next
		{
			curFunction->inLoop_of_Function--;
			$$=new_ForeachNode($2,$4,$8,$5);
			pop_operlist();
		}

	| BEGSEQ curseq {
			CM;
			curFunction->seqNo_of_Function = ++curFunction->allSeqNo_of_Function;
			if (!curFunction->seqLevel_of_Function)
			{
				seq_line = cl_line;
				seq_file = currentFile();
			}
			curFunction->seqLevel_of_Function++;
			ARR_REALLOC(int, curFunction->seqStack_of_Function, curFunction->seqLevel_of_Function);
			curFunction->seqStack_of_Function[curFunction->seqLevel_of_Function-1]=$2;
		}
	  operlist
		{
			pop_operlist();
		}
	  recover
	  END   {
			CM;
			$$=new_SeqNode($4,$6.using,$6.operlist,$6.curseq);
		}

	| BREAK nexpr
		{  CM; $$=new_BreakNode($2, 1); }

	| IF '(' nilexpr ',' nilexpr ',' nilexpr ')'
		{
			Coll *cp=new_Coll(NULL,NULL);
			append_Coll(cp,$3);
			append_Coll(cp,$5);
			$$=new_OperExprNode(new_IfNode(cp,$7));
			CM;
		}

	| IF expr '\n'
		{
			if ($2->isExprList && getCount_Node($2)>1 )
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
		}
	  operlist  elseif  else  endif
		{
			CM;
			prepend_Coll($6,$5);
			prepend_Coll($6,$2);
			$$=new_IfNode( $6, $7);
			if_depth--;
			pop_operlist();
		}

	| DOCASE '\n' caselist  otherwise  endcase
		{
			CM;
			$$=new_IfNode( $3, $4);
		}

	| SWITCH expr '\n' switchlist otherwise endswitch
		{
			$$=new_SwitchNode($2, $4, $5);
		}
	;

type_def:	{ $$=0; }
	| AS NAME { $$=$2; }
	| REF NAME { $$=$2; }
	;

switchlist:	switchbeg
	| switchlist switchlabel '\n' operlist
		{
			SwitchEl *nl = NEW(SwitchEl);
			nl->labels_of_SwitchEl = $2;
			nl->operlist_of_SwitchEl = $4;
			insert_Coll($1, nl);
			$$ = $1;
			pop_operlist();
		}
	;

switchbeg:	{
			$$ = new_Coll(0, 0);
		}
	| switchbeg '\n'
	;

switchlabel: CASE mconstant
		{
			$$ = new_Coll(0,0);
			append_Coll($$, $2);
		}
	| switchlabel ',' mconstant
		{
			$$ = $1;
			append_Coll($$, $3);
		}
	;

for: FOR
		{  CM;
			if (!curFunction->inLoop_of_Function)
				{ loop_line = cl_line; loop_file = currentFile(); }
			curFunction->inLoop_of_Function++;
		}
	;

next:	NEXT
	| END
	;

keys:		{ $$ = 0; }
	| KEYS	{ $$ = 1; }
	;

recoverr:  function NAME paramlist type_def '\n'	{}
	| procedure NAME paramlist type_def '\n'	{}
	;

recover:	{
			$$.curseq = curFunction->seqNo_of_Function;
			$$.operlist = 0;
			$$.using = 0;
			curFunction->seqNo_of_Function=curFunction->seqStack_of_Function[curFunction->seqLevel_of_Function-1];
			curFunction->seqLevel_of_Function--;
		}
	| recoverr
		{
			yyerror("unclosed BEGIN SEQUENCE");
			curFunction->seqNo_of_Function=curFunction->seqStack_of_Function[curFunction->seqLevel_of_Function-1];
			curFunction->seqLevel_of_Function--;
			YYERROR;
		}
	| RECOVER curseq using '\n'
		{
			curFunction->seqNo_of_Function=curFunction->seqStack_of_Function[curFunction->seqLevel_of_Function-1];
			curFunction->seqLevel_of_Function--;
		}
	 operlist
		{
			$$.operlist = $6;
			$$.using = $3;
			$$.curseq = $2;
			pop_operlist();
		}
	;

curseq:		{$$ = curFunction->seqNo_of_Function; }
	;

using:		{ $$=NULL; }
	| USING name { $$=$2; }
	;

step:			{ $$=new_NumberConstNode(strdup("1"),0); }
	| STEP expr	{ $$=$2; }
	;

enddo:	  END
	| ENDDO
	;

iif:	  IIF
	| IF
	;

elseif:		{ $$=new_Coll(NULL,NULL); }
	| elseif ELSEIF expr '\n'  operlist
		{
			$$=$1;
			append_Coll($$, $3);
			append_Coll($$, $5);
			pop_operlist();
		}
	;

else:		{ $$=NULL; }
	| ELSE '\n' operlist
		{
			$$=$3;
			pop_operlist();
		}
	;

endif: 	  END
	| ENDIF
	;

casebeg:		{ $$=new_Coll(NULL,NULL); }
	| casebeg '\n'
	;

caselist: casebeg
	| caselist CASE expr '\n'  operlist
		{
			$$=$1;
			append_Coll($$, $3);
			append_Coll($$, $5);
			pop_operlist();
		}
	;

otherwise:	{ $$=NULL; }
	| OTHERWISE '\n' operlist
		{
			$$=$3;
			pop_operlist();
		}
	;

endcase:	END
	|	ENDCASE
	;

endswitch:	END
	|	ENDSWITCH
	;

inalias:  		{ $$=NULL; }
	| IN NAME	{ $$=$2; }
	;

name: NAME	{ $$=installName($1); }
	| memvarptr NAME	{ $$=new_MemvarNode(new_Var($2)); }
	/*| FIELDPTR NAME		{ $$=new2_FieldNode(NULL,NULL,$2,NULL); }*/
	/*| FIELDPTR mname	{  }*/
	| memvarptr mname	{ $$=$2; }
	;

iname: NAME	{ $$=installName($1); }
	| memvarptr NAME	{ $$=new_MemvarNode(new_Var($2)); }
	| fieldptr NAME		{ $$=new2_FieldNode(NULL,NULL,$2,NULL); }
	;

mname:	 '&' iname 	{ $$=new_MacroNode($2,0); }
	| '&' iname '.'	{ $$=new_MacroNode($2,0); }
	| NAME '&' iname	{ $$=new_MacroNode(new_OpNode(new_StringConstNode($1),$3,'+'),0);  }
	| '&' iname '.' NAME	{ $$=new_MacroNode(new_OpNode($2,new_StringConstNode($4),'+'),0); }
	| '&' iname NUMBER	{ char *s = $3+strspn($3,",.-+");
			$$=new_MacroNode(new_OpNode($2,new_StringConstNode(s),'+'),0); }
	| '&' '(' expr ')'	{ $$=new_MacroNode($3,1); }
	;

expr:	constant
	| ISTRING	{
				int i, l, bad;
				l = strlen($1);
				bad = 0;
				for(i=0; i<l; i++)
				{
					if ( $1[i] & 0x80 )
					{
						bad = 1;
						break;
					}
				}
				if (l && !bad)
				{
					put_locale_string($1);
					$$=new_LocaleStringNode($1);
				}
				else
				{
					$$=new_StringConstNode($1);
				}
			}
	| ISTRING POW expr {
				Node *e1 = new_StringConstNode($1);
				Node *e2 = new_StringConstNode(strdup($1));
				$$ = new3_CallNode("_clic_ngettext", e1, e2, $3);
			}
	| DATE		{
				Coll *cp = new_Coll(0,0);
				append_Coll(cp, new_StringConstNode($1));
				$$=new_CallNode(strdup("CTOD"), cp, 0);
			}
	| name
	| mname
	| '(' expr_list ')' 	{
		/*((ExprListNode*)$2)->last=1; ; */
		$$=reduce_ExprListNode($2);
		}
	| '@' expr { $$=new_RefNode($2); }

	| NAME '(' arglist ')' { $$=new_CallNode( $1, $3.coll, $3.haveRest); }
	| mname '(' arglist ')' { $$=new_CallNameNode($1,$3.coll); }

	| arr	{ $$=new_ArrElNode($1,NULL,0); }
	| name assign expr  %prec ASSIGN	{ $$=new_AssignNode($1,$3,$2); }
	| arr assign expr %prec ASSIGN	{ $$=new_ArrElNode($1,$3,$2); }
	| mname assign expr  %prec ASSIGN	{ $$=new_AssignNode($1,$3,$2); }

	| expr ':' NAME '(' arglist ')' { $$=new_MethodNode($1,$3,$5.coll, $5.haveRest); }
	| arr '(' arglist ')'
		{
			Node *arr = $1;
			Node *last;
			char *name;
			last_List(&arr->list);
			last = (Node *) arr->list.current_of_List;
			if( !last || strcmp(last->name, "const") || !(name=((ConstNode*)last)->rval) )
			{
				yyerror("invalid array call");
				YYERROR;
			}
			remove_List(&arr->list);
			$$ = new_MethodNode(new_ArrElNode(arr,0,0), name, $3.coll, $3.haveRest);
		}
	| DSEMICOLON NAME '(' arglist ')' {
		add_self();
		$$=new_MethodNode(installName(strdup("__SELF__")),$2,$4.coll, $4.haveRest);
		}
	| fieldptr NAME	{$$=new2_FieldNode(NULL,NULL,$2,NULL);}
	| NAME RPTR NAME  {$$=new2_FieldNode($1,NULL,$3,NULL); }
	| fieldptr mname { $$=new2_FieldNode(NULL,NULL,NULL,$2); }

	| NAME	REPTR '(' expr ')'	{ $$=$4; }

	/* !!!!!! */
	| fieldptr NAME RPTR NAME	 { $$=new2_FieldNode($2,NULL,$4,NULL); }
	| fieldptr NAME RPTR mname { $$=new2_FieldNode($2,NULL,NULL,$4); }
	| NAME RPTR '(' expr_list ')' { $$=new2_FieldNode($1,NULL,NULL,reduce_ExprListNode($4)); }
	| fieldptr NAME RPTR '(' expr_list ')' { $$=new2_FieldNode($2,NULL,NULL,reduce_ExprListNode($5)); }
	| NAME RPTR mname { $$=new2_FieldNode($1,NULL,NULL,$3); }

	| '(' expr_list ')' RPTR '(' expr_list ')' {$$=new2_FieldNode(NULL,reduce_ExprListNode($2),NULL,reduce_ExprListNode($6));}
	| '(' expr_list ')' RPTR mname { $$=new2_FieldNode(NULL,reduce_ExprListNode($2),NULL,$5); }
	| '(' expr_list ')' RPTR NAME { $$=new2_FieldNode(NULL,reduce_ExprListNode($2),$5,NULL); }
	| fieldptr '(' expr_list ')' RPTR '(' expr_list ')' {$$=new2_FieldNode(NULL,reduce_ExprListNode($3),NULL,reduce_ExprListNode($7));}
	| fieldptr '(' expr_list ')' RPTR mname { $$=new2_FieldNode(NULL,reduce_ExprListNode($3),NULL,$6); }
	| fieldptr '(' expr_list ')' RPTR NAME { $$=new2_FieldNode(NULL,reduce_ExprListNode($3),$6,NULL); }

	| mname RPTR NAME	{$$=new2_FieldNode(NULL,$1,$3,NULL);}
	/* !!!!!!! */
	| fieldptr mname RPTR NAME	{$$=new2_FieldNode(NULL,$2,$4,NULL);}
	| mname RPTR '(' expr_list ')'	{$$=new2_FieldNode(NULL,$1,NULL,reduce_ExprListNode($4));}
	| fieldptr mname RPTR '(' expr_list ')'	{$$=new2_FieldNode(NULL,$2,NULL,reduce_ExprListNode($5));}
	| mname RPTR mname	{$$=new2_FieldNode(NULL,$1,NULL,$3);}
	| fieldptr mname RPTR mname	{$$=new2_FieldNode(NULL,$2,NULL,$4);}

	| NAME RPTR NAME assign expr %prec ASSIGN { CM;$$=(assop_node($4,$1,NULL,$3,0,$5)); }
	| fieldptr NAME RPTR NAME assign expr  %prec ASSIGN { CM; $$=(assop_node($5, $2,NULL,$4,0,$6)); }
	| fieldptr NAME RPTR mname assign expr  %prec ASSIGN { CM;$$=(assop_node($5, $2,NULL,0,$4,$6)); }
	| fieldptr NAME assign expr  %prec ASSIGN {CM;$$=(assop_node($3,NULL,NULL,$2,0,$4));}
	| mname RPTR NAME assign expr  %prec ASSIGN { CM;$$=(assop_node($4,NULL,$1,$3,0,$5)); }

	| '(' expr_list ')' RPTR NAME assign expr  %prec ASSIGN { CM;$$=(assop_node($6,NULL,reduce_ExprListNode($2),$5,0,$7)); }
	| '(' expr_list ')' RPTR mname assign expr  %prec ASSIGN { CM;$$=(assop_node($6,NULL,reduce_ExprListNode($2),0, $5,$7)); }
	| '(' expr_list ')' RPTR '(' expr_list ')' assign expr  %prec ASSIGN { CM;$$=(assop_node($8,NULL,reduce_ExprListNode($2),0,reduce_ExprListNode($6),$9)); }
	| fieldptr '(' expr_list ')' RPTR NAME assign expr  %prec ASSIGN { CM;$$=(assop_node($7,NULL,reduce_ExprListNode($3),$6,0,$8)); }
	| fieldptr '(' expr_list ')' RPTR mname assign expr  %prec ASSIGN { CM;$$=(assop_node($7,NULL,reduce_ExprListNode($3),0, $6,$8)); }
	| fieldptr '(' expr_list ')' RPTR '(' expr_list ')' assign expr  %prec ASSIGN { CM;$$=(assop_node($9,NULL,reduce_ExprListNode($3),0,reduce_ExprListNode($7),$10)); }

	| fieldptr mname RPTR NAME assign expr  %prec ASSIGN { CM;$$=(assop_node($5,NULL,$2,$4,0,$6)); }
	| NAME RPTR mname assign expr  %prec ASSIGN { CM;$$=(assop_node($4,$1,NULL,0,$3,$5)); }
	| fieldptr mname assign expr  %prec ASSIGN {CM;$$=(assop_node($3,NULL,NULL,0,$2,$4));}
	| mname RPTR mname assign expr  %prec ASSIGN { CM;$$=(assop_node($4,NULL,$1,0,$3,$5)); }
	| fieldptr mname RPTR mname assign expr %prec ASSIGN  { CM;$$=(assop_node($5,NULL,$2,0,$4,$6)); }

	| expr '=' expr		{ $$=new_OpNode($1,$3,'e');  }
	| expr EQ expr		{ $$=new_OpNode($1,$3,'E');  }
	| expr NE expr		{ $$=new_OpNode($1,$3,'N');  }
	| expr '<' expr		{ $$=new_OpNode($1,$3,'<');  }
	| expr '>' expr		{ $$=new_OpNode($1,$3,'>');  }
	| expr LE expr		{ $$=new_OpNode($1,$3,'L');  }
	| expr GE expr		{ $$=new_OpNode($1,$3,'G');  }
	| expr '+' expr		{ $$=new_OpNode($1,$3,'+');  }
	| expr '$' expr		{ $$=new_OpNode($1,$3,'$');  }
	/*| expr NOT '$' expr		{ $$=new_OpNode($1,$4,'$');  }*/
	| expr '-' expr		{ $$=new_OpNode($1,$3,'-');  }
	| expr '*' expr		{ $$=new_OpNode($1,$3,'*');  }
	| expr '/' expr		{ $$=new_OpNode($1,$3,'/');  }
	| expr '%' expr		{ $$=new_OpNode($1,$3,'%');  }
	| expr POW expr		{ $$=new_OpNode($1,$3,'^');  }
	| expr AND expr		{ $$=new_AndNode($1,$3);  }
	| expr OR expr		{ $$=new_OrNode($1,$3);  }

	| NOT expr	{ $$=new_NotNode($2);  }

	| INCR expr	{ $$=new_IncrNode($2,1,0); }
	| expr INCR %prec POSTINCR	{ $$=new_IncrNode($1,1,1); }
	| DECR expr	{ $$=new_IncrNode($2,0,0); }
	| expr DECR %prec POSTINCR	{ $$=new_IncrNode($1,0,1); }
	| '-' expr %prec MINUS	{ $$=new_MinusNode($2); }
	| '+' expr %prec MINUS	{ $$=$2; }

	| '{' exprlist '}'	{$$=new_ArrayInitNode($2); }
	| '{' pairlist '}'	{$$=new_ArrayInitNode($2); }
	| '{' '|' func code  exprlist '}'	{
			Node *np=$4;
			Node *el=$5;
			CodeNode *npp = (CodeNode*)np;
			int haveMacro = 0;
			fin_Function(curFunction);

			append_Node(curFunction->Node_body_of_Function,new_ReturnNode(el,1));

			npp->func->Node_body_of_Function->pass(npp->func->Node_body_of_Function, SearchMacro, 0, &haveMacro);

			if (haveMacro)
			{
				/* transform into CODEBLOCK(exprlist)" */
				remove_Coll(&curFile->Coll_codeblocks_of_File, npp->func);
				np = new_MacroNode(new_CodestrNode(el,1,curFunction->VarColl_params_of_Function),0);
			}
			curFunction = $3;

			$$=np;
		}
	| CGET '(' arg_list ')'	{ $$=new_CGetNode($3); }
	| PCOUNT	{ $$=new_PcountNode(); }
	| PSHIFT	{ $$=new_PshiftNode(); }
	| PARBEG expr ']' { $$=new_ParnNode($2); }
	| PARAMBEG expr ')' { $$=new_ParnNode($2); }

	| CODESTR bparam_list {  curFunction->VarColl_code_params_of_Function=$2; }
		exprlist ')' { $$=new_CodestrNode($4,0,$2); curFunction->VarColl_code_params_of_Function=0; }
	| CODEBLOCK bparam_list {  curFunction->VarColl_code_params_of_Function=$2; }
		exprlist ')' { $$=new_MacroNode(new_CodestrNode($4,1,$2),0); curFunction->VarColl_code_params_of_Function=0; }

	| LOCAL '(' func
		{
			if (curFunction->Function_upfunc_of_Function)
				curFunction = curFunction->Function_upfunc_of_Function;
		}
		vardef ')'
		{
			Node *np;
			curFunction = $3;
			np = new_LocalDefNode($5, 0, 1);
			prepend_Node(curFunction->Node_body_of_Function, np);
			$$ = new_NilConstNode();
			CM;
		}

	| iif '(' nilexpr ',' nilexpr ',' nilexpr ')'
		{
			Coll *cp=new_Coll(NULL,NULL);
			append_Coll(cp,$3);
			append_Coll(cp,$5);
			$$=new_IfNode(cp,$7);
		}


	| EBREAK  nexpr ')'
		{  $$=new_BreakNode($2,0); }
	;


bparam_list:	{ $$=0; }
	| '|' param_list '|' { $$=$2; }
	;

nilexpr:	{ $$=new_NilConstNode(); }
	| expr	{ $$=$1; }
	;

eassign: ASSIGN
	| '='
	;

fieldptr: FIELDPTR
	| fieldptr FIELDPTR
	| memvarptr FIELDPTR
	;

memvarptr: MEMVARPTR
	| fieldptr MEMVARPTR
	| memvarptr MEMVARPTR
	;

assign: ASSIGN	{ $$='='; }
	| ADDA	{ $$='+'; }
	| SUBA	{ $$='-'; }
	| MULA	{ $$='*'; }
	| DIVA	{ $$='/'; }
	| MODA	{ $$='%'; }
	| POWA	{ $$='^'; }
	;

ass_op: '=' { $$='='; }
	;


func:	{ $$=curFunction; }
	;

code:	param_list '|' { $$=new_CodeNode($1); }
	;

arr:	 expr '[' expr_list ']' { $$=$3; prepend_Node($3,$1); }
	| arr '[' expr_list ']'	{ $$=$1; join_Node($1,$3); }
	| expr ':' NAME	{
		$$=new_ExprListNode();
		append_Node($$, $1);
		upstr($3);
		append_Node($$, new_HashConstNode($3));
		}
	| DSEMICOLON NAME	{
		$$=new_ExprListNode();
		add_self();
		append_Node($$, installName(strdup("__SELF__")));
		upstr($2);
		append_Node($$, new_HashConstNode($2));
		}
	| arr ':' NAME  	{
		$$=$1;
		upstr($3);
		append_Node($$,new_HashConstNode($3));
	}
	;

vardef:  var  type_def	{
			$$=new_VarColl();
			$1->type = $2;
			add_VarColl($$, $1);
			if (def_node)
				append_Node( curr_operlist, def_node($$));

		}
	| vardef ',' var type_def
		{
			if (def_node)
				$$=new_VarColl();
			else
				$$=$1;
			$3->type = $4;
			add_VarColl($$, $3);
			if (def_node)
				append_Node( curr_operlist, def_node($$));
		}
	;

dimdef:  dim	{
			$$=new_VarColl();
			add_VarColl($$, $1);
		}
	| dimdef ',' dim
		{
			$$=$1;
			add_VarColl($$, $3);
		}
	;

dim:	NAME dim_list	{ $$=newArrInit_Var($1,$2); }
	| mname dim_list	{ $$=mnewArrInit_Var($1,$1); }
	;

dim_list:  '(' expr_list ')' { $$=$2; }
	| dim_list '(' expr_list ')'	{ $$=$1; join_Node($1,$3); }
	;

var:	  NAME		{ $$=new_Var($1); }
	| NAME arr_list	{ $$=newArrInit_Var($1,$2); }
	| NAME ASSIGN expr	{ $$=newInit_Var($1,$3); }
	| name '=' expr	{ yyerror("only inline assign := allowed in initialisation"); $$=new_Var(strdup("")); }

	| mname		{ $$=mnew_Var($1); }
	| mname arr_list	{ $$=mnewArrInit_Var($1,$1); }
	| mname ASSIGN expr	{ $$=mnewInit_Var($1,$3);  }
	| mname '=' expr	{ yyerror("only inline assign := allowed in initialisation"); $$=new_Var(strdup("")); }
	;

arr_list:  '[' expr_list ']' { $$=$2; }
	| arr_list '[' expr_list ']'	{ $$=$1; join_Node($1,$3); }
	;

param_list:  { $$=new_VarColl(); }
	| NAME type_def	{
		Var *vp=new_Var($1);
		vp->type = $2;
		$$=new_VarColl();
		vp->no=0;
		insert_Coll( &$$->coll, vp);
		insert_Coll( &$$->unsorted, vp);
		}
	| param_list ',' NAME type_def{
		Var *vp=new_Var($3);
		vp->type = $4;
		$$=$1;
		vp->no = $$->unsorted.count_of_Coll;
		insert_Coll( &$$->coll, vp);
		insert_Coll( &$$->unsorted, vp);
		}
	;


namelist: 	{$$=new_Coll(free,strcmp);}
	| NAME  {
			$$=new_Coll(free,strcmp);
			insert_Coll($$, $1);
		}
	| namelist ',' NAME
		{
			int no;
			$$=$1;
			if ( search_Coll( $$, $3, &no) )
			{
				yyerror("name '%s' duplication", $3);
				free($3);
			}
			else
				insert_Coll( $$, $3 );
		}
	;

varlist:  NAME	{
			$$=new_VarColl();
			add_VarColl($$, new_Var($1));
		}
	| varlist ',' NAME
		{
			$$=$1;
			add_VarColl($$, new_Var($3));
		}
	;

arglist: arg_list		{ $$.coll=$1; $$.haveRest=0; }
	| arg_list ',' PALL	{ $$.coll=$1; $$.haveRest=1; }
	| PALL	{ $$.coll=new_Coll(NULL,NULL); $$.haveRest=1;  }
	;

with_arglist: { $$.coll=new_Coll(NULL,NULL); $$.haveRest=0; }
	| WITH with_arg_list		{ $$.coll=$2; $$.haveRest=0; }
	| WITH with_arg_list ',' PALL	{ $$.coll=$2; $$.haveRest=1; }
	| WITH PALL	{ $$.coll=new_Coll(NULL,NULL); $$.haveRest=1;  }
	;
/*
with:			{ $$.coll=new_Coll(NULL,NULL); $$.haveRest=0; }
	| WITH with_arglist; 	{
			Coll *cp;
			int i;
			Node *np;

			$$=$2;
			cp = $$.coll;
			for(i=0; i<cp->count; i++)
			{
				np = (Node*) cp->items[i];
				if (np->isRef||np->isConst||(!np->isLval && !np->isField))
					continue;
				cp->items[i] = new_RefNode(np);
			}
		}
	;
*/
with_arg_list: arg	{
			$$=new_Coll(NULL,NULL);
			if ($1) {
				Coll *cp;
				Node *np;

				insert_Coll($$,$1);
				cp = $$;
				np = (Node*) cp->items_of_Coll[0];
				if(!(np->isRef||np->isConst||(!np->isLval && !np->isField)))
					cp->items_of_Coll[0] = new_RefNode(np);
			}
		}
	| '(' arg ')' {
			$$=new_Coll(NULL,NULL);
			if ($2)
				insert_Coll($$,$2);
	}
	| with_arg_list ',' arg 	{
			$$=$1;
			if (!$$->count_of_Coll)
				insert_Coll($$,new_ArgNode(new_NilConstNode(),0));
			if ($3) {
				Coll *cp;
				Node *np;
				insert_Coll($$,$3);
				cp = $$;
				np = (Node*) cp->items_of_Coll[cp->count_of_Coll-1];
				if(!(np->isRef||np->isConst||(!np->isLval && !np->isField)))
					cp->items_of_Coll[cp->count_of_Coll-1] = new_RefNode(np);
			}
			else
				insert_Coll($$,new_ArgNode(new_NilConstNode(),0));
		}
	| with_arg_list ',' '(' arg ')'	{
			$$=$1;
			if (!$$->count_of_Coll)
				insert_Coll($$,new_ArgNode(new_NilConstNode(),0));
			if ($4)
				insert_Coll($$,$4);
			else
				insert_Coll($$,new_ArgNode(new_NilConstNode(),0));
		}
	;

arg_list: arg	{
			$$=new_Coll(NULL,NULL);
			if ($1)
				insert_Coll($$,$1);
		}
	| arg_list ',' arg 	{
			$$=$1;
			if (!$$->count_of_Coll)
				insert_Coll($$,new_ArgNode(new_NilConstNode(),0));
			if ($3)
				insert_Coll($$,$3);
			else
				insert_Coll($$,new_ArgNode(new_NilConstNode(),0));
		}
	;

arg:		{ $$=NULL; }
	| expr	{ $$=$1; /*$$=new_ArgNode($1,0);*/ }
	/*| '@' expr { $$=new_ArgNode($2,1); }*/
	;


pairlist: pair {}
	| pairlist ',' pair 	{}
	;

pair:	  NAME '#'  expr	{}
	| NUMBER '#'  expr	{}
	;


exprlist: nexpr			{
					$$=new_ExprListNode();
					$$->isConst=1;
					if ($1)
					{
						append_Node($$, $1);
						if (!$1->isConst)
							$$->isConst=0;
						else
							$$->isConst=1;
					}
				}
	| exprlist ',' nexpr 	{
					$$=$1;
					if (getCount_Node($$) == 0)
						append_Node($$,new_NilConstNode());
					if ($3)
					{
						append_Node($$, $3);
						if (!$3->isConst)
							$$->isConst=0;
					}
					else
						append_Node($$,new_NilConstNode());
				}
	;

nexpr:		{ $$=NULL; }
	| expr
	;

expr_list: expr	{ $$=new_ExprListNode(); append_Node($$, $1); }
	| expr_list ',' expr { $$=$1; append_Node($$, $3); }
	;

constant: STRING		{ $$=new_StringConstNode($1); }
	| TRUE			{ $$=new_LogicConstNode(1); }
	| FALSE			{ $$=new_LogicConstNode(0); }
	| NIL			{ $$=new_NilConstNode(); }
	| NUMBER		{ $$=new_NumberConstNode($1,0); }
	;

mconstant: constant
	| '-' constant	{ $$=$2; $$->isMinus = 1; }
	;

%%

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
	curFile->mname_of_File = strdup(path);
	if (nomain_flag)
		strncat(path,"_m",sizeof(path));

	curFunction=curFile->Function_main_of_File=mainFunction=new_Function(strdup(path),new_VarColl(),nomain_flag?0:1,1,0,0);
	fin_Function(curFunction);
	fileStatics=curStatics=mainFunction->VarColl_statics_of_Function;
	push_operlist(curFunction->Node_body_of_Function);
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
	if (!mainFunction && curFunction==curFile->Function_main_of_File)
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
	if (curFunction->inLoop_of_Function>0)
	{
		yyerror("unclosed loop statement (level %d, begin at line %d file %s)",
			curFunction->inLoop_of_Function, loop_line, fileName(loop_file) );
		/*curFunction->inLoop = 0;*/
	}
	if (if_depth)
	{
		yyerror("unclosed IF statement (level %d, begin at line %d file %s)",
			if_depth, if_line, fileName(if_file) );
		if_depth = 0;
	}
	if (curFunction->seqLevel_of_Function>0)
	{
		yyerror("unclosed SEQUENCE statement (level %d, begin at line %d file %s)",
			curFunction->seqLevel_of_Function, seq_line, fileName(seq_file) );
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
	if (operlist_stack->count_of_Coll>0)
	{
		atRemove_Coll (operlist_stack, operlist_stack->count_of_Coll-1);
		curr_operlist = (Node*) operlist_stack->items_of_Coll[operlist_stack->count_of_Coll-1];
	}
}

static Node *
reduce_ExprListNode(Node *np)
{
	if (strcmp(np->name, "exprlist"))
		return np;
	if (getCount_Node(np) == 1)
	{
		Node *p = (Node*) np->list.head_of_List;
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

