/*
    Copyright (C) 2001  ITK
    Authors  : Uri Hnykin <uri@itk.ru>, Paul Lasarev <paul@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

*/

#include <string.h>
#include <sys/types.h>
#include <regex.h>

#include "ci_clip.h"
#include "ci_error.ch"

#define NEW(type)	((type*)calloc(sizeof(type),1))
#ifndef RE_NREGS
#define RE_NREGS	30
#endif

/*
   if found return registers: {start position, end position}
   reg:={}
   search("90 asdf 12 ", "[a-z]+", reg) -->
   reg={{4, 8}}
*/
int
clip_SEARCH(ClipMachine * ClipMachineMemory)
{
   int       c, sl, length, i;

   unsigned char *s = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &sl);	/* pattern */

   unsigned char *string = (unsigned char *) _clip_parcl(ClipMachineMemory, 2, &length);	/* string */

   unsigned char *str, *buf;

   int       start = _clip_parni(ClipMachineMemory, 4) - 1;	/*from (start position) */

   int       range = _clip_parni(ClipMachineMemory, 5) - 1;	/*range */

   ClipVar  *rg = _clip_par(ClipMachineMemory, 3);	/*registers */

   regex_t   preg;

   regmatch_t rmatch[RE_NREGS];

   if (s == NULL || string == NULL)
    {
       _clip_retl(ClipMachineMemory, 0);
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "SEARCH");
    }
   if (_clip_parinfo(ClipMachineMemory, 0) < 4)
      start = 0;
   if (_clip_parinfo(ClipMachineMemory, 0) < 5)
      range = length;
   if (range < 0)
      start += range + 1;
   if (start < 0)
      start = 0;
   range = abs(range) < length ? abs(range) : length;

   str = (unsigned char *) malloc(range + 1);
   memcpy(str, string + start, range);
   str[range] = 0;

   buf = (unsigned char *) malloc(sl + 1);
   memcpy(buf, s, sl);
   buf[sl] = 0;

   for (i = 0; i < sl; i++, s++)
    {
       if ((*s) > 127)
	  buf[i] = _clip_cmptbl[(*s)];
    }
   for (i = 0; i < range; i++, string++)
    {
       if (*(string + start) > 127)
	  str[i] = _clip_cmptbl[*(string + start)];

    }
   memset(&preg, 0, sizeof(preg));

   regcomp(&preg, (const char *) buf, REG_EXTENDED | (buf[0] == '^' ? REG_NEWLINE : 0));

   if (regexec(&preg, (const char *) str, RE_NREGS, rmatch, 0 /*REG_NOTBOL */ ) == 0)
    {
       int       j;

       for (j = 0; j < RE_NREGS; j++)
	{

	   ClipVar  *st;

	   ClipVar  *ed;

	   ClipVar  *el;

	   if ((_clip_parinfo(ClipMachineMemory, 0) < 3) || rmatch[j].rm_so == -1)
	      break;

	   st = NEW(ClipVar);
	   ed = NEW(ClipVar);
	   el = NEW(ClipVar);

	   el->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = ARRAY_type_of_ClipVarType;
	   el->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;

	   st->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	   st->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	   st->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	   st->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = rmatch[j].rm_so + 1 + start;

	   c = ++el->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
	   el->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar =
	    (ClipVar *) realloc(el->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar, sizeof(ClipVar) * c);
	   memset(el->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + c - 1, 0, sizeof(ClipVar));
	   _clip_clone(ClipMachineMemory, el->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + c - 1, st);

	   ed->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	   ed->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	   ed->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	   ed->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = rmatch[j].rm_eo + 1 + start;

	   c = ++el->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
	   el->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar =
	    (ClipVar *) realloc(el->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar, sizeof(ClipVar) * c);
	   memset(el->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + c - 1, 0, sizeof(ClipVar));
	   _clip_clone(ClipMachineMemory, el->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + c - 1, ed);
	   c = ++rg->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
	   rg->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar =
	    (ClipVar *) realloc(rg->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar, sizeof(ClipVar) * c);
	   memset(rg->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + c - 1, 0, sizeof(ClipVar));
	   _clip_dup(ClipMachineMemory, rg->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + c - 1, el);

	   _clip_delete(ClipMachineMemory, st);
	   _clip_delete(ClipMachineMemory, ed);
	   _clip_delete(ClipMachineMemory, el);

	}
       if (_clip_parinfo(ClipMachineMemory, 0) > 2)
	  _clip_clone(ClipMachineMemory, RETPTR(ClipMachineMemory), rg);

       _clip_retl(ClipMachineMemory, 1);

    }
   else
      _clip_retl(ClipMachineMemory, 0);
   regfree(&preg);
   free(str);
   free(buf);
   return 0;
}

