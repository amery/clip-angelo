/*
	Copyright (C) 2001  ITK
	Author  : Rushad Nabiullin <rust@itk.ru>
	License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI
*/
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "ci_error.ch"
#include "ci_rdd.h"

#define ADDLEX(lex) if(rd->rmfilter) astrcat(&rd->rmfilter,&rd->rmflen,(lex))

static void
astrcat(char **d, int *l, char *s)
{
   int ls = strlen(s);

   *d = realloc(*d, *l + ls + 1);
   strcpy(*d + *l, s);
   *l += ls;
}

int
rm_yylex(RDD_DATA * rd, int nomove)
{
   int r = 0;
   char c = *rd->ptr;
   char quote = 0;
   int ps = 0;
   char *oldptr = rd->ptr;

   if (!c)
      return RM_END_EXPR;

   if (rd->yylval)
      {
	 free(rd->yylval);
	 rd->yylval = NULL;
      }

   if (c != '\'' && c != '"' && c != '[')
      while (*(++rd->ptr) == ' ');
   else
      rd->ptr++;
   while (!r)
      {
	 if (quote)
	    {
	       if (c == quote)
		  {
		     quote = 0;
/*				while(*(rd->ptr)==' ') rd->ptr++;
				c = *rd->ptr;
				rd->ptr++;*/
		  }
	    }
	 else
	    {
	       if (c == '"' || c == '\'')
		  quote = c;
	    }
	 if (c == RM_END_EXPR)
	    {
	       if (rd->word)
		  {
		     rd->word = realloc(rd->word, rd->wlen + 1);
		     rd->word[rd->wlen] = 0;
		     rd->yylval = strdup(rd->word);
		     r = RM_WORD;
		     free(rd->word);
		     rd->word = NULL;
		     while (*(--rd->ptr) == ' ');
		  }
	       else
		  {
		     r = RM_ERROR;
		  }
	    }
	 else if (!quote && (c == RM_LP))
	    {
	       r = RM_LP;
	    }
	 else if (!quote && (c == RM_RP))
	    {
	       if (rd->word)
		  {
		     rd->word = realloc(rd->word, rd->wlen + 1);
		     rd->word[rd->wlen] = 0;
		     rd->yylval = strdup(rd->word);
		     r = RM_WORD;
		     free(rd->word);
		     rd->word = NULL;
		     while (*(--rd->ptr) == ' ');
		  }
	       else
		  {
		     r = RM_RP;
		  }
	    }
	 else if (!quote && (c == '.' && isalpha(*rd->ptr) && *(rd->ptr + 1) != '.'))
	    {
	       if (rd->word)
		  {
		     rd->word = realloc(rd->word, rd->wlen + 1);
		     rd->word[rd->wlen] = 0;
		     rd->yylval = strdup(rd->word);
		     r = RM_WORD;
		     free(rd->word);
		     rd->word = NULL;
		     while (*(--rd->ptr) == ' ');
		  }
	       else if (strncasecmp(rd->ptr, "OR.", 3) == 0)
		  {
		     r = RM_OR;
		     rd->ptr += 3;
		     while (*rd->ptr == ' ')
			rd->ptr++;
		  }
	       else if (strncasecmp(rd->ptr, "AND.", 4) == 0)
		  {
		     r = RM_AND;
		     rd->ptr += 4;
		     while (*rd->ptr == ' ')
			rd->ptr++;
		  }
	       else if (strncasecmp(rd->ptr, "NOT.", 4) == 0)
		  {
		     r = RM_NOT;
		     rd->ptr += 4;
		     while (*rd->ptr == ' ')
			rd->ptr++;
		  }
	    }
	 else if (!quote && (c == '=') && !ps)
	    {
	       if (rd->word)
		  {
		     rd->word = realloc(rd->word, rd->wlen + 1);
		     rd->word[rd->wlen] = 0;
		     rd->yylval = strdup(rd->word);
		     r = RM_WORD;
		     free(rd->word);
		     rd->word = NULL;
		     while (*(--rd->ptr) == ' ');
		  }
	       else if (*rd->ptr == '=')
		  {
		     r = RM_EEQU;
		     rd->ptr++;
		     while (*rd->ptr == ' ')
			rd->ptr++;
		  }
	       else
		  {
		     r = RM_EQU;
		     while (*rd->ptr == ' ')
			rd->ptr++;
		  }
	    }
	 else if (!quote && (c == '>') && !ps)
	    {
	       if (rd->word)
		  {
		     rd->word = realloc(rd->word, rd->wlen + 1);
		     rd->word[rd->wlen] = 0;
		     rd->yylval = strdup(rd->word);
		     r = RM_WORD;
		     free(rd->word);
		     rd->word = NULL;
		     while (*(--rd->ptr) == ' ');
		  }
	       else if (*rd->ptr == '=')
		  {
		     r = RM_LARE;
		     rd->ptr++;
		     while (*rd->ptr == ' ')
			rd->ptr++;
		  }
	       else
		  {
		     r = RM_LAR;
		     while (*rd->ptr == ' ')
			rd->ptr++;
		  }
	    }
	 else if (!quote && (c == '<') && !ps)
	    {
	       if (rd->word)
		  {
		     rd->word = realloc(rd->word, rd->wlen + 1);
		     rd->word[rd->wlen] = 0;
		     rd->yylval = strdup(rd->word);
		     r = RM_WORD;
		     free(rd->word);
		     rd->word = NULL;
		     while (*(--rd->ptr) == ' ');
		  }
	       else if (*rd->ptr == '=')
		  {
		     r = RM_LESE;
		     rd->ptr++;
		     while (*rd->ptr == ' ')
			rd->ptr++;
		  }
	       else if (*rd->ptr == '>')
		  {
		     r = RM_NEQU;
		     rd->ptr++;
		     while (*rd->ptr == ' ')
			rd->ptr++;
		  }
	       else
		  {
		     r = RM_LES;
		     while (*rd->ptr == ' ')
			rd->ptr++;
		  }
	    }
	 else if (!quote && (c == '!') && !ps)
	    {
	       if (rd->word)
		  {
		     rd->word = realloc(rd->word, rd->wlen + 1);
		     rd->word[rd->wlen] = 0;
		     rd->yylval = strdup(rd->word);
		     r = RM_WORD;
		     free(rd->word);
		     rd->word = NULL;
		     while (*(--rd->ptr) == ' ');
		  }
	       else if (*rd->ptr == '=')
		  {
		     r = RM_NEQU;
		     rd->ptr++;
		     while (*rd->ptr == ' ')
			rd->ptr++;
		  }
	       else
		  {
		     r = RM_NOT;
		     while (*rd->ptr == ' ')
			rd->ptr++;
		  }
	    }
	 else
	    {
	       if (c == '(')
		  ps++;
	       else if (c == ')')
		  ps--;
	       if (!rd->word)
		  {
		     rd->word = malloc(1);
		     rd->wlen = 1;
		  }
	       else
		  {
		     rd->wlen++;
		     rd->word = realloc(rd->word, rd->wlen);
		  }
	       rd->word[rd->wlen - 1] = c;
	       r = 0;
	       c = *rd->ptr;
	       rd->ptr++;
	    }
      }
   if (nomove)
      rd->ptr = oldptr;
   return r;
}

