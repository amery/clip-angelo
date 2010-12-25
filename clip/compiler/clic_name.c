#include <stdio.h>
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#if !YYDEBUG
#define static
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
int yymaxtoken=sizeof(yytname)/sizeof(char*);
#endif
