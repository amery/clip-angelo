/*
    Copyright (C) 2001  ITK
    Author   : Paul Lasarev <paul@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

*/
%{
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "ci_clic.h"

static char *i_name;
static int comment_caller;
static int brack;
static int string_caller;

%}

%x begfunc func comment string string1

wt	[ \t\n\r]
eol  	\r?(\n|\032)

%%

[^C\n\r/\"\']+	ECHO;
{eol}		fputc('\n',yyout);

CLIPPER{wt}+[^\(]+\([^\)]*\)	{
		char *s, *beg=0, *end=0, nl=0;
                for(s=yytext+7; *s; ++s)
                {
                	if (*s=='\n')
                        {
                                nl++;
			}
			if (!beg)
                        {
                        	switch(*s)
                                {
                                case ' ':
                                case '\n':
                                case '\r':
                                	break;
				default:
                                	beg=s;
                                }
                        }
                        else if (!end)
                        {
                        	switch(*s)
                                {
                                case ' ':
                                case '\n':
                                case '\r':
                                case '(':
                                	end=s;
                                	break;
                                }
                        }
		}
                for(s=beg; s<end;++s)
                	*s=toupper(*s);

                fprintf(yyout, "int ");
                fwrite(beg, end-beg, 1, yyout);
                fprintf(yyout, "( ClipMachine *_mp )");

		for(; nl>0; --nl)
                	fputc('\n',yyout);
		BEGIN(begfunc);
	}


<INITIAL,begfunc,func,comment>\"	ECHO; string_caller=YYSTATE; BEGIN(string);
<INITIAL,begfunc,func,comment>\'	ECHO; string_caller=YYSTATE; BEGIN(string1);
<string>[^\\\"]+	ECHO;
<string>\\\"	ECHO;
<string>\"	ECHO; BEGIN(string_caller);
<string1>[^\\\']+	ECHO;
<string1>\\\'	ECHO;
<string1>\'	ECHO; BEGIN(string_caller);
<string,string1>.	ECHO;

<INITIAL,begfunc,func>"/*"	ECHO; comment_caller=YYSTATE; BEGIN(comment);
<comment>[^*]+	ECHO;
<comment>"*/"	ECHO; BEGIN(comment_caller);
<comment>.	ECHO;

<begfunc>[^/{]+	ECHO;
<begfunc>\{	ECHO; BEGIN(func); brack=1;
<begfunc>.	ECHO;

<func>[^/r{}]+	ECHO;
<func>\{		ECHO; brack++;
<func>\}		{
		brack--;
		if (!brack)
		{
			fprintf(yyout, "return 0;}");
			BEGIN(INITIAL);
                }
                else
                {
                	ECHO;
		}
	}
<func>return{wt}*;   {
			char *s;
			fprintf(yyout, "return 0;");
                        for(s=yytext+6; *s; ++s)
                        	if (*s=='\n')
                                	fputc('\n', yyout);
                }
<func>.		ECHO;

.	ECHO;

%%
int yywrap()
{
	return 1;
}

int
preprocCFile( char *iname, char *oname )
{
   	FILE *ifile;
   	FILE *ofile;

   	ifile=fopen(iname, "r");
        if (!ifile)
        {
        	yyerror("cannot open file '%s': %s", iname, strerror(errno));
        	return 0;
	}
   	if (oname)
        {
   		ofile=fopen(oname, "w");
        	if (!ofile)
        	{
        		yyerror("cannot open file '%s': %s", oname, strerror(errno));
        		fclose(ifile);
        		return 0;
		}
        }
        else
        	ofile=stdout;

	i_name=iname;
   	yyin=ifile;
        yyout=ofile;
        fprintf(yyout, "#include \"ci_clip.h\"\n");
        fprintf(yyout, "#line 1 \"%s\"\n", i_name);

	while( yylex()>0 )
        	;

	fclose(ifile);
        if (oname)
        	fclose(ofile);
        return 1;
}