void
destroy_c_regex(void *item)
{
   regex_t  *preg = (regex_t *) item;

   regfree(preg);
   free(preg);
}

int
clip_RGCOMP(ClipMachine * ClipMachineMemory)
{
   int       ret, sl, rc, i;

   unsigned char *s = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &sl);	/* pattern */

   unsigned char *buf;

   regex_t  *preg = NULL;

   if (s == NULL)
    {
       _clip_retl(ClipMachineMemory, 0);
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "RGCOMP");
    }
   preg = (regex_t *) malloc(sizeof(regex_t));

   buf = (unsigned char *) malloc(sl + 1);
   memcpy(buf, s, sl);
   buf[sl] = 0;

   for (i = 0; i < sl; i++, s++)
    {
       if ((*s) > 127)
	  buf[i] = _clip_cmptbl[(*s)];
    }
   rc = regcomp(preg, (const char *) buf, REG_EXTENDED | (buf[0] == '^' ? REG_NEWLINE : 0));

   ret = _clip_store_c_item(ClipMachineMemory, preg, _C_ITEM_TYPE_REGEX, destroy_c_regex);
   _clip_retni(ClipMachineMemory, ret);
   free(buf);

   return 0;
}

int
clip_RGEXEC(ClipMachine * ClipMachineMemory)
{
   int       c, length, i;

   int       item = _clip_parni(ClipMachineMemory, 1);	/* container pattern */

   unsigned char *string = (unsigned char *) _clip_parcl(ClipMachineMemory, 2, &length);	/* string */

   unsigned char *str;

   int       start = _clip_parni(ClipMachineMemory, 4) - 1;	/*from (start position) */

   int       range = _clip_parni(ClipMachineMemory, 5) - 1;	/*range */

   int       rerr;

   ClipVar  *rg = _clip_par(ClipMachineMemory, 3);	/*registers */

   regex_t  *preg = NULL;

   regmatch_t rmatch[RE_NREGS];

   if (string == NULL)
    {
       _clip_retl(ClipMachineMemory, 0);
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "SEARCH");
    }
   if (_clip_parinfo(ClipMachineMemory, 0) < 4)
      start = 0;
   if (_clip_parinfo(ClipMachineMemory, 0) < 5)
      range = length;
   if (range < 0)
      start += range + 1;
   if (start < 0)
      start = 0;
   range = abs(range) < length ? abs(range) : length;

   str = (unsigned char *) malloc(range + 1);
   memcpy(str, string + start, range);
   str[range] = 0;

   for (i = 0; i <= abs(range) - start; i++, string++)
    {
       if (*(string + start) > 127)
	  str[i] = _clip_cmptbl[*(string + start)];

    }

   preg = (regex_t *) _clip_fetch_c_item(ClipMachineMemory, item, _C_ITEM_TYPE_REGEX);
   rerr = regexec(preg, (const char *) str, RE_NREGS, rmatch, 0);
   if (rerr == 0)
    {
       int       j;

       for (j = 0; j < RE_NREGS; j++)
	{

	   ClipVar  *st;

	   ClipVar  *ed;

	   ClipVar  *el;

	   if ((_clip_parinfo(ClipMachineMemory, 0) < 3) || rmatch[j].rm_so == -1)
	      break;

	   st = NEW(ClipVar);
	   ed = NEW(ClipVar);
	   el = NEW(ClipVar);

	   el->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = ARRAY_type_of_ClipVarType;
	   el->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;

	   st->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	   st->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	   st->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	   st->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = rmatch[j].rm_so + 1 + start;

	   c = ++el->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
	   el->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar =
	    (ClipVar *) realloc(el->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar, sizeof(ClipVar) * c);
	   memset(el->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + c - 1, 0, sizeof(ClipVar));
	   _clip_clone(ClipMachineMemory, el->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + c - 1, st);

	   ed->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	   ed->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	   ed->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	   ed->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = rmatch[j].rm_eo + 1 + start;

	   c = ++el->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
	   el->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar =
	    (ClipVar *) realloc(el->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar, sizeof(ClipVar) * c);
	   memset(el->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + c - 1, 0, sizeof(ClipVar));
	   _clip_clone(ClipMachineMemory, el->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + c - 1, ed);
	   c = ++rg->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
	   rg->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar =
	    (ClipVar *) realloc(rg->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar, sizeof(ClipVar) * c);
	   memset(rg->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + c - 1, 0, sizeof(ClipVar));
	   _clip_dup(ClipMachineMemory, rg->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + c - 1, el);

	   _clip_delete(ClipMachineMemory, st);
	   _clip_delete(ClipMachineMemory, ed);
	   _clip_delete(ClipMachineMemory, el);

	}
       if (_clip_parinfo(ClipMachineMemory, 0) > 2)
	  _clip_clone(ClipMachineMemory, RETPTR(ClipMachineMemory), rg);

       _clip_retl(ClipMachineMemory, 1);

    }
   else
    {
       regerror(rerr, preg, (char *) str, range);
       _clip_retl(ClipMachineMemory, 0);
    }
   free(str);
   return 0;
}

