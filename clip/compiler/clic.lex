/*
	Copyright (C) 2001,2002  ITK
	Author   : Paul Lasarev <paul@itk.ru>
	License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI
*/
%{
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <errno.h>
#if defined(__GNUC__)
   #include <unistd.h>
   #include "ci_clipcfg.h"
#else
   #include "ci_wintypes.h"
#endif


#include "ci_clic.h"
#include "clic.tab.h"
#include "../libclipscreen/ci_charset.h"
#include "../libclipscreen/ci_screen.h"

#define FREE(s)	{if (s) {free(s); (s)=0;}}

#define YY_INPUT(buf,result,max_size) { (result) = do_input((buf), (max_size)); }


#define FOUR_BYTE


#undef YY_FATAL_ERROR
#define YY_FATAL_ERROR(msg) yyerror(msg)

#define MAX_STR_CONST 4096

static Coll unput_strings;
static StrBuf unput_buffer = {0,0,0,0};
static int subst_count = 0; /* count of substs in current str */
static int subst_flag = 0; /* do only subst on current str */
static int do_input(char *buf, int max_size);


int choose_quot(char *str,int *lquot, int *rquot, int simple);
char *quot_str(char *str, int simple);
static void set_last(Coll *coll);
static int is_id(int ch);
static void vo_printf(int vlevel, int level, const char *fmt, ...);
static int xdefine = 0;
static int nnl = 0;

static int nlflag=0;
static void command_end(void);

long clic_line = 0, all_lines=0, clic_pos = 0, clic_word=0, cl_line, cl_pos,
	corr_line = 0;
int preproc_flag=0;
int command_flag=0;

static int comment_eol=0;
static int comment_beg=0;
static int field_state;
static int bracket_level=0;
static void prbrack(void);
StrBuf bracket_buf={0,0,0,0};
static void mecho(void);
static void lecho(int l);
static void pecho(char *p);
static void secho(int ch1, char *s, int ch2);
#define MECHO mecho();
#define LECHO(l) lecho(l);
#define PECHO(p) pecho(p);
#define SECHO(ch1,ch2) secho(ch1, string_buf, ch2);
static void print_candidates(char *cmd);

static int comment_depth=0;
static int clic_quote, clic_brack1, clic_brack2, clic_brack3;
static int ifdef_caller, comment_caller, message_caller, str_caller, str_return;
static int strLine, strPos, strFile;
static int commentLine, commentPos, commentFile;
static int full_string_flag;
static char string_buf[MAX_STR_CONST];
static char *string_buf_ptr;
static void add_string_buf(char *buf, int len);
static void addch_string_buf(int ch);

static void newline(void);
static void newpos(void);
static void newpos_w(void);
static void newpos_wt(int l);
static void newpos_t(void);

#if 0
static void unput_buf(void);
static void unput_char(int ch);
static void unput_str(const char *str);
static void unput_mem(const char *str, int len);
static void unput_str_free(char *str);
#else
static void put_buf(void);
static void put_char(int ch);
static void put_str(const char *str);
static void put_mem(const char *str, int len);
static void put_str_free(char *str);
static void unputstr(const char *str);
static void unputmem(const char *str, int l);
#endif


static char *textout=0, *textend=0;
static Coll text;

static int startCurInclude(void);
static void set_charset(char *charset, const char *file);
static void push_charset(void);
static void pop_charset(void);
static void recode_string(char *str);
static void load_file_charset(const char *path);
static int cyyleng;


static int last_ret;
#define MRETURN(r) { if (subst_flag) {last_ret=(r); break;} else return m_return(r); }
#define MRETURN_NAME { if (subst_flag) {last_ret=NAME;break;} else return n_return(); }

static int m_return(int r);

static int
n_return(void)
{
	if (!strcasecmp(yylval.string,"if"))
	{
		free(yylval.string);
		return m_return(IF);
	}

	return m_return(NAME);
}

static char *mem_dup(const char *str, int len);
static char *mem_Dup(const char *str, int len);
static int back_strsize(void *s1, void *s2);

static int callEol=0;

typedef struct
{
	YY_BUFFER_STATE buf;
	long line;
	int type;   /* '<' or '"' */
	char *name;
} IncludeFile;

typedef struct
{
	char * name;
	char * body;
	int xdefine;
} Macro;

typedef struct
{
	char *text;
	int len;
	int argno;
} MacroPart;

typedef struct
{
	Macro macro;
	Coll parts;
	Coll macroArgs;
} MacroFunc;

typedef enum
{
	M_NONE=0,
	SM_literal,
	SM_word,
	SM_norm,
	SM_extend,
	SM_list,
	SM_enum,
	SM_patt,
	SM_option,
	TM_literal,
	TM_word,
	TM_norm,
	TM_noni,
	TM_str,
	TM_intel,
	TM_block,
	TM_log,
	TM_option,
	TM_join,
	TM_var,
} CommandPartType;

typedef struct CommandPart
{
	char *text;
	CommandPartType type;
	int no;
	Coll *option; /* coll of CommandPart */
	struct CommandPart *prev; /* save */
	Coll *prevList; /* save */
	Coll *delim;
	int isLast;
	int isWord;	/* begin with SM_word */
} CommandPart;

typedef struct
{
	char *name;
	int isX;
	int isTranslate;
	int isAX;
	Coll pattern; /* coll of CommandPart */
	Coll patternIt; /* iterator coll */
	Coll subst;   /* coll of CommandPart */
	Coll substIt; /* iterator coll */
} Command;

typedef struct
{
	char *text;
	Coll *list;
	Coll *rawlist;
	CommandPart *cp;
	char *val;
	int listcomma;
} CommandArg;

typedef struct
{
	long begline;
	long elseline;
	int state;
	int condition;
	int ifdef;
} IfdefState;

Coll fileList;

Coll includePaths;
Coll predefines;
Coll reffuncs;

static Coll includes;
static Coll macros, xmacros;
static Coll ifdefs;
static Coll macroFuncs, xmacroFuncs;
static Coll macroArgs;
static IncludeFile *curInclude;
static IfdefState *curIfdef;
static Macro *curMacro;
static MacroFunc *curMacroFunc;
static Coll charsets;

static Macro* new_Macro(char *Name, char *Body);
static void free_Macro(void *item);
static int compare_macro(void *k1, void *k2);
static int xcompare_macro(void *k1, void *k2);
static MacroFunc *new_MacroFunc(char *Name);
static void free_MacroFunc(void *item);
static MacroPart *new_MacroPart(char *Text, int no);

static IfdefState *new_IfdefState(int Cond, int If);
static IncludeFile *new_IncludeFile( char *Name, long Line );
static void free_IncludeFile(void *item);

static Macro * findMacro(const char *name);
static MacroFunc* findMacroFunc(const char *name);
static void finale_MacroFunc(MacroFunc *self);

#define MAX_PART_DEEP 32
static Coll macroCommands;
static Coll commandArgs; /* coll of CommandArg */
static Command *curCommand=NULL;
static int optLevel=0;   /* level of optional subcommands */
static CommandPart *curCommandPart;
static Coll *curCommandList;
static Coll *curItList;
static int commandInd;
static int stringQuota, brack_level, stringQuota2;
static char *callName=0;

static Command *new_Command(const char *Name, int isX, int isTranslate);
static void delete_Command(void *item);
static int compare_Command(void *item1, void *item2);
static CommandPart *new_CommandPart(CommandPartType type, const char *text);
static void delete_CommandPart(void *arg);
static Command* findFirst_Command(const char *name, int *ind);
static Command* findNext_Command(const char *name, int *ind);
static Command* findFirst_Translate(const char *name, int *ind);
static CommandArg *new_CommandArg(CommandPart *cp, const char *text, int len);
static CommandArg *newList_CommandArg(CommandPart *cp);
static CommandArg *newLog_CommandArg(CommandPart *cp, const char* val, int len);
static void delete_CommandArg(void *arg);
static void check_Command(Command *cp);
static int matchCurCommand(int *pos);
static void substCurCommand(int nl);
static void insertCommandList(CommandPart*cp);
static void finaleCommandList(void);

static void print_Command(Command *cp);
static void check_CommandPart(CommandPart*cp);


static int ctextPos;
static char *ctextPtr;
static int ctextLen;
static int ctextQuota;
static void addCtext(int s);
static Coll *sm_enum_coll = 0;

static char *match_delim(CommandPart *cp, char *str);

static int command_state = 0;
static int bol_translate = 0;
static int bad_translate = 0;
static int macro_call_state = 0;
static int macro_call_bol = 0;
static int command_next_state = 0;
static int match_translate = 0;

static char *strdup_bs(const char *s);

static char *sm_enum_word = 0;

static char *marker_id(char *str);

static void my_yy_set_bol(int val)
{
	yy_set_bol(val);
}


%}

%array
%x lexString lexString1 lexString2 lexString3 cmdString cmdString2
%x lexComment lexEolComment
%x ctextEolState ctextState
%x defineState defineBody defineFuncBody defineArgs undefState
%x macroCallBegin macroCall macroDelim
%x ifdefState skipState
%x includeState
%x commandState substState commandCall cmdComment cmdEolComment
%x smenumState
%x keyword messageState
%x textState
%s field_words for_words recover_words foreach_words
%s field
%s vardef_words
%s static_words