static void
rm_checkpar(char *c, char *end, int func)
{
   int ps = 1;
   char *b = c;
   int squote = 0;
   int dquote = 0;
   int comma = 0;

   while (ps && (++c < end))
      {
	 if ((*c == '\"') && !squote)
	    {
	       dquote = !dquote;
	    }
	 else if ((*c == '\'') && !dquote)
	    {
	       squote = !squote;
	    }
	 if (squote || dquote)
	    continue;
	 if (*c == ')')
	    {
	       ps--;
	    }
	 else if (*c == '(')
	    {
	       ps++;
	    }
	 else if (*c == ',' && ps == 1)
	    {
	       comma = 1;
	    }
	 else if ((*c == '<' || (*c == '>' && *(c - 1) != '-') || *c == '!' || *c == '=' || (*c == '.' && isalpha(*(c + 1)) && isalpha(*(c + 2)))) && !func)
	    {
	       return;
	    }
      }
   if (!ps)
      {
	 (*b) = RM_LP;
	 (*c) = RM_RP;
      }
}

int
rm_init(ClipMachine * ClipMemoryStore, RDD_DATA * rd, char *str, const char *__PROC__)
{
   char *end = str + strlen(str);
   int squote = 0;
   int dquote = 0;
   int ps = 0;

   for (rd->ptr = str; rd->ptr < end; rd->ptr++)
      {
	 if ((*rd->ptr == '\"') && !squote)
	    {
	       dquote = !dquote;
	    }
	 else if ((*rd->ptr == '\'') && !dquote)
	    {
	       squote = !squote;
	    }
	 if (squote || dquote)
	    continue;
	 if (*rd->ptr == '(')
	    {
	       if (!ps)
		  {
		     char *c = rd->ptr;
		     while (--c >= str && *c == ' ');
		     rm_checkpar(rd->ptr, end, !(c < str || *c == '.' || *c == '!' || *c == '('));
		     if (*rd->ptr == RM_LP)
			ps++;
		  }
	       else
		  {
		     *rd->ptr = RM_LP;
		     ps++;
		  }
	    }
	 else if (*rd->ptr == RM_RP)
	    {
	       ps--;
	    }
	 else if (*rd->ptr == ')' && ps)
	    {
	       *rd->ptr = RM_RP;
	       ps--;
	    }
      }
   dquote = 0;
   for (rd->ptr = str; rd->ptr < end; rd->ptr++)
      {
	 if (!dquote)
	    {
	       if (*rd->ptr == '\"' || *rd->ptr == '\'')
		  dquote = *rd->ptr;
	    }
	 else if (*rd->ptr == dquote)
	    {
	       dquote = 0;
	    }
	 if (dquote)
	    continue;
	 if (*rd->ptr == '(')
	    *rd->ptr = RM_LP;
	 else if (*rd->ptr == ')')
	    *rd->ptr = RM_RP;
	 else if (*rd->ptr == RM_LP)
	    *rd->ptr = '(';
	 else if (*rd->ptr == RM_RP)
	    *rd->ptr = ')';
	 else if (*rd->ptr == '>' && rd->ptr != str && *(rd->ptr - 1) == '-')
	    *rd->ptr = RM_AL;
      }
   rd->ptr = str;
   rd->word = 0;
   rd->wlen = 0;
   return 0;
}

static void
rm_and(unsigned int *lval, unsigned int *rval, int *lopt, int ropt, int bytes)
{
  //int i, b;
   unsigned int *rv = rval;
  //unsigned int *lv = lval;
   unsigned int *end = lval + bytes;

 beg_:
   if (lval >= end)
      goto end_;
   *lval &= *rv;
   lval++;
   rv++;
   goto beg_;
 end_:
   free(rval);
   *lopt += ropt;
   if (*lopt == 4)
      *lopt = 2;
   else if (*lopt > 0)
      *lopt = 1;
}

static void
rm_or(unsigned int *lval, unsigned int *rval, int *lopt, int ropt, int bytes)
{
   int i;

   for (i = 0; i < bytes; i++)
      lval[i] = lval[i] | rval[i];
   free(rval);
   if (*lopt == 2 && ropt == 2)
      *lopt = 2;
   else if ((*lopt == 2 && ropt == 1) || (*lopt == 1 && ropt == 2))
      *lopt = 1;
   else if (*lopt == 1 && ropt == 1)
      *lopt = 1;
   else
      *lopt = 0;
}

static void
rm_not(unsigned int *lval, unsigned int *rval, int bytes)
{
   int i;

   for (i = 0; i < bytes; i++)
      lval[i] = ~rval[i];
   free(rval);
}

static int
rm_invoper(int oper)
{
   switch (oper)
      {
      case RM_LAR:
	 return RM_LES;
      case RM_LARE:
	 return RM_LESE;
      case RM_LES:
	 return RM_LAR;
      case RM_LESE:
	 return RM_LARE;
      }
   return oper;
}