int
clip_RGCANCEL(ClipMachine * ClipMachineMemory)
{
   int       item = _clip_parni(ClipMachineMemory, 1);	/* container pattern */

   _clip_destroy_c_item(ClipMachineMemory, item, _C_ITEM_TYPE_REGEX);
   return 0;
}

int
clip_SPLIT(ClipMachine * ClipMachineMemory)
{
   int       sl, length, i;

   unsigned char *string = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &length);	/* string */

   unsigned char *s = (unsigned char *) _clip_parcl(ClipMachineMemory, 2, &sl);	/* pattern */

   unsigned char *str, *buf;

   unsigned char *ustr;

   int       start = 0;		/*from (start position) */

   int       range = length;	/*range */

   int       b;

   long      l[2];

   ClipVar  *rg = RETPTR(ClipMachineMemory);

   regex_t   preg;

   regmatch_t rmatch[RE_NREGS];

   if (s == NULL || string == NULL)
    {
       _clip_retl(ClipMachineMemory, 0);
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "SEARCH");
    }

   str = (unsigned char *) malloc(range + 1);
   memcpy(str, string + start, range);
   str[range] = 0;

   buf = (unsigned char *) malloc(sl + 1);
   memcpy(buf, s, sl);
   buf[sl] = 0;

   for (i = 0; i < sl; i++, s++)
    {
       if ((*s) > 127)
	  buf[i] = _clip_cmptbl[(*s)];
    }
   ustr = string;
   for (i = 0; i < range; i++, string++)
    {
       if (*(string + start) > 127)
	  str[i] = _clip_cmptbl[*(string + start)];

    }
   memset(&preg, 0, sizeof(preg));

   regcomp(&preg, (const char *) buf, REG_EXTENDED | (buf[0] == '^' ? REG_NEWLINE : 0));

   l[0] = 0;
   _clip_array(ClipMachineMemory, rg, 1, l);

   b = 0;
   while (b <= range)
    {
       if (regexec(&preg, (const char *) (str + b), RE_NREGS, rmatch, 0 /*REG_NOTBOL */ ) == 0)
	{
	   int       j = 0;

	   ClipVar  *st;

	   j = rmatch[0].rm_so;
	   if (j == -1)
	    {
	       st = NEW(ClipVar);

	       j = range - b;
	       st->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
	       st->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf =
		realloc(st->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, j + 1);
	       memcpy(st->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, ustr + b, j);
	       st->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf[j] = 0;
	       st->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = j;
	       _clip_aadd(ClipMachineMemory, rg, st);

	       _clip_delete(ClipMachineMemory, st);
	       break;
	    }

	   st = NEW(ClipVar);

	   st->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
	   st->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf =
	    realloc(st->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, j + 1);
	   memcpy(st->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, ustr + b, j);
	   st->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf[j] = 0;
	   st->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = j;
	   _clip_aadd(ClipMachineMemory, rg, st);

	   _clip_delete(ClipMachineMemory, st);

	   b += rmatch[0].rm_eo;
	}
       else
	{
	   ClipVar  *st;

	   int       j;

	   st = NEW(ClipVar);

	   j = range - b;
	   st->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
	   st->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf =
	    realloc(st->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, j + 1);
	   memcpy(st->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, ustr + b, j);
	   st->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf[j] = 0;
	   st->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = j;
	   _clip_aadd(ClipMachineMemory, rg, st);

	   _clip_delete(ClipMachineMemory, st);
	   break;
	}
    }				// while

   regfree(&preg);
   free(str);
   free(buf);

   return 0;
}