dig	[0-9]
odig	[0-7]
hex	[0-9a-fA-F]
id	[_a-zA-Z][_a-zA-Z0-9]*
acid	[.@?$:!_a-zA-Z0-9]
bcid	[.@?$:!_a-zA-Z0-9\-\\>]
ccid	{acid}{acid}*|[.@?$:]+{bcid}
cid	[\[@?$&!_a-zA-Z0-9][\[@?$&!_a-zA-Z0-9]*
notidbeg	[^_\r\na-zA-Z\x80-\xff\\/& \t;]
wt	[ \t]
wtnl	[ \t]*(;[ \t]*{eol})?[ \t]*
delim	[ \t\r\n.,;:\'\"\]\[{}()*&^%$#@!~`\-+=/?<>\\|\032]
eol  	\r?(\n|\032)
ceol  	\r?(\n|\032|;)
noteol	[^\r\n\032]
eolcomment ("//"|"&&")
sm_norm <{wt}*{cid}{wt}*>
sm_extend <\({wt}*{cid}{wt}*\)>
sm_list <{wt}*{cid}{wt}*,{wt}*"..."{wt}*>
sm_enum <{cid}\:{wt}*{cid}([ \t,]+{cid})*{wt}*>
sm_enum_beg <{wt}*{cid}\:{wt}*
sm_patt <\*{wt}*{cid}{wt}*\*>
tm_norm <{wt}*{cid}{wt}*>
tm_noni \#<{wt}*{cid}{wt}*>
tm_str  <(\"|\'){wt}*{cid}{wt}*(\"|\')>
tm_intel <\({wt}*{cid}{wt}*\)>
tm_block <\{{wt}*{cid}{wt}*\}>
tm_log  <\.{wt}*{cid}{wt}*\.>
tm_var  <#{wt}*{cid}{wt}*#>
tm_join "##"
cmdword	[._a-zA-Z][.()_a-zA-Z0-9]*

%%

^{eol}	   {
			BEGIN(INITIAL);
			if (preproc_flag && !subst_flag)
				;
			else
				PECHO("\n");
			newline();
	 }
{eol}    {
	  newline();
	  PECHO("\n");
	  MRETURN('\n')
	}

^{wt}*"*"[^cr\n\r\032]{noteol}+	newpos(); comment_caller=YYSTATE; full_string_flag=1;  BEGIN(lexEolComment);
^{wt}*"*"cmdstr{wt}*	newpos_t(); message_caller=2; BEGIN(messageState);
^{wt}*"*"charset{wt}*	newpos_t();  message_caller=3; BEGIN(messageState);
^{wt}*"*"reference{wt}*	newpos_t();  message_caller=4; BEGIN(messageState);
^{wt}*"*"{eol}+		newpos(); newline();
^{wt}*"*"{noteol}	newpos(); comment_caller=YYSTATE; full_string_flag=1;  BEGIN(lexEolComment);

^{wt}*"#!"{noteol}*	newpos(); comment_caller=YYSTATE; full_string_flag=1;  BEGIN(lexEolComment);
"//"{noteol}*		newpos(); comment_caller=YYSTATE; full_string_flag=0;  BEGIN(lexEolComment);
"&&"{noteol}*		newpos(); comment_caller=YYSTATE; full_string_flag=0;  BEGIN(lexEolComment);

"**"    newpos_w(); MECHO; MRETURN(POW)
":="	newpos_w(); MECHO; MRETURN(ASSIGN)
"+="    newpos_w(); MECHO; MRETURN(ADDA)
"-="    newpos_w(); MECHO; MRETURN(SUBA)
"*="    newpos_w(); MECHO; MRETURN(MULA)
"/="    newpos_w(); MECHO; MRETURN(DIVA)
"%="    newpos_w(); MECHO; MRETURN(MODA)
"^="    newpos_w(); MECHO; MRETURN(MODA)

"m"{wt}*"->"{wt}*	{
			newpos_w();
			if (subst_flag)
				{ REJECT; }
			MECHO;
			MRETURN(MEMVARPTR)
			}
"memv"(a|ar)?{wt}*"->"{wt}*   {
			newpos_w();
			if (subst_flag)
				{ REJECT; }
			MECHO;
			MRETURN(MEMVARPTR)
			}

"m"{wt}*".or."	newpos_w(); PECHO("m"); yyless(yyleng-4); if (!subst_flag) yylval.string=strdup("M"); MRETURN(NAME)
"m"{wt}*".and."	newpos_w(); PECHO("m"); yyless(yyleng-5); if (!subst_flag) yylval.string=strdup("M"); MRETURN(NAME)
"m"{wt}*".T."	newpos_w(); PECHO("m"); yyless(yyleng-3); if (!subst_flag) yylval.string=strdup("M"); MRETURN(NAME)
"m"{wt}*".F."	newpos_w(); PECHO("m"); yyless(yyleng-3); if (!subst_flag) yylval.string=strdup("M"); MRETURN(NAME)
"m"{wt}*".Y."	newpos_w(); PECHO("m"); yyless(yyleng-3); if (!subst_flag) yylval.string=strdup("M"); MRETURN(NAME)
"m"{wt}*".N."	newpos_w(); PECHO("m"); yyless(yyleng-3); if (!subst_flag) yylval.string=strdup("M"); MRETURN(NAME)
"m"{wt}*"."{wt}*	newpos_w(); MECHO;  MRETURN(MEMVARPTR)

"f"{wt}*"->"{wt}*   {
	/*printf("found 'f'");*/
	newpos_w(); PECHO("f"); yyless(yyleng-2); if (!subst_flag) yylval.string=strdup("F"); MRETURN(NAME)
	}
"_field"{wt}*"->"{wt}*	{
	/*printf("found '_field'");*/
	newpos_w(); MECHO;  MRETURN(FIELDPTR)
	}
"field"{wt}*"->"{wt}* {
	/*printf("found 'field'");*/
	newpos_w(); MECHO;  MRETURN(FIELDPTR)
	}

"->"    {
		newpos_w();
		/*vo_printf(4,0,"found ->: bad_translate=%d callName=%s\n",
			bad_translate, callName?callName:"");*/
		if ( !bad_translate && !callName && (curCommand=findFirst_Translate(yytext,&commandInd)) )
		{
			callEol=0;
			free(callName);
			callName=mem_dup(yytext, yyleng);
			command_state = YYSTATE;
			command_next_state = YYSTATE;
			bol_translate = YY_AT_BOL();
			vo_printf(6,0, "find_translate1 %s state=%d next=%d\n", curCommand->name, command_state, command_next_state);
			match_translate = 1;
			BEGIN(commandCall);
		}
		else
		{
			MECHO; /*BEGIN(INITIAL);*/ MRETURN(RPTR)
		}
	}
"=>"    newpos_w(); MECHO; MRETURN(REPTR)

"++"    newpos_w(); MECHO; MRETURN(INCR)
"--"    newpos_w(); MECHO; MRETURN(DECR)

".OR."  newpos_w(); MECHO; MRETURN(OR)
".AND." newpos_w(); MECHO; MRETURN(AND)
".NOT."	newpos_w(); MECHO; MRETURN(NOT)
">="    newpos_w(); MECHO; MRETURN(GE)
"<="    newpos_w(); MECHO; MRETURN(LE)
"=="    newpos_w(); MECHO; MRETURN(EQ)
"!="    newpos_w(); MECHO; MRETURN(NE)
"<>"    newpos_w(); MECHO; MRETURN(NE)
"#"    newpos_w(); MECHO; MRETURN(NE)
"!"	newpos_w(); MECHO; MRETURN(NOT)
"::"	{
		newpos_w();
		if ( !bad_translate && !callName && (curCommand=findFirst_Translate(yytext,&commandInd)) )
		{
			callEol=0;
			free(callName);
			callName=mem_dup(yytext, yyleng);
			command_state = YYSTATE;
			command_next_state = YYSTATE;
			bol_translate = YY_AT_BOL();
			vo_printf(6,0, "find_translate1 %s state=%d next=%d\n", curCommand->name, command_state, command_next_state);
			match_translate = 1;
			BEGIN(commandCall);
		}
		else
		{
			MECHO; MRETURN(DSEMICOLON)
		}
}



0x{hex}+  {
	unsigned long h;
	char buf[16];
	newpos_w();
	h = strtoul(yytext, 0, 16);
	snprintf(buf, sizeof(buf), "%lu", h);
	if (!subst_flag) yylval.string=strdup(buf);
	MECHO;
	MRETURN(NUMBER)
	}

{dig}+\.({dig}+)?  {
	newpos_w();
	if (!subst_flag) yylval.string=strdup(yytext);
	MECHO;
	MRETURN(NUMBER)
	}

\.{dig}+  {
	newpos_w();
	if (!subst_flag) yylval.string=strdup(yytext);
	MECHO;
	MRETURN(NUMBER)
	}

{dig}+\.[eE][-+]?{dig}+    |
{dig}*\.{dig}+[eE][-+]?{dig}+  {
	newpos_w();
	if (!subst_flag) yylval.string=strdup(yytext);
	MECHO;
	MRETURN(NUMBER)
	}

{dig}+/\.{id}      {
	newpos_w();
	if (!subst_flag) yylval.string=strdup(yytext);
	MECHO;
	MRETURN(NUMBER)
	}

{dig}+\.([eE][-+]?{dig}+)?{id}    |
{dig}*\.{dig}+([eE][-+]?{dig}+)?{id}  {
	newpos_w();
	MECHO;
	yyerror("invalid chars in number");
	}

{dig}+{wt}*"->"    {
		int l1, l2;
		char *s;
		for(s=yytext; (s=strchr(s,'\n')); s++)
			newline();
		l1=strspn(yytext, " \t");
		l2=strcspn(yytext+l1, " \t;->");
		yylval.string=mem_Dup(yytext+l1,l2);
		unput('>');
		unput('-');
		newpos();
		PECHO(yylval.string);
		MRETURN(NAME)
	}

{dig}+    {
	newpos_w();
	if (!subst_flag) yylval.string=strdup(yytext);
	MECHO;
	MRETURN(NUMBER)
	}


\"      {
	newpos(); string_buf_ptr = string_buf; strLine=clic_line;
	strPos=clic_pos; strFile=currentFile();
	str_caller = YYSTATE; str_return = 1; BEGIN(lexString);
	}

\'      {
	newpos(); string_buf_ptr = string_buf; strLine=clic_line;
	strPos=clic_pos; strFile=currentFile(); str_caller = YYSTATE;
	str_return = 1; BEGIN(lexString1);
	}

\`      {
	newpos(); string_buf_ptr = string_buf; strLine=clic_line;
	strPos=clic_pos; strFile=currentFile(); str_caller = YYSTATE;
	str_return = 1; BEGIN(lexString3);
	}

<lexString,lexString1,lexString2,lexString3>\&\\. |
<lexString,lexString1,lexString2,lexString3>\&        {
	char *clic_text_ptr = yytext;
	newpos_t();
	while ( *clic_text_ptr )
		addch_string_buf(*clic_text_ptr++);
	}

<lexString,lexString1,lexString2,lexString3>\&\\{odig}{odig}{odig} {
	newpos_t();
		addch_string_buf(strtol(yytext+2,0,8));
	}

<lexString>\"        { /* saw closing clic_quote - all done */
	BEGIN(str_caller);
	newpos_w();
	if (str_return==1)
	{
		addch_string_buf(0);
		recodeString(string_buf);
		SECHO('"','"');
		if (!subst_flag)
			yylval.string=strdup(string_buf);
		MRETURN(STRING)
	}
	else if (str_return==0)
	{
		addch_string_buf('"');
		addch_string_buf( 0 );
		string_buf_ptr--;
		recode_string(string_buf);
	}
	else if (str_return==2)
	{
		addch_string_buf('"');
		addch_string_buf( 0 );
		string_buf_ptr--;
		recode_string(string_buf);
		insert_Coll(& curMacroFunc->parts, new_MacroPart(strdup(string_buf),0));
	}
	}

<lexString1>\'        {
	BEGIN(str_caller);
	newpos_w();
	if (str_return==1)
	{
		addch_string_buf(0);
		recodeString(string_buf);
		SECHO('\'','\'');
		if (!subst_flag) yylval.string=strdup(string_buf);
		MRETURN(STRING)
	}
	else if (str_return==0)
	{
		addch_string_buf('\'');
		addch_string_buf(0);
		string_buf_ptr--;
		recode_string(string_buf);
	}
	else if (str_return==2)
	{
		addch_string_buf('\'');
		addch_string_buf(0);
		string_buf_ptr--;
		recode_string(string_buf);
		insert_Coll(& curMacroFunc->parts, new_MacroPart(strdup(string_buf),0));
	}
	}

<lexString3>\`        {
	char buf[32], *s;
	BEGIN(str_caller);
	newpos_w();
	addch_string_buf(0);
	recodeString(string_buf);
	for(s = string_buf; *s; ++s)
		*s = toupper(*s);
	snprintf(buf, sizeof(buf), "%ld", _clip_hashstr(string_buf));
	PECHO(buf);
	if (!subst_flag)
		yylval.string=strdup(buf);
	MRETURN(NUMBER)
	}

<lexString2>\]        {
	BEGIN(str_caller);
	newpos_w();
	if (str_return==1)
	{
		addch_string_buf(0);
		recodeString(string_buf);
		SECHO('[',']');
		if (!subst_flag)
			yylval.string=strdup(string_buf);
		MRETURN(ISTRING)
	}
	else if (str_return==0)
	{
		addch_string_buf(']');
		addch_string_buf(0);
		string_buf_ptr--;
		recode_string(string_buf);
	}
	else if (str_return==2)
	{
		addch_string_buf(']');
		addch_string_buf(0);
		string_buf_ptr--;
		recode_string(string_buf);
		insert_Coll(& curMacroFunc->parts, new_MacroPart(strdup(string_buf),0));
	}
	}

<lexString,lexString1,lexString2,lexString3>{eol}        {
	yyerror("newline in string");
	newline();
	BEGIN(INITIAL);
	/*addch_string_buf(*yytext);*/
	}

<lexString>[^\&\r\n\"]+        {
	char *clic_text_ptr = yytext;
	newpos_t();
	while ( *clic_text_ptr )
		addch_string_buf(*clic_text_ptr++);
	}

<lexString1>[^\&\r\n\']+        {
	char *clic_text_ptr = yytext;
	newpos_t();
	while ( *clic_text_ptr )
		addch_string_buf(*clic_text_ptr++);
	}


<lexString2>[^\&\r\n\]]+        {
	char *clic_text_ptr = yytext;
	newpos_t();
	while ( *clic_text_ptr )
		addch_string_buf(*clic_text_ptr++);
	}

<lexString3>[^\&\r\n\`]+        {
	char *clic_text_ptr = yytext;
	newpos_t();
	while ( *clic_text_ptr )
		addch_string_buf(*clic_text_ptr++);
	}

"/*"      	{	newpos(); comment_depth++; comment_caller=YYSTATE;
			commentLine=clic_line; commentPos=clic_pos; commentFile=currentFile();
			comment_eol = 0;
			BEGIN(lexComment);
		}
<skipState>"/*" {
		newpos(); comment_depth++; comment_caller=skipState;
		commentLine=clic_line; commentPos=clic_pos; commentFile=currentFile();
		comment_eol = 0;
		BEGIN(lexComment);
		}

<INITIAL,skipState>"*/"	newpos(); yyerror("end-of-comment without begin");

<lexComment>[^*/\r\n\032]*      newpos();
<lexComment>"*"+[^*/\r\n\032]*  newpos();
<lexComment>{eol}             	newline(); comment_eol++;
<lexComment>"/*"      		newpos(); comment_depth++;
<lexComment>"*"+"/"	{
		newpos();
		if (--comment_depth==0)
			{
				BEGIN(comment_caller);
				/*if (comment_eol)
					my_yy_set_bol(1);*/
			}
		}
<lexComment>.			newpos();

<skipState>"//"		newpos(); comment_caller=skipState; full_string_flag=1; BEGIN(lexEolComment);
<skipState>"&&"		newpos(); comment_caller=skipState; full_string_flag=1; BEGIN(lexEolComment);
<skipState>^[ \t]*"*"{noteol}*	newpos(); comment_caller=skipState; full_string_flag=1; BEGIN(lexEolComment);
<lexEolComment>{noteol}*   newpos();
<lexEolComment>{eol}	{
		if ( !full_string_flag )
		{
			put_char('\n');
		}
		else
			newline();
		BEGIN(comment_caller);
	}

"@@"			newpos(); BEGIN(ctextEolState);
<ctextEolState>[^\n]*\n {
				newline();
				BEGIN(INITIAL);
				if (!subst_flag) yylval.string=strdup(yytext);
				return CTEXT;
			}

{wt}*"@{"		{
				newpos();
				ctextLen=256;
				ctextPos=0;
				ctextPtr=(char*)malloc(ctextLen);
				ctextQuota='{';
				/*for(i=0;i<yyleng-2;i++)
					addCtext(yytext[i]);*/
				addCtext('{');
				BEGIN(ctextState);
			}
"@("			{
				newpos();
				ctextLen=256;
				ctextPos=0;
				ctextPtr=(char*)malloc(ctextLen);
				ctextQuota='(';
				BEGIN(ctextState);
			}
<ctextState>[^@]*	{
			  char *s;
			  for( s=yytext ; *s ; s++ )
			   {
				if (*s=='\n')
				  clic_line++;
				addCtext(*s);
			   }
			  newpos();
			}
<ctextState>"@}"     	{
			  newpos();
			  if (ctextQuota!='{')
			  {
				addCtext('@');
				addCtext('}');
			  }
			  else
			  {
				addCtext('}');
				addCtext('\n');
				ctextPtr[ctextPos]=0;
				BEGIN(INITIAL);
				yylval.string=ctextPtr;
				return CTEXT;
			  }
			}
<ctextState>"@)"     	{
			  newpos();
			  if (ctextQuota!='(')
			  {
			  addCtext('@');
			  addCtext(')');
			  }
			  else
			  {
			  ctextPtr[ctextPos]=0;
			  BEGIN(INITIAL);
			  yylval.string=ctextPtr;
			  return CTEXT;
			  }
			}
<ctextState>"@"/[^}]	{
			  char *s;
			  for( s=yytext ; *s ; s++ )
			   {
				if (*s=='\n')
				  clic_line++;
				addCtext(*s);
			   }
			  newpos();
			}

"@}"			newpos();  yyerror("'@}' without '@{'");


<keyword>{wt}+	{newpos(); MECHO;

/*
<keyword>static{wt}+func(t|ti|tio|tion)?/{delim}	newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(SFUNCTION)
<keyword>static{wt}+proc(e|ed|edu|edur|edure)?/{delim}	newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(SPROCEDURE)
<keyword>stat(i|ic)?/{delim}	newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(STATIC)
<static_words>.		BEGIN(INITIAL); yyless(0);
*/
}

<keyword>stat(i|ic)?/{delim}	newpos_w(); MECHO; BEGIN(static_words); MRETURN(STATIC)
<static_words>func(t|ti|tio|tion)?/{delim}	newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(FUNCTION)
<static_words>proc(e|ed|edu|edur|edure)?/{delim}	newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(PROCEDURE)
<static_words>{wt}+		newpos();MECHO;
<static_words>.		BEGIN(INITIAL); yyless(0);


<keyword>publ(i|ic)?/{delim}   	newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(PUBLIC)
<keyword>priv(a|at|ate)?/{delim}   	newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(PRIVATE)
<keyword>decl(a|ar|are)?/{delim}   	newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(PRIVATE)
<keyword>dimension/{delim}   	newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(DIMENSION)
<INITIAL,keyword>local?/{delim}	newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(LOCAL)
<keyword>exte(r|rn|rna|rnal)?/{delim}	newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(EXTERN)
<keyword>para(m|me|met|mete|meter|meters)?/{delim}	newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(PARAMETERS)
<keyword>retu(r|rn)?	newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(RETURN)

<keyword>request/{delim}	newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(REQUEST)
<keyword>announce/{delim}	newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(ANNOUNCE)


<keyword>memvar/{delim}	{
	newpos_w(); MECHO;
	BEGIN(INITIAL); MRETURN(MEMVAR)
	}

<keyword>field/{delim}	{
	/* [^-_a-ZA_Z0-9] */
	newpos_w();
	MECHO;
	BEGIN(field_words);
	/*printf("FIELD: %s\n", yytext);*/
	MRETURN(FIELD)
	}
<keyword>for/{delim}	newpos_w(); MECHO; BEGIN(for_words); MRETURN(FOR)
<keyword>next/{delim}	newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(NEXT)
<keyword>text{wt}*{id}{wt}*","{wt}*{id}{wt}*{eol} {
		char *s = yytext+4, *e;
		if (subst_flag)
		{
			mecho();
			break;
		}
		for(;isspace(*s); ++s)
			;
		for(e=s; is_id(*e);++e)
			;
		FREE(textout);
		FREE(textend);
		textout = mem_dup(s, e-s);
		for(s=e; *s!=','; ++s)
			;
		for(++s; isspace(*s);++s)
			;
		for(e=s; is_id(*e);++e)
			;
		textend = mem_dup(s, e-s);
		newline();
		freeAll_Coll(&text);
		BEGIN(textState);
	}
<textState>^{wt}*endtext{wt}*{eol}	{
	int i;
	newline();
	if (subst_flag)
	{
		mecho();
		break;
	}

	/*for(i=text.count-1; i>=0;--i)*/
	for(i=0; i<text.count_of_Coll;++i)
	{
		clic_line --;
		put_str(textout);
		put_char('(');
		put_str_free(quot_str((char*) text.items_of_Coll[i], 0));
		put_char(')');
		put_char('\n');
	}

	clic_line --;
	put_str(textend);
	put_char('(');
	put_char(')');
	put_char('\n');


	BEGIN(INITIAL);
	}

<textState>^{noteol}*{eol}	{
		int l=yyleng;
		newline();
		if (subst_flag)
		{
			mecho();
			break;
		}
		for(l=yyleng; l>0 ; --l)
		{
			int s= yytext[l-1];
			if (s!='\n'&&s!='\r'&&s!=032)
				break;
		}
		append_Coll(&text, mem_dup(yytext,l));
	}

<keyword>begin?({wt}+sequ(e|en|enc|ence)?)?/{delim}  newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(BEGSEQ)
<keyword>reco(v|ve|ver)?/{delim}	newpos_w(); MECHO; BEGIN(recover_words); MRETURN(RECOVER)
<INITIAL,keyword>break{wt}*"("	newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(EBREAK)
<keyword>break?/{delim}	newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(BREAK)
<recover_words>using/{delim} newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(USING)

<keyword>func(t|ti|tio|tion)?/{delim}	newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(FUNCTION)
<keyword>proc(e|ed|edu|edur|edure)?/{delim}	newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(PROCEDURE)
<keyword>init{wt}+proc(e|ed|edu|edur|edure)?/{delim}	newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(IPROCEDURE)
<keyword>exit{wt}+proc(e|ed|edu|edur|edure)?/{delim}	newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(EPROCEDURE)
<keyword>retu(r|rn)/{delim}   	newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(RETURN)
<keyword>end/{delim}		newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(END)


<for_words>if/{delim}	newpos_w(); MECHO; MRETURN(IF)
<keyword>if/{delim}	newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(IF)
<for_words>iif/{delim}	newpos_w(); MECHO; MRETURN(IIF)
iif/{delim}	newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(IIF)
<keyword>elseif/{delim}		newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(ELSEIF)
<keyword>else/{delim}		newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(ELSE)
<keyword>endif?/{delim}		newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(ENDIF)


<keyword>do/{delim}		newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(DO)
<keyword>call/{delim}		newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(CALL)
<keyword,INITIAL>with/{delim}		newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(WITH)
<keyword>while?/{delim}		newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(WHILE)
<keyword>do{wt}+while?/{delim}	newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(WHILE)
<keyword>enddo?/{delim}		newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(ENDDO)
<keyword>loop/{delim}		newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(LOOP)
<keyword>exit/{delim}		newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(EXIT)

<keyword>do{wt}+case/{delim}	newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(DOCASE)
<keyword>case/{delim}		newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(CASE)
<keyword>endc(a|as|ase)?/{delim}	newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(ENDCASE)
<keyword>othe(r|rw|rwi|rwis|rwise)?/{delim}	newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(OTHERWISE)
<keyword>switch/{delim}	newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(SWITCH)
<keyword>endswitch/{delim}	newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(ENDSWITCH)



<keyword>{id}{wt}*"("	{
		/*yylval.string=mem_Dup(yytext,yyleng);*/
		newpos_w();
		BEGIN(INITIAL);
		/*MECHO;*/
		if (callName)
		{
			/*yyerror("id (: non-matched command '%s'", yytext);
			print_candidates(callName);
			return 0;*/
			free(callName);
			callName=0;
		}
		else
		{
			/*MRETURN(NAME)*/
		}
			/*yyless(0);*/
			REJECT;
	}

<keyword>{id}	{
		/*yylval.string=mem_Dup(yytext,yyleng);*/
		newpos_w();
		BEGIN(INITIAL);
		/*MECHO;*/
		if (callName)
		{
			/*yyerror("id: non-matched command '%s'", yytext);*/
			yywarning("id: non-matched command '%s'", yytext);
			print_candidates(callName);
			yyless(0);
			/*return 0;*/
		}
		else
		{
			/*MRETURN(NAME)*/
			yyless(0);
		}
	}


<keyword>.	{
		BEGIN(INITIAL);
		MECHO;
		newpos_w();
		if (callName)
		{
			/*yyerror("non-matched command '%s'", yytext);*/
			yywarning("non-matched command '%s'", yytext);
			print_candidates(callName);
			MRETURN(*yytext)
			/*return 0;*/
		}
		else
		{
			MRETURN(*yytext)
		}
	}

<field_words>in/{delim} newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(IN);
<for_words>to/{delim} newpos_w(); MECHO; MRETURN(TO);
<for_words>step/{delim} newpos_w(); MECHO; MRETURN(STEP);
<for_words>in/{delim} newpos_w(); MECHO; MRETURN(IN);
<for_words>keys/{delim} newpos_w(); MECHO; MRETURN(KEYS);

<vardef_words>as/{delim} newpos_w(); MECHO; MRETURN(AS);
<vardef_words>ref/{delim} newpos_w(); MECHO; MRETURN(REF);

".t."/{delim}		newpos_w(); MECHO; MRETURN(TRUE)
".f."/{delim}		newpos_w(); MECHO; MRETURN(FALSE)
".y."/{delim}		newpos_w(); MECHO; MRETURN(TRUE)
".n."/{delim}		newpos_w(); MECHO; MRETURN(FALSE)

nil/{delim}		newpos_w(); MECHO; MRETURN(NIL)

"$*"/{delim}		newpos_w(); MECHO; MRETURN(PALL)
"$<"/{delim}		newpos_w(); MECHO; MRETURN(PSHIFT)

"_GET_"/{delim}		newpos_w(); MECHO; MRETURN(CGET)
"pcount"{wt}*"("{wt}*")"	newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(PCOUNT)
"param"{wt}*"("	newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(PARAMBEG)
"codestr"{wt}*"("	newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(CODESTR)
"codeblock"{wt}*"("	newpos_w(); MECHO; BEGIN(INITIAL); MRETURN(CODEBLOCK)
HASH_{id}  	{
		char buf[32];
		snprintf(buf, sizeof(buf), "%ld", _clip_hashstr(yytext+5));
		PECHO(buf);
		if (!subst_flag) yylval.string=strdup(buf);
		MRETURN(NUMBER)
		}
__FILE__  	{ char *file=((IncludeFile*) includes.items_of_Coll[includes.count_of_Coll-1])->name;
		  sprintf(string_buf, "\"%s\"", file); put_buf(); }
__BASE_FILE__  	{ char *file=((IncludeFile*) includes.items_of_Coll[0])->name;
		  sprintf(string_buf, "\"%s\"", file); put_buf(); }
__LINE__  	{ sprintf(string_buf, "\"%ld\"", clic_line ); put_buf(); }

__VERSION__	{ sprintf(string_buf, "\"%s\"", CLIP_VERSION ); put_buf(); }

__SYSTEM__  	{ sprintf(string_buf, "\"%s\"", OSNAME  ); put_buf(); }

__DATE__  	{ time_t timer; time(&timer);
		  strftime(string_buf, 32, "\"%b %d %Y\"", localtime(&timer) );
		  put_buf(); }

__TIME__  	{ time_t timer; time(&timer);
		  strftime(string_buf, 32, "\"%H:%M:%S\"", localtime(&timer) );
		  put_buf(); }
__FIELD__	{ newpos_w(); MECHO; field_flag = !field_flag; }

__CHARSET__  	{
		char *s = targetCharset;
		if (!s || (!strcasecmp(s, "c")))
			s = sourceCharset;
		if (!s)
			s = "c";
		sprintf(string_buf, "\"%s\"", s);
		put_buf();
		}

__CLIP_MODULE__ {
			sprintf(string_buf, "\"%s\"", CLIP_MODULE);
			put_buf();
		}

^{wt}*"#"{wt}*error{wt}*      	newpos_t(); message_caller=0; BEGIN(messageState);
^{wt}*"#"{wt}*stdout{wt}*	newpos_t(); message_caller=1; BEGIN(messageState);
<messageState>[^\r\n\032&/]	newpos_t(); yymore();
<messageState>{eol}	{
		newline();
		if (message_caller==0)
			yyerror("%s", yytext);
		else if (message_caller==1)
			printf("%s", yytext);
		else if (message_caller==2)
		{
			int argc, l;
			char *str = strdup(yytext);
			char *argv[32], *s;
			for(l=yyleng-1; l>=0; l--)
				if (yytext[l]=='\n'||yytext[l]=='\r')
					yytext[l]=0;
				else
					break;

			for (argc = 0; argc < 32 && (s = strtok(str, " \t\r\n")) != NULL; ++argc)
			{
				argv[argc] = s;
				str = NULL;
			}
			get_opt(argc, argv);
		}
		else if (message_caller==3)
		{
			char *b, *e, *s;
			int l;
			for(b=yytext; *b ; b++)
				if (!isspace(*b))
					break;
			for(e=yytext+yyleng; e>b; e--)
				if (!isspace(e[-1]))
					break;
			l = e-b;
			s = (char*) malloc(l+1);
			memcpy(s, b, l);
			s[l] = 0;
			set_charset(s, fileList.items_of_Coll[fileList.count_of_Coll-1]);
		}
		else if (message_caller==4)
		{
			char *b, *e, *s;
			int l;
			for(b=yytext, e=yytext+yyleng; b<e ; )
			{
				l = strspn(b, " \t\n\r");
				b += l;
				if (b>=e)
					break;
				l = strcspn(b, " \t\n\r");
				if (!l)
					break;
				s = (char*) malloc(l+1);
				memcpy(s, b, l);
				s[l] = 0;
				vo_printf(4, 0, "add reffunc '%s'\n", s);
				if (insert_Coll(&reffuncs, s))
				{
					for(;*s; s++)
						*s = toupper(*s);
				}
				else
					free(s);
				b += l;
			}
		}
		BEGIN(INITIAL);
	}
<messageState>{eolcomment}{noteol}*{eol} {
		newline();
		if (!message_caller)
			yyerror("%s", yytext);
		else
			printf("%s", yytext);
		BEGIN(INITIAL);
	}
<messageState>"/*"      	{	newpos(); comment_depth++; comment_caller=YYSTATE;
			commentLine=clic_line; commentPos=clic_pos; commentFile=currentFile();
			comment_eol = 0;
			BEGIN(lexComment);
		}
<messageState>.		newpos_t(); yymore();


^{wt}*"#"{wt}*include{wt}*      newpos_t(); curInclude=new_IncludeFile(0,clic_line); BEGIN(includeState);

<includeState>[^\"\'<\r\n\032]*     	newpos_t();
<includeState>\"[^\"]*\" |
<includeState>\'[^\']*\' |
<includeState><[^\>]*>		{
				char *name=strdup(yytext+1);
				name[strlen(name)-1]=0;
				curInclude->name=name;
				for(;*name;++name)
					switch(*name)
					{
					case '\\':
						*name = '/';
						break;
					default:
						*name = tolower(*name);
					}
				curInclude->type=*yytext;
				newpos_t();
				}

<includeState>{eol}     {
			if (!startCurInclude())
				yyterminate();
			 }
<includeState>{eolcomment}{noteol}*{eol} {
			if (!startCurInclude())
				yyterminate();
			 }

<includeState>"/*"      	{	newpos(); comment_depth++; comment_caller=YYSTATE;
			commentLine=clic_line; commentPos=clic_pos; commentFile=currentFile();
			comment_eol = 0;
			BEGIN(lexComment);
		}

^{wt}*"#"{wt}*ifdef{wt}+       	{
					  newpos_t(); ifdef_caller=INITIAL;
					  curIfdef=new_IfdefState(0,1);
					  BEGIN(ifdefState);
				}
<skipState>^{wt}*"#"{wt}*ifdef{wt}+  	{
					  newpos_t(); ifdef_caller=skipState;
					  curIfdef=new_IfdefState(0,1);
					  BEGIN(ifdefState);
				}
^{wt}*"#"{wt}*ifndef{wt}+       {
					  newpos_t(); ifdef_caller=INITIAL;
					  curIfdef=new_IfdefState(0,0);
					  BEGIN(ifdefState);
				}
<skipState>^{wt}*"#"{wt}*ifndef{wt}+  	{
					  newpos_t(); ifdef_caller=skipState;
					  curIfdef=new_IfdefState(0,0);
					  BEGIN(ifdefState);
				}

<ifdefState>{notidbeg}+		newpos();
<ifdefState>{id}	{
			newpos();
			if ( findMacro(yytext)
				|| findMacroFunc(yytext) )
			  {
			   if (curIfdef->ifdef)
				 curIfdef->condition|=1;
			  }
			else
			  {
			   if ( !curIfdef->ifdef )
				 curIfdef->condition|=1;
			  }
			}
<ifdefState>{eol}          {
			newline();
			curIfdef->state=ifdef_caller;
			insert_Coll(&ifdefs, curIfdef);
			if ( curIfdef->condition)
				BEGIN(ifdef_caller);
			else
				BEGIN(skipState);
			}
<ifdefState>{eolcomment}{noteol}*{eol} {
			newline();
			curIfdef->state=ifdef_caller;
			insert_Coll(&ifdefs, curIfdef);
			if ( curIfdef->condition)
				BEGIN(ifdef_caller);
			else
				BEGIN(skipState);
			}
<ifdefState>"/*"      	{	newpos(); comment_depth++; comment_caller=YYSTATE;
			commentLine=clic_line; commentPos=clic_pos; commentFile=currentFile();
			comment_eol = 0;
			BEGIN(lexComment);
		}

<ifdefState>.		newpos_t();

<INITIAL,skipState>^{wt}*"#"{wt}*else{noteol}*{eol}   {
			newline();
			if (ifdefs.count_of_Coll<1)
			  yyerror("#else without #ifdef or #ifndef");
			else
			  {
				curIfdef=(IfdefState*)ifdefs.items_of_Coll[ifdefs.count_of_Coll-1];
				curIfdef->elseline=clic_line;
				if (curIfdef->condition)
				  BEGIN(skipState);
				else
				  BEGIN(curIfdef->state);
			  }
			}

<INITIAL,skipState>^{wt}*"#"{wt}*endif{noteol}*{eol}  {
			newline();
			if (ifdefs.count_of_Coll<1)
			  yyerror( "#endif without #ifdef or #ifndef");
			else
			  {
				curIfdef=(IfdefState*)ifdefs.items_of_Coll[ifdefs.count_of_Coll-1];
				BEGIN(curIfdef->state);
				atFree_Coll(&ifdefs, ifdefs.count_of_Coll-1);
			  }
			}

<skipState>[^\r\n\032#]*/"/*"    newpos();
<skipState>[^\r\n\032#]*/"//"    newpos();
<skipState>{eol}		   newline();
<skipState>.		   newpos();

^{wt}*"#"{wt}*undef{wt}+ { newpos_t(); BEGIN(undefState); }

<undefState>"//".*/\r?\n      newpos();
<undefState>"//".*/\032      newpos();
<undefState>{notidbeg}+    newpos();
<undefState>{id}	{
			newpos();
			if ( (curMacroFunc=findMacroFunc(yytext))!=0 )
			{
			   remove_Coll(&macroFuncs,curMacroFunc);
			   remove_Coll(&xmacroFuncs,curMacroFunc);
			}
			if ( (curMacro=findMacro(yytext))!=0 )
			{
			   remove_Coll(&macros,curMacro);
			   remove_Coll(&xmacros,curMacro);
			}
			}

<undefState>{eol}		newline(); BEGIN(INITIAL);
<undefState>{eolcomment}{noteol}*{eol} {
				newline(); BEGIN(INITIAL);
			}
<undefState>"/*"      	{	newpos(); comment_depth++; comment_caller=YYSTATE;
			commentLine=clic_line; commentPos=clic_pos; commentFile=currentFile();
			comment_eol = 0;
			BEGIN(lexComment);
		}
<undefState>.		newpos();


{wt}*"#"{wt}*def(i|in|ine){wt}* { newpos_t(); xdefine=0; BEGIN(defineState); }
{wt}*"#"{wt}*xdefine{wt}* { newpos_t(); xdefine=1; BEGIN(defineState); }

<defineState>;{eol}	 {
		   newline();
		 }

<defineState>{eol}	 {
			newline();
			yyerror("invalid #define directive");
			BEGIN(INITIAL);
		 }

<defineState>{eolcomment}{noteol}*{eol} {
			newline();
			yyerror("invalid #define directive");
			BEGIN(INITIAL);
		}
<defineState>"/*"      	{	newpos(); comment_depth++; comment_caller=YYSTATE;
			commentLine=clic_line; commentPos=clic_pos; commentFile=currentFile();
			comment_eol = 0;
			BEGIN(lexComment);
		}

<defineState>{id}"("  {
		   char *name=strdup(yytext);
		   newpos();
		   name[strlen(name)-1]=0;
			if ( (curMacroFunc=findMacroFunc(name))!=0 )
			{
			   yywarning("macro function redefined");
			   remove_Coll(&macroFuncs,curMacroFunc);
			   remove_Coll(&xmacroFuncs,curMacroFunc);
			}
			else if ( (curMacro=findMacro(name))!=0 )
			{
			   yywarning("macro redefined");
			   remove_Coll(&macros,curMacro);
			   remove_Coll(&xmacros,curMacro);
			}
			curMacroFunc=new_MacroFunc(name);
		   BEGIN(defineArgs);
		   }



<defineState>{id}  {
		   int c;
		   newpos();
		   *string_buf=0;
		   string_buf_ptr=string_buf;
			if ( (curMacro=findMacro(yytext))!=0 )
			{
			   yywarning("macro redefined");
			   remove_Coll(&macros,curMacro);
			   remove_Coll(&xmacros,curMacro);
			}
			else if ( (curMacroFunc=findMacroFunc(yytext))!=0 )
			{
			   yywarning("macro function redefined");
			   remove_Coll(&macroFuncs,curMacroFunc);
			   remove_Coll(&xmacroFuncs,curMacroFunc);
			}
			curMacro=new_Macro(strdup(yytext),0);

		   while( (c=input())==' ' || c=='\t' )
			;
		   unput(c);
		   BEGIN(defineBody);
		   }

<defineState>"$"{id}  {
		   int c;
		   char *text;
		   newpos();
		   text = getenv(yytext+1);
		   if (!text)
			text = yytext;

		   string_buf[0] = 0;
		   string_buf_ptr=string_buf;
			if ( (curMacro=findMacro(text))!=0 )
			{
			   yywarning("macro redefined");
			   remove_Coll(&macros,curMacro);
			   remove_Coll(&xmacros,curMacro);
			}
			else if ( (curMacroFunc=findMacroFunc(text))!=0 )
			{
			   yywarning("macro function redefined");
			   remove_Coll(&macroFuncs,curMacroFunc);
			   remove_Coll(&xmacroFuncs,curMacroFunc);
			}
			curMacro=new_Macro(strdup(text),0);
		   while( (c=input())==' ' || c=='\t' )
			;
		   unput(c);
		   BEGIN(defineBody);
		   }

<defineArgs>{id}   { newpos();
			 insert_Coll(& curMacroFunc->macroArgs, strdup(yytext));
		   }


<defineArgs>{wt}*")"    {
			 int c;
			 newpos_t();
			 while( (c=input())==' ' || c=='\t' ); unput(c);
			 BEGIN(defineFuncBody);
		   }

<defineArgs>[ \t,]*  newpos();

<defineArgs>;{wt}*{eol}   newline();

<defineArgs>{eol}   {
			newline();
			yyerror("invalid #define directive");
			BEGIN(INITIAL);
		}

<defineArgs>{eolcomment}{noteol}*{eol} {
			newline();
			yyerror("invalid #define directive");
			BEGIN(INITIAL);
		}
<defineArgs>"/*"      	{	newpos(); comment_depth++; comment_caller=YYSTATE;
			commentLine=clic_line; commentPos=clic_pos; commentFile=currentFile();
			comment_eol = 0;
			BEGIN(lexComment);
		}

<defineBody>[^;&/\r\n\032'"\[]+	 {
		   newpos();
		   add_string_buf(yytext, yyleng);
		 }

<defineBody>;{wt}*{eol}	 {
		   newline();
		 }

<defineBody>\"   {
		newpos();
		addch_string_buf('"');
		/*
		strLine=clic_line; strPos=clic_pos; strFile=currentFile();
		str_caller = YYSTATE; str_return = 0; BEGIN(lexString);
		*/
		}

<defineBody>\'	{
		newpos();
		addch_string_buf('\'');
		/*
		strLine=clic_line; strPos=clic_pos; strFile=currentFile();
		str_caller = YYSTATE; str_return = 0; BEGIN(lexString1);
		*/
		}

<defineBody>\[	{
		newpos();
		addch_string_buf('[');
		/*
		strLine=clic_line; strPos=clic_pos; strFile=currentFile();
		str_caller = YYSTATE; str_return = 0; BEGIN(lexString2);
		*/
/*
<defineBody>"/ *"      	{	newpos(); comment_depth++; comment_caller=YYSTATE;
			commentLine=clic_line; commentPos=clic_pos; commentFile=currentFile();
			comment_eol = 0;
			BEGIN(lexComment);
		}
*/
		}


<defineBody>{eol}	 {
		   newline();
		   while(string_buf_ptr>string_buf && isspace(string_buf_ptr[-1]))
			--string_buf_ptr;
		   *string_buf_ptr=0;
		   recode_string(string_buf);
		   curMacro->body=strdup(string_buf);
		   if (curMacro->xdefine)
			insert_Coll(&xmacros,curMacro);
		   else
			insert_Coll(&macros,curMacro);
		   /*printf("defined macro: '%s'='%s'\n",curMacro->name,  string_buf);*/
		   BEGIN(INITIAL);
		 }

<defineBody>.	 {
		   newpos();
		   addch_string_buf(*yytext);
		 }

<defineFuncBody>{id} {
			int find=0,i;
			newpos();
			for(i=0;!find && i<curMacroFunc->macroArgs.count_of_Coll;i++)
			  if (!strcmp(yytext, (char*)curMacroFunc->macroArgs.items_of_Coll[i]))
			{ insert_Coll(& curMacroFunc->parts,new_MacroPart(0,i)); find=1; }
			if (!find)
			   insert_Coll(& curMacroFunc->parts, new_MacroPart(strdup(yytext),0));
		 }

<defineFuncBody>;{wt}*{eol}	{
		   newline();
		   #if 0
		   insert_Coll(& curMacroFunc->parts, new_MacroPart("\n",0));
		   #endif
		}

<defineFuncBody>{notidbeg}+    {
		   newpos();
		   insert_Coll(& curMacroFunc->parts, new_MacroPart(strdup(yytext),0));
		 }

<defineFuncBody>{wt}+    {
		   newpos();
		   insert_Coll(& curMacroFunc->parts, new_MacroPart(strdup(yytext),0));
		 }

<defineFuncBody>\"   {
		newpos(); string_buf_ptr = string_buf;
		strLine=clic_line; strPos=clic_pos; strFile=currentFile();
		str_caller = YYSTATE; str_return = 2; BEGIN(lexString);
		}

<defineFuncBody>\'	{
		newpos(); string_buf_ptr = string_buf;
		strLine=clic_line; strPos=clic_pos; strFile=currentFile();
		str_caller = YYSTATE; str_return = 2; BEGIN(lexString1);
		}

<defineFuncBody>\[	{
		newpos(); string_buf_ptr = string_buf;
		strLine=clic_line; strPos=clic_pos; strFile=currentFile();
		str_caller = YYSTATE; str_return = 2; BEGIN(lexString2);
		}


<defineFuncBody>{eol}   {
		newline();
		finale_MacroFunc(curMacroFunc);
		if (curMacroFunc->macro.xdefine)
			insert_Coll(&xmacroFuncs, curMacroFunc);
		else
			insert_Coll( &macroFuncs, curMacroFunc);
		BEGIN(INITIAL);
		}
<defineFuncBody>{eolcomment}{noteol}*{eol} {
		newline();
		finale_MacroFunc(curMacroFunc);
		if (curMacroFunc->macro.xdefine)
			insert_Coll(&xmacroFuncs, curMacroFunc);
		else
			insert_Coll( &macroFuncs, curMacroFunc);
		BEGIN(INITIAL);
		}

<defineFuncBody>"/*"      	{	newpos(); comment_depth++; comment_caller=YYSTATE;
			commentLine=clic_line; commentPos=clic_pos; commentFile=currentFile();
			comment_eol = 0;
			BEGIN(lexComment);
		}

<defineFuncBody>.    {
		   newpos();
		   insert_Coll(& curMacroFunc->parts, new_MacroPart(strdup(yytext),0));
		 }


<macroCallBegin>{wt}	newpos_t();
<macroCallBegin>{eol}	newline();
<macroCallBegin>"("{wt}* 	{
	newpos_t();
	string_buf[0]=0;
	string_buf_ptr=string_buf;
	clic_quote=0;
	clic_brack1=clic_brack2=clic_brack3=0;
	freeAll_Coll(&macroArgs);
	BEGIN(macroCall);
	}
<macroCallBegin>[^\r\n \t(]+	{ yyerror("invalid macro call"); }
<macroCallBegin>.	{ }

<macroCall>[^, \t\\\"\'\`\[\](){}\r\n\032]+  newpos();add_string_buf(yytext, yyleng);
<macroCall>{wt}*;{wt}*{eol}	newline();
<macroCall>{eol}   newline();
<macroCall>\"   newpos();if (clic_quote=='\"') clic_quote=0; else if (!clic_quote) clic_quote='\"';addch_string_buf(*yytext);
<macroCall>\'   newpos();if (clic_quote=='\'') clic_quote=0; else if (!clic_quote) clic_quote='\'';addch_string_buf(*yytext);
<macroCall>\`   newpos();if (clic_quote=='`') clic_quote=0; else if (!clic_quote) clic_quote='`';addch_string_buf(*yytext);
<macroCall>\[   newpos();if (!clic_quote) clic_brack1++; addch_string_buf(*yytext);
<macroCall>\{   newpos();if (!clic_quote) clic_brack3++; addch_string_buf(*yytext);
<macroCall>\(   newpos();if (!clic_quote) clic_brack2++; addch_string_buf(*yytext);
<macroCall>\]   newpos();if (clic_brack1&&!clic_quote) clic_brack1--; addch_string_buf(*yytext);
<macroCall>\}   newpos();if (clic_brack3&&!clic_quote) clic_brack3--; addch_string_buf(*yytext);
<macroCall>{wt}*,    {
	newpos_t();
	if (clic_quote || clic_brack1 || clic_brack2 || clic_brack3)
	{
		add_string_buf(yytext,yyleng);
	}
	else
	{ /* end of argument */
		*string_buf_ptr=0;
		if ( string_buf[0] )
		  insert_Coll(&macroArgs,strdup(string_buf));
		else
		  insert_Coll(&macroArgs,0);
		string_buf_ptr=string_buf;
		BEGIN(macroDelim);
	}
	}
<macroCall>{wt}*\) 	{
	newpos_t();
	if (clic_brack2 || clic_quote)
	  {
		if (!clic_quote)
			clic_brack2--;

		add_string_buf(yytext,yyleng);
	  }
	else
	  { /* end of macro call */
		int i;
		*string_buf_ptr=0;
		if ( string_buf[0] )
		  insert_Coll(&macroArgs,strdup(string_buf));
		string_buf_ptr=string_buf;
		if (macroArgs.count_of_Coll != curMacroFunc->macroArgs.count_of_Coll)
		  yyerror("invalid number of macro arguments (%d instead %d)",
			macroArgs.count_of_Coll, curMacroFunc->macroArgs.count_of_Coll );
		else
		 for(i=0; i<curMacroFunc->parts.count_of_Coll; i++)
		 {
		MacroPart *mp=(MacroPart*) curMacroFunc->parts.items_of_Coll[i];
		if (mp->text)
		 {
			put_mem(mp->text, mp->len);
		 }
		else
		 { /* argument substitution */
		   char *s0;
		   s0=(char *)macroArgs.items_of_Coll[ mp->argno ];
		   if (s0)
		   {
			put_str(s0);
		   }
		 }
		 }

		BEGIN(macro_call_state);
		/*my_yy_set_bol(macro_call_bol);*/
		vo_printf(6,0,"macro_call_bol = %d\n", macro_call_bol);

	  }
	}
<macroCall>.   newpos(); addch_string_buf(*yytext);


<macroDelim>{wt}*;{wt}*{eol}	newline();
<macroDelim>{wt}*{eol}	newline();
<macroDelim>{wt}*/.	newpos();BEGIN(macroCall);


^{wt}*"#"{wt}*comm(a|an|and)?{wt}+{ccid} {
		char *id;
		int l;
		newpos_t();
		for(id=yytext+yyleng-1; *id!=' ' && *id!='\t';id--)
			;
		id++;
		l=strlen(id);
#if 0
#ifdef FOUR_BYTE
		if (l>4)
			id[4]=0;
#endif
#endif
		curCommand=new_Command(id,0,0);
		curCommandList=&curCommand->pattern;
		curItList=&curCommand->patternIt;
		curCommandPart=NULL;
		optLevel=0;
		BEGIN(commandState);
	}

^{wt}*"#"{wt}*tran(s|sl|sla|slat|slate)?{wt}+({ccid}|"->"|"::") {
		char *id;
		int l;
		newpos_t();
		for(id=yytext+yyleng-1; *id!=' ' && *id!='\t';id--)
			;
		id++;
		l=strlen(id);
#if 0
#ifdef FOUR_BYTE
		if (l>4)
			id[4]=0;
#endif
#endif
		curCommand=new_Command(id,0,1);
		curCommandList=&curCommand->pattern;
		curItList=&curCommand->patternIt;
		curCommandPart=NULL;
		optLevel=0;
		BEGIN(commandState);
	}

^{wt}*"#"{wt}*xcom(m|ma|man|mand)?{wt}+{ccid} {
		char *id;
		newpos_t();
		for(id=yytext+yyleng-1; *id!=' ' && *id!='\t';id--)
			;
		curCommand=new_Command(id+1,1,0);
		curCommandList=&curCommand->pattern;
		curItList=&curCommand->patternIt;
		curCommandPart=NULL;
		optLevel=0;
		BEGIN(commandState);
	}

^{wt}*"#"{wt}*xtra(n|ns|nsl|nsla|nslat|nslate)?{wt}+({ccid}|"->"|"::") {
		char *id;
		newpos_t();
		for(id=yytext+yyleng-1; *id!=' ' && *id!='\t';id--)
			;
		curCommand=new_Command(id+1,1,1);
		curCommandList=&curCommand->pattern;
		curItList=&curCommand->patternIt;
		curCommandPart=NULL;
		optLevel=0;
		BEGIN(commandState);
	}

<commandState>{sm_norm}  {
		insertCommandList( new_CommandPart(SM_norm, marker_id(yytext)));
		newpos();
	}
<commandState>{sm_extend}  {
		insertCommandList( new_CommandPart(SM_extend, marker_id(yytext)));
		newpos();
	}
<commandState>{sm_list}  {
		insertCommandList( new_CommandPart(SM_list,marker_id(yytext)));
		newpos();
	}
<commandState>{sm_enum_beg}  {
		char *s;
		CommandPart *cp;

		Coll *coll=new_Coll(free,back_strsize);
		s = marker_id(yytext);
		cp = new_CommandPart(SM_enum,s);
		cp->option = coll;
		insertCommandList( cp);
		sm_enum_coll = coll;
		BEGIN(smenumState);
		sm_enum_word = 0;
		newpos();
	}

<smenumState>{cid}([ ]*{cid})*	{
		insert_Coll(sm_enum_coll, strdup(yytext));
		newpos();
	}

<smenumState>[ \t]*[,][ \t]*	{
		newpos();
	}

<smenumState>;{wt}*{eol} {
			newline();
			 }

<smenumState>;{wt}*{eolcomment}{noteol}*{eol} {
			newline();
			 }

<smenumState>{wt}*{eol} {
			yyerror("unexpected EOL in SM_ENUM");
			newline();
			 }

<smenumState>"/*"      	{	newpos(); comment_depth++; comment_caller=YYSTATE;
			commentLine=clic_line; commentPos=clic_pos; commentFile=currentFile();
			comment_eol = 0;
			BEGIN(lexComment);
		}

<smenumState>{wt}*">"	{
		BEGIN(commandState);
		newpos();
	}

<smenumState>.	{
		BEGIN(commandState);
		yyerror("bad symbol in SM_ENUM: '%s'", yytext);
		newpos();
	}

<commandState>{sm_patt}  {
		yytext[yyleng-2]=0;
		insertCommandList( new_CommandPart(SM_patt,yytext+2));
		newpos();
	}

<commandState>{wt}+  	{
		newpos_t();
	}

<commandState>"=>"    {
		newpos();
		if (optLevel)
		{
			yyerror("non-balanced '[' in #command");
			optLevel=0;
		}
		finaleCommandList();
		set_last(curCommandList);
		curCommandList=&curCommand->subst;
		curItList=&curCommand->substIt;
		curCommandPart=NULL;
		BEGIN(substState);
	}
<commandState>{id}  	{ /* word */
		char *s;
#if 0
#ifdef FOUR_BYTE
		if ( !curCommand->isX && yyleng>4 )
			yytext[4]=0;
#endif
#endif
		for(s=yytext; *s; ++s)
			*s=toupper(*s);
		insertCommandList( new_CommandPart(SM_word,yytext));
		newpos();
	}
<commandState>\[	{
		Coll *coll=new_Coll(delete_CommandPart,NULL);
		CommandPart *cp=new_CommandPart(SM_option,NULL);
		cp->option=coll;
		newpos();
		insertCommandList( cp);
		cp->prev=curCommandPart;
		cp->prevList=curCommandList;
		curCommandPart=cp;
		curCommandList=coll;
		optLevel++;
	}
<commandState>\]	{
		newpos();
		optLevel--;
		if ( optLevel<0 )
		{
			/*yywarning("non-balanced ']' in #command");*/
			optLevel=0;
			REJECT;
		}
		finaleCommandList();
		curCommandList=curCommandPart->prevList;
		curCommandPart=curCommandPart->prev;
	}
<commandState>[^\.=<>!+\-:*/% \t;\[\]\r\n\032_a-zA-Z]+  	{ /* literal text */
		insertCommandList( new_CommandPart(SM_literal, yytext));
		newpos();
	}
<commandState>">="|"<="|"!="|"->"|"=="|":="|"<>"|"+="|"-="|"/="|"*="|"^="|"%="		{

		insertCommandList( new_CommandPart(SM_literal, yytext));
		newpos();
	}
<commandState>;{wt}*{eol}	{ newline();/* skip \n */ }
<commandState>;{wt}*"&&"{noteol}*{eol}	{ newline(); }
<commandState>;{wt}*"//"{noteol}*{eol}	{ newline(); }
<commandState>{eol}  {
		newline();
		yyerror("unexpected end of #command directive");
		optLevel=0;
		BEGIN(INITIAL);
	}
<commandState>.  	{ /* literal text */
		insertCommandList( new_CommandPart(SM_literal, yytext));
		newpos();
	}

<substState>{tm_norm} {
		insertCommandList( new_CommandPart(TM_norm, marker_id(yytext)));
		newpos();
	}
<substState>{tm_noni} {
		insertCommandList( new_CommandPart(TM_noni, marker_id(yytext)));
		newpos();
	}
<substState>{tm_str} {
		insertCommandList( new_CommandPart(TM_str, marker_id(yytext)));
		newpos();
	}
<substState>{tm_intel} {
		insertCommandList( new_CommandPart(TM_intel, marker_id(yytext)));
		newpos();
	}
<substState>{tm_block} {
		insertCommandList( new_CommandPart(TM_block, marker_id(yytext)));
		newpos();
	}
<substState>{tm_log} {
		insertCommandList( new_CommandPart(TM_log, marker_id(yytext)));
		newpos();
	}
<substState>{tm_var} {
		insertCommandList( new_CommandPart(TM_var, marker_id(yytext)));
		newpos();
	}
<substState>{tm_join} {
		insertCommandList( new_CommandPart(TM_join, yytext));
		newpos();
	}

<substState>{wt}+  	{
		newpos_t();
	}

<substState>\[	{
		Coll *coll=new_Coll(delete_CommandPart,NULL);
		CommandPart *cp=new_CommandPart(TM_option,NULL);
		cp->option=coll;
		newpos();
		insertCommandList( cp);
		cp->prev=curCommandPart;
		cp->prevList=curCommandList;
		curCommandPart=cp;
		curCommandList=coll;
		optLevel++;
	}

<substState>\]	{
		newpos();
		optLevel--;
		if ( optLevel<0 )
		{
			/*yywarning("non-balanced ']' in #command");*/
			optLevel=0;
			REJECT;
		}
		check_CommandPart(curCommandPart);
		curCommandList=curCommandPart->prevList;
		curCommandPart=curCommandPart->prev;
	}

<substState>[^#< \\\t;\[\]\r\n\032_a-zA-Z."']+  	{ /* literal text */
		insertCommandList( new_CommandPart(TM_literal, yytext));
		newpos();
	}

<substState>\"[^\"]*\"  	{ /* literal text */
		insertCommandList( new_CommandPart(TM_literal, yytext));
		newpos();
	}

<substState>\'[^\']*'  	{ /* literal text */
		insertCommandList( new_CommandPart(TM_literal, yytext));
		newpos();
	}

<substState>{cmdword}	{
		/*char *s;
		for(s=yytext; *s; ++s)
			*s=toupper(*s);*/
		insertCommandList( new_CommandPart(TM_word, yytext));
		newpos();
	}

<substState>">="|"<="|"!="|"->"|"=="|":="|"<>"|"+="|"-="|"/="|"*="|"^="|"%="		{

		insertCommandList( new_CommandPart(TM_literal, yytext));
		newpos();
	}
<substState>\\[<>]([=-])?		{
		insertCommandList( new_CommandPart(TM_literal, yytext+1));
		newpos();
	}
<substState>\\\[		{
		insertCommandList( new_CommandPart(TM_literal, "["));
		newpos();
	}
<substState>\\\]		{
		insertCommandList( new_CommandPart(TM_literal, "]"));
		newpos();
	}
<substState>\\\{		{
		insertCommandList( new_CommandPart(TM_literal, "{"));
		newpos();
	}
<substState>\\\}		{
		insertCommandList( new_CommandPart(TM_literal, "}"));
		newpos();
	}

<substState>;{wt}*{eol}	{
		/* skip \n */
		newline();
	}
<substState>;{wt}*"&&"{noteol}*{eol}	{ newline(); }
<substState>;{wt}*"//"{noteol}*{eol}	{ newline(); }

<substState>"//"{noteol}*{eol}|"&&"{noteol}*{eol}|{eol}	{
		/* end of state */
		newline();
		if (optLevel)
		{
			yyerror("non-balanced '[' in #command");
			optLevel=0;
		}
		insert_Coll(&macroCommands, curCommand);
		check_Command(curCommand);
		if (command_flag>1)
			print_Command(curCommand);
		BEGIN(INITIAL);
	}
<substState>{eol}  {
		/* end of state */
		newline();
		if (optLevel)
		{
			yyerror("non-balanced '[' in #command");
			optLevel=0;
		}
		insert_Coll(&macroCommands, curCommand);
		check_Command(curCommand);
		if (command_flag>1)
			print_Command(curCommand);
		BEGIN(INITIAL);
		goto clic_eof;
	}
<substState>.  	{ /* literal text */
		insertCommandList( new_CommandPart(TM_literal, yytext));
		newpos();
	}

<commandCall>{wt}+  	{
		newpos_t();
		yymore();
	}

<commandCall>[^ ./&\t;\[\(\{\"\'\r\n\032_a-zA-Z]+  	{
		newpos();
		yymore();
	}

<commandCall>".or." 	newpos(); yymore();
<commandCall>".and." 	newpos(); yymore();
<commandCall>".not." 	newpos(); yymore();
<commandCall>"."[tfyn]"." 	newpos(); yymore();

<commandCall>{id}	{
#if 1
			newpos();
			yymore();
#else
		char *s;
		/*vo_printf(3, 0, "command call id yytext: '%s'\n", yytext);*/
		for(s = yytext+yyleng; s>yytext && is_id(s[-1]); --s)
			;
		vo_printf(5, 0, "command call id: '%s', yytext: '%s'\n", s, yytext);
		if ( (curMacro=findMacro(s))  )
		{
			int l = strlen(s);
			vo_printf(3,0,"unput macro '%s'\n", curMacro->body);
			memset(s,' ',l);
			put_str(curMacro->body);
			newpos();
			yymore();
		}
		else
		{
			newpos();
			yymore();
		}
		vo_printf(5, 0, "end of command call id: '%s', yytext: '%s'\n", s, yytext);
#endif
	}

<commandCall>"//"	comment_beg = yyleng-2; newpos(); BEGIN(cmdEolComment); yymore();
<commandCall>"&&"	comment_beg = yyleng-2; newpos(); BEGIN(cmdEolComment); yymore();
<commandCall>"/*"      	comment_beg = yyleng-2; newpos(); BEGIN(cmdComment); yymore();
<cmdEolComment>[^\r\n\032]*   newpos(); yymore();
<cmdEolComment>{eol}  	{
		int i;
		for(i=comment_beg; i<yyleng; i++)
			yytext[i]=' ';
		unput('\n');
		BEGIN(commandCall);
		yymore();
	}
<cmdComment>[^*\r\n\032]*       newpos(); yymore();
<cmdComment>"*"+[^*/\r\n\032]*   newpos(); yymore();
<cmdComment>{eol}             newline(); yymore();
<cmdComment>"*"+"/"	{
		int i;
		newpos();
		for(i=comment_beg; i<yyleng; i++)
			yytext[i]=' ';
		BEGIN(commandCall);
		yymore();
	}



<commandCall>\"  newpos(); stringQuota='"'; strLine=clic_line; strPos=clic_pos; strFile=currentFile(); BEGIN(cmdString); yymore();
<commandCall>\'  newpos(); stringQuota='\''; strLine=clic_line; strPos=clic_pos; strFile=currentFile(); BEGIN(cmdString); yymore();
<commandCall>\`  newpos(); stringQuota='`'; strLine=clic_line; strPos=clic_pos; strFile=currentFile(); BEGIN(cmdString); yymore();
<commandCall>\[  newpos(); stringQuota=']'; strLine=clic_line; strPos=clic_pos; strFile=currentFile(); brack_level=1; BEGIN(cmdString); yymore();
<commandCall>\{  newpos(); stringQuota='}'; strLine=clic_line; strPos=clic_pos; strFile=currentFile(); brack_level=1; BEGIN(cmdString); yymore();
<commandCall>\(  newpos(); stringQuota=')'; strLine=clic_line; strPos=clic_pos; strFile=currentFile(); brack_level=1; BEGIN(cmdString); yymore();

<cmdString>&\\. |
<cmdString>&   {
	newpos_t();
	yymore();
	}

<cmdString>\"|\'|\`	{
		newpos_t();
		if (yytext[yyleng-1] == stringQuota)
		{
			stringQuota = 0;
			BEGIN(commandCall);
		}
		else if (stringQuota == '"' || stringQuota == '\''
			|| stringQuota == '`' || stringQuota == ']' )
		{
		}
		else
		{
			stringQuota2 = yytext[yyleng-1];
			BEGIN(cmdString2);
		}
		yymore();
	}

<cmdString2>\"|\'|\`	{
		newpos_t();
		if (yytext[yyleng-1] == stringQuota2)
		{
		/*printf("cmsString2:%d:%s\n", yyleng, yytext);*/
			BEGIN(cmdString);
		}
		newpos_t();
		yymore();
	}

<cmdString2>[^\"\'\`;]+ {
		newpos_t();
		yymore();
	}

<cmdString>\[|\]	{
		if (stringQuota==']')
		{
			switch(yytext[yyleng-1])
			{
			case '[':
				brack_level++;
				break;
			case ']':
				brack_level--;
				if (brack_level==0)
					BEGIN(commandCall);
				break;
			}
		}
		newpos_t();
		yymore();
	}
<cmdString>\{|\}	{
		if (stringQuota=='}')
		{
			switch(yytext[yyleng-1])
			{
			case '{':
				brack_level++;
				break;
			case '}':
				brack_level--;
				if (brack_level==0)
					BEGIN(commandCall);
				break;
			}
		}
		newpos_t();
		yymore();
	}

<cmdString>\(|\)	{
		if (stringQuota==')')
		{
			/*printf("cmdString:1:%d:%c:%s:%d\n", yyleng, stringQuota, yytext,brack_level);*/
			switch(yytext[yyleng-1])
			{
			case '(':
				brack_level++;
				break;
			case ')':
				brack_level--;
				if (brack_level==0)
					BEGIN(commandCall);
				break;
			}
		}
		newpos_t();
		yymore();
	}

<cmdString>(;{wt}*{eol})|(;{wt}*"&&"{noteol}*{eol})|(;{wt}*"//"{noteol}*{eol}) {
		if (stringQuota==')'||stringQuota =='}'||stringQuota==']')
		{
			int i;
			for(i=cyyleng; i<yyleng;i++)
				yytext[i] = ' ';
			/*
			char *s;
			for(s=yytext+yyleng-1; *s!=';'; s--)
				*s=' ';
			*s=' ';
			*/
		}
		newline();
		yymore();
	}

<cmdString2>(;{wt}*{eol})|(;{wt}*"&&"{noteol}*{eol})|(;{wt}*"//"{noteol}*{eol}) {
		if (stringQuota2==')'||stringQuota2 =='}'||stringQuota2==']')
		{
			int i;
			for(i=cyyleng; i<yyleng;i++)
				yytext[i] = ' ';
			/*
			char *s;
			for(s=yytext+yyleng-1; *s!=';'; s--)
				*s=' ';
			*s=' ';
			*/
		}
		newline();
		yymore();
	}

<cmdString,cmdString2>;	{
	newpos_t(); yymore();
	}

<cmdString,cmdString2>{eol}        {
	yyerror("cmdstr: newline in string");
	newline();

	BEGIN(INITIAL);
	/*yymore();*/
	}

<cmdString>[^\&\r\n\"\'\`\[\]\(\)\{\};]+        {
	newpos_t();
	yymore();
	}

<commandCall>(;{wt}*{eol})|(;{wt}*"&&"{noteol}*{eol})|(;{wt}*"//"{noteol}*{eol}) {
		/* skip ;\n */
		char *s;
		newline();
		/*callEol++;*/
		for(s=yytext+yyleng-1; *s!=';'; s--)
			*s=' ';
		*s=' ';
		yymore();
	}

<commandCall>;{wt}* { /* /[^ \r\n\032]  */
	char *s;
/*        vo_printf(3, 0, "commandCall; yytext: '%s'\n", yytext);*/
	s = yytext+yyleng-1;
	for(; *s!=';'; s--)
		*s=' ';
	*s=' ';
	s = yytext;
/*        vo_printf(3, 0, "commandCall; yytext: '%s'\n", s);*/
	put_char('\n');
/*        command_end();*/
	/* *s='\n'; */
	yymore();
}


<commandCall>{eol}  {
		nlflag = 1;
		/*printf("command_end:%d:'%.*s'\n", yyleng, yyleng, yytext);*/
		/*if (match_translate)
			nlflag = 0;*/
		command_end();
		vo_printf(6,0, "eol command_end %s: state=%d\n", curCommand->name, YYSTATE);
	}

<commandCall>.  	{
		newpos();
		yymore();
	}


^{wt}*{id}/{wt}*"("	{
	char *s;
	int l;
	int cw = clic_word;
	for(s=yytext; *s==' '||*s=='\t'; ++s)
		;
	l=yyleng-(s-yytext);
	newpos_wt(s-yytext);
	LECHO(s-yytext);
	if ( (curMacroFunc=findMacroFunc(s))!=0 )
	{ /* macro function */
		freeAll_Coll(&macroArgs);
		macro_call_state = YYSTATE;
		macro_call_bol = cw?0:1;
		vo_printf(6,0, "^macro func call: clic_word=%d\n", cw);
		BEGIN(macroCallBegin);
	}
	else if ( (curMacro=findMacro(s))==0  )
	{
		if ( (curCommand=findFirst_Command(s,&commandInd)) == NULL )
		{
			/* keyword or NAME */
			BEGIN(keyword);
			free(callName);
			callName = 0;
			yyless(yyleng-l);
		}
		else /* command name */
		{
			callEol=0;
			free(callName);
			callName=mem_dup(s, l);
			command_state = YYSTATE;
			command_next_state = keyword;
			vo_printf(6,0, "find_command %s state=%d next=%d\n", curCommand->name, command_state, command_next_state);
			match_translate = 0;
			BEGIN(commandCall);
		}
	}
	else
	{ /* macro name */
		put_str(curMacro->body);
	}
	}

^{wt}*{id}{wtnl}*"->"	{
		int l1, l2;
		char *s;
		for(s=yytext; (s=strchr(s,'\n')); s++)
			newline();
		l1=strspn(yytext, " \t");
		l2=strcspn(yytext+l1, " \t;->");
		if ( (l2==1 && !strncasecmp(yytext+l1, "M", l2))
			 || (l2==6 && !strncasecmp(yytext+l1, "MEMVAR", l2))	)
		{
			PECHO("MEMVAR->");
			MRETURN(MEMVARPTR)
		}
		if ( (l2==1 && !strncasecmp(yytext+l1, "F", l2))
			 || (l2==6 && !strncasecmp(yytext+l1, "_FIELD", l2))
			 || (l2==5 && !strncasecmp(yytext+l1, "FIELD", l2))
			 )
		{
			PECHO("FIELD->");
			MRETURN(FIELDPTR)
		}
		unput('>');
		unput('-');
		s = mem_Dup(yytext+l1,l2);
		if ( (curMacro=findMacro(s))==0  )
		{
			yylval.string = s;
			PECHO(yylval.string);
			MRETURN(NAME)
		}
		else
		{
			put_str(curMacro->body);
			free(s);
			/*my_yy_set_bol(1);*/
		}
	}

^{wt}*{id}{wt}*[-+=/*^%$:<>!#][-+=/*^%$:<>!#]?	{
		/* [-+/=*^%$:<>!#] */
		int l1, l2, force_name=0, i, l3;
		char *s, *op;
		if (subst_flag!=2 && subst_flag!=0)
		{
			last_ret = 0;
			REJECT;
		}

		for(s=yytext; (s=strchr(s,'\n')); s++)
			newline();
		l1=strspn(yytext, " \t\n\r");
		l2=strcspn(yytext+l1, "- \n\t\r;=+/*^%$:<>!#[]");
		l3=strspn(yytext+l1+l2, " \t\n\r");
		s = mem_dup(yytext+l1,l2);
		for(i=yyleng; i>0; i--)
			if ( !strchr("-+=/*^%$:<>!#[]", yytext[i-1] ))
				break;

		op = yytext+i;
		if ( !l3
			|| !strcmp(op, ":=")
			|| !strcmp(op, "=")
			|| !strcmp(op, "#")
			|| !strcmp(op, "$")
			/*|| !strcmp(op, "/")*/
			/*|| !strcmp(op, "*")*/
			|| !strcmp(op, "%")
			|| !strcmp(op, "^")
			|| !strcmp(op, "+=")
			|| !strcmp(op, "-=")
			|| !strcmp(op, "/=")
			|| !strcmp(op, "*=")
			|| !strcmp(op, "%=")
			|| !strcmp(op, "^=")
			|| !strcmp(op, "!=")
			|| !strcmp(op, "==")
			|| !strcmp(op, "<>")
			|| !strcmp(op, "<=")
			|| !strcmp(op, "=>")
			|| !strcmp(op, "<")
			|| !strcmp(op, ">")
			/*|| !strcmp(op, ":")*/
			)
			force_name = 1;


		if ( (curMacro=findMacro(s)) )
		{
			put_str(curMacro->body);
			put_char(' ');
			put_str(op);
			free(s);
			/*my_yy_set_bol(1);*/
		}
		else if ((curCommand=findFirst_Translate(s, &commandInd)))
		{
			unputstr(op);
			unput(' ');
			/*if (force_name)
			{
				yylval.string = s;
				PECHO(yylval.string);
				MRETURN(NAME)
			}
			else*/
			{
				callEol=0;
				free(callName);
				callName=mem_dup(s, l2);
				command_state = YYSTATE;
				command_next_state = keyword;
				bol_translate = 1;
				vo_printf(6,0, "find_command %s state=%d next=%d\n", curCommand->name, command_state, command_next_state);
				match_translate = 0;
				BEGIN(commandCall);
				free(s);
			}
		}
		else if ((curCommand=findFirst_Command(s, &commandInd)))
		{
			unputstr(op);
			unput(' ');
			if (force_name)
			{
				yylval.string = s;
				PECHO(yylval.string);
				MRETURN(NAME)
			}
			else
			{
				callEol=0;
				free(callName);
				callName=mem_dup(s, l2);
				command_state = YYSTATE;
				command_next_state = keyword;
				bol_translate = 1;
				vo_printf(6,0, "find_command %s state=%d next=%d\n", curCommand->name, command_state, command_next_state);
				match_translate = 0;
				BEGIN(commandCall);
				free(s);
			}
		}
		else
		{
			if (force_name)
			{
			/*
				free(s);
				REJECT;
			*/
				unputstr(op);
				unput(' ');
				yylval.string = s;
				PECHO(yylval.string);
				MRETURN(NAME)
			}
			else
			{
				unputstr(op);
				unput(' ');
				BEGIN(keyword);
				free(callName);
				callName = 0;
				unputstr(s);
				free(s);
			}
		}
	}

^{wt}*{id}  { /*  +[^=]  */ /* /[;\r\n \t\032] */
	char *s = 0;
	int l;

cmd_id:
	for(s=yytext; *s==' '||*s=='\t'; ++s)
		;
	l=yyleng-(s-yytext);
	newpos_wt(s-yytext);
	LECHO(s-yytext);

	if ( (curMacro=findMacro(s))==0  )
	{
		if ( (curCommand=findFirst_Command(s,&commandInd)) == NULL )
		{
			/* keyword or NAME */
			BEGIN(keyword);
			vo_printf(6,0, "BEGIN(keyword) at %s\n", yytext);
			free(callName);
			callName = 0;
			yyless(yyleng-l);
		}
		else /* command name */
		{
			callEol=0;
			free(callName);
			callName=mem_dup(s, l);
			command_state = YYSTATE;
			command_next_state = keyword;
			bol_translate = 1;
			vo_printf(6,0, "find_command %s state=%d next=%d\n", curCommand->name, command_state, command_next_state);
			match_translate = 0;
			BEGIN(commandCall);
		}
	}
	else
	{ /* macro name */
		put_str(curMacro->body);
		/*put_char('\n');*/
	}
	}

{id}/{wt}*"(" { /* {id}/{wt}*"(" */
	int cw = clic_word;
	newpos_w();
	if ( (curMacroFunc=findMacroFunc(yytext))!=0 )
	{ /* macro function */
		freeAll_Coll(&macroArgs);
		macro_call_state = YYSTATE;
		macro_call_bol = cw?0:1;
		vo_printf(6,0, "macro func call: clic_word=%d\n", cw);
		BEGIN(macroCallBegin);
	}
	else if ( (curMacro=findMacro(yytext))!=0  )
	{ /* macro name */
		put_str(curMacro->body);
	}
	else if ( !bad_translate && !callName && (curCommand=findFirst_Translate(yytext,&commandInd)) )
	{
		callEol=0;
		callName=mem_dup(yytext, yyleng);
		command_state = YYSTATE;
		command_next_state = YYSTATE;
		bol_translate = YY_AT_BOL();
		vo_printf(6,0, "find_translate2 %s state=%d next=%d\n", curCommand->name, command_state, command_next_state);
		match_translate = 1;
		BEGIN(commandCall);
	}
	else
	{
		/*bad_translate = 0;*/
		REJECT;
	}
	}

{id}	{
	vo_printf(6,0, "{id}: %s state=%d\n", yytext, YYSTATE);
	if (feof(yyin) && clic_pos==1)
	{
		int c;
		c = input();
		if (c==EOF || c=='\n')
		{
			goto cmd_id;
		}
		else
			unput(c);
	}
	if ( (curMacro=findMacro(yytext))==0  )
	{
		if ( !bad_translate && !callName &&  (curCommand=findFirst_Translate(yytext,&commandInd)) )
		{
			callEol=0;
			free(callName);
			callName=mem_dup(yytext, yyleng);
			command_state = YYSTATE;
			command_next_state = YYSTATE;
			bol_translate = YY_AT_BOL();
			vo_printf(6,0, "find_translate3 %s state=%d next=%d\n", curCommand->name, command_state, command_next_state);
			match_translate = 1;
			BEGIN(commandCall);
		}
		else
		{
			bad_translate = 0;
			yylval.string=mem_Dup(yytext,yyleng);
			newpos_w();
			MECHO;
			MRETURN_NAME
			/*MRETURN(NAME)*/
		}
	}
	else
	{ /* macro name */
		put_str(curMacro->body);
		newpos();
	}
	}

^{wt}*[@!?$.]+	{
		char *s;
		int l;
		if (subst_flag!=2)
		{
			REJECT;
		}
		for(s=yytext; *s==' '||*s=='\t'; ++s)
			;
		l=yyleng-(s-yytext);
		LECHO(s-yytext);
		if ( (curCommand=findFirst_Command(s,&commandInd)) == NULL )
		{
			newpos_w();
			yylval.string=mem_Dup(s,l);
			PECHO(yylval.string);
			MRETURN(NAME)
		}
		else /* command name */
		{
			callEol=0;
			free(callName);
			callName=mem_dup(s, l);
			command_state = YYSTATE;
			command_next_state = keyword;
			bol_translate = 1;
			vo_printf(6,0, "find_command %s state=%d next=%d\n", curCommand->name, command_state, command_next_state);
			match_translate = 0;
			BEGIN(commandCall);
		}
	}


[@?$!.:]+{bcid}+	{
		if (subst_flag!=2)
		{
			REJECT;
		}
		if ( !bad_translate && !callName && (curCommand=findFirst_Translate(yytext,&commandInd)) )
		{
			callEol=0;
			free(callName);
			callName=mem_dup(yytext, yyleng);
			command_state = YYSTATE;
			command_next_state = YYSTATE;
			bol_translate = YY_AT_BOL();
			vo_printf(6,0, "find_translate4 %s state=%d next=%d\n", curCommand->name, command_state, command_next_state);
			match_translate = 1;
			BEGIN(commandCall);
		}
		else
		{
			/*bad_translate = 0;*/
			REJECT;
		}
	}

[@?$!.:]+	{
		if (subst_flag!=2)
		{
			REJECT;
		}
		if ( !bad_translate && !callName && (curCommand=findFirst_Translate(yytext,&commandInd)) )
		{
			callEol=0;
			free(callName);
			callName=mem_dup(yytext, yyleng);
			command_state = YYSTATE;
			command_next_state = YYSTATE;
			bol_translate = YY_AT_BOL();
			vo_printf(6,0, "find_translate5 %s state=%d next=%d\n", curCommand->name, command_state, command_next_state);
			match_translate = 1;
			BEGIN(commandCall);
		}
		else
		{
			/*bad_translate = 0;*/
			REJECT;
		}
	}

{wt}+  { newpos_t();
	if (clic_errorcount>25)
	{
		yyerror("too much errors");
		yyterminate();
	}
	MECHO;
	}

<commandCall><<EOF>> {
			command_end();
			/*goto clic_eof;*/
		}


<<EOF>>		{
	clic_eof:
			if ( includes.count_of_Coll<2 )
			  {
				 if (ifdefs.count_of_Coll>0)
				yyerror("#ifdef or #ifndef without #endif");

				 if ( YYSTATE==lexString|| YYSTATE==lexString1
				  || YYSTATE==lexString2 || YYSTATE==lexString3 ||
				  (YYSTATE==cmdString && (stringQuota=='\''||stringQuota=='"'||stringQuota=='`') )
				)
				  yyerror("unterminated string constant (possible begin at line %d pos %d file '%s')"
					, strLine, strPos, fileName(strFile));
				 /* { ([ */
				 else if (  YYSTATE==cmdString && (stringQuota==']'||stringQuota=='}'||stringQuota==')') )
				  yyerror("unmatched bracket '%c' (possible begin at line %d pos %d file '%s')",
					stringQuota, strLine, strPos, fileName(strFile));
				 else if ( YYSTATE==lexComment)
				  yyerror("unclosed comment (possible begin at line %d pos %d file '%s')",
					commentLine, commentPos, fileName(commentFile));
				 else if ( YYSTATE==textState )
				yyerror("no ENDTEXT");
				 eof_checks();
				 yyterminate();
			  }
			else
			  {
				 int last=includes.count_of_Coll-1;
				 curInclude=(IncludeFile*)includes.items_of_Coll[last];
				 clic_line=curInclude->line+1;
				 fclose(yyin);
				 pop_charset();
				 yy_switch_to_buffer( curInclude->buf);
				 atFree_Coll(&includes,last);
				 insert_Coll(&fileList,strdup(((IncludeFile*)includes.items_of_Coll[last-1])->name));
			  }
			 }

;	put_char('\n');  newpos();

"^"	newpos_w(); MECHO; MRETURN(POW)

"{"{dig}+[.]{dig}+[.]{dig}+"}"	{
		yylval.string=mem_dup(yytext+1, yyleng-2);
		newpos_w(); MECHO; MRETURN(DATE)
	}

"{"	{
		if(bracket_level <= 0)
			bracket_buf.ptr_of_StrBuf = bracket_buf.buf_of_StrBuf;
		bracket_level++;
		newpos_w(); MECHO; MRETURN(*yytext)
}

"}"	{
		newpos_w(); MECHO;
		bracket_level--;
		if (bracket_level<=0)
			putByte_StrBuf(&bracket_buf, 0);
		MRETURN(*yytext)
}

\[      {
		newpos_w();
		if (subst_flag==1)
			goto str;
		switch(last_ret)
		{
		case ']':
		case NAME:
		case '&':
		case '.':
		case ')':
		case '}':
		case RPTR:
		case STRING:
		case ISTRING:
		case NUMBER:
		case PCOUNT:
		case PSHIFT:
		case TRUE:
		case FALSE:
		case NIL:
			MECHO;
			MRETURN('[');
			break;
		default:
		 str:
			string_buf_ptr = string_buf;
			str_caller = YYSTATE;
			strLine=clic_line; strPos=clic_pos; strFile=currentFile();
			str_return = 1;
			/*printf("BEGIN(lexString2)\n");*/
			BEGIN(lexString2);
			break;
		}
	}

.	newpos_w(); MECHO; MRETURN(*yytext)




%%

/* ][ common */
/*"#"     newpos_w(); MECHO; MRETURN(NE)*/

static void
newline(void)
{
	cyyleng = yyleng;
	cl_line=clic_line;
	cl_pos=clic_pos;
	clic_pos=1;
	clic_word=0;

	if (subst_flag)
		return;

	clic_line++;
	corr_line--;

	if (corr_line<0)
		corr_line=0;

	all_lines++;
}

static void
newpos(void)
{
	cyyleng = yyleng;
	clic_pos+=yyleng;
	cl_pos=clic_pos;
	cl_line=clic_line;
}

static void
newpos_w(void)
{
	cyyleng = yyleng;
	clic_pos+=yyleng;
	clic_word++;
	cl_pos=clic_pos;
	cl_line=clic_line;
}

static void
newpos_t(void)
{
	char *s;
	int i;
	cyyleng = yyleng;
	for(s=yytext,i=0;i<yyleng;i++,s++)
		if (*s=='\t')
			clic_pos=(((clic_pos-1)/8)+1)*8+1;
		else
			clic_pos++;
}

static void
newpos_wt(int l)
{
	char *s;
	int i;
	cyyleng = yyleng;
	for(s=yytext,i=0;i<l;i++,s++)
		if (*s=='\t')
			clic_pos=(((clic_pos-1)/8)+1)*8+1;
		else
			clic_pos++;
}

int
yywrap()
{
	return 1;
}

void
init_lex()
{
	int i;

	init_Coll(&text, free, 0);
	init_Coll(&fileList, free, 0);
	init_Coll(&includes, free_IncludeFile, 0);
	init_Coll(&ifdefs, free, 0);
	init_Coll(&macros, free_Macro, compare_macro);
	init_Coll(&xmacros, free_Macro, xcompare_macro);
	init_Coll(&macroFuncs, free_MacroFunc, compare_macro);
	init_Coll(&xmacroFuncs, free_MacroFunc, xcompare_macro);
	init_Coll(&macroArgs, free, 0);
	init_Coll(&macroCommands, delete_Command, compare_Command);
	init_Coll(&charsets, free, 0);
	macroCommands.duplicates_of_Coll=1;
	init_Coll(&commandArgs, delete_CommandArg, NULL);
	init_Coll(&reffuncs, free, strcasecmp);

	for(i=0;i<predefines.count_of_Coll;i++)
	{
		char *s=(char*)predefines.items_of_Coll[i];
		char *e=strchr(s,'=');
		char *m;
		if (e)
		{
			m=mem_dup(s,e-s);
			e=strdup(e+1);
		}
		else
			m=strdup(s);

		insert_Coll(&macros,new_Macro(m,e));
	}

	if (!bracket_buf.buf_of_StrBuf)
	{
		bracket_buf.buf_of_StrBuf = bracket_buf.ptr_of_StrBuf = (char *) malloc(64);
		bracket_buf.size_of_StrBuf = 64;
		bracket_buf.end_of_StrBuf = bracket_buf.buf_of_StrBuf + 64;
	}

	init_Coll(&unput_strings, (void(*)(void*)) delete_StrBuf, 0);
	unput_buffer.ptr_of_StrBuf = unput_buffer.buf_of_StrBuf;
	subst_count = 0;
}

void
resume_lex()
{

	destroy_Coll(&unput_strings);

	destroy_Coll(&text);
	destroy_Coll(&includes);
	destroy_Coll(&ifdefs);
	destroy_Coll(&macros);
	destroy_Coll(&xmacros);
	destroy_Coll(&macroFuncs);
	destroy_Coll(&xmacroFuncs);
	destroy_Coll(&macroArgs);
	destroy_Coll(&fileList);
	destroy_Coll(&macroCommands);
	destroy_Coll(&commandArgs);
	destroy_Coll(&charsets);
	destroy_Coll(&reffuncs);

#ifdef MEMDBG
	printf("\nm_sum/m_num: %d; c_sum: %d; s_sum: %d\n", (m_sum-c_sum-s_sum)/
		(m_num-c_num-s_num),
		c_sum/c_num, s_sum/s_num);
#endif
}

void
clear_lex(void)
{
	int i;

	clic_line = 1;
	all_lines = 1;
	clic_pos = 1;
	comment_depth = 0;
	nnl = 0;

	freeAll_Coll(&includes);
	freeAll_Coll(&ifdefs);
	freeAll_Coll(&macros);
	freeAll_Coll(&xmacros);
	freeAll_Coll(&macroFuncs);
	freeAll_Coll(&xmacroFuncs);
	freeAll_Coll(&macroCommands);
	freeAll_Coll(&macroArgs);
	freeAll_Coll(&fileList);
	freeAll_Coll(&charsets);
	freeAll_Coll(&reffuncs);

	for(i=0;i<predefines.count_of_Coll;i++)
	{
		char *s=(char*)predefines.items_of_Coll[i];
		char *e=strchr(s,'=');
		char *m;
		if (e)
		{
			m=mem_dup(s,e-s);
			e=strdup(e+1);
		}
		else
			m=strdup(s);

		insert_Coll(&macros,new_Macro(m,e));
	}
	bracket_buf.ptr_of_StrBuf = bracket_buf.buf_of_StrBuf;

}

static int
start_include(char *filename)
{
	clic_line=0;
	curInclude=new_IncludeFile(strdup(filename),0);
	curInclude->type='<';
	if (!startCurInclude())
		return -1;
	clic_line=1;
	cl_line=1;
	return 0;
}

int
lex_new_file(FILE *file, const char *filename)
{
	int i;

	clear_lex();
	insert_Coll(&includes, new_IncludeFile(strdup(filename),1));
	insert_Coll(&fileList, strdup(filename));
	push_charset();
	load_file_charset(filename);
	yyin=file;
	yyrestart(file);
	BEGIN(INITIAL);
	if (std_ch_filename)
	{
		if ( start_include((char*)std_ch_filename) )
			return -1;
	}
	for (i=include_files.count_of_Coll-1; i>=0; i--)
	{
		if (start_include ((char*) include_files.items_of_Coll[i] ) )
			return -1;
	}
	return 0;
}

/* common][Macro */

static Macro *
findMacro(const char *name)
{
	Macro buf;
	int ind;
	buf.name=(char*)name;
	buf.xdefine = 0;
	if ( search_Coll(&macros, &buf, &ind) )
		return (Macro*)macros.items_of_Coll[ind];
	buf.xdefine = 1;
	if ( search_Coll(&xmacros, &buf, &ind) )
		return (Macro*)xmacros.items_of_Coll[ind];
	return 0;
}

static MacroFunc*
findMacroFunc(const char *name)
{
	MacroFunc buf;
	int ind;
	buf.macro.name=(char*)name;
	buf.macro.xdefine = 0;
	if ( search_Coll(&macroFuncs, &buf, &ind) )
		return (MacroFunc*)macroFuncs.items_of_Coll[ind];
	buf.macro.xdefine = 1;
	if ( search_Coll(&xmacroFuncs, &buf, &ind) )
		return (MacroFunc*)xmacroFuncs.items_of_Coll[ind];
	return 0;
}

static void
finale_MacroFunc(MacroFunc *self)
{
	int i, j, l, le=0, len;
	MacroPart *pmp=0, *begmp=0;
	int begi=0;
#if 0
	if (self->macroArgs.count == 0)
		yyerror("macro function with 0 count of arguments");
#endif
	for(i=0; i<self->parts.count_of_Coll; i++)
	{
		MacroPart *mp=(MacroPart*)self->parts.items_of_Coll[i];
		if (mp->text)
		{
			recode_string(mp->text);
			le+=strlen(mp->text);
		}
		else
			le+=strlen((char*)self->macroArgs.items_of_Coll[mp->argno]);
	}

	self->macro.body= (char*) malloc(le+1);
	self->macro.body[le]=0;
	len=le;
	le=0;
	for(i=0; i<self->parts.count_of_Coll; i++)
	{
		MacroPart *mp=(MacroPart*)self->parts.items_of_Coll[i];
		if (mp->text)
		{
			l=strlen(mp->text);
			memcpy(self->macro.body+le, mp->text, l);
			free(mp->text);
			mp->text=self->macro.body+le;
			mp->len=l;
			le+=l;
		}
		else
		{
			char *s=(char *)self->macroArgs.items_of_Coll[mp->argno];
			l=strlen(s);
			memcpy(self->macro.body+le, s, l);
			le+=l;
		}
	}

	for(i=0; i<self->parts.count_of_Coll; i++)
	{
		MacroPart *mp=(MacroPart *)self->parts.items_of_Coll[i];
		if (mp->text)
		{
			if(!pmp)
				{begmp=mp; begi=i;}
			pmp=mp;
		}
		else
		{
			if (begmp && begmp!=pmp)
			{ /* glue parts */
				begmp->len= (pmp->text-begmp->text) + pmp->len;
				for(j=begi+1; j<i; j++)
					((MacroPart*)self->parts.items_of_Coll[j])->len=-1;
			}
			pmp=begmp=0;
		}
	}

	if (begmp && begmp!=pmp)
	{ /* glue parts */
		begmp->len= (pmp->text-begmp->text) + pmp->len;
		for(j=begi+1; j<i; j++)
			((MacroPart*)self->parts.items_of_Coll[j])->len=-1;
	}

	for(i=0; i<self->parts.count_of_Coll; i++)
	{
		MacroPart *mp=(MacroPart*)self->parts.items_of_Coll[i];
		if (mp->text && mp->len==-1)
		{
			atFree_Coll(& self->parts, i);
			i--;
		}
	}

}

static void
free_Macro(void *item)
{
	Macro *p=(Macro*)item;
	free(p->name);
	free(p->body);
	free(p);
}

static Macro*
new_Macro(char *Name, char *Body)
{
	Macro *ret=(Macro*)calloc(1,sizeof(Macro));
	ret->name=Name;
	ret->body=Body;
	ret->xdefine = xdefine;
	return ret;
}

static int
compare_macro(void *k1, void *k2)
{
	Macro *m1 = (Macro*) k1;
	Macro *m2 = (Macro*) k2;
	return strcmp( m1->name, m2->name );
}

static int
xcompare_macro(void *k1, void *k2)
{
	Macro *m1 = (Macro*) k1;
	Macro *m2 = (Macro*) k2;
	return strcasecmp( m1->name, m2->name );
}

static void
free_MacroFunc(void *item)
{
	MacroFunc *p=(MacroFunc*)item;
	destroy_Coll(& p->parts);
	destroy_Coll(& p->macroArgs);
	free(p->macro.name);
	free(p->macro.body);
	free(p);
}

static MacroFunc *
new_MacroFunc(char *Name)
{
	MacroFunc *ret=(MacroFunc*)calloc(1,sizeof(MacroFunc));
	ret->macro.name=Name;
	ret->macro.body=0;
	ret->macro.xdefine = xdefine;
	init_Coll( & ret->parts, free, 0 );
	init_Coll( & ret->macroArgs, free, 0);
	return ret;
}


static MacroPart *
new_MacroPart(char *Text, int no)
{
	MacroPart *ret=(MacroPart*)calloc(1,sizeof(MacroPart));
	ret->text=Text;
	ret->argno=no;
	return ret;
}

/* Macro ][ Includes */

static IfdefState *
new_IfdefState(int Cond, int If)
{
	IfdefState *ret=(IfdefState*)calloc(1,sizeof(IfdefState));
	ret->condition=Cond;
	ret->ifdef=If;
	ret->begline=clic_line;
	ret->elseline=clic_line;
	ret->state=0;
	return ret;
}

static IncludeFile *
new_IncludeFile( char *Name, long Line )
{
	IncludeFile *ret=(IncludeFile*)calloc(1,sizeof(IncludeFile));
	ret->name=Name;
	ret->line=Line;
	ret->type=0;
	return ret;
}


static void
free_IncludeFile(void *item)
{
	IncludeFile *p=(IncludeFile *)item;
	free(p->name);
	free(p);
}



int currentFile(void)
{
	return includes.count_of_Coll-1;
}

char *fileName(int file)
{
	if (file>=0 && file<includes.count_of_Coll)
		return ((IncludeFile*)includes.items_of_Coll[file])->name;
	else
		return "";
}


int
get_include(int *index, int *line, char **filename)
{
	IncludeFile *p;
	if (*index<0)
		*index=includes.count_of_Coll-1;
	if (*index<0)
		{ *line=-1; *filename=""; return 0; }
	p=(IncludeFile*) includes.items_of_Coll[*index];
	*line=p->line;
	*filename=p->name;
	--*index;
	if ( *index < 0 )
		return 0;
	return 1;
}

#if 0
static unsigned long strToChar(char *str)
{
	int l=strlen(str), i;
	unsigned long ret=0;
	unsigned char *s=(unsigned char*)str;
	for( i=l-1; i>=0 ;i--)
		ret |= (((unsigned long)s[i]) << i);
	return ret;
}
#endif
/* Includes ][ Command */

static char *
strdup_bs(const char *s)
{
	int l;
	char *r, *ret;

	l = strlen(s);
	ret = r = (char*) malloc(l+1);


	for(; *s; s++,r++)
	{
		if (*s=='\\' && s[1])
		{
			s++;
		}
		*r = *s;
	}
	*r = 0;

	return ret;
}


static Command *
new_Command(const char *Name, int isX, int isTranslate)
{
	Command *ret=(Command*)calloc(sizeof(Command),1);

	vo_printf(4, 0, "new_Command: '%s' isX=%d isTranslate=%d\n", Name, isX, isTranslate);
	ret->name=strdup_bs(Name);
	ret->isX=isX;
	ret->isTranslate=isTranslate;
	init_Coll(&ret->pattern, delete_CommandPart, NULL);
	init_Coll(&ret->patternIt, NULL, NULL);
	init_Coll(&ret->subst, delete_CommandPart, NULL);
	init_Coll(&ret->substIt, NULL, NULL);
	return ret;
}

static void
delete_Command(void *item)
{
	Command *cp=(Command*)item;
	if (!cp)
		return;
	free(cp->name);
	destroy_Coll(&cp->pattern);
	destroy_Coll(&cp->patternIt);
	destroy_Coll(&cp->subst);
	destroy_Coll(&cp->substIt);
	free(cp);
}

static int
compare_Command(void *item1, void *item2)
{
	Command *cp1=(Command*)item1;
	Command *cp2=(Command*)item2;
	int l1 = strlen(cp1->name);
	int l2 = strlen(cp2->name);
#ifdef FOUR_BYTE
	if (cp1->isAX || cp2->isAX)
		return strcasecmp( cp1->name, cp2->name);
	else if (cp1->isX && cp2->isX)
		return strcasecmp( cp1->name, cp2->name );
	else if (l2>=4 && l2<l1)
		return strncasecmp( cp1->name, cp2->name, l2 );
	else
		return strcasecmp( cp1->name, cp2->name);
#else
	if (cp1->isAX || cp2->isAX)
		return strcasecmp( cp1->name, cp2->name);
	else if (cp1->isX || cp2->isX)
		return strcasecmp( cp1->name, cp2->name );
	else
		return strcasecmp( cp1->name, cp2->name);
#endif
}

static Command *
findFirst_Command(const char *name, int *ind)
{
	Command buf, *cp;

	if (subst_flag==1)
		return 0;

	buf.name=(char*)name;
	buf.isX=1;
	buf.isAX=0;
	if ( search_Coll(&macroCommands, &buf, ind) )
	{
		cp=(Command*)macroCommands.items_of_Coll[*ind];
		if (cp->isTranslate || clic_word==0)
			return cp;
	}
	buf.isX=1;
	buf.isAX=1;
	if ( search_Coll(&macroCommands, &buf, ind) )
	{
		cp=(Command*)macroCommands.items_of_Coll[*ind];
		if (cp->isTranslate || clic_word==0)
			return cp;
	}
	return 0;
}

static Command *
findNext_Command(const char *name, int *ind)
{
	Command buf, *cp;

again:
	++(*ind);
	if (*ind>=macroCommands.count_of_Coll)
		return NULL;
	buf.name=(char*)name;
	cp = (Command*)macroCommands.items_of_Coll[*ind];

	buf.isX=1;
	buf.isAX=0;
	if ( !compare_Command(&buf, cp ))
	{
		if (match_translate && !cp->isTranslate)
			goto again;
		return cp;
	}
	buf.isX=1;
	buf.isAX=1;
	if ( !compare_Command(&buf, cp ))
	{
		if (match_translate && !cp->isTranslate)
			goto again;
		return cp;
	}
	return NULL;
}

static Command *
findFirst_Translate(const char *name, int *ind)
{
	Command buf, *cp;

	if (subst_flag==1)
		return 0;

	buf.name=(char*)name;
	buf.isX=1;
	buf.isAX=0;
	if ( search_Coll(&macroCommands, &buf, ind) )
	{
		cp=(Command*)macroCommands.items_of_Coll[*ind];
		if (cp->isTranslate)
			return cp;
	}
	buf.isX=1;
	buf.isAX=1;
	if ( search_Coll(&macroCommands, &buf, ind) )
	{
		cp=(Command*)macroCommands.items_of_Coll[*ind];
		if (cp->isTranslate)
			return cp;
	}
	return 0;
}


static CommandPart *
new_CommandPart(CommandPartType type, const char *text)
{
	CommandPart *ret=(CommandPart *)calloc(1,sizeof(CommandPart));
	ret->type=type;
	if (text)
		ret->text=strdup_bs(text);
	return ret;
}


static void
delete_CommandPart(void *arg)
{
	CommandPart *cp=(CommandPart *)arg;
	if (!cp)
		return;
	if (cp->text)
		free(cp->text);
	if (cp->option)
		delete_Coll(cp->option);
	if (cp->delim)
		delete_Coll(cp->delim);
	free(cp);
}

static CommandArg *
new_CommandArg(CommandPart *cp, const char *text, int len)
{
	CommandArg *ret=(CommandArg*)calloc(1,sizeof(CommandArg));
	while(len>0 && isspace(text[len-1]))
		len--;

	ret->text=(char*)malloc(len+1);
	memcpy(ret->text, text, len);
	ret->text[len]=0;
	ret->cp=cp;
	return ret;
}

static CommandArg *
newList_CommandArg(CommandPart *cp)
{
	CommandArg *ret=(CommandArg*)calloc(1,sizeof(CommandArg));
	ret->cp=cp;
	ret->list=new_Coll(free, NULL);
	return ret;
}

static CommandArg *
newLog_CommandArg(CommandPart *cp, const char *val, int len)
{
	CommandArg *ret=(CommandArg*)calloc(1,sizeof(CommandArg));
	ret->cp=cp;
	ret->val=mem_Dup(val,len);
	return ret;
}

static void
delete_CommandArg(void *arg)
{
	CommandArg *cp=(CommandArg*)arg;
	if (!cp)
		return;
	if (cp->text)
		free(cp->text);
	if (cp->val)
		free(cp->val);
	if (cp->list)
		delete_Coll(cp->list);
	if (cp->rawlist)
		delete_Coll(cp->rawlist);
	free(cp);
}


static void
insertCommandList(CommandPart *cp)
{
	insert_Coll(curCommandList, cp);
	switch (cp->type)
	{
	case M_NONE:
	case SM_option:
	case SM_word:
	case SM_literal:
	case TM_option:
	case TM_literal:
	case TM_word:
		break;
	default:
		cp->no=curItList->count_of_Coll;
		insert_Coll(curItList, cp);
		break;
	}
}

static void
cadd_coll(Coll **cpp, char *text)
{
	if (!*cpp)
		*cpp = new_Coll(0,0);
	append_Coll(*cpp, text);
}

static Coll *
clone_coll(Coll *cp)
{
	Coll *ret;
	int i;

	if (!cp)
		return 0;

	ret = new_Coll(0, 0);
	for(i=0; i<cp->count_of_Coll; i++)
		append_Coll(ret, cp->items_of_Coll[i]);

	return ret;
}

static void
get_list_delim(Coll *coll, int beg, int is_opt, Coll **del)
{
	int j, hard=0;

	for(j=beg; !hard && j<coll->count_of_Coll; j++)
	{
		CommandPart *cpp=(CommandPart*)coll->items_of_Coll[j];

		switch(cpp->type)
		{
		case SM_word:
		case SM_literal:
			if (!hard)
			{
				cadd_coll(del, cpp->text);
				hard++;
			}
			break;
		case SM_enum:
			if (!hard)
			{
				int i;
				for(i=0 ; i<cpp->option->count_of_Coll/*-1*/ ; i++)
					cadd_coll(del, cpp->option->items_of_Coll[i]);
				hard++;
			}
			break;
		case SM_option:
			get_list_delim(cpp->option, 0, 1, del);
			break;
		default:
			break;
		}
	}

	/* only for options: collect all previous options delimiters */
	for( j=beg-1; is_opt && j>=0; j-- )
	{
		CommandPart *cpp=(CommandPart*)coll->items_of_Coll[j];

		switch(cpp->type)
		{
		default:
			return;
		case SM_option:
			get_list_delim(cpp->option, 0, 1, del);
			break;
		}
	}

}

static void
set_last(Coll *coll)
{
	CommandPart *cp;
	if (!coll || !coll->count_of_Coll)
		return;

	cp=(CommandPart*)coll->items_of_Coll[coll->count_of_Coll-1];
	if (cp->type==SM_option)
		set_last(cp->option);
	else
		cp->isLast=1;
}

static void
set_list_delim(Coll *coll, Coll *delim, int is_opt)
{
	int i;
	for(i=0;i<coll->count_of_Coll;i++)
	{
		CommandPart *cp=(CommandPart*)coll->items_of_Coll[i];
		switch (cp->type)
		{
		case SM_option:
			{
				Coll *del = clone_coll(delim);
				CommandPart *cpp;
				get_list_delim(coll, i+1, 1, &del);
				set_list_delim(cp->option,del,1);
				cpp = (CommandPart*)cp->option->items_of_Coll[0];
				switch(cpp->type)
				{
				case SM_word:
				case SM_enum:
				case SM_literal:
					cp->isWord++;
					break;
				default:
					break;
				}
			}
			break;
		case SM_norm:
			{
				int j, hard=0;
				Coll *del=clone_coll(delim);
				for(j=i+1; !hard && j<coll->count_of_Coll;j++)
				{
					CommandPart *cpp=(CommandPart*)coll->items_of_Coll[j];
					switch (cpp->type)
					{
					case SM_word:
					case SM_literal:
						cadd_coll(&del, cpp->text);
						hard++;
						break;
					case SM_option:
						get_list_delim(cpp->option, 0, 1, &del);
						break;
					default:
						break;
					}
				}
				cp->delim=del;
			}
			break;
		case SM_list:
			cp->delim=clone_coll(delim);
			break;
		default:
			break;
		}
	}
}

static void
reorder_list(Coll *coll)
{
	int i;
	int list_pos=-1;
	CommandPart *lp=0;

	for(i=0;i<coll->count_of_Coll;i++)
	{
		CommandPart *cp=(CommandPart*)coll->items_of_Coll[i];
		switch (cp->type)
		{
		case SM_option:
			{
				CommandPart *cp1;

				if (cp->option->count_of_Coll!=1)
					continue;
				cp1=(CommandPart*)cp->option->items_of_Coll[0];
				if (cp1->type != SM_list)
					continue;
				if (list_pos != -1)
					continue;
				list_pos = i;
				lp = cp;
			}
			break;
		default:
			return;
		}
	}

	if (list_pos!=-1)
	{
		atRemove_Coll(coll, list_pos);
		append_Coll(coll, lp);
	}

}

static void
finaleCommandList(void)
{
	reorder_list(curCommandList);
	set_list_delim(curCommandList, NULL,0);
}


static void
putOffs(int level)
{
	while (level)
	{
		putchar('\t');
		level--;
	}
}




static void printCommandList(char *msg, Coll *coll, int level);

static void
vo_printf(int vlevel, int level, const char *fmt, ...)
{
	va_list ap;
	if (vlevel>verbose)
		return;

	putOffs(level);
	va_start(ap,fmt);
	vprintf(fmt,ap);
	va_end(ap);
}

static void
printCommandPart(CommandPart* cp, int level)
{
	/*vo_printf(0,level,"");*/
	switch(cp->type)
	{
	case M_NONE    :
		vo_printf(0,level,"M_NONE");
		break;
	case SM_literal:
		vo_printf(0,level,"SM_literal: %s", cp->text);
		/*printf("%s", cp->text);*/
		break;
	case SM_word   :
		vo_printf(0,level,"SM_word: %s", cp->text);
		/*printf(" %s ", cp->text);*/
		break;
	case SM_norm   :
		vo_printf(0,level,"SM_norm: <%s>", cp->text);
		/*printf("<%s>", cp->text);*/
		break;
	case SM_extend :
		vo_printf(0,level,"SM_extend: <(%s)>", cp->text);
		/*printf("<(%s)>", cp->text);*/
		break;
	case SM_list   :
		vo_printf(0,level,"SM_list: <%s,...>", cp->text);
		/*printf("<%s,...>", cp->text);*/
		break;
	case SM_enum   :
	{
		int i;
		vo_printf(0,level,"SM_enum: (%d) <%s:", cp->option->count_of_Coll, cp->text);
		/*printf("<%s: ", cp->text);*/
		for(i=0;i<cp->option->count_of_Coll;i++)
		{
			vo_printf(0,0,"%s", (char*)cp->option->items_of_Coll[i]);
			if (i<cp->option->count_of_Coll-1)
				printf(",");
		}
		vo_printf(0,0, ">");
		break;
	}
	case SM_patt   :
		vo_printf(0,level,"SM_patt: <*%s*>", cp->text);
		/*printf("<*%s*>", cp->text);*/
		break;
	case SM_option :
		/*vo_printf(0,0,"\n");*/
		/*vo_printf(0,level,"[\n");*/
		printCommandList("SM_option", cp->option, level);
		vo_printf(0,level,"end_option");
		/*vo_printf(0,level,"]");*/
		break;
	case TM_literal:
		vo_printf(0,level,"TM_literal: %s", cp->text);
		/*printf(" %s", cp->text);*/
		break;
	case TM_word   :
		vo_printf(0,level,"TM_word: %s", cp->text);
		/*printf(" %s", cp->text);*/
		break;
	case TM_norm   :
		vo_printf(0,level,"TM_norm: <%s>", cp->text);
		/*printf("<%s>", cp->text);*/
		break;
	case TM_noni   :
		vo_printf(0,level,"TM_noni: #<%s>", cp->text);
		/*printf("#<%s>", cp->text);*/
		break;
	case TM_str    :
		vo_printf(0,level,"TM_str: <\"%s\">", cp->text);
		/*printf("<\"%s\">", cp->text);*/
		break;
	case TM_intel  :
		vo_printf(0,level,"TM_intel: <(%s)>", cp->text);
		/*printf("<(%s)>", cp->text);*/
		break;
	case TM_block  :
		vo_printf(0,level,"TM_block: <{%s}>", cp->text);
		/*printf("<{%s}>", cp->text);*/
		break;
	case TM_log    :
		vo_printf(0,level,"TM_log: <.%s.>", cp->text);
		/*printf("<.%s.>", cp->text);*/
		break;
	case TM_var    :
		vo_printf(0,level,"TM_var: <#%s#>", cp->text);
		/*printf("<#%s#>", cp->text);*/
		break;
	case TM_join    :
		vo_printf(0,level,"TM_join: %s", cp->text);
		/*printf("%s", cp->text);*/
		break;
	case TM_option :
		vo_printf(0,level,"\n");
		/*vo_printf(0,level,"[\n");*/
		printCommandList("TM_option", cp->option, level);
		/*vo_printf(0,level,"]");*/
		vo_printf(0,level,"end_option");
		break;
	}
	if (cp->isWord)
	{
		vo_printf(0,0,"\n");
		vo_printf(0,level+1,"isWord='%d'", cp->isWord);
	}
	if (cp->isLast)
	{
		vo_printf(0,0,"\n");
		vo_printf(0,level+1, "isLast=1 ");
	}
	if (cp->delim)
	{
		int i;
		vo_printf(0,0,"\n");
		vo_printf(0, level+1,"%d delims:", cp->delim->count_of_Coll);
		for (i=0; i<cp->delim->count_of_Coll; i++)
			vo_printf(0, 0, " '%s'", (char*)cp->delim->items_of_Coll[i]);
	}
	vo_printf(0,0,"\n");
}

static void
printCommandList(char *msg, Coll *coll, int level)
{
	CommandPart *cp;
	int i;

	if(verbose>0)
	{
		putOffs(level);
		printf("%s: (%d)\n", msg, coll->count_of_Coll);
	}
	for(i=0; i< coll->count_of_Coll; i++)
	{
		cp=(CommandPart*)coll->items_of_Coll[i];
		printCommandPart(cp, level+1);
	}

}

static CommandPart *
findPart(Command *cp, CommandPart *sp)
{
	int j;
	for(j=0; j<cp->patternIt.count_of_Coll;j++)
	{
		CommandPart *pp=(CommandPart*)cp->patternIt.items_of_Coll[j];
		if (!strcmp(sp->text, pp->text))
			return pp;
	}
	return NULL;
}

static void
check_Command(Command *cp)
{
	int i;

	for(i=0; i<cp->substIt.count_of_Coll;i++)
	{
		CommandPart *sp=(CommandPart*)cp->substIt.items_of_Coll[i];
		if (sp->type == TM_join)
			continue;
		if (!findPart(cp, sp))
			yyerror("no subst marker '%s' in pattern part of command", sp->text);

	}
}

static void
skip_space(int *pos)
{
	for(;;)
		switch(yytext[*pos])
		{
		case ' ':
		case '\t':
		case '\r':
		case '\n':
			++(*pos);
			break;
		default:
			return;
		}
}

static void
skip_to_space(int *pos, char *initial, int ilen)
{
	int i;

	for(i=0; i<ilen; i++)
	{
		if (yytext[*pos] != initial[i])
			break;
		++(*pos);
	}

	for(;;)
		switch(yytext[*pos])
		{
		case ' ':
		case '\t':
		case '\r':
		case '\n':
		case 0:
			return;
		default:
			++(*pos);
			break;
		}
}

static void skip_string(int *pos, int quota);

static void
skip_brack(int *pos, int l, int r, int first)
{
	int level=1, c;

	for ( ; *pos< yyleng ; ++(*pos) )
	{
		c = yytext[*pos];
		if (!first || l!='[')
		{
		if (c=='"')
		{
			++(*pos);
			skip_string(pos, c);
			continue;
		}
		if (c=='\'')
		{
			++(*pos);
			skip_string(pos, c);
			continue;
		}
		}
		if (c==l)
		{
			level++;
			continue;
		}
		if (c==r)
		{
			level--;
			if (level<1)
				return;
			continue;
		}
		if (c=='\n'||c=='\r')
		{
			(*pos)--;
			return;
		}
	}
	if (*pos==yyleng)
		yyerror("unmatched bracket '%c' ", l);
}

static void
skip_string(int *pos, int quota)
{
	while ( *pos< yyleng && (yytext[*pos]!=quota ||
		(*pos>2 && yytext[(*pos)-1]=='\\' && yytext[(*pos)-2]=='&') ) )
	{
		if ( yytext[(*pos)+1] == '\n' )
			return;
		++(*pos);
	}
}

static int
is_gr(int ch)
{
	if (isalnum(ch))
		return 1;
	switch(ch)
	{
	case '"':
	case '\'':
	/*case '.':*/
	case '_':
	case '(':
	case ')':
	case '[':
	case ']':
	case '&':
		return 1;
	}
	return 0;
}

static int
is_id(int ch)
{
	if (isalnum(ch)||ch=='_')
		return 1;
	return 0;
}

static int
is_ident(char *s)
{
	if (!isalpha(*s) && *s!='_')
		return 0;
	for(++s; *s; ++s)
		if (!is_id(*s))
			return 0;
	return 1;
}

static int
is_op(int ch)
{
	switch(ch)
	{
	case '.':
	case ':':
	case '!':
	case '<':
	case '>':
	case '=':
	case '#':
	case '-':
	case '+':
	case '*':
	case '/':
	case '%':
	case '^':
	case '$':
		return 1;
	}
	return 0;
}

static int
is_lconst(int plch)
{
	switch(plch)
	{
	case 'f':
	case 'F':
	case 'n':
	case 'N':
	case 't':
	case 'T':
	case 'y':
	case 'Y':
		return 1;
	}
	return 0;
}

static int
is_pat(int ch)
{
	if (ch=='.' || ch=='*' || ch=='?')
		return 1;
	else
		return 0;
}

static int
is_expr(int plch, int lch, int rch, int nch, int pplch, int nnch)
{
	if ( lch == '.' && is_lconst(plch) && pplch=='.' && is_gr(rch) )
		return 0;
	if ( rch == '.' && toupper(lch)=='M' && !is_id(plch))
		return 1;
	if ( rch == '.' && is_lconst(nch) && nnch=='.' && is_id(lch) )
		return 0;
	if ( is_pat(lch) && is_pat(rch))
		return 1;
	if ( (is_op(lch) && (is_gr(rch)||rch==':'))
		|| (is_id(lch) && (rch=='('||rch=='['))
		|| (lch==':' && (rch==':'||rch=='='))
		|| (lch=='.' && rch=='.'  )
		|| (lch=='-' && rch=='>'  )
		|| (lch=='<' && rch=='>'  )
		|| (rch=='=' && (lch=='='||lch=='-'||lch=='+'||lch=='*'||lch=='/'||lch=='^'||lch=='<'||lch=='>'||lch=='!')  )
		|| (is_op(lch) && (rch=='!'||rch=='.'||rch=='-'||rch=='+'||rch=='{'))
		|| ((lch==')'||lch=='}'||is_gr(lch)) && is_op(rch))
		)
		return 1;
	return 0;
}

static int
delim_char(Coll *delims, int ch, int pos)
{
	int i;

	if (!delims)
		return 0;

	for(i=0; i<delims->count_of_Coll; i++)
	{
		char *s = (char*) delims->items_of_Coll[i];
#if 1
		if (s[0]==ch)
		{
			int l = strlen(s);
			int yl = yyleng-pos;
			if (l<=yl && !memcmp(s, yytext+pos, l))
				return 1;
		}
#else
		if (s[1]==0 && s[0]==ch)
			return 1;
#endif
	}

	return 0;
}

static void
skip_norm(int *pos,int extend, Coll *delims)
{
	int done;
	int plastch=0, lastch=0, ch, first=1, nch, bch, b, pplastch=0, nnch;
	for(done=0; *pos<yyleng ; ++(*pos))
	{
		bch = ch = yytext[*pos];
		b = *pos;
		switch(ch)
		{
		case '(': /* ) */
			if (delim_char(delims, ch, *pos))
				return;
			++(*pos);
			skip_brack(pos,'(',')',first);
			ch=')';
			break;
		case '{': /* } */
			if (delim_char(delims, ch, *pos))
				return;
			++(*pos);
			skip_brack(pos,'{','}',first);
			ch='}';
			break;
		case '[': /* ] */
			if (delim_char(delims, ch, *pos))
				return;
			++(*pos);
			skip_brack(pos,'[',']',first);
			ch=']';
			break;
		case '"':
		case '\'':
			++(*pos);
			skip_string(pos, yytext[*pos-1]);
			break;
		case ' ':
		case '\t':
				while( *pos<yyleng && ( yytext[*pos]==' ' || yytext[*pos]=='\t' ) )
					++(*pos);

		case ':':
		case '=':
		case '+':
		case '-':
		case '.':
		case '*':
		case '/':
		case '<':
		case '>':
		case '%':
		case '$':
		case '&':
		case '|':
			{
				if (first && (*pos<(yyleng-2)) && yytext[*pos]==':' && yytext[(*pos)+1]=='=')
				{
					return;
				}

				if ( extend && (ch=='.'||ch=='/'||ch==':'||ch=='&') )
					break;
				if (lastch==']' && yytext[*pos]=='[')
					continue;
				if (delims)
				{
					int j;
					int beg = *pos;
					char *s = yytext+beg;

					for(j=0; j<delims->count_of_Coll; j++)
					{
					int pl, len, found = 0;
					char *delim = (char*) delims->items_of_Coll[j];
					pl = strlen(delim);
					len = yyleng-beg;
					if (pl>len)
						break;
					if (pl <= len && !strncasecmp(s, delim, pl))
					{
						if (len==pl || !isalnum(s[pl]) || !isalnum(delim[0]))
							found=1;
					}
					else if (pl>4) /* partial match! */
					{
						int pp = beg;
						int l1;
						skip_to_space(&pp,delim,pl);
						l1 = pp-beg;
						if (l1 >= 4 && !strncasecmp(s, delim, l1))
						{
							found = 1;
							pl = l1;
							break;
						}
					}
					if (found)
					{
						v_printf(4, "skip_norm: match delim '%s', return at '%s'\n",
							delim, yytext+*pos);
						return;
					}
					}
				}

				ch = yytext[*pos];
				if (*pos < (yyleng-1))
					nch = yytext[*pos+1];
				else
					nch = 0;
				if (*pos < (yyleng-2))
					nnch = yytext[*pos+2];
				else
					nnch = 0;
				if ( *pos>0 && ( bch=='&' && is_id(yytext[*pos-1]) ))
				{
				}
				else if ( lastch && !is_expr(plastch, lastch, ch, nch, pplastch, nnch) )
				{
					*pos=b;
					return;
				}
				if (bch==' '||bch=='\t')
				{
					--(*pos);
					continue;
				}
			}
			break;
		case '\\':
		/*case '/':*/
			if (extend)
				break;
		case ',':
		case '\r':
		case '\n':
		case ')':
		case 0:
			done=1;
			break;
		}
		pplastch=plastch;
		plastch=lastch;
		lastch=ch;
		if (done)
			break;
		first=0;
	}
}

static int matchCommandPart(CommandPart *p, int *pos);


static int
matchCommandPartList(Coll *coll, int *pos, int top)
{
	int i, j, r, k;
	int count=coll->count_of_Coll;

	for(i=0; i<count;)
	{
		CommandPart *cp, *cpp;
		int beg, end;

		cp=(CommandPart*)coll->items_of_Coll[i];
		if (cp->type!=SM_option)
		{
			if (!matchCommandPart(cp, pos))
				return 0;
			++i;
			continue;
		}

		beg=i;
		for(j=i+1; j<count;j++)
		{
			cpp=(CommandPart*)coll->items_of_Coll[j];
			/*if ( cpp->type==SM_literal || cpp->type==SM_word )*/
			if (cpp->type!=SM_option)
				break;
		}
		end=j;

		if (end<count)
		{
			/* check first non-option arg */
			cpp=(CommandPart*)coll->items_of_Coll[end];
			if (
				(cpp->type==SM_literal||cpp->type==SM_word) &&
				matchCommandPart(cpp, pos) )
			{
				i=end+1;
				continue;
			}
		}
		for(k=beg; k<end;++k)
		{
			for(j=beg; j<end; ++j)
			{
				cpp=(CommandPart*)coll->items_of_Coll[j];
				if (cpp->isWord)
					r=matchCommandPart(cpp,pos);
				else
					continue;
				if (r)
					break;
			}
			if (j!=end)
			{
				skip_space(pos);
				continue;
			}
			for(j=beg; j<end; ++j)
			{
				cpp=(CommandPart*)coll->items_of_Coll[j];
				if (!cpp->isWord)
					r=matchCommandPart(cpp,pos);
				else
					continue;
				if (r)
					break;
			}
			if (j==end)
				break;
			skip_space(pos);
		}
		i = end;
	}


	return 1;
}

static char *
mem_Dup(const char *str, int len)
{
	char *r=(char*)malloc(len+1);
	int i;
	for( i=0; i<len; ++i)
		r[i]=toupper(str[i]);
	r[len]=0;
	return r;
}

static char *
mem_dup(const char *str, int len)
{
	char *r=(char*)malloc(len+1);
	memcpy(r, str, len);
	r[len]=0;
	return r;
}

static int
matchCommandPart(CommandPart *p, int *pos)
{
	skip_space(pos);
	switch(p->type)
	{
	case SM_option :
		{
			int r, save_pos, found=0, one=1, scmd_pos;
			CommandPart *bp = (CommandPart *)p->option->items_of_Coll[0];
			int cmdarg_pos= commandArgs.count_of_Coll;

			if ( bp->type == SM_literal && !strcmp(bp->text,",") )
				one=0;
#if 1
			if (bp->delim)
			{
				int pbeg = *pos;
				char *m = match_delim(bp, yytext+pbeg);
				if (m)
				{
					vo_printf (3,0,"match delim '%s' (at '%s')\n", m, yytext+*pos);
					return 0;
				}
			}
#endif

			do
			{
				save_pos=*pos;
				scmd_pos = commandArgs.count_of_Coll;
				vo_printf (3,0,"try match SM_option\n");
				r=matchCommandPartList(p->option, pos,0);
				if (!r)
					*pos=save_pos;
				else
					++found;

				if (r/*found>1*/)
				{
#if 0
					if (scmd_pos > cmdarg_pos)
					{
						int j, n;
						n=commandArgs.count-scmd_pos;
						vo_printf (3,0,"move from %d to %d %d items\n", scmd_pos, cmdarg_pos, n);
						for(j=0; j<n; j++)
						{
							void *p = commandArgs.items[scmd_pos+j];
							atRemove_Coll(&commandArgs, scmd_pos+j);
							atInsert_Coll(&commandArgs, p, cmdarg_pos+j);
						}
					}
#endif
				}

				if (*pos==save_pos)
					break;
				if (one)
					break;
			}
			while(r);
			vo_printf(3,0,"SM_option match: %d\n", found);
			if (!found)
			{
				while(commandArgs.count_of_Coll>cmdarg_pos)
					atFree_Coll(&commandArgs, commandArgs.count_of_Coll-1);
			}
			return found;
		}
		break;
	case SM_patt   :
		{
			int e;

			vo_printf (3,0,"try match SM_patt\n");
			if (*pos==yyleng)
				return 0;

			skip_space(pos);

			/* trim spaces */
			for(e=yyleng-1; e > *pos; --e)
				switch(yytext[e-1])
				{
				case ' ':
				case '\t':
				case '\n':
				case '\r':
					break;
				default:
					goto end;
				}
			end:
			vo_printf(3,0,"matched SM_patt: '%.*s'\n", e-*pos,yytext+*pos);
			insert_Coll(&commandArgs, new_CommandArg(p, yytext+*pos, e-*pos));
			*pos=yyleng;
		}
		break;
	case SM_word   :
		{
			char *s=yytext+*pos;
			char *e=yytext+yyleng;
			char *t=p->text;
			char *b = s;
			int len=0, r=1, i;
			vo_printf (3,0,"try match SM_word '%s' at '%s'\n", t, s);
			for( i=0 ;
#ifdef FOUR_BYTE
				(curCommand->isX || i<4) &&
#endif
					s<e && *t ; s++, t++, i++)
				if ( toupper(*s)!=toupper(*t) )
				{
					r=0;
					s=yytext+*pos;
					t=p->text;
					break;
				}
#ifdef FOUR_BYTE
			if (r && i==4)
			{
				for(;*s && *t && is_id(*s);s++,t++)
				{
					if(toupper(*s)!=toupper(*t))
					{
						r = 0;
						break;
					}
				}
			}
			else
#endif
			{
				if(*t)
					r=0;
			}

			if (r && is_id(*s))
				r = 0;
			len=s-b;
			if (r)
			{
				insert_Coll(&commandArgs, new_CommandArg(p,
					yytext+*pos, len));
				vo_printf (3,0,"SM_word match '%.*s'\n", len, yytext+*pos);
			}
			*pos+=len;
			return r;
		}
		break;
	case SM_literal:
		{
			char *s=yytext+*pos;
			char *e=yytext+yyleng;
			char *t=p->text;
			int len, r=1, i;
			vo_printf (3,0,"try match SM_literal '%s'\n", t);
			for( i=0 ; s<e && *t ; s++, t++, i++)
				if ( *s!=*t )
				{
					r=0;
					break;
				}
			if(*t)
				r=0;
			len=s-(yytext+*pos);
			if (r)
			{
				insert_Coll(&commandArgs, new_CommandArg(p,
					yytext+*pos, len));
				vo_printf (3,0,"SM_literal match '%.*s'\n", len, yytext+*pos);
			}
			*pos+=len;
			return r;
		}
		break;
	case SM_norm   :
	case SM_extend :
		{
			int beg=*pos;
			if (p->type==SM_norm)
				vo_printf (3,0,"try match SM_norm %s\n", p->text);
			else
				vo_printf (3,0,"try match SM_extend %s\n", p->text);
			if (beg==yyleng)
				return 0;

			#if 0
			if (p->delim && p->delim->count)
			{
				int j;
				int pbeg;
				int l;
				for(pbeg=beg; pbeg<yyleng;++pbeg)
				{
					for (j=0; j<p->delim->count; j++)
					{
					char *delim = (char*) p->delim->items[j];
					if (!delim)
						continue;
					l = strlen(delim);
					if ( l==1  && !is_id(delim[0]) && delim[0]!=','
						&& (!strncasecmp(yytext+pbeg,delim,l) ))
					{
						*pos = pbeg;
						insert_Coll(&commandArgs, new_CommandArg(p,
							yytext+beg, *pos-beg));
						vo_printf (3,0,"SM_norm match delim '%s' (at '%s')\n", delim, yytext+*pos);
						return 1;
					}

					}
				}
			}
			#endif

			skip_norm(pos, p->type==SM_extend, p->delim);
			insert_Coll(&commandArgs, new_CommandArg(p,
					yytext+beg, *pos-beg));
			vo_printf (3,0,"SM_%s match norm '%.*s'\n", p->type==SM_extend?"extend":"norm",*pos-beg, yytext+beg);
		}
		break;
	case SM_list   :
		{
			CommandArg *ap=newList_CommandArg(p);
			vo_printf (3,0,"try match SM_list\n");
			if (p->delim && p->delim->count_of_Coll)
			{

				int beg =*pos;
				int found=0;
				/*int pbeg;*/
				int single=0;
				char *delim = (char *)p->delim->items_of_Coll[0];
				int pl = strlen(delim);
				if ( !(p->delim->count_of_Coll == 1 && pl==1 && !is_id(delim[0]))  )
				/*
				if ( p->delim->count == 1 && pl==1 && !is_id(delim[0]) )
					single = 1;
				if ( (p->delim->count != 1 || (pl==1 && !is_id(delim[0])))  )
				*/
				{
				for(;*pos<yyleng;)
				{
					skip_space(pos);
					beg=*pos;
					skip_norm(pos,0,single?0:p->delim);
					if( *pos>beg  )
					{
						int ech;
						int j;
						ech = yytext[*pos];

						for(j = 0; j<p->delim->count_of_Coll; j++)
						{
							delim = (char*) p->delim->items_of_Coll[j];
							pl = strlen(delim);

							vo_printf(3,0,"try delim '%s' at '%s'\n", delim, yytext+beg);
							if ( (pl!=1 || is_id(delim[0]))
							   && (pl<=((*pos)-beg) && !strncasecmp(yytext+beg,delim,pl) && !is_id(yytext[beg+pl]) /*&& (!ech||isspace(ech))*/ )
							   )
							{
								found=1;
								*pos = beg;
								vo_printf (3,0,"SM_list match  delim '%s', ech='%c', rest='%s'\n", delim, ech, yytext+*pos);
								goto br;
							}
						}
						insert_Coll(ap->list, mem_dup(yytext+beg, *pos-beg));
						vo_printf (3,0,"SM_list match norm1 '%.*s'\n", *pos-beg, yytext+beg);
						found++;
					}
					skip_space(pos);
					if (yytext[*pos]==',')
					{
						if (p->isLast)
						{
							char *s = mem_dup(",", 1);
							if (!ap->rawlist)
								ap->rawlist = new_Coll(0,0);
							append_Coll(ap->rawlist, s );
							append_Coll(ap->list, s );
							vo_printf (3,0,"SM_list: 1:   ',' in last part\n");
						}
						else
						{
							ap->listcomma++;
							vo_printf (3,0,"SM_list match comma ','\n");
						}
						++(*pos);
					}
					if (beg==*pos)
						break;
				}
				}
				else
				#if 1
					goto nodelim;
				#else
				{
				for(pbeg=beg; pbeg<yyleng;++pbeg)
					if (!strncasecmp(yytext+pbeg,delim,pl))
					{
						*pos = pbeg;
						insert_Coll(ap->list, mem_dup(yytext+beg, *pos-beg));
						vo_printf (3,0,"SM_list match delim '%s'\n", delim);
						found=1;
						goto br;
						break;
					}
				}
				#endif

				if (!found)
				{
					delete_CommandArg(ap);
					*pos=beg;
					return 0;
				}
				/*else
					*pos=beg;*/
				br:
				;
			}
			else
			{
			int i;
		nodelim:
			for(i=0;*pos<yyleng;i++)
			{
				int beg;
				skip_space(pos);
				beg =*pos;
				if (beg>=yyleng)
					break;
				skip_norm(pos, 0,p->delim);
				if (*pos>beg)
				{
					insert_Coll(ap->list, mem_dup(yytext+beg, *pos-beg));
					vo_printf (3,0,"SM_list match norm2 '%.*s'\n", *pos-beg, yytext+beg);
				}
				else
					break;
				skip_space(pos);
				if (yytext[*pos]==',')
				{
					if (p->isLast)
					{
						char *s = mem_dup(",", 1);
						if (!ap->rawlist)
							ap->rawlist = new_Coll(0,0);
						append_Coll(ap->rawlist, s );
						append_Coll(ap->list, s );
						vo_printf (3,0,"SM_list: 2:  ',' in last part\n");
					}
					else
					{
						ap->listcomma++;
						vo_printf (3,0,"SM_list match comma ','\n");
					}
					++(*pos);
				}
#if 0
				else
					break;
#else
				else if (i && ap->listcomma)
				{
					vo_printf(3,0,"SM_list match: not first arg and was comma: reject\n");
					if (p->isLast)
						vo_printf(3,0,"SM_list match: but !isLast: reject reject :)");
					else
						break;
				}
				else if (!p->isLast)
				{
					break;
				}
#endif
			}
			}
			if (ap->list->count_of_Coll==0)
			{
				delete_CommandArg(ap);
				return 0;
			}

			insert_Coll(&commandArgs, ap);
		}
		break;
	case SM_enum   :
		{
			int beg=*pos, i, len, found=0, l=0;
			char *s=yytext+beg;

			len = yyleng-beg;

			vo_printf (3,0,"try match SM_enum: at %s\n", s);
			for(i=0;i<p->option->count_of_Coll;i++)
			{
				char *op = (char*) p->option->items_of_Coll[i];
				l = strlen(op);

				vo_printf (4,0,"try match SM_enum option: %s\n", op);
				if (l==1 && *op=='&')
				{
					if (*s=='&')
					{
						int pp = beg+1;
						skip_norm(&pp, 0, p->delim);
						l = pp - beg;
						found=1;
						break;
					}
				}
				else if (l <= len && !strncasecmp(s, op, l))
				{
					if (len==l || !isalnum(s[l]))
					{
						found=1;
						break;
					}
				}
				else if (l>4) /* partial match! */
				{
					int pp = beg;
					int l1;
					skip_to_space(&pp,op,l);
					l1 = pp-beg;
					if (l1 >= 4 && !strncasecmp(s, op, l1))
					{
						found = 1;
						l = l1;
						break;
					}
				}
			}
			if (!found)
				return 0;
			else
				vo_printf(3,0,"matched '%.*s'\n", l, s);
			insert_Coll(&commandArgs, newLog_CommandArg(p,s,l));
			*pos += l;
		}
		break;
	default:
		break;
	}
	return 1;
}

static int
matchCurCommand(int *pos)
{
	Command *savecp = curCommand;
	do
	{
		int i, r;
		*pos=0;

		freeAll_Coll(&commandArgs);
		if (command_flag)
		{
			printf("\ntry match command:");
			print_Command(curCommand);
			printf("yytext: '%s'\n", yytext);
		}

		r = matchCommandPartList(&curCommand->pattern, pos, 1);

		if (command_flag)
			printf("result: %d\n", r);

		if (!r)
			goto next;

		if (!curCommand->isTranslate)
		{
			for(i=*pos; i<yyleng; i++)
				switch ( yytext[i] )
				{
				case ' ':
				case '\t':
				case '\r':
				case '\n':
				case 032:
				case 0:
					break;
				default:
					if (command_flag)
						printf("command have a rest: result overrided to 0; rest='%s'\n",yytext+i);
					goto next;
				}
		}
		return 1;
		 next:
		;
	} while ( (curCommand=findNext_Command(curCommand->name, &commandInd))!=NULL );
	*pos=0;
	curCommand=savecp;
	return 0;
}

static int
findCommandPart(CommandPart *pp, const char *text)
{
		switch(pp->type)
		{
		case SM_word:
			return 0;
		default:
			break;
		}
		if (!strcasecmp(text, pp->text))
		{
			return 1;
		}
		return 0;
}

static CommandArg *
findArg(CommandPart *sp, int *beg, int reverse, int parno)
{
	int j;
	int found = 0;
	int foundpos = 0;
	CommandArg *foundap = 0;

	if (reverse)
	{
		for(j=*beg; j>=0; j--)
		{
			CommandArg *ap=(CommandArg*)commandArgs.items_of_Coll[j];
			CommandPart *pp=ap->cp;
			if (findCommandPart(pp, sp->text))
			{
				switch(pp->type)
				{
				case SM_literal:
				case SM_word:
					break;
				default:
					found++;
					if (found==2 && parno==1)
						goto ret;
					else if (found != parno || parno==1)
					{
						foundap = ap;
						foundpos = j-1;
						continue;
					}
				}

				*beg = j-1;
				return ap;
			}
		}
	}
	else
	{
		for(j=*beg; j<commandArgs.count_of_Coll;j++)
		{
			CommandArg *ap=(CommandArg*)commandArgs.items_of_Coll[j];
			CommandPart *pp=ap->cp;
			if (findCommandPart(pp, sp->text))
			{
				switch(pp->type)
				{
				case SM_literal:
				case SM_word:
					break;
				default:
					found++;
					if (found==2 && parno==1)
						goto ret;
					else if (found != parno || parno==1)
					{
						foundap = ap;
						foundpos = j+1;
						continue;
					}
				}
				*beg = j+1;
				return ap;
			}
		}
	}

ret:
	if (foundap && ((found==1 && parno==0)||(found>1&&parno==1)))
	{
		*beg = foundpos;
		return foundap;
	}

	return 0;
}


static int
testAllDefs(Coll *coll, int beg, int parno)
{
	int i;

	for(i=0; i<coll->count_of_Coll; ++i)
	{
		CommandPart *cp=(CommandPart*) coll->items_of_Coll[i];
		int Beg=beg;
		switch(cp->type)
		{
			case TM_norm:
			case TM_noni:
			case TM_str:
			case TM_intel:
			case TM_block:
			case TM_log:
			case TM_var:
				if (!findArg(cp,&Beg,0,parno))
					return 0;
			default:
				break;
		}
	}
	return 1;
}

static void
check_CommandPart(CommandPart*cp)
{
	if (!cp)
		return;
	switch (cp->type)
	{
	case TM_option:
		{
			Coll *option = cp->option;
			int i;
			int good=0;

			for(i=0; option && i<option->count_of_Coll; i++)
			{
				CommandPart *cpp;
				cpp = (CommandPart*) option->items_of_Coll[i];
				switch(cpp->type)
				{
					case TM_literal:
					case TM_word:
						break;
					default:
						good++;
						break;
				}
			}
			if (!good)
				yyerror("bad command definition: OPTION have only literals");

		}
		break;
	default:
		break;
	}
}

static void
next_quot(int *lquot, int *rquot)
{
	switch(*lquot)
	{
	case '"':
		*lquot = '\'';
		*rquot = '\'';
		break;
	case '\'':
#if 0
		*lquot = '[';
		*rquot = ']';
		break;
	case '[':
#endif
		*lquot = '"';
		*rquot = '"';
		break;
	}
}

char *
quot_str(char *str, int simple)
{
	int lq = '"', rq = '"';
	int l, nq, r, n, i;
	char *s, *d, *e;

	l = strlen(str);
	e = str + l;
	r = choose_quot(str, &lq, &rq, simple);
	if (!r)
	{
		if (lq && rq)
		{
			s = (char*) malloc(l+3);
			memcpy(s+1, str, l);
			s[0] = lq;
			s[l+1] = rq;
			s[l+2] = 0;
			return s;
		}
		else
		{
			return strdup(str);
		}
	}

	e = str + l - 1;
	for(s=str, nq=0; *s; s++)
	{
		switch(*s)
		{
		case '&':
			if (s[1]=='\\' && s+2 < e)
			{
				s++;
				s++;
			}
			break;
		case '\'':
		case '"':
		case '`':
		case '[':
		case ']':
			nq++;
			break;
		}
	}


	if ( l>1 && (str[0] == '"' || str[0]=='\'' || str[0]=='`' )
		&& str[l-1] == str[0] )
	{
		n = l-1;
		i = 1;
	}
	else
	{
		n = l;
		i = 0;
	}

	s = (char*) malloc(n + nq * 2 + 3);
	d = s;
	*d++ = '"';


	for( ; i<n; i++, d++)
	{
		switch(str[i])
		{
		case '&':
			if (str[i+1]=='\\' && str+i+2 < e)
			{
				*d++ = str[i];
				*d++ = str[i+1];
				*d = str[i+2];
				i += 2;
				continue;
			}
			break;
		case '\'':
		case '"':
		case '`':
		case '[':
		case ']':
			*d++ = '&';
			*d++ = '\\';
			break;
		}
		*d = str[i];
	}
	*d++ = '"';
	*d++ = 0;

	return s;
}

int
choose_quot(char *str,int *lquot, int *rquot, int simple)
{
	char *s;
	int ch,n=0, l;
	l = strlen(str);

	if (l>1 && !simple)
	{
		if ( (str[0] == '"' && str[l-1] == '"' && !memchr(str+1, '"', l-2) )
			|| (str[0] == '\'' && str[l-1] == '\'' && !memchr(str+1, '\'', l-2))
			|| (str[0] == '[' && str[l-1] == ']' && !memchr(str+1, ']', l-2))
		   )
		{
			*lquot = *rquot = 0;
			return 0;
		}
	}

	do
	{
		int lq = *lquot;
		int rq = *rquot;

		for(s=str; (ch=*s); ++s)
			if (ch==lq || ch==rq)
				break;
		if (!ch)
			return 0;
		next_quot(lquot, rquot);
	}
	while (n++<3);

	return -1;
	/*yyerror("cannot choose quota characters for string '%s'", str);*/
}

static void
substCommandPartList(Coll *coll, int *beg, int reverse, int parno)
{
	int i, isExpr, cnt = 0, prev_type = 0;
	int Beg=*beg, end=*beg;

	for(i = 0; i<coll->count_of_Coll; i++)
	{
		CommandArg *ap;
		CommandPart *cp=(CommandPart*) coll->items_of_Coll[i];
		*beg=Beg;
		if (cnt)
		{
			if (cp->type != TM_join && prev_type != TM_join)
				put_char(' ');
		}
		else
		{
			cnt++;
		}

		prev_type = cp->type;

		switch(cp->type)
		{
		case TM_word   :
		case TM_literal:
			put_str(cp->text);
			break;
		case TM_norm   :
			ap=findArg(cp,beg,reverse,parno);
			if (!ap)
				break;
			if (ap->text)
				put_str(ap->text);
			else if (ap->list)
			{
				int i;
				for(i=0; i<ap->list->count_of_Coll ;i++)
				{
					if (i>0)
					{
						if (ap->listcomma)
							put_char(',');
						else
							put_char(' ');
					}
					put_str((char*)ap->list->items_of_Coll[i]);
				}
			}
			else if(ap->val)
				put_str(ap->val);
			break;
		case TM_noni   :
			ap=findArg(cp,beg,reverse,parno);
			if (!ap)
			{
				put_str("\"\"");
				break;
			}
			else if (ap->text)
			{
				put_str_free(quot_str(ap->text, 0));
			}
			else if (ap->list)
			{
				int i;
				put_char('"');
				for(i = 0; i < ap->list->count_of_Coll; i++)
				{
					if (i>0)
					{
						if (ap->listcomma)
							put_char(',');
						else
							put_char(' ');
					}
					put_str((char*)ap->list->items_of_Coll[i]);
				}
				put_char('"');
			}
			else if(ap->val)
			{
				put_str_free(quot_str(ap->val,0));
			}
			break;
		case TM_str    :
			{
			ap=findArg(cp,beg,reverse,parno);
			if (!ap)
				break;
			if (ap->text)
			{
				int l = strlen(ap->text);
				if (ap->text[0]=='&' && is_ident(ap->text+1))
				{
					put_mem(ap->text+1, l-1);
				}
				else if ( l>3 && ap->text[0]=='&'
					&&  ap->text[1]=='(' && ap->text[l-1]==')' )
				{
					put_mem(ap->text+2, l-3);
				}
				else
				{
					put_str_free(quot_str(ap->text,1));
				}
			}
			else if (ap->list)
			{
				int i;
				for(i = 0; i < ap->list->count_of_Coll ;i++)
				{
					char *s = (char*)ap->list->items_of_Coll[i];
					if (i > 0)
					{
						if (ap->listcomma)
							put_char(',');
						else
							put_char(' ');
					}
					if (ap->rawlist && indexOf_Coll(ap->rawlist, s, 0))
						put_str(s);
					else
						put_str_free(quot_str(s,0));
				}
			}
			else if(ap->val)
			{
				if (ap->val[0]=='&' && is_ident(ap->val+1))
				{
					put_str(ap->val+1);
				}
				else
				{
					put_str_free(quot_str(ap->val,0));
				}
			}
			}
			break;
		case TM_intel  :
			ap=findArg(cp,beg,reverse,parno);
			if (!ap)
				break;
			vo_printf(4, 0, "TM_intel: %d %s\n", i, ap->text? ap->text:"LIST");
			if (ap->text)
			{
				int isMacro=0;
				char *s;

				if ( ap->text[0]=='(')
					isExpr=1;
				else if (ap->text[0]=='&')
					{
						if ( ap->text[1]=='(' ||  is_ident(ap->text+1) )
						{
							isExpr=1;
							isMacro=1;
						}
						else
							isExpr=0;
					}
				else
					isExpr=0;

				if (isMacro)
					s = ap->text+1;
				else
					s = ap->text;
				if (isExpr)
					put_str(s);
				else
					put_str_free(quot_str(s,0));

			}
			else if (ap->list)
			{
				int i;
				for(i = 0; i < ap->list->count_of_Coll ;i++)
				{
					char *text=(char*)ap->list->items_of_Coll[i];


					if (*text=='(')
						isExpr=1;
					else
						isExpr=0;

					if (i > 0)
					{
						if (ap->listcomma)
							put_char(',');
						else
							put_char(' ');
					}

					if (isExpr||!strcmp(text,","))
						put_str(text);
					else
						put_str_free(quot_str(text,0));

				}
			}
			else if(ap->val)
			{
				int isMacro=0;
				char *s;

				if ( ap->val[0]=='(')
					isExpr=1;
				else if (ap->val[0]=='&')
					{
						if ( ap->val[1]=='(' ||  is_ident(ap->val+1) )
						{
							isExpr=1;
							isMacro=1;
						}
						else
							isExpr=0;
					}
				else
					isExpr=0;

				if (isMacro)
					s = ap->val+1;
				else
					s = ap->val;
				if (isExpr)
					put_str(s);
				else
					put_str_free(quot_str(s,0));
			}
			break;
		case TM_block  :
			{
			int isBlock=0;
			ap=findArg(cp,beg,reverse,parno);
			if (!ap)
				break;
			if (ap->text)
			{
				if (ap->text[0]=='{')
					isBlock=1;
				else
					isBlock=0;
				if (!isBlock)
					put_str("{||");
				put_str(ap->text);
				if (!isBlock)
					put_char('}');
			}
			else if (ap->list)
			{
				int i;
				for(i=0;i<ap->list->count_of_Coll;i++)
				{
					char *text = (char*)ap->list->items_of_Coll[i];
					if (!strcmp(text, ","))
					{
						put_char(',');
						continue;
					}
					if (i>0)
					{
						if (ap->listcomma)
							put_char(',');
						else
							put_char(' ');
					}
					if (text[0]=='{')
						isBlock=1;
					else
						isBlock=0;
					if (!isBlock)
						put_str("{||");
					put_str(text);
					if (!isBlock)
						put_char('}');
				}
			}
			else if(ap->val)
			{
				if (ap->val[0]=='{')
					isBlock=1;
				else
					isBlock=0;
				if (!isBlock)
					put_str("{||");
				put_str(ap->val);
				if (!isBlock)
					put_char('}');
			}
			else
				put_str("{||.F.}");
			}
			break;
		case TM_log    :
			ap=findArg(cp,beg,reverse,parno);
			if (ap && (ap->text || (ap->list && ap->list->count_of_Coll) || ap->val))
				put_str(".T.");
			else
				put_str(".F.");
			break;
		case TM_var    :
			ap=findArg(cp,beg,reverse,parno);
			if (ap)
			{
				char name[128];
				snprintf(name, sizeof(name), "_%s_%d", cp->text, parno);
				put_str(name);
			}
			break;
		case TM_join:
			break;
		case TM_option :
			{
			int jj;
			for(jj=0;jj<64;jj++)
			{
				int b=0;
				if (jj)
				{
					if (testAllDefs(cp->option,b,jj))
					{
						vo_printf(4, 0, "TM_option: %d %d %d\n", i, b, jj);
						substCommandPartList(cp->option,&b,0,jj);
					}
					else
						break;
				}
				else
				{
					int kk, bb=b;
					for(kk=0; testAllDefs(cp->option,b,jj); kk++)
					{
						vo_printf(4, 0, "TM_option: %d %d %d\n", i, b, jj);
						substCommandPartList(cp->option,&b,0,jj);
						if (kk>64 && b==bb)
						{
							yyerror("endless subst loop detected");
							break;
						}
					}
				}
			}
			if (jj==64)
				yyerror("too many entries in OPTIONAL list: > %d\n", jj);
			}
			break;
		default:
			break;
		}
		if (*beg>end)
			end=*beg;
	}
	*beg=end;
}

static void
substCurCommand(int nl)
{
	int beg=0;
	vo_printf(4, 0, "substCurCommand: %s\n", curCommand->name);
	substCommandPartList(&curCommand->subst,&beg,0,0);

	if (nl)
		put_char('\n');
	else
		put_char(' ');

}


static void
print_Command(Command *cp)
{
	vo_printf(0,0,"#%s%s %s\n", cp->isX?"x":"", cp->isTranslate?"translate":"command", cp->name);
	printCommandList("pattern", &cp->pattern, 0);
	/*vo_printf(0,0,"\n=>\n");*/
	printCommandList("subst", &cp->subst, 0);
	vo_printf(0,0,"\n");
}

/* Command ] */

static void
load_file_charset(const char * cpath)
{
	char *e;
	int l;
	FILE *file;
	char path[256];

	strncpy(path, cpath, sizeof(path));

	e = strrchr(path, '/');
	if (!e)
		e = path;
	else
		e++;
	l = e-path;
	snprintf(e, sizeof(path)-l, ".clipcharset");
	file = fopen(path, "rt");
	if (file)
	{
		while (fgets(path, sizeof(path), file))
		{
			if (path[0] == '#')
				continue;
			l = strlen(path);
			if (l<2)
				continue;
			path[l-1] = 0;
			set_charset(strdup(path), cpath);
		}
		fclose(file);
	}
}


static int
startCurInclude(void)
{
	FILE *file=NULL;

	char path[256];

	if ( !curInclude->name )
	{
		yyerror("bad #include directive");
		free_IncludeFile(curInclude);
		return 0;
	}

	if (curInclude->name[0]=='/')
	{
		file=fopen(curInclude->name, "rt");
	}
	else if(curInclude->type=='<')
	{ /* "standard path" */
		int i;
		for(i=includePaths.count_of_Coll-1; i>=0; --i)
		{
			snprintf(path, sizeof(path), "%s/%s",
			(char*)includePaths.items_of_Coll[i], curInclude->name);
			file=fopen(path, "rt");
			if (file)
				break;
		}
	}
	else
	{ /* "current path" */
		int i;
		for(i=0;i<includePaths.count_of_Coll; ++i)
		{
			snprintf(path, sizeof(path), "%s/%s",
			(char*)includePaths.items_of_Coll[i], curInclude->name);
			file=fopen(path, "rt");
			if (file)
				break;
		}
	}

	if ( !file )
	{
		yyerror("cannot open include file '%s': %s", curInclude->name,
			strerror(errno));
		free_IncludeFile(curInclude);
		newline();
		/*yyrestart(NULL);*/
		return 0;
	}
	else
	{
		curInclude->buf=YY_CURRENT_BUFFER;
		curInclude->line=clic_line;
		newline();
		if (includes.count_of_Coll==1)
			insert_Coll(fileIncludes, strdup(curInclude->name));
		insert_Coll(&includes,curInclude);
		insert_Coll(&fileList,strdup(curInclude->name));
		yyin=file;
		yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));
		clic_line=1;
		push_charset();
		load_file_charset(path);
	}

	BEGIN(INITIAL);
	return 1;
}
static unsigned char
pg2pgch(unsigned char ch)
{
	switch (ch)
	{
	case PG_HLINE:
		return PGCH_HLINE;
	case PG_LLCORNER:
		return PGCH_LLCORNER;
	case PG_LRCORNER:
		return PGCH_LRCORNER;
	case PG_TTEE:
		return PGCH_TTEE;
	case PG_RTEE:
		return PGCH_RTEE;
	case PG_LTEE:
		return PGCH_LTEE;
	case PG_BTEE:
		return PGCH_BTEE;
	case PG_ULCORNER:
		return PGCH_ULCORNER;
	case PG_URCORNER:
		return PGCH_URCORNER;
	case PG_VLINE:
		return PGCH_VLINE;

	case PG_HLINE2:
		return PGCH_HLINE2;
	case PG_LLCORNER2:
		return PGCH_LLCORNER2;
	case PG_LRCORNER2:
		return PGCH_LRCORNER2;
	case PG_TTEE2:
		return PGCH_TTEE2;
	case PG_RTEE2:
		return PGCH_RTEE2;
	case PG_LTEE2:
		return PGCH_LTEE2;
	case PG_BTEE2:
		return PGCH_BTEE2;
	case PG_ULCORNER2:
		return PGCH_ULCORNER2;
	case PG_URCORNER2:
		return PGCH_URCORNER2;
	case PG_VLINE2:
		return PGCH_VLINE2;

	case PG_PLUS:
		return PGCH_PLUS;
	case PG_PLUS2:
		return PGCH_PLUS2;
	case PG_BLOCK:
		return PGCH_BLOCK;

	case PG_LARROW:
		return PGCH_LARROW;
	case PG_RARROW:
		return PGCH_RARROW;

	}
	return 0;
}



static char *source_charset=0;
static int change_charset=1;
char *sourceCharset=0;
char *targetCharset=0;
int changeCharset=1;

static unsigned char transtbl[256];
static unsigned char pgtbl[256];

void
recodeString(char *str)
{
	unsigned char *s;



	if (subst_flag)
		return;
	v_printf(3, "recodeString: subst %d: '%s'->'%s': '%s'\n", subst_flag, sourceCharset, targetCharset, str);
	/*recode_string(str);*/

	if (!sourceCharset || !targetCharset
		|| !strcmp(sourceCharset,targetCharset))
		return;

	if (changeCharset)
	{
		char *p1, *p2;
		cons_CharsetEntry *cs1 = 0, *cs2 = 0;
		int len1 = 0, len2 = 0;

		changeCharset=0;
		p1 = sourceCharset;
		p2 = targetCharset;
		if (!p1 || !p2 || !*p1 || !*p2)
			return;

		if (load_charset_name(p1, &cs1, &len1))
		{
			yywarning("cannot load charset '%s': %s",
				p1, strerror(errno));
			return;
		}

		if (load_charset_name(p2, &cs2, &len2))
		{
			yywarning( "cannot load charset '%s': %s",
				p2, strerror(errno));
			return;
		}

		make_translation(cs1, len1, cs2, len2, transtbl);

		memset(pgtbl, 0, sizeof(pgtbl));
		if (!charset_have_pg(cs1, len1) && charset_have_pg(cs2, len2))
		{
			load_charset_tables(p1, 0, 0, 0, 0, pgtbl);
		}

		free(cs1);
		free(cs2);
	}


	for(s=(unsigned char*)str; *s; ++s)
	{
		int s1, s2;

		s1 = *s;
		if ( (s1 & 0x80) && (s2 = pgtbl[s1]))
			*s = pg2pgch(s2);
		else if (s1>127)
			*s = transtbl[s1];
	}
}

static unsigned char trans_tbl[256];
static unsigned char pg_tbl[256];

static void
recode_string(char *str)
{
	unsigned char *s;


	v_printf(3, "recode_string: subst %d: '%s'->'%s': '%s'\n", subst_flag, source_charset, sourceCharset, str);
	if (subst_flag )
		return;


	if (!source_charset || !sourceCharset
		|| !strcmp(source_charset,sourceCharset))
		return;

	if (change_charset)
	{
		char *p1, *p2;
		cons_CharsetEntry *cs1 = 0, *cs2 = 0;
		int len1 = 0, len2 = 0;

		change_charset=0;
		p1 = source_charset;
		p2 = sourceCharset;
		if (!p1 || !p2 || !*p1 || !*p2)
			return;

		if (load_charset_name(p1, &cs1, &len1))
		{
			yywarning("cannot load charset '%s': %s",
				p1, strerror(errno));
			return;
		}

		if (load_charset_name(p2, &cs2, &len2))
		{
			yywarning( "cannot load charset '%s': %s",
				p2, strerror(errno));
			return;
		}

		make_translation(cs1, len1, cs2, len2, trans_tbl);
		memset(pg_tbl, 0, sizeof(pg_tbl));
		if (!charset_have_pg(cs1, len1) && charset_have_pg(cs2, len2))
		{
			load_charset_tables(p1, 0, 0, 0, 0, pg_tbl);
		}


		free(cs1);
		free(cs2);
	}

	for(s=(unsigned char*)str; *s; ++s)
	{
		int s1, s2;

		s1 = *s;
		if ( (s1 & 0x80) && (s2 = pg_tbl[s1]))
			*s = pg2pgch(s2);
		else if (s1>127)
			*s = trans_tbl[s1];
	}
}

static void
addCtext(int s)
{
	ctextPtr[ctextPos++]=s;
	if (ctextPos>=ctextLen)
	{
		ctextLen=ctextLen*3/2;
		ctextPtr=(char*)realloc(ctextPtr, ctextLen);
	}
}


static void
print_candidates(char *cmd)
{
	int ind;
	Command *cp;

	if (verbose<2)
		return;

	printf("candidates:\n");
	clic_word=0;
	for( cp = findFirst_Command(cmd, &ind); cp; cp = findNext_Command(cmd,&ind) )
	{
		print_Command(cp);
	}
}

static long yyin_pos = 0;
static long yyin_pos_count=0;
#define MAX_YYIN_POS_COUNT 16384

static void
command_end(void)
{
	int pos=0,l=0,found;
	char *s;
	long fpos;

	fpos = ftell(yyin);
	if (fpos == yyin_pos)
	{
		yyin_pos_count++;
		if (yyin_pos_count >= MAX_YYIN_POS_COUNT)
		{
			yyerror("circular loop in macro/command substitution detected");
			return;
		}
	}
	else
	{
		yyin_pos_count = 0;
		yyin_pos = fpos;
	}

	for(s=yytext+yyleng-1; *s=='\r'|| *s=='\n' || *s==032 || *s==';' ; s--)
	{
		if (*s=='\n')
		{
			/*newline();*/
			/*callEol++;*/
			/*clic_line--; all_lines--;*/
		}
		else
			*s=' ';
	}

	vo_printf(3, 0 ,"command_end:%d:'%.*s'\n", yyleng, yyleng, yytext);

	if ((found=matchCurCommand(&pos)))
	{
		l = yyleng-pos;
		substCurCommand(curCommand->isTranslate?0:1);

		if (curCommand->isTranslate)
		{
			if ( l)
				put_mem(yytext+pos, l);
			else
				put_char('\n');
		}

		free(callName);
		callName = 0;

	}
	else
	{
		unputmem(yytext, yyleng);
	}


	if ( curCommand->isTranslate && !bol_translate )
	{
		/*my_yy_set_bol(0);*/
	}

	if (!found)
	{
		if (!match_translate)
			unput(' ');

		if (curCommand->isTranslate)
			bad_translate = 1;
		else
			bad_translate = 0;

		unputstr(callName);
		free(callName);
		callName=0;
		vo_printf(6,0, "not subst %s state=%d bad_translate=%d\n", curCommand->name, command_next_state, bad_translate);
		BEGIN(command_next_state);
	}
	else
	{
		bad_translate = 0;
		vo_printf(6,0, "end subst %s state=%d nstr=%d buf '%.*s'\n",
			curCommand->name, command_state,
			unput_strings.count_of_Coll, unput_buffer.ptr_of_StrBuf-unput_buffer.buf_of_StrBuf, unput_buffer.buf_of_StrBuf);
		BEGIN(command_state);
	}
	if (found)
		newline();
}

void
prbrack(void)
{
	if (bracket_level <= 0)
		return;
	write_StrBuf(&bracket_buf, yytext, yyleng);
}

static void
set_charset(char *charset, const char *file)
{
	char *s = strchr(file, '.');
	int prgfile = 0;

	free(source_charset);
	change_charset = 1;
	if (s && !strcasecmp(s, ".prg"))
		prgfile = 1;

	charsets.items_of_Coll[charsets.count_of_Coll-1] = source_charset = charset;
	if (prgfile)
		sourceCharset = charset;

	if (!preproc_flag)
		v_printf(2, "set file '%s' charset to '%s'\n", file, charset);
}

static void
push_charset(void)
{
	change_charset = 1;
	source_charset = strdup(sourceCharset);
	append_Coll(&charsets, source_charset);
	v_printf(4, "push charset '%s' %d\n", source_charset, charsets.count_of_Coll);
}

static void
pop_charset(void)
{
	change_charset = 1;
	atFree_Coll(&charsets, charsets.count_of_Coll-1);
	source_charset = (char*) (charsets.items_of_Coll[charsets.count_of_Coll-1]);
	v_printf(4, "pop charset '%s' %d\n", source_charset, charsets.count_of_Coll);
}

static char *
match_delim(CommandPart *cp, char *str)
{
	int i;
	int len;

	if (!cp || !cp->delim)
		return 0;

	len = strcspn(str, " \t\n");

	for(i=0; i<cp->delim->count_of_Coll; i++)
	{
		int l;
		char *s = (char*) cp->delim->items_of_Coll[i];
		l = strlen(s);
		if ( l == len && !strncasecmp(s, str, l))
			return s;
	}

	return 0;
}

void
lex_vardef(int set)
{
	static int save = INITIAL;
	if (!compat_as_word)
		return;

	vo_printf(6, 0, "lex_vardef %d save=%d\n", set,save);

	if (set)
	{
		save = YYSTATE;
		BEGIN(vardef_words);
	}
	else
	{
		/*BEGIN(save);*/
		BEGIN(INITIAL);
	}
}


static int
m_return(int r)
{
/*
	if (YYSTATE==static_words && r!=STATIC)
	{
		BEGIN(INITIAL);
	}
*/
	last_ret = r;
	/*printf("mreturn: %d\n", r);*/
/*
	if (r=='\n')
		nnl++;
	else
		nnl=0;
*/
	/*if (preproc_flag)*/
	vo_printf(6,0,"\nm_return: %d %s %c state=%d\n", r,
		(r==NAME||r==STRING)?yylval.string:""
		, (r>32&&r<256)?r:' ', YYSTATE);
	return r;
}


static int
is_preproc_str(char *ibuf, int result)
{
	int i;
	for(i=0; i<result; i++)
	{
		int ch = ibuf[i];
		if (ch==' '||ch=='\t')
			continue;
		else if (ch=='#')
			return 1;
		else
			return 0;
	}
	return 0;
}

static int
is_empty_str(char *ibuf, int result)
{
	int i;
	for(i=0; i<result; i++)
	{
		int ch = ibuf[i];
		if (ch==' ' || ch=='\t' || ch=='\r' || ch=='\n')
			continue;
		else
			return 0;
	}
	return 1;
}

static int
get_input_str(char **ibuf, int *max_size)
{
	char *str;
	int result = 0;

	if (unput_buffer.ptr_of_StrBuf > unput_buffer.buf_of_StrBuf)
	{
		int l;
		l = unput_buffer.ptr_of_StrBuf - unput_buffer.buf_of_StrBuf;
		if (l >= *max_size)
		{
			*ibuf = (char *) realloc(*ibuf, l+1);
			*max_size = l+1;
		}
		memcpy(*ibuf, unput_buffer.buf_of_StrBuf, l);
		unput_buffer.ptr_of_StrBuf = unput_buffer.buf_of_StrBuf;
		result = l;
		if (subst_flag==1)
			subst_flag = 2;
		else
			subst_flag = 0;
	}
	else if (unput_strings.count_of_Coll)
	{
		int ind, l;
		StrBuf *sp;

		ind = unput_strings.count_of_Coll - 1;
		sp = (StrBuf*) unput_strings.items_of_Coll[ind];

		l = sp->ptr_of_StrBuf - sp->buf_of_StrBuf;
		if (l >= *max_size)
		{
			*ibuf = (char *) realloc(*ibuf, l+1);
			*max_size = l+1;
		}
		memcpy(*ibuf, sp->buf_of_StrBuf, l);
		result = l;

		atFree_Coll(&unput_strings, ind);
		subst_flag = 1;
		subst_count = 1;
	}
	else
	{
		str = fgets(*ibuf, *max_size - 1, yyin);
		if (!str)
		{
			if (ferror(yyin))
				YY_FATAL_ERROR( "input in flex scanner failed" );
			result = YY_NULL;
		}
		else
		{
			int l = strlen(str);
			memcpy(*ibuf, str, l);
			result = l;
		}
		subst_flag = 1;
		subst_count = 1;
	}

	if (result > 0)
	{
		if ((*ibuf)[result - 1] != '\n')
		{
			(*ibuf)[result] = '\n';
			result++;
			(*ibuf)[result] = 0;
		}
		if (is_empty_str( *ibuf, result ))
		{
			(*ibuf)[0] = '\n';
			(*ibuf)[1] = 0;
			return 1;
		}
	}
	else
		subst_flag = 0;


	return result;
}


static int
split_input_str(char *buf, int size)
{
	int count;
	int ins_ind = unput_strings.count_of_Coll;

	for(count = 0 ; size > 0 ; count++)
	{
		StrBuf *sp;
		char *b;
		int l;

		b = (char*) memchr(buf, '\000', size);
		if (!b)
		{
			if (!count)
				return 0;
			else
				l = size;
		}
		else
			l = b + 1 - buf;

		sp = new_StrBuf();
		write_StrBuf(sp, buf, l-1);
		putByte_StrBuf(sp, '\n');
		/*atInsert_Coll(&unput_strings, sp, 0);*/
		atInsert_Coll(&unput_strings, sp, ins_ind);
		if (b)
		{
			clic_line--;
			all_lines--;
		}

		buf += l;
		size -= l;
	}

	return count;
}

typedef enum
{
	PP_Normal,
	PP_Bracket,
	PP_Quote1,
	PP_Quote2,
	PP_Quote3,
	PP_Comment,
}
PP_State;

typedef struct
{
	PP_State pstate;
	PP_State rstate;
	char **obuf;
	int opos;
	int *olen;
	int brack_level;
	int comment_level;
	int pch;
}
P_State;

#define ISNAME( c )  ( isalnum( ( int ) c ) || ( c ) == '_' || ( c ) > 0x7E )

static void
addch_pp(P_State *pp, int ch)
{
	if(pp->opos >= (*pp->olen))
	{
		(*pp->olen) *= 2;
		(*pp->obuf) = (char *) realloc(*pp->obuf, *pp->olen);
	}

	(*pp->obuf)[pp->opos]=ch;
	(pp->opos)++;
}

/* return not 0 in need more strings */
static int
process_input_str(char *ibuf, int ilen, P_State *pp)
{
	int ch, i, space = 1;
	int do_comment = 0;
	int is_preproc = 0;
	int prev_opos = pp->opos;

	if ( is_preproc_str(ibuf, ilen)||is_preproc_str(*pp->obuf, pp->opos) )
	{
		is_preproc = 1;
	}
	if (pp->pstate != PP_Comment && is_preproc)
	{
		do_comment = 1;
	}

	for(i=0; i<ilen; i++)
	{
		ch = ibuf[i];

		switch(pp->pstate)
		{
		case PP_Normal:
		case PP_Bracket:
			switch(ch)
			{
			case '[':              /* { */
				/*if (do_comment)
					break;*/
				/*if (is_preproc)
					break;*/
				if (pp->pstate == PP_Bracket)
					pp->brack_level++;
				else if( !do_comment && (ISNAME(pp->pch) || strchr( ")]}.", pp->pch )) )
				{
					pp->rstate = pp->pstate;
					pp->pstate = PP_Bracket;
					pp->brack_level = 1;
				}
				else
				{
					pp->rstate = pp->pstate;
					pp->pstate = PP_Quote3;
				}
				break;

			case ']':
				/*if (do_comment)
					break;*/
				/*if (is_preproc)
					break;*/
				if (pp->pstate == PP_Bracket)
				{
					pp->brack_level--;
					if (pp->brack_level == 0)
					{
						pp->pstate = pp->rstate;
					}
				}
				break;

			case '\"':
				/*if (do_comment)
					break;*/
				/*if (is_preproc)
					break;*/
				pp->rstate = pp->pstate;
				pp->pstate = PP_Quote1;
				break;

			case '\'':
				/*if (do_comment)
					break;*/
				/*if (is_preproc)
					break;*/
				pp->rstate = pp->pstate;
				pp->pstate = PP_Quote2;
				break;

			case '/':
				if ((pp->opos)>0 && (*pp->obuf)[(pp->opos)-1] == '/')
				{
					--(pp->opos);
					addch_pp(pp,'\n');
					goto ret;
				}
				break;

			case '&':
				if ((pp->opos)>0 && (*pp->obuf)[(pp->opos)-1] == '&')
				{
					--(pp->opos);
					addch_pp(pp,'\n');
					goto ret;
				}
				break;

			case '*':
				if (space)
				{
					if (prev_opos)
						break;
					if (ilen>8 && !strncmp(ibuf, "*charset", 8))
						break;
					if (ilen>10 && !strncmp(ibuf, "*reference", 10))
						break;
					if (ilen>7 && !strncmp(ibuf, "*cmdstr", 7))
						break;
					addch_pp(pp,'\n');
					goto ret;
				}
				if ((pp->opos)>0 && (*pp->obuf)[(pp->opos)-1] == '/')
				{
					pp->rstate = pp->pstate;
					pp->comment_level = 1;
					pp->pstate = PP_Comment;
					pp->pch = 0;
					space = 0;
					(pp->opos)--;
					continue;
				}
				break;

			case ';': /* Split line later!!! */
				if (do_comment)
					break;

				addch_pp(pp,'\000');
				continue;

			default:
				break;
			}

			if (ch != ' ' && ch != '\t')
				pp->pch = ch;
			break;


		case PP_Quote1:
			if (ch == '\"')
			{
				pp->pstate = pp->rstate;
				pp->pch = ch;
			}
			break;

		case PP_Quote2:
			if (ch == '\'')
			{
				pp->pstate = pp->rstate;
				pp->pch = ch;
			}
			break;

		case PP_Quote3:
			if (ch == ']')
			{
				pp->pstate = pp->rstate;
				pp->pch = ch;
			}
			break;

		case PP_Comment:
			if (pp->pch == '/' && ch == '*')
				pp->comment_level++;
			else if (pp->pch == '*' && ch == '/')
				pp->comment_level--;

			if (pp->comment_level == 0)
				pp->pstate = pp->rstate;

			pp->pch = ch;
			continue;
		}

		if (space && ch != ' ' && ch!= '\t')
			space = 0;

		addch_pp(pp,ch);
	}

	  ret:
	switch(pp->pstate)
	{
	case PP_Comment:
		return 1;
	case PP_Quote1:
	case PP_Quote2:
	case PP_Quote3:
		if (is_preproc)
			pp->pstate = PP_Normal;
		/*else
			return 1;*/
	default:
		break;
	}

	for(i = pp->opos-1; i >= 0; i--)
	{
		switch((*pp->obuf)[i])
		{
		case ' ':
		case '\t':
		case '\r':
		case '\n':
			continue;
		case '\000':
			(*pp->obuf)[i] = ' ';
			pp->opos = i+1;
			return 1;
		case ';':
			if (do_comment)
			{
				(*pp->obuf)[i] = ' ';
				pp->opos = i+1;
				return 1;
			}
		default:
			break;
		}
		break;
	}

	return 0;
}

/*#define PP_READ_SIZE (YY_READ_BUF_SIZE*8)*/
#define PP_READ_SIZE 4096

static int
do_input(char *buf, int max_size)
{
	static char *ibuf = 0;
	static char *obuf = 0;
	static int ilen = 0, olen = 0;
	static int subst_state=0;

	int r = 0;
	P_State pstate;


	if (!ibuf)
	{
		ilen = PP_READ_SIZE;
		ibuf = (char*) malloc(ilen);
		olen = PP_READ_SIZE;
		obuf = (char*) malloc(olen);
	}

	pstate.obuf = &obuf;
	pstate.olen = &olen;
	pstate.opos = 0;
	pstate.pstate = pstate.rstate = PP_Normal;
	pstate.pch = 0;

	if (!subst_flag)
		subst_state = YYSTATE;

	for(;;)
	{
		r = get_input_str(&ibuf, &ilen);
		if (r <= 0 )
			break;

		if ( process_input_str(ibuf, r, &pstate) )
		{
			clic_line++;
			all_lines++;
			continue;
		}

		if ( split_input_str(*pstate.obuf, pstate.opos) )
		{
			pstate.pstate = PP_Normal;
			pstate.rstate = PP_Normal;
			pstate.opos = 0;
			pstate.pch = 0;
			continue;
		}

		break;
	}

	if (r>=0)
	{
		memcpy(buf, *pstate.obuf, pstate.opos);
		r = pstate.opos;
		if (is_preproc_str(buf, r))
		{
			subst_flag = 0;
			subst_count = 0;
		}
	}

	if (subst_count)
		subst_flag = 1;
	else if (subst_flag == 1)
		subst_flag = 2;

	if (YYSTATE == skipState)
		subst_flag = 0;

	subst_count = 0;

	if (preproc_flag)
	{
		int ind = -1, line = 0;
		char *file;
		get_include(&ind, &line, &file);
		vo_printf(3,0, "do_input: %d: subst_flag=%d: %d: '%.*s'\n", clic_line, subst_flag, r, r, buf);
		vo_printf(4,0, "state: %d at: file %s line %d\n", subst_state, file, line);
	}

	if (!subst_flag)
	{
		switch (YYSTATE)
		{
		case skipState:
		default:
			BEGIN(subst_state);
			break;
		case field_words:
		case for_words:
		case recover_words:
		case foreach_words:
		case field:
		case vardef_words:
		case static_words:
			BEGIN(INITIAL);
			break;
		}
		my_yy_set_bol(1);
		clic_pos=1;
		clic_word=0;
	}

	return r;
}

static void
put_buf(void)
{
	put_str( string_buf );
}

static void
put_char(int ch)
{
	subst_count++;
	if (ch=='\n')
	{
		StrBuf *sp;
		if (unput_buffer.ptr_of_StrBuf > unput_buffer.buf_of_StrBuf && unput_buffer.ptr_of_StrBuf[-1] == '\n')
		{
			sp = calloc(sizeof(StrBuf), 1);
			memcpy(sp, &unput_buffer, sizeof(StrBuf));
			memset(&unput_buffer, 0, sizeof(StrBuf));
			append_Coll(&unput_strings, sp);
		}
	}

	if (ch)
		putByte_StrBuf(&unput_buffer, ch);
}

static void
put_str(const char *str)
{
	int l=strlen(str);
	put_mem(str, l);
}

static void
unputstr(const char *str)
{
	int l=strlen(str);
	unputmem(str, l);
}

static void
unputmem(const char *str, int l)
{
	const char *s = str+l-1;
	for(; s >= str; --s)
		unput(*s);
}


static void
put_mem(const char *str, int l)
{
	const char *s = str+l;
	for(; str < s; ++str)
		put_char(*str);
}

static void
put_str_free(char *str)
{
	put_str(str);
	free(str);
}

static void
mecho(void)
{
	if (subst_flag)
		write_StrBuf(&unput_buffer, yytext, yyleng);
	else if (preproc_flag)
		fwrite( yytext, yyleng, 1, stdout );
	else
		prbrack();
}

static void
lecho(int l)
{
	if (subst_flag)
		write_StrBuf(&unput_buffer, yytext, l);
	else if (preproc_flag)
		fwrite(yytext, l, 1, stdout);
	else
		prbrack();
}

static void
pecho(char *p)
{
	int l = strlen(p);
	if (subst_flag)
		write_StrBuf(&unput_buffer, p, l);
	else if (preproc_flag)
		fwrite(p, l, 1, stdout);
	else
		prbrack();
}

static void
secho(int ch1, char *s, int ch2)
{
	if (subst_flag)
	{
		putByte_StrBuf(&unput_buffer, ch1);
		putString_StrBuf(&unput_buffer, s);
		putByte_StrBuf(&unput_buffer, ch2);
	}
	else if (preproc_flag)
	{
		fprintf(stdout, "%c%s%c", ch1, string_buf, ch2);
	}
}

static void
add_string_buf(char *buf, int len)
{
	if ( (string_buf_ptr + len) >= (string_buf + sizeof(string_buf) - 1) )
		yyerror("too long string");
	else
	{
		memcpy(string_buf_ptr, buf, len);
		string_buf_ptr += len;
	}
}

static void
addch_string_buf(int ch)
{
	if ( string_buf_ptr >= (string_buf + sizeof(string_buf) - 1) )
	{
		yyerror("too long string");
	}
	else
	{
		*string_buf_ptr = ch;
		string_buf_ptr++;
	}
}


static int
back_strsize(void *s1, void *s2)
{
	int l1 = strlen((char *)s1);
	int l2 = strlen((char *)s2);
	int r = l2-l1;

	if (r)
		return r;
	else
		return strcasecmp((char*)s1, (char*)s2);
}

void
lex_initial(void)
{
	BEGIN(INITIAL);
}

static char *
marker_id(char *str)
{
	char *b = str;
	char *e = b + strlen(b);
	char *s;
	int l;

	for (;b<e; b++)
	{
		if (is_id(*b))
			break;
	}

	for(;b<e; e--)
	{
		if (is_id(e[-1]))
			break;
	}

	l = e-b;
	s = mem_dup(b, l);

	return s;
}

#if 0
<macroCall>\\.  newpos(); add_string_buf(yytext, yyleng);
#endif