static int
rm_setscope(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_FILTER * fp, unsigned int *map, int oper, char *lval, char *rval, int bytes, int *optimize, int test, int *ic, const char *__PROC__)
{
   int i, j;
   ClipVar v, vv, vt, vvt;
   ClipVar *vp;
   char *b = NULL;
   char *e = NULL;
   char *l = malloc(strlen(lval) + 1);
   char *r = malloc(strlen(rval) + 1);
   char *s, *d, *l1, *r1;

   if (ic)
      *ic = 0;
   memset(&v, 0, sizeof(ClipVar));
   memset(&vv, 0, sizeof(ClipVar));
   memset(&vt, 0, sizeof(ClipVar));
   memset(&vvt, 0, sizeof(ClipVar));
   *optimize = 0;
   for (s = lval, d = l; *s; s++)
      {
	 if (*s != ' ')
	    *(d++) = *s;
      }
   *d = 0;
   while ((d = strstr(l, "FIELD->")))
      memmove(d, d + 7, strlen(d + 7) + 1);
   for (s = rval, d = r; *s; s++)
      {
	 if (*s != ' ')
	    *(d++) = *s;
      }
   *d = 0;
   while ((d = strstr(r, "FIELD->")))
      memmove(d, d + 7, strlen(d + 7) + 1);
   l1 = strdup(l);
   l1[strlen(l1) - 1] = 0;
   r1 = strdup(r);
   r1[strlen(r1) - 1] = 0;
   for (i = 0; i < rd->ords_opened; i++)
      {
	 if (rd->orders[i]->bforexpr.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
	    continue;
	 if (rd->orders[i]->unique)
	    continue;
	 if ((strcasecmp(rd->orders[i]->expr, l) == 0) || (strcasecmp(rd->orders[i]->expr, l1 + 1) == 0))
	    {
	       b = rval;
	       e = lval;
	    }
	 else if ((strcasecmp(rd->orders[i]->expr, r) == 0) || (strcasecmp(rd->orders[i]->expr, r1 + 1) == 0))
	    {
	       b = lval;
	       e = rval;
	       oper = rm_invoper(oper);
	    }
	 if (b)
	    {
	       char expr[1024];

	       if (!rd->orders[i]->vtbl->setscope)
		  {
		     *optimize = 0;
		     break;
		  }

	       rdd_expandmacro(rd->area, rd->rdhandle, b, expr);
	       if (_clip_eval_macro(ClipMemoryStore, expr, strlen(expr), &vv))
		  break;
	       if (_clip_eval(ClipMemoryStore, _clip_vptr(&vv), 0, NULL, &v))
		  break;
	       vp = _clip_vptr(&v);

	       rdd_expandmacro(rd->area, rd->rdhandle, e, expr);
	       if (_clip_eval_macro(ClipMemoryStore, expr, strlen(expr), &vvt))
		  break;
	       if (_clip_eval(ClipMemoryStore, _clip_vptr(&vvt), 0, NULL, &vt))
		  break;

	       if (ic)
		  *ic = rd->orders[i]->ic;
	       if (!test)
		  {
		     if (oper == RM_EQU)
			{
			   if (rd->orders[i]->vtbl->setscope(ClipMemoryStore, rd, rd->orders[i], vp, vp, map, fp->size, ClipMemoryStore->flags & EXACT_FLAG, __PROC__))
			      goto err;
			}
		     else if (oper == RM_EEQU)
			{
			   if (rd->orders[i]->vtbl->setscope(ClipMemoryStore, rd, rd->orders[i], vp, vp, map, fp->size, 1, __PROC__))
			      goto err;
			}
		     else if (oper == RM_NEQU)
			{
			   unsigned int *tm = calloc(sizeof(unsigned int), bytes);
			   if (rd->orders[i]->vtbl->setscope(ClipMemoryStore, rd, rd->orders[i], vp, vp, tm, fp->size, 0, __PROC__))
			      goto err;
			   rm_not(map, tm, bytes);
			}
		     else if (oper == RM_LAR)
			{
			   unsigned int *tm = calloc(sizeof(unsigned int), bytes);
			   if (rd->orders[i]->vtbl->setscope(ClipMemoryStore, rd, rd->orders[i], vp, NULL, map, fp->size, 0, __PROC__))
			      goto err;
			   if (rd->orders[i]->vtbl->setscope(ClipMemoryStore, rd, rd->orders[i], vp, vp, tm, fp->size, 0, __PROC__))
			      goto err;
			   for (j = 0; j < bytes; j++)
			      map[j] ^= tm[j];
			   free(tm);
			}
		     else if (oper == RM_LARE)
			{
			   if (rd->orders[i]->vtbl->setscope(ClipMemoryStore, rd, rd->orders[i], vp, NULL, map, fp->size, 0, __PROC__))
			      goto err;
			}
		     else if (oper == RM_LES)
			{
			   unsigned int *tm = calloc(sizeof(unsigned int), bytes);
			   if (rd->orders[i]->vtbl->setscope(ClipMemoryStore, rd, rd->orders[i], NULL, vp, map, fp->size, 0, __PROC__))
			      goto err;
			   if (rd->orders[i]->vtbl->setscope(ClipMemoryStore, rd, rd->orders[i], vp, vp, tm, fp->size, 0, __PROC__))
			      goto err;
			   for (j = 0; j < bytes; j++)
			      map[j] ^= tm[j];
			   free(tm);
			}
		     else if (oper == RM_LESE)
			{
			   if (rd->orders[i]->vtbl->setscope(ClipMemoryStore, rd, rd->orders[i], NULL, vp, map, fp->size, 0, __PROC__))
			      goto err;
			}
		  }
	       *optimize = 2;
	       break;
	    }
      }
   if (i == rd->ords_opened)
      ClipMemoryStore->m6_error = 2009;
   _clip_destroy(ClipMemoryStore, &v);
   _clip_destroy(ClipMemoryStore, &vv);
   _clip_destroy(ClipMemoryStore, &vt);
   _clip_destroy(ClipMemoryStore, &vvt);
   free(l);
   free(r);
   free(l1);
   free(r1);
   if (!(*optimize))
      memset(map, 0xff, bytes << 2);
   return 0;
 err:
   _clip_destroy(ClipMemoryStore, &v);
   _clip_destroy(ClipMemoryStore, &vv);
   free(l);
   free(r);
   free(l1);
   free(r1);
   return 1;
}

static int
rm_cmp(ClipMachine * ClipMemoryStore, int oper, ClipVar * vp1, ClipVar * vp2, int ic)
{
   int r;

   if (ic && (vp1->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType) && (vp2->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType))
      {
	 if (oper == RM_EEQU)
	    {
	       if (vp1->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf != vp2->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf)
		  r = vp1->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf - vp2->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
	       else
		  r = _clip_strnncasecmp(vp1->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, vp2->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf,
vp1->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, vp2->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
	    }
	 else
	    {
	       if (!vp1->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf && vp2->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf)
		  r = -1;
	       else
		  r = _clip_strncasecmp(vp1->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, vp2->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf,
min(vp1->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, vp2->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf));
	    }
      }
   else
      {
	 _clip_cmp(ClipMemoryStore, vp1, vp2, &r, 1);
      }
   switch (oper)
      {
      case RM_EQU:
	 return r;
      case RM_EEQU:
	 return r;
      case RM_NEQU:
	 return !r;
      case RM_LAR:
	 return !(r > 0);
      case RM_LARE:
	 return !(r >= 0);
      case RM_LES:
	 return !(r < 0);
      case RM_LESE:
	 return !(r <= 0);
      }
   return 0;
}

static int
rm_checkscope(ClipMachine * ClipMemoryStore, RDD_DATA * rd, unsigned int *map, int size, int oper, char *fname, ClipVar * val, int ic, const char *__PROC__)
{
   ClipVar key;
   unsigned int bytes = ((size + 1) >> 5) + 1;
   int i, b, bb, t, tt, fno;
   unsigned int oldrecno = rd->recno;
   int oldeof = rd->eof;
   int er;

   rd->eof = 0;
   memset(&key, 0, sizeof(ClipVar));
   fno = _rdd_fieldno(rd, _clip_casehashword(fname, strlen(fname)));
   for (i = 0; i < bytes; i++)
      {
	 if (map[i])
	    {
	       for (b = (i << 2), bb = 0; bb < 4; b++, bb++)
		  {
		     if (((char *) map)[b])
			{
			   for (t = (b << 3) + 1, tt = 0; tt < 8; t++, tt++)
			      {
				 if (_rm_getbit(map, size, t))
				    {
				       if ((er = rd->vtbl->rawgo(ClipMemoryStore, rd, t, 0, __PROC__)))
					  goto err;
				       if (rdd_takevalue(ClipMemoryStore, rd, fno, &key, __PROC__))
					  goto err;
/*
							if(_clip_eval(ClipMemoryStore,block,0,NULL,&key)) goto err;
*/
				       if (rm_cmp(ClipMemoryStore, oper, _clip_vptr(&key), val, ic))
					  {
					     _rm_clrbit(map, size, t);
					  }
				       _clip_destroy(ClipMemoryStore, &key);
				    }
			      }
			}
		  }
	    }
      }
   if ((er = rd->vtbl->rawgo(ClipMemoryStore, rd, oldrecno, 1, __PROC__)))
      goto err;
   rd->eof = oldeof;
   return 0;
 err:
   return 1;
}

static int
rm_intersectscope(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_FILTER * fp, unsigned int *map, int oper, char *lval, char *rval, int bytes, int *optimize, int test, int *ic, const char *__PROC__)
{
   int i;
   ClipVar v, vv /*,vt,vvt */ ;
   ClipVar *vp /*,*vpt */ ;
   char *b = NULL;
   char *e = NULL;
   char *l = malloc(strlen(lval) + 1);
   char *r = malloc(strlen(rval) + 1);
   char *s, *d, *l1, *r1;

   if (ic)
      *ic = 0;
   memset(&v, 0, sizeof(ClipVar));
   memset(&vv, 0, sizeof(ClipVar));
/*
	memset(&vt,0,sizeof(ClipVar));
	memset(&vvt,0,sizeof(ClipVar));
*/
   *optimize = 0;
   for (s = lval, d = l; *s; s++)
      {
	 if (*s != ' ')
	    *(d++) = *s;
      }
   *d = 0;
   while ((d = strstr(l, "FIELD->")))
      memmove(d, d + 7, strlen(d + 7) + 1);
   for (s = rval, d = r; *s; s++)
      {
	 if (*s != ' ')
	    *(d++) = *s;
      }
   *d = 0;
   while ((d = strstr(r, "FIELD->")))
      memmove(d, d + 7, strlen(d + 7) + 1);
   l1 = strdup(l);
   l1[strlen(l1) - 1] = 0;
   r1 = strdup(r);
   r1[strlen(r1) - 1] = 0;
   for (i = 0; i < rd->ords_opened; i++)
      {
	 if (rd->orders[i]->bforexpr.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
	    continue;
	 if (rd->orders[i]->unique)
	    continue;
	 if ((strcasecmp(rd->orders[i]->expr, l) == 0) || (strcasecmp(rd->orders[i]->expr, l1 + 1) == 0))
	    {
	       b = rval;
	       e = lval;
	    }
	 else if ((strcasecmp(rd->orders[i]->expr, r) == 0) || (strcasecmp(rd->orders[i]->expr, r1 + 1) == 0))
	    {
	       b = lval;
	       e = rval;
	       oper = rm_invoper(oper);
	    }
	 if (b)
	    {
	       char expr[1024];

	       if (!rd->orders[i]->vtbl->setscope)
		  {
		     *optimize = 0;
		     break;
		  }

	       rdd_expandmacro(rd->area, rd->rdhandle, b, expr);
	       if (_clip_eval_macro(ClipMemoryStore, expr, strlen(expr), &vv))
		  break;
	       if (_clip_eval(ClipMemoryStore, _clip_vptr(&vv), 0, NULL, &v))
		  break;
	       vp = _clip_vptr(&v);
/*
			rdd_expandmacro(rd->area,rd->rdhandle,e,expr);
			if(_clip_eval_macro(ClipMemoryStore,expr,strlen(expr),&vvt))
				break;
			if(_clip_eval(ClipMemoryStore,_clip_vptr(&vvt),0,NULL,&vt))
				break;
			vpt = _clip_vptr(&vvt);
*/
	       if (ic)
		  *ic = rd->orders[i]->ic;
	       if (!test)
		  {
		     if (rm_checkscope(ClipMemoryStore, rd, map, fp->size, oper, e, vp, *ic, __PROC__))
			goto err;
/*
				if(oper == RM_EQU){
					if(rd->orders[i]->vtbl->setscope(ClipMemoryStore,rd,rd->orders[i],vp,vp,map,
						fp->size,ClipMemoryStore->flags & EXACT_FLAG,__PROC__)) goto err;
				} else if(oper == RM_EEQU){
					if(rd->orders[i]->vtbl->setscope(ClipMemoryStore,rd,rd->orders[i],vp,vp,map,
						fp->size,1,__PROC__)) goto err;
				} else if(oper == RM_NEQU){
					unsigned int* tm = calloc(sizeof(unsigned int),bytes);
					if(rd->orders[i]->vtbl->setscope(ClipMemoryStore,rd,rd->orders[i],vp,vp,tm,
						fp->size,0,__PROC__)) goto err;
					rm_not(map,tm,bytes);
				} else if(oper == RM_LAR){
					unsigned int* tm = calloc(sizeof(unsigned int),bytes);
					if(rd->orders[i]->vtbl->setscope(ClipMemoryStore,rd,rd->orders[i],vp,NULL,map,
						fp->size,0,__PROC__)) goto err;
					if(rd->orders[i]->vtbl->setscope(ClipMemoryStore,rd,rd->orders[i],vp,vp,tm,
						fp->size,0,__PROC__)) goto err;
					for(j=0;j<bytes;j++)
						map[j] ^= tm[j];
					free(tm);
				} else if(oper == RM_LARE){
					if(rd->orders[i]->vtbl->setscope(ClipMemoryStore,rd,rd->orders[i],vp,NULL,map,
						fp->size,0,__PROC__)) goto err;
				} else if(oper == RM_LES){
					unsigned int* tm = calloc(sizeof(unsigned int),bytes);
					if(rd->orders[i]->vtbl->setscope(ClipMemoryStore,rd,rd->orders[i],NULL,vp,map,
						fp->size,0,__PROC__)) goto err;
					if(rd->orders[i]->vtbl->setscope(ClipMemoryStore,rd,rd->orders[i],vp,vp,tm,
						fp->size,0,__PROC__)) goto err;
					for(j=0;j<bytes;j++)
						map[j] ^= tm[j];
					free(tm);
				} else if(oper == RM_LESE){
					if(rd->orders[i]->vtbl->setscope(ClipMemoryStore,rd,rd->orders[i],NULL,vp,map,
						fp->size,0,__PROC__)) goto err;
				}
*/
		  }
	       *optimize = 2;
	       break;
	    }
      }
   if (i == rd->ords_opened)
      ClipMemoryStore->m6_error = 2009;
   _clip_destroy(ClipMemoryStore, &v);
   _clip_destroy(ClipMemoryStore, &vv);
/*
	_clip_destroy(ClipMemoryStore,&vt);
	_clip_destroy(ClipMemoryStore,&vvt);
*/
   free(l);
   free(r);
   free(l1);
   free(r1);
   if (!(*optimize))
      memset(map, 0xff, bytes << 2);
   return 0;
 err:
   _clip_destroy(ClipMemoryStore, &v);
   _clip_destroy(ClipMemoryStore, &vv);
   free(l);
   free(r);
   free(l1);
   free(r1);
   return 1;
}

static char *
rm_alias2name(int npseudo, RDD_PSEUDO * pseudo, char *alias)
{
   int i;

   for (i = 0; i < npseudo; i++)
      {
	 if (strcasecmp(alias, pseudo[i].alias) == 0)
	    return strdup(pseudo[i].name);
      }
   return strdup(alias);
}

static unsigned int *
rm_prim(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_FILTER * fp, int bytes, int *optimize, int npseudo, RDD_PSEUDO * pseudo, int test, unsigned int *obm, const char *__PROC__)
{
   unsigned int *bm;
   int oper;
   char *lval;
   char *rval;
   char *p;

   if (!obm)
      bm = calloc(sizeof(unsigned int), bytes);
   else
      bm = obm;
   rd->curlex = rm_yylex(rd, 0);
   switch (rd->curlex)
      {
      case RM_NOT:
	 {
	    unsigned int *tm;
	    ADDLEX("!");
	    tm = rm_prim(ClipMemoryStore, rd, fp, bytes, optimize, npseudo, pseudo, test, NULL, __PROC__);
	    if (!tm)
	       {
		  free(bm);
		  return NULL;
	       }
	    if (*optimize)
	       rm_not(bm, tm, bytes);
	    else
	       {
		  memcpy(bm, tm, bytes * 4);
		  free(tm);
	       }
	    return bm;
	 }
      case RM_LP:
	 ADDLEX("(");
	 free(bm);
	 bm = rm_expr(ClipMemoryStore, rd, fp, bytes, optimize, npseudo, pseudo, test, __PROC__);
	 if (rd->curlex != RM_RP)
	    {
	       free(bm);	// Right bracket expected
	       return NULL;
	    }
	 ADDLEX(")");
	 break;
      case RM_WORD:
	 {
	    ClipVar lb, rb, lv, rv;
	    char expr[1024];
	    int er;

	    p = lval = rm_alias2name(npseudo, pseudo, rd->yylval);
	    oper = rd->curlex = rm_yylex(rd, 0);
	    while (*p)
	       {
		  if (*p == RM_AL)
		     *p = '>';
		  p++;
	       }
	    if (oper != RM_EQU && oper != RM_EEQU && oper != RM_NEQU && oper != RM_LAR && oper != RM_LARE && oper != RM_LES && oper != RM_LESE)
	       {
		  rdd_expandmacro(rd->area, rd->rdhandle, lval, expr);
		  er = _clip_eval_macro(ClipMemoryStore, expr, strlen(expr), &lb);
		  if (er)
		     {
			ClipMemoryStore->m6_error = 2219;
			_clip_destroy(ClipMemoryStore, &lb);
			free(lval);
			free(bm);
			return NULL;
		     }
		  ClipMemoryStore->noerrblock++;
		  er = _clip_eval(ClipMemoryStore, _clip_vptr(&lb), 0, NULL, &lv);
		  ClipMemoryStore->noerrblock--;
		  _clip_destroy(ClipMemoryStore, &lb);
		  _clip_destroy(ClipMemoryStore, &lv);
		  if (er)
		     {
			ClipMemoryStore->m6_error = 2221;
			ADDLEX(".T.");
			if (!obm)
			   memset(bm, 0xff, bytes << 2);
		     }
		  else
		     {
			ADDLEX(lval);
			if (obm)
			   {
			      if (rm_intersectscope(ClipMemoryStore, rd, fp, bm, RM_EEQU, lval, ".T.", bytes, optimize, test, NULL, __PROC__))
				 return NULL;
			   }
			else
			   {
			      if (rm_setscope(ClipMemoryStore, rd, fp, bm, RM_EEQU, lval, ".T.", bytes, optimize, test, NULL, __PROC__))
				 return NULL;
			   }
		     }
		  free(lval);
		  return bm;
	       }
	    rd->curlex = rm_yylex(rd, 0);
	    if (rd->curlex != RM_WORD)
	       {
		  rdd_err(ClipMemoryStore, EG_ARG, 0, __FILE__, __LINE__, __PROC__, "Expression expected");
		  return NULL;
	       }
	    p = rval = rm_alias2name(npseudo, pseudo, rd->yylval);
	    while (*p)
	       {
		  if (*p == RM_AL)
		     *p = '>';
		  p++;
	       }
	    rdd_expandmacro(rd->area, rd->rdhandle, lval, expr);
	    if ((er = _clip_eval_macro(ClipMemoryStore, expr, strlen(expr), &lb)))
	       {
		  ClipMemoryStore->m6_error = 2219;
		  _clip_destroy(ClipMemoryStore, &lb);
		  free(lval);
		  free(rval);
		  free(bm);
		  return NULL;
	       }
	    ClipMemoryStore->noerrblock++;
	    er = _clip_eval(ClipMemoryStore, _clip_vptr(&lb), 0, NULL, &lv);
	    ClipMemoryStore->noerrblock--;
	    _clip_destroy(ClipMemoryStore, &lb);
	    _clip_destroy(ClipMemoryStore, &lv);
	    if (!er)
	       {
		  rdd_expandmacro(rd->area, rd->rdhandle, rval, expr);
		  if ((er = _clip_eval_macro(ClipMemoryStore, expr, strlen(expr), &rb)))
		     {
			ClipMemoryStore->m6_error = 2219;
			_clip_destroy(ClipMemoryStore, &rb);
			free(lval);
			free(rval);
			free(bm);
			return NULL;
		     }
		  ClipMemoryStore->noerrblock++;
		  er = _clip_eval(ClipMemoryStore, _clip_vptr(&rb), 0, NULL, &rv);
		  ClipMemoryStore->noerrblock--;
		  _clip_destroy(ClipMemoryStore, &rb);
		  _clip_destroy(ClipMemoryStore, &rv);
	       }
	    if (er)
	       {
		  ClipMemoryStore->m6_error = 2221;
		  ADDLEX(".T.");
		  if (!obm)
		     memset(bm, 0xff, bytes << 2);
	       }
	    else
	       {
		  int ic = 0;
		  if (obm)
		     {
			if (rm_intersectscope(ClipMemoryStore, rd, fp, bm, oper, lval, rval, bytes, optimize, test, &ic, __PROC__))
			   return NULL;
		     }
		  else
		     {
			if (rm_setscope(ClipMemoryStore, rd, fp, bm, oper, lval, rval, bytes, optimize, test, &ic, __PROC__))
			   return NULL;
		     }
		  if (ic)
		     ADDLEX("xupper(");
		  ADDLEX(lval);
		  if (ic)
		     ADDLEX(")");
		  switch (oper)
		     {
		     case RM_EQU:
			ADDLEX("=");
			break;
		     case RM_EEQU:
			ADDLEX("==");
			break;
		     case RM_NEQU:
			ADDLEX("!=");
			break;
		     case RM_LAR:
			ADDLEX(">");
			break;
		     case RM_LARE:
			ADDLEX(">=");
			break;
		     case RM_LES:
			ADDLEX("<");
			break;
		     case RM_LESE:
			ADDLEX("<=");
			break;
		     }
		  if (ic)
		     ADDLEX("xupper(");
		  ADDLEX(rval);
		  if (ic)
		     ADDLEX(")");
	       }
	    free(lval);
	    free(rval);
	    break;
	 }
      default:
	 return NULL;
      }
   rd->curlex = rm_yylex(rd, 0);
   return bm;
}

static unsigned int *
rm_term(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_FILTER * fp, int bytes, int *optimize, int npseudo, RDD_PSEUDO * pseudo, int test, const char *__PROC__)
{
   unsigned int *bm = rm_prim(ClipMemoryStore, rd, fp, bytes, optimize, npseudo, pseudo, test,
			      NULL, __PROC__);
   unsigned int *bm2;
   int opt2, recs;

   if (!bm)
      return NULL;
   for (;;)
      {
	 if (rd->curlex == RM_AND)
	    {
	       if (*optimize == 2)
		  {
		     unsigned int bytes = ((fp->size + 1) >> 5) + 1;
		     int i;	//,b,bb,t,tt;
		     unsigned int tmp1;	//,tmp2;

		     recs = 0;
		     for (i = 0; i < bytes; i++)
			{
			   tmp1 = bm[i];
			   if (!tmp1)
			      continue;
			  //if (tmp1 & 0x000000FF)
			   recs += ((tmp1 & 0x00000001) != 0) +
			      ((tmp1 & 0x00000002) != 0) +
			      ((tmp1 & 0x00000004) != 0) +
			      ((tmp1 & 0x00000008) != 0) + ((tmp1 & 0x00000010) != 0) + ((tmp1 & 0x00000020) != 0) + ((tmp1 & 0x00000040) != 0) + ((tmp1 & 0x00000080) != 0);
			  //if (tmp1 & 0x0000FF00)
			   recs += ((tmp1 & 0x00000100) != 0) +
			      ((tmp1 & 0x00000200) != 0) +
			      ((tmp1 & 0x00000400) != 0) +
			      ((tmp1 & 0x00000800) != 0) + ((tmp1 & 0x00001000) != 0) + ((tmp1 & 0x00002000) != 0) + ((tmp1 & 0x00004000) != 0) + ((tmp1 & 0x00008000) != 0);
			  //if (tmp1 & 0x00FF0000)
			   recs += ((tmp1 & 0x00010000) != 0) +
			      ((tmp1 & 0x00020000) != 0) +
			      ((tmp1 & 0x00040000) != 0) +
			      ((tmp1 & 0x00080000) != 0) + ((tmp1 & 0x00100000) != 0) + ((tmp1 & 0x00200000) != 0) + ((tmp1 & 0x00400000) != 0) + ((tmp1 & 0x00800000) != 0);
			  //if (tmp1 & 0xFF000000)
			   recs += ((tmp1 & 0x01000000) != 0) +
			      ((tmp1 & 0x02000000) != 0) +
			      ((tmp1 & 0x04000000) != 0) +
			      ((tmp1 & 0x08000000) != 0) + ((tmp1 & 0x10000000) != 0) + ((tmp1 & 0x20000000) != 0) + ((tmp1 & 0x40000000) != 0) + ((tmp1 & 0x80000000) != 0);
			  /*
			     for(b=(i<<2),bb=0;bb<4;b++,bb++){
			     if( !(((char*)bm)[b]) )
			     continue;
			     for(t=(b<<3)+1,tt=0;tt<8;t++,tt++){
			     if(_rm_getbit(bm,fp->size,t))
			     recs++;
			     }
			     }
			   */
			}
		  }
	       else
		  {
		     recs = fp->size;
		  }
	       ADDLEX(" .and. ");
	       if ((recs < (fp->size >> 10)) && (rm_yylex(rd, 1) != RM_LP))
		  {
		     if (!(rm_prim(ClipMemoryStore, rd, fp, bytes, &opt2, npseudo, pseudo, test, bm, __PROC__)))
			return NULL;
		  }
	       else
		  {
		     if (!(bm2 = rm_prim(ClipMemoryStore, rd, fp, bytes, &opt2, npseudo, pseudo, test, NULL, __PROC__)))
			return NULL;
		     rm_and(bm, bm2, optimize, opt2, bytes);
		  }
	    }
	 else
	    break;
      }
   return bm;
}

unsigned int *
rm_expr(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_FILTER * fp, int bytes, int *optimize, int npseudo, RDD_PSEUDO * pseudo, int test, const char *__PROC__)
{
   unsigned int *bm = rm_term(ClipMemoryStore, rd, fp, bytes, optimize, npseudo, pseudo, test,
			      __PROC__);
   unsigned int *bm2;
   int opt2;

   if (!bm)
      return NULL;
   for (;;)
      {
	 if (rd->curlex == RM_OR)
	    {
	       ADDLEX(" .or. ");
	       if (!(bm2 = rm_term(ClipMemoryStore, rd, fp, bytes, &opt2, npseudo, pseudo, test, __PROC__)))
		  return NULL;
	       rm_or(bm, bm2, optimize, opt2, bytes);
	    }
	 else
	    break;
      }
   return bm;
}

int
rm_checkjoin(ClipMachine * ClipMemoryStore, RDD_FILTER * f1, RDD_FILTER * f2, const char *__PROC__)
{
   if (!f1 || !f2)
      {
	 rdd_err(ClipMemoryStore, EG_ARG, 0, __FILE__, __LINE__, __PROC__, "Invalid handle to filter");
	 goto err;
      }
   if (f1->custom != f2->custom)
      {
	 rdd_err(ClipMemoryStore, EG_ARG, 0, __FILE__, __LINE__, __PROC__, "Trying to join custom and ordinary filter");
	 goto err;
      }
   if (f1->rd != f2->rd)
      {
	 rdd_err(ClipMemoryStore, EG_ARG, 0, __FILE__, __LINE__, __PROC__, "Trying to join different areas filters");
	 goto err;
      }
   if ((f1->rmap && !f2->rmap) || (!f1->rmap && f2->rmap))
      {
	 rdd_err(ClipMemoryStore, EG_ARG, 0, __FILE__, __LINE__, __PROC__, "Trying to join optimized and non-optimized filters");
	 goto err;
      }
   if (f1->size != f2->size)
      {
	 rdd_err(ClipMemoryStore, EG_ARG, 0, __FILE__, __LINE__, __PROC__, "Trying to join filters of different lengths");
	 goto err;
      }
   if (!f1->custom)
      {
	 if (!f1->sfilter || !f2->sfilter)
	    {
	       rdd_err(ClipMemoryStore, EG_ARG, 0, __FILE__, __LINE__, __PROC__, "No expression on filter");
	       goto err;
	    }
      }
   return 0;
 err:
   return 1;
}

int
rm_union(ClipMachine * ClipMemoryStore, RDD_FILTER * f1, RDD_FILTER * f2, const char *__PROC__)
{
   int i;
   unsigned int bytes = ((f1->size + 1) >> 5) + 1;

   if (f1->rmap)
      {
	 for (i = 0; i < bytes; i++)
	    {
	       f1->rmap[i] |= f2->rmap[i];
	    }
      }
   if (f1->sfilter && f2->sfilter)
      {
	 char *tmp = strdup(f1->sfilter);

	 f1->sfilter = realloc(f1->sfilter, strlen(f1->sfilter) + strlen(f2->sfilter) + 11);
	 sprintf(f1->sfilter, "(%s) .OR. (%s)", tmp, f2->sfilter);
	 free(tmp);
      }
   else
      {
	 if (f1->sfilter)
	    free(f1->sfilter);
	 f1->sfilter = NULL;
      }
   f1->nfps++;
   f1->fps = (RDD_FPS *) realloc(f1->fps, f1->nfps * sizeof(RDD_FPS));
   _clip_clone(ClipMemoryStore, &f1->fps[f1->nfps - 1].bfilter, &f2->fps->bfilter);
   f1->fps[f1->nfps - 1].op = 1;

   if (f1->optimize == 2 && f2->optimize == 2)
      f1->optimize = 2;
   else if ((f1->optimize == 2 && f2->optimize == 1) || (f1->optimize == 1 && f2->optimize == 2))
      f1->optimize = 1;
   else if (f1->optimize == 1 && f2->optimize == 1)
      f1->optimize = 1;
   else
      f1->optimize = 0;
   return 0;
}

int
rm_intersect(ClipMachine * ClipMemoryStore, RDD_FILTER * f1, RDD_FILTER * f2, const char *__PROC__)
{
   int i;
   unsigned int bytes = ((f1->size + 1) >> 5) + 1;

   if (f1->rmap)
      {
	 for (i = 0; i < bytes; i++)
	    {
	       f1->rmap[i] &= f2->rmap[i];
	    }
      }
   if (f1->sfilter && f2->sfilter)
      {
	 char *tmp = strdup(f1->sfilter);

	 f1->sfilter = realloc(f1->sfilter, strlen(f1->sfilter) + strlen(f2->sfilter) + 12);
	 sprintf(f1->sfilter, "(%s) .AND. (%s)", tmp, f2->sfilter);
	 free(tmp);
      }
   else
      {
	 if (f1->sfilter)
	    free(f1->sfilter);
	 f1->sfilter = NULL;
      }
   f1->nfps++;
   f1->fps = (RDD_FPS *) realloc(f1->fps, f1->nfps * sizeof(RDD_FPS));
   _clip_clone(ClipMemoryStore, &f1->fps[f1->nfps - 1].bfilter, &f2->fps->bfilter);
   f1->fps[f1->nfps - 1].op = 2;

   f1->optimize += f1->optimize;
   if (f1->optimize == 4)
      f1->optimize = 2;
   else if (f1->optimize > 0)
      f1->optimize = 1;
   return 0;
}

int
rm_difference(ClipMachine * ClipMemoryStore, RDD_FILTER * f1, RDD_FILTER * f2, const char *__PROC__)
{
   int i;
   unsigned int bytes = ((f1->size + 1) >> 5) + 1;

   if (f1->rmap)
      {
	 for (i = 0; i < bytes; i++)
	    {
	       f1->rmap[i] ^= f2->rmap[i];
	    }
      }
   return 0;
}

int
rm_evalpartial(ClipMachine * ClipMemoryStore, RDD_FILTER * fp, ClipVar * block, unsigned int *ret, unsigned int *cnt, const char *__PROC__)
{
   ClipVar v;
   unsigned int oldrecno = fp->rd->recno;
   int oldbof = fp->rd->bof;
   int oldeof = fp->rd->eof;
   int recno;

   *ret = 0;
   if (cnt)
      *cnt = 0;

   if (!fp->rmap && cnt)
      {
	 if (fp->list)
	    *cnt = fp->listlen;
	 else if (rdd_lastrec(ClipMemoryStore, fp->rd, (int *) cnt, __PROC__))
	    goto err;
	 return 0;
      }

   memset(&v, 0, sizeof(ClipVar));
   fp->rd->bof = fp->rd->eof = 0;
   for (recno = 1; recno <= fp->size; recno++)
      {
	 if (_rm_getbit(fp->rmap, fp->size, recno))
	    {
	       if (fp->optimize == 2)
		  {
		     if (cnt)
			(*cnt)++;
		  }
	       else
		  {
		     int fok;

		     if (fp->rd->vtbl->rawgo(ClipMemoryStore, fp->rd, recno, 0, __PROC__))
			goto err;
		     if (rdd_childs(ClipMemoryStore, fp->rd, __PROC__))
			return 1;
		     if (block)
			{
			   ClipVar v, *vp;

			   if (rdd_calc(ClipMemoryStore, fp->rd->area, block, &v, 0))
			      goto err;
			   vp = _clip_vptr(&v);
			   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
			      {
				 _clip_destroy(ClipMemoryStore, &v);
				 rdd_err(ClipMemoryStore, EG_SYNTAX, 0, __FILE__, __LINE__, __PROC__, "Codeblock returns non-logical value");
				 goto err;
			      }
				   fok = vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;
			   _clip_destroy(ClipMemoryStore, &v);
			}
		     else
			{
			   if (rdd_calcfilter(ClipMemoryStore, fp->rd, &fok, __PROC__))
			      return 1;
			}
		     if (!fok)
			_rm_clrbit(fp->rmap, fp->size, fp->rd->recno);
		     else if (cnt)
			(*cnt)++;
		  }
	       (*ret)++;
	    }
      }
   if (fp->rd->vtbl->rawgo(ClipMemoryStore, fp->rd, oldrecno, 0, __PROC__))
      goto err;
   fp->rd->bof = oldbof;
   fp->rd->eof = oldeof;
   if (rdd_childs(ClipMemoryStore, fp->rd, __PROC__))
      return 1;
   return 0;
 err:
   return 1;
}

int
rm_inverse(ClipMachine * ClipMemoryStore, RDD_FILTER * fp, const char *__PROC__)
{
   int i;
   unsigned int bytes = ((fp->size + 1) >> 5) + 1;

   if (fp->rmap)
      {
	 for (i = 0; i < bytes; i++)
	    {
	       fp->rmap[i] = ~fp->rmap[i];
	    }
      }
   if (fp->sfilter)
      {
	 char *tmp = strdup(fp->sfilter);

	 fp->sfilter = realloc(fp->sfilter, strlen(fp->sfilter) + 4);
	 sprintf(fp->sfilter, "!(%s)", tmp);
	 free(tmp);
      }
   fp->fps->op = 4;
   return 0;
}

int
rm_copyfilter(ClipMachine * ClipMemoryStore, RDD_FILTER * fp, RDD_FILTER ** ret, const char *__PROC__)
{
   RDD_FILTER *r = calloc(sizeof(RDD_FILTER), 1);
   int i;

   r->custom = fp->custom;
   r->optimize = fp->optimize;
   r->nfps = fp->nfps;
   r->fps = calloc(fp->nfps, sizeof(RDD_FPS));
   for (i = 0; i < fp->nfps; i++)
      {
	 _clip_clone(ClipMemoryStore, &r->fps[i].bfilter, &fp->fps[i].bfilter);
	 r->fps[i].op = fp->fps[i].op;
      }
   if (fp->sfilter)
      r->sfilter = strdup(fp->sfilter);
   r->size = fp->size;
   if (fp->rmap)
      {
	 int i;
	 int bytes = ((r->size + 1) >> 5) + 1;

	 r->rmap = calloc(sizeof(unsigned int), bytes);
	 for (i = 0; i < bytes; i++)
	    r->rmap[i] = fp->rmap[i];
      }
   r->rd = fp->rd;
   r->handle = _clip_store_c_item(ClipMemoryStore, r, _C_ITEM_TYPE_RYO, destroy_ryo);
   *ret = r;
   return 0;
}

int
rm_isfiltrec(ClipMachine * ClipMemoryStore, RDD_FILTER * fp, unsigned int rec, int *r, const char *__PROC__)
{
   *r = 0;
   if (fp->rmap && _rm_getbit(fp->rmap, fp->size, rec))
      {
	 unsigned int oldrecno = fp->rd->recno;
	 fp->rd->recno = rec;
	 if (rdd_calcfilter(ClipMemoryStore, fp->rd, r, __PROC__))
	    return 1;
	 fp->rd->recno = oldrecno;
      }
   return 0;
}
