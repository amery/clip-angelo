/*
	Copyright (C) 2001  ITK
	Author  : Rushad Nabiullin <rust@itk.ru>
	License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI
*/
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include "ci_rdd.h"
#include "ci_error.ch"
#include "ci_btree.h"
#ifdef HAVE_MMAN_H
#include <sys/mman.h>
#endif

#define MAX_BTREE_DEEP 64
#define MAX_TAG_LEN 10

typedef struct _NTX_HEADER_
{
   unsigned char sig[2];
   unsigned char ver[2];
   unsigned char root[4];
   unsigned char fuu[4];
   unsigned char itemsize[2];
   unsigned char keysize[2];
   unsigned char dec[2];
   unsigned char fullpage[2];
   unsigned char halfpage[2];
   char key[256];
   char unique;
   char reserved1;
   char descend;
   char reserver2;
   char forexpr[256];
   char tag[12];
   char custom;
   char reserved3;
   unsigned char reserved2[472];
} NTX_HEADER;

typedef struct _CTX_TAG_
{
   unsigned char tagname[MAX_TAG_LEN + 1];
   unsigned char res1;
   unsigned char header[4];
} CTX_TAG;

typedef struct _CTX_HEADER_
{
   unsigned char sig[2];
   unsigned char ntags;
   unsigned char res1[13];
   CTX_TAG tags[63];
} CTX_HEADER;

typedef struct _NTX_PAGE_
{
   unsigned char nkeys[2];
   unsigned char offs[1022];
} NTX_PAGE;

typedef struct _NTX_FREEPAGE_
{
   unsigned char next[4];
   unsigned char unused[1020];
} NTX_FREEPAGE;

typedef struct _NTX_BUCKET_
{
   unsigned char left[4];
   unsigned char recno[4];
   unsigned char key[1];
} NTX_BUCKET;

#define _ntx_header(ro,hdr) if((er = rdd_read(ClipMemoryStore,&ro->index->file,ro->header,sizeof(NTX_HEADER),(char*)hdr,__PROC__))) return er
#define _ntx_page(ro,pageoffs,page)	if((er = rdd_read(ClipMemoryStore,&ro->index->file,pageoffs,sizeof(NTX_PAGE),(char*)page,__PROC__))) return er
#define _ntx_buck(page,no) (NTX_BUCKET*)((char*)page+_rdd_ushort(&((page)->offs[(no)*2])))

static RDD_INDEX_VTBL *ntx_vtbl();
static RDD_INDEX_VTBL *ctx_vtbl();

static int ntx_open(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_INDEX * ri, const char *tag, const char *__PROC__);
static int ntx_close(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_INDEX * ri, const char *__PROC__);
static int ntx_create(ClipMachine * ClipMemoryStore, RDD_DATA * rd,
		      RDD_INDEX * ri, RDD_ORDER ** rop, const char *tag, const char *expr, ClipVar * block, int unique, int first, unsigned int header, const char *__PROC__);
static int ntx_reindex(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_INDEX * ri, const char *__PROC__);
static int ntx_zap(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_INDEX * ri, const char *__PROC__);
static int ntx_gotop(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__);
static int ntx_gobottom(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__);
static int ntx_next(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__);
static int ntx_prev(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__);
static int ntx_seek(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_ORDER * ro, ClipVar * v, int soft, int last, int *found, const char *__PROC__);

static int ntx_lastkey(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_ORDER * ro, int *lastkey, const char *__PROC__);
static int ntx_keyno(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_ORDER * ro, int *keyno, const char *__PROC__);

static int ntx_formatkey(ClipMachine * ClipMemoryStore, RDD_ORDER * ro, ClipVar * var, void *res, const char *__PROC__);

static int _ntx_calc_key(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__);

static int _ntx_put_node(ClipMachine * ClipMemoryStore, RDD_DATA * rd,
			 RDD_ORDER * ro, NTX_PAGE * page, unsigned int pageoffs, char *key, unsigned int recno, unsigned int child, int buckno, int isright, int hostcs, const char *__PROC__);
static int _ntx_add_node(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_ORDER * ro, char *key, int recno, int child, const char *__PROC__);

static int ntx__rlock(ClipMachine * ClipMemoryStore, RDD_ORDER * ro, const char *__PROC__);
static int ntx__wlock(ClipMachine * ClipMemoryStore, RDD_ORDER * ro, const char *__PROC__);
static int ntx__ulock(ClipMachine * ClipMemoryStore, RDD_ORDER * ro, const char *__PROC__);

#define er_unlock           _clip_gettext("Unlock error")
#define er_readlock         _clip_gettext("Shared lock error")
#define er_writelock        _clip_gettext("Exclusive lock error")
#define er_badforexpr       _clip_gettext("Bad FOR expression")
#define er_corruption       _clip_gettext("Index corrupted")
#define er_deepexceeded     _clip_gettext("MAX_BTREE_DEEP exceeded")
#define er_baddata          _clip_gettext("Bad data")
#define er_longkey          _clip_gettext("Key length is too long")
#define er_badwhileexpr     _clip_gettext("Bad WHILE expression")
#define er_badevalexpr      _clip_gettext("Bad EVAL expression")

int
clip_INIT_NTX(ClipMachine * ClipMemoryStore)
{
   rdd_registerindexdriver(ClipMemoryStore, ntx_vtbl());
   return 0;
}

int
clip_INIT_CTX(ClipMachine * ClipMemoryStore)
{
   rdd_registerindexdriver(ClipMemoryStore, ctx_vtbl());
   return 0;
}

static int
ntx__rlock(ClipMachine * ClipMemoryStore, RDD_ORDER * ro, const char *__PROC__)
{
   if (ro->index->rd->shared)
      {
	 struct flock fl;

	 fl.l_type = F_RDLCK;
	 fl.l_whence = SEEK_SET;
	 fl.l_start = 1000000000;
	 fl.l_len = 1;

	 if (fcntl(ro->index->file.fd, F_SETLKW, &fl))
	    return rdd_err(ClipMemoryStore, EG_LOCK, errno, __FILE__, __LINE__, __PROC__, er_readlock);
	 ro->wlocked = 0;
      }
   return 0;
}

static int
ntx__wlock(ClipMachine * ClipMemoryStore, RDD_ORDER * ro, const char *__PROC__)
{
   if (ro->index->rd->shared)
      {
	 struct flock fl;

	 fl.l_type = F_WRLCK;
	 fl.l_whence = SEEK_SET;
	 fl.l_start = 1000000000;
	 fl.l_len = 1;

	 if (fcntl(ro->index->file.fd, F_SETLKW, &fl))
	    return rdd_err(ClipMemoryStore, EG_LOCK, errno, __FILE__, __LINE__, __PROC__, er_writelock);
	 ro->wlocked = 1;
      }
   return 0;
}

static int
ntx__ulock(ClipMachine * ClipMemoryStore, RDD_ORDER * ro, const char *__PROC__)
{
   int er = 0;

   if (ro->index->rd->shared)
      {
	 struct flock fl;

	 if (ro->wlocked)
	    {
	       char cnt[2];

	       er = rdd_read(ClipMemoryStore, &ro->index->file, 2, 2, cnt, __PROC__);
	       ro->cnt = _rdd_ushort((unsigned char *) cnt) + 1;
	       _rdd_put_ushort((unsigned char *) cnt, ro->cnt);
	       er = rdd_write(ClipMemoryStore, &ro->index->file, 2, 2, cnt, __PROC__);
	    }
	 fl.l_type = F_UNLCK;
	 fl.l_whence = SEEK_SET;
	 fl.l_start = 1000000000;
	 fl.l_len = 1;

	 if (fcntl(ro->index->file.fd, F_SETLKW, &fl))
	    return rdd_err(ClipMemoryStore, EG_LOCK, errno, __FILE__, __LINE__, __PROC__, er_unlock);
	 ro->wlocked = 0;
      }
   return er;
}

static int
_ntx_checkscope(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_ORDER * ro, int *res, const char *__PROC__)
{
   int er;

   *res = 1;
   if (ro->scopetop || ro->scopebottom)
      if ((er = _ntx_calc_key(ClipMemoryStore, rd, ro, __PROC__)))
	 return er;
   if (ro->scopetop)
      if (memcmp(ro->scopetop, ro->key, ro->stoplen) > 0)
	 *res = 0;
   if (ro->scopebottom)
      if (memcmp(ro->scopebottom, ro->key, ro->sbotlen) < 0)
	 *res = 0;
   return 0;
}

static int
_ntx_compare(RDD_DATA * rd, char unique, char descend, char *s1, int l1, unsigned int recno1, char *s2, int l2, unsigned int recno2)
{
   int l = (l1 < l2 ? l1 : l2);
   int r = loc_cmp(rd->loc, (unsigned char *) s1, (unsigned char *) s2, l);

   if (!r)
      {
	 if (unique || recno1 == 0 || recno2 == 0)
	    return 0;
	 if (recno1 < recno2)
	    r = -1;
	 else if (recno1 > recno2)
	    r = 1;
	 else
	    r = 0;
      }
   if (descend)
      r = -r;
   return r;
}

static int
_ntx_search_page(RDD_DATA * rd, RDD_ORDER * ro, NTX_PAGE * page, char *key, int len, unsigned int recno, int *no, int exact)
{
   int l;
   int h;
   int i = 0;
   int c = 0;
   int count;
   int f = 0;
   NTX_BUCKET *buck;

   *no = 0;
   count = _rdd_ushort(page->nkeys);
   l = 0;
   h = count - 1;
   while (l <= h)
      {
	 i = (l + h) / 2;
	 buck = _ntx_buck(page, i);
	 c = _ntx_compare(rd, ro->unique, ro->descend, (char *) buck->key, ro->keysize, exact ? _rdd_uint(buck->recno) : 0, key, len, exact ? recno : 0);
	 if (c && f)
	    return 1;
	 if (c < 0)
	    l = i + 1;
	 else if (c > 0)
	    h = i - 1;
	 else
	    {
	       if (exact)
		  {
		     while (!c && i--)
			{
			   buck = _ntx_buck(page, i);
			   c = _ntx_compare(rd, ro->unique, ro->descend, (char *) buck->key, ro->keysize, _rdd_uint(buck->recno), key, len, recno);
			}
		     *no = i + 1;
		  }
	       else
		  {
		     *no = i;
		     f = 1;
		     h = i - 1;
		  }
	       return 1;
	    }
      }
   if (c < 0)
      i++;
   *no = i;

   return 0;
}

static int
_ntx_loadpage(ClipMachine * ClipMemoryStore, RDD_ORDER * ro, unsigned int offs, const char *__PROC__)
{
   if (offs == ro->curoffs)
      return 0;
   ro->curoffs = offs;
   return rdd_read(ClipMemoryStore, &ro->index->file, offs, sizeof(NTX_PAGE), (char *) ro->curpage, __PROC__);
}

static int
__ntx_last(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_ORDER * ro, unsigned int root, const char *__PROC__)
{
   NTX_PAGE *page = ro->curpage;
   NTX_BUCKET *buck;
   int er;

   if ((er = _ntx_loadpage(ClipMemoryStore, ro, root, __PROC__)))
      return er;
   buck = _ntx_buck(page, 0);
   ro->level++;

   if (ro->level >= MAX_BTREE_DEEP)
      return rdd_err(ClipMemoryStore, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_corruption);

   ro->stack[ro->level].page = root;
   ro->stack[ro->level].pos = _rdd_ushort(page->nkeys);
   if (!_rdd_uint(buck->left))
      {
	 if (ro->stack[ro->level].pos > 0)
	    {
	       ro->stack[ro->level].pos--;
	       buck = _ntx_buck(page, ro->stack[ro->level].pos);
	       return rd->vtbl->rawgo(ClipMemoryStore, rd, _rdd_uint(buck->recno), 1, __PROC__);
	    }
      }
   else
      {
	 buck = _ntx_buck(page, ro->stack[ro->level].pos);
	 if ((er = __ntx_last(ClipMemoryStore, rd, ro, _rdd_uint(buck->left), __PROC__)))
	    return er;
      }
   return 0;
}

static int
__ntx_first(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_ORDER * ro, unsigned int root, const char *__PROC__)
{
   NTX_PAGE *page = ro->curpage;
   NTX_BUCKET *buck;
   int er;

   do
      {
	 ro->level++;
	 if (ro->level >= MAX_BTREE_DEEP)
	    return rdd_err(ClipMemoryStore, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_corruption);
	 ro->stack[ro->level].page = root;
	 ro->stack[ro->level].pos = 0;
	 if ((er = _ntx_loadpage(ClipMemoryStore, ro, root, __PROC__)))
	    return er;
	 buck = _ntx_buck(page, 0);
	 root = _rdd_uint(buck->left);
      }
   while (root);
   return rd->vtbl->rawgo(ClipMemoryStore, rd, _rdd_uint(buck->recno), 1, __PROC__);
}

static int
__ntx_prev(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_ORDER * ro, int *out, const char *__PROC__)
{
   NTX_PAGE *page = ro->curpage;
   NTX_BUCKET *buck;
   int level, er;

   *out = 0;
   level = ro->level;

   if ((er = _ntx_loadpage(ClipMemoryStore, ro, ro->stack[level].page, __PROC__)))
      return er;
   buck = _ntx_buck(page, ro->stack[level].pos);

   if (!_rdd_uint(buck->left))
      {
	 if (ro->stack[level].pos > 0)
	    {
	       ro->stack[level].pos--;
	       buck = _ntx_buck(page, ro->stack[level].pos);
	       return rd->vtbl->rawgo(ClipMemoryStore, rd, _rdd_uint(buck->recno), 1, __PROC__);
	    }
	 else
	    {
	       while (level >= 0 && ro->stack[level].pos == 0)
		  level--;
	       if (level < 0)
		  {
		     *out = 1;
		  }
	       else
		  {
		     ro->level = level;
		     ro->stack[level].pos--;
		     if ((er = _ntx_loadpage(ClipMemoryStore, ro, ro->stack[level].page, __PROC__)))
			return er;
		     buck = _ntx_buck(page, ro->stack[level].pos);
		     return rd->vtbl->rawgo(ClipMemoryStore, rd, _rdd_uint(buck->recno), 1, __PROC__);
		  }
	    }
      }
   else
      {
	 if ((er = __ntx_last(ClipMemoryStore, rd, ro, _rdd_uint(buck->left), __PROC__)))
	    return er;
      }
   return 0;
}

static int
__ntx_next(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_ORDER * ro, int *out, const char *__PROC__)
{
   NTX_PAGE *page = ro->curpage;
   NTX_BUCKET *buck;
   int level, er;

   *out = 0;
   level = ro->level;

   if ((er = _ntx_loadpage(ClipMemoryStore, ro, ro->stack[level].page, __PROC__)))
      return er;
   buck = _ntx_buck(page, ro->stack[level].pos);

   if (!_rdd_uint(buck->left))
      {
	 if (ro->stack[level].pos < _rdd_ushort(page->nkeys) - 1)
	    {
	       ro->stack[level].pos++;
	       buck = _ntx_buck(page, ro->stack[level].pos);
	       return rd->vtbl->rawgo(ClipMemoryStore, rd, _rdd_uint(buck->recno), 1, __PROC__);
	    }
	 else
	    {
	       while (--level >= 0)
		  {
		     if ((er = _ntx_loadpage(ClipMemoryStore, ro, ro->stack[level].page, __PROC__)))
			return er;
		     if (ro->stack[level].pos < _rdd_ushort(page->nkeys))
			break;
		  }
	       if (level < 0)
		  {
		     *out = 1;
		  }
	       else
		  {
		     ro->level = level;
		     buck = _ntx_buck(page, ro->stack[level].pos);
		     return rd->vtbl->rawgo(ClipMemoryStore, rd, _rdd_uint(buck->recno), 1, __PROC__);
		  }
	    }
      }
   else
      {
	 ro->stack[level].pos++;
	 buck = _ntx_buck(page, ro->stack[level].pos);
	 if ((er = __ntx_first(ClipMemoryStore, rd, ro, _rdd_uint(buck->left), __PROC__)))
	    return er;
      }
   return 0;
}

static int
__ntx_tuneseek(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_ORDER * ro, char *key, int len, const char *__PROC__)
{
   NTX_PAGE page;
   NTX_BUCKET *buck;
   int c;
   int out, er;

   _ntx_page(ro, ro->stack[ro->level].page, &page);
   buck = _ntx_buck(&page, ro->stack[ro->level].pos);
   c = _ntx_compare(rd, 0, ro->descend, (char *) buck->key, ro->keysize, 0, key, len, 0);
   while (!c)
      {
	 if ((er = __ntx_prev(ClipMemoryStore, rd, ro, &out, __PROC__)))
	    return er;
	 if (out)
	    break;
	 _ntx_page(ro, ro->stack[ro->level].page, &page);
	 buck = _ntx_buck(&page, ro->stack[ro->level].pos);
	 c = _ntx_compare(rd, 0, ro->descend, (char *) buck->key, ro->keysize, 0, key, len, 0);
      }
   if (c)
      {
	 if ((er = __ntx_next(ClipMemoryStore, rd, ro, &out, __PROC__)))
	    return er;
      }
   return 0;
}

static int
__ntx_tuneprev(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_ORDER * ro, char *key, int len, unsigned int recno, int *ok, const char *__PROC__)
{
   NTX_PAGE page;
   NTX_BUCKET *buck;
   int c;
   int out, er;

   _ntx_page(ro, ro->stack[ro->level].page, &page);
   buck = _ntx_buck(&page, ro->stack[ro->level].pos);
   c = _ntx_compare(rd, 0, ro->descend, (char *) buck->key, ro->keysize, 0, key, len, 0);
   while (!c && _rdd_uint(buck->recno) != recno)
      {
	 if ((er = __ntx_prev(ClipMemoryStore, rd, ro, &out, __PROC__)))
	    return er;
	 if (out)
	    break;
	 _ntx_page(ro, ro->stack[ro->level].page, &page);
	 buck = _ntx_buck(&page, ro->stack[ro->level].pos);
	 c = _ntx_compare(rd, 0, ro->descend, (char *) buck->key, ro->keysize, 0, key, len, 0);
      }
   *ok = _rdd_uint(buck->recno) == recno;
   return 0;
}

static int
__ntx_tunenext(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_ORDER * ro, char *key, int len, unsigned int recno, int *ok, const char *__PROC__)
{
   NTX_PAGE page;
   NTX_BUCKET *buck;
   int c;
   int out, er;

   _ntx_page(ro, ro->stack[ro->level].page, &page);
   buck = _ntx_buck(&page, ro->stack[ro->level].pos);
   c = _ntx_compare(rd, 0, ro->descend, (char *) buck->key, ro->keysize, 0, key, len, 0);
   while (!c && _rdd_uint(buck->recno) != recno)
      {
	 if ((er = __ntx_next(ClipMemoryStore, rd, ro, &out, __PROC__)))
	    return er;
	 if (out)
	    break;
	 _ntx_page(ro, ro->stack[ro->level].page, &page);
	 buck = _ntx_buck(&page, ro->stack[ro->level].pos);
	 c = _ntx_compare(rd, 0, ro->descend, (char *) buck->key, ro->keysize, 0, key, len, 0);
      }
   *ok = _rdd_uint(buck->recno) == recno;
   return 0;
}

static int
_ntx_search_tree(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_ORDER * ro, char *key, int len, unsigned int recno, int *res, int exact, const char *__PROC__)
{
   NTX_PAGE page;
   NTX_BUCKET *buck;
   int no;
   long offs = ro->stack[ro->level].page;
   int ok, er;

   _ntx_page(ro, offs, &page);
   *res = _ntx_search_page(rd, ro, &page, key, len, recno, &no, exact);
   buck = _ntx_buck(&page, no);
   ro->stack[ro->level].pos = no;
   if (*res)
      {
	 if (recno && !ro->unique)
	    {
	       RDD_STACK stack[MAX_BTREE_DEEP];
	       int level = ro->level;

	       memcpy(stack, ro->stack, sizeof(stack));
	       if ((er = __ntx_tunenext(ClipMemoryStore, rd, ro, key, len, recno, &ok, __PROC__)))
		  return er;
	       if (!ok)
		  {
		     ro->level = level;
		     memcpy(ro->stack, stack, sizeof(stack));
		     if ((er = __ntx_tuneprev(ClipMemoryStore, rd, ro, key, len, recno, &ok, __PROC__)))
			return er;
		  }
	       if (!ok)
		  {
		     ro->level = level;
		     memcpy(ro->stack, stack, sizeof(stack));
		     *res = 1;
		     return 0;
		  }
	    }
	 else if (!exact)
	    {
	       if ((er = __ntx_tuneseek(ClipMemoryStore, rd, ro, key, len, __PROC__)))
		  return er;
	    }
	 *res = 0;
	 return 0;
      }

   offs = _rdd_uint(buck->left);
   if (!offs)
      {
	 if (no == _rdd_ushort(page.nkeys))
	    {
	       ro->eof = 1;
	    }
	 *res = 1;
	 return 0;
      }

   (ro->level)++;
   if (ro->level >= MAX_BTREE_DEEP)
      {
	 *res = 1;
	 return 0;
      }
   ro->stack[ro->level].page = offs;

   return _ntx_search_tree(ClipMemoryStore, rd, ro, key, len, recno, res, exact, __PROC__);
}

static int
_ntx_calc_key(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__)
{
   int er;

   if (ro->simpexpr)
      {
	 ClipVar v;

	 memset(&v, 0, sizeof(ClipVar));
	 if ((er = rdd_takevalue(ClipMemoryStore, rd, ro->simpfno, &v, __PROC__)))
	    return er;
	 if (v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
	    {
	       memcpy(ro->key, v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
	    }
	 else if (v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
	    {
			 if (v.ClipNumVar_n_of_ClipVar.double_of_ClipNumVar >= 0)
		  {
			  _clip_dtostr(ro->key, ro->keysize, ro->dec, v.ClipNumVar_n_of_ClipVar.double_of_ClipNumVar, 1);
		  }
	       else
		  {
		     char *s;

			  _clip_dtostr(ro->key, ro->keysize, ro->dec, -v.ClipNumVar_n_of_ClipVar.double_of_ClipNumVar, 1);
		     for (s = ro->key; *s; ++s)
			switch (*s)
			   {
			   case '0':
			      *s = ',';
			      break;
			   case '1':
			      *s = '+';
			      break;
			   case '2':
			      *s = '*';
			      break;
			   case '3':
			      *s = ')';
			      break;
			   case '4':
			      *s = '(';
			      break;
			   case '5':
			      *s = '\'';
			      break;
			   case '6':
			      *s = '&';
			      break;
			   case '7':
			      *s = '%';
			      break;
			   case '8':
			      *s = '$';
			      break;
			   case '9':
			      *s = '#';
			      break;
			   }
		  }
	    }
	 else if (v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == DATE_type_of_ClipVarType)
	    {
	       int dd, mm, yy, ww;
	       if (v.ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar == 0)
		  {
		     strcpy(ro->key, "00000000");
		  }
	       else
		  {
		     _clip_cdate(v.ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar, &dd, &mm, &yy, &ww);
		     sprintf(ro->key, "%04d%02d%02d", yy, mm, dd);
		  }
	    }
	 else if (v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == LOGICAL_type_of_ClipVarType)
	    {
	       ro->key[0] = v.ClipLogVar_l_of_ClipVar.value_of_ClipLogVar ? 'T' : 'F';
	    }
	 _clip_destroy(ClipMemoryStore, &v);
      }
   else
      {
	 ClipVar vv, *vp;
	 char ckey[1024];

	 if ((er = rdd_calc(ClipMemoryStore, rd->area, &ro->block, &vv, 0)))
	    return er;
	 vp = _clip_vptr(&vv);
	 if ((er = ntx_formatkey(ClipMemoryStore, ro, vp, ckey, __PROC__)))
	    return er;
	 memcpy(ro->key, ckey, ro->keysize);
	 _clip_destroy(ClipMemoryStore, &vv);
      }
   return 0;
}

static int
ntx_formatkey(ClipMachine * ClipMemoryStore, RDD_ORDER * ro, ClipVar * var, void *res, const char *__PROC__)
{
   int len, dd, mm, yy, ww;

   switch (var->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
      {
      case CHARACTER_type_of_ClipVarType:
	 len = min(var->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, ro->keysize);
	 memcpy(res, var->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, len);
	 memset(res + len, 32, ro->keysize - len);
	 break;
      case NUMERIC_type_of_ClipVarType:
			if (var->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar >= 0)
	    {
			 _clip_dtostr(res, ro->keysize, ro->dec, var->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar, 1);
	    }
	 else
	    {
	       char *s;

			 _clip_dtostr(res, ro->keysize, ro->dec, -var->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar, 1);
	       for (s = res; *s; ++s)
		  switch (*s)
		     {
		     case '0':
			*s = ',';
			break;
		     case '1':
			*s = '+';
			break;
		     case '2':
			*s = '*';
			break;
		     case '3':
			*s = ')';
			break;
		     case '4':
			*s = '(';
			break;
		     case '5':
			*s = '\'';
			break;
		     case '6':
			*s = '&';
			break;
		     case '7':
			*s = '%';
			break;
		     case '8':
			*s = '$';
			break;
		     case '9':
			*s = '#';
			break;
		     }
	    }
	 break;
      case DATE_type_of_ClipVarType:
	 if (var->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar == 0)
	    {
	       strcpy(res, "00000000");
	    }
	 else
	    {
	       _clip_cdate(var->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar, &dd, &mm, &yy, &ww);
	       sprintf(res, "%04d%02d%02d", yy, mm, dd);
	    }
	 break;
      case LOGICAL_type_of_ClipVarType:
	 ((char *) res)[0] = var->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar ? 'T' : 'F';
	 break;
      default:
	 ((char *) res)[0] = 0;
	 break;
      }
   return 0;
}

static int
_ntx_init_page(ClipMachine * ClipMemoryStore, RDD_ORDER * ro, NTX_PAGE * page, unsigned int rchield, const char *__PROC__)
{
   NTX_HEADER hdr;
   NTX_BUCKET *buck;
   unsigned short int offs;
   int i, er;

   _ntx_header(ro, &hdr);
   offs = 2 * (ro->fullpage + 2);
   memset(page, 0, sizeof(NTX_PAGE));
   _rdd_put_ushort(page->nkeys, 0);

   buck = (NTX_BUCKET *) (((char *) page) + offs);
   _rdd_put_uint(buck->left, rchield);
   _rdd_put_uint(buck->recno, 0);

   for (i = 0; i <= ro->fullpage; ++i, offs += _rdd_ushort(hdr.itemsize))
      _rdd_put_ushort(page->offs + i * 2, offs);

   return 0;
}

static int
_ntx_free_page(ClipMachine * ClipMemoryStore, RDD_ORDER * ro, unsigned int *page, const char *__PROC__)
{
   NTX_HEADER hdr;
   unsigned int fsize;
   int er;

   _ntx_header(ro, &hdr);
   *page = _rdd_uint(hdr.fuu);
   if (!*page)
      {
	 int delta;
	 if (ro->index->file.md == (caddr_t) - 1)
	    {
	       struct stat st;
	       fstat(ro->index->file.fd, &st);
	       fsize = st.st_size;
	    }
	 else
	    fsize = ro->index->file.mapsize;

	 delta = ((fsize - sizeof(NTX_HEADER)) / sizeof(NTX_PAGE)) / 5;
	 if (delta < 1)
	    delta = 1;
	 *page = fsize;
	 if ((er = rdd_write(ClipMemoryStore, &ro->index->file, *page + delta * sizeof(NTX_PAGE), 0, "", __PROC__)))
	    return er;
	 if (delta > 1)
	    {
	       int i;
	       NTX_FREEPAGE fp;

	       memset(&fp, 0, sizeof(NTX_FREEPAGE));

	       _rdd_put_uint(hdr.fuu, *page + sizeof(NTX_PAGE));
	       for (i = 1; i < delta; i++)
		  {
		     if (i < delta - 1)
			_rdd_put_backuint(fp.next, *page + (i + 1) * sizeof(NTX_PAGE));
		     else
			_rdd_put_backuint(fp.next, 0);
		     if ((er = rdd_write(ClipMemoryStore, &ro->index->file, *page + i * sizeof(NTX_PAGE), sizeof(NTX_FREEPAGE), (char *) &fp, __PROC__)))
			return er;
		  }
	    }
      }
   else
      {
	 char next[4];
	 if ((er = rdd_read(ClipMemoryStore, &ro->index->file, *page, 4, next, __PROC__)))
	    return er;
	 _rdd_put_uint(hdr.fuu, _rdd_backuint((unsigned char *) next));
      }
   if ((er = rdd_write(ClipMemoryStore, &ro->index->file, ro->header, sizeof(NTX_HEADER), (char *) &hdr, __PROC__)))
      return er;
   return 0;
}

static int
_ntx_split_node(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_ORDER * ro, char *kk, int rn, unsigned int cc, const char *__PROC__)
{
   NTX_PAGE rpp, lpp;
   NTX_PAGE *pp;
   int count, mediane, i, j;
   unsigned int lnode, rchild;
   unsigned int rnode = ro->stack[ro->level].page;
   int buckno = ro->stack[ro->level].pos;
   char *pbuf = (char *) malloc(sizeof(NTX_PAGE));
   NTX_BUCKET *bp;
   char key[1024];
   unsigned int recno, child;
   char mkey[1024];
   unsigned int mrecno, mchild;
   int er;

   if ((er = _ntx_free_page(ClipMemoryStore, ro, &lnode, __PROC__)))
      return er;

   _ntx_page(ro, rnode, &rpp);
   memcpy(pbuf, &rpp, sizeof(NTX_PAGE));
   pp = (NTX_PAGE *) pbuf;
   count = _rdd_ushort(pp->nkeys);
   mediane = count / 2;

   _ntx_page(ro, lnode, &lpp);

   if (buckno == mediane)
      {
	 memcpy(key, kk, ro->keysize);
	 loc_write(rd->loc, (unsigned char *) key, ro->keysize);
	 key[ro->keysize] = 0;
	 recno = rn;
	 rchild = cc;
      }
   else
      {
	 bp = _ntx_buck(&rpp, mediane);
	 memcpy(key, bp->key, ro->keysize);
	 key[ro->keysize] = 0;
	 recno = _rdd_uint(bp->recno);
	 rchild = _rdd_uint(bp->left);
      }

   child = lnode;

   bp = _ntx_buck(pp, count);
   if ((er = _ntx_init_page(ClipMemoryStore, ro, &rpp, _rdd_uint(bp->left), __PROC__)))
      return er;
   if ((er = _ntx_init_page(ClipMemoryStore, ro, &lpp, rchild, __PROC__)))
      return er;

   for (i = 0, j = 0; i < mediane; i++, j++)
      {
	 if (i == buckno)
	    if ((er = _ntx_put_node(ClipMemoryStore, rd, ro, &lpp, lnode, kk, rn, cc, j++, 0, 1, __PROC__)))
	       return er;
	 bp = _ntx_buck(pp, i);
	 mrecno = _rdd_uint(bp->recno);
	 mchild = _rdd_uint(bp->left);
	 memcpy(mkey, bp->key, ro->keysize);
	 mkey[ro->keysize] = 0;
	 if ((er = _ntx_put_node(ClipMemoryStore, rd, ro, &lpp, lnode, mkey, mrecno, mchild, j, 0, 0, __PROC__)))
	    return er;
      }
   j = 0;
   if (buckno == mediane)
      {
	 bp = _ntx_buck(pp, mediane);
	 mrecno = _rdd_uint(bp->recno);
	 mchild = _rdd_uint(bp->left);
	 memcpy(mkey, bp->key, ro->keysize);
	 mkey[ro->keysize] = 0;
	 if ((er = _ntx_put_node(ClipMemoryStore, rd, ro, &rpp, rnode, mkey, mrecno, mchild, j++, 0, 0, __PROC__)))
	    return er;
      }
   for (i = mediane + 1; i < count; i++, j++)
      {
	 if (i == buckno)
	    if ((er = _ntx_put_node(ClipMemoryStore, rd, ro, &rpp, rnode, kk, rn, cc, j++, 0, 1, __PROC__)))
	       return er;
	 bp = _ntx_buck(pp, i);
	 mrecno = _rdd_uint(bp->recno);
	 mchild = _rdd_uint(bp->left);
	 memcpy(mkey, bp->key, ro->keysize);
	 mkey[ro->keysize] = 0;
	 if ((er = _ntx_put_node(ClipMemoryStore, rd, ro, &rpp, rnode, mkey, mrecno, mchild, j, 0, 0, __PROC__)))
	    return er;
      }
   if (buckno == count)
      if ((er = _ntx_put_node(ClipMemoryStore, rd, ro, &rpp, rnode, kk, rn, cc, j, 0, 1, __PROC__)))
	 return er;

   if ((er = rdd_write(ClipMemoryStore, &ro->index->file, lnode, sizeof(NTX_PAGE), (char *) &lpp, __PROC__)))
      return er;
   if ((er = rdd_write(ClipMemoryStore, &ro->index->file, rnode, sizeof(NTX_PAGE), (char *) &rpp, __PROC__)))
      return er;

   if (ro->level < 1)
      {
	 unsigned int root;
	 NTX_HEADER hdr;
	 NTX_PAGE page;

	 if ((er = _ntx_free_page(ClipMemoryStore, ro, &root, __PROC__)))
	    return er;

	 if ((er = _ntx_init_page(ClipMemoryStore, ro, &page, rnode, __PROC__)))
	    return er;
	 if ((er = rdd_write(ClipMemoryStore, &ro->index->file, root, sizeof(NTX_PAGE), (char *) &page, __PROC__)))
	    return er;
	 _ntx_header(ro, &hdr);
	 _rdd_put_uint(hdr.root, root);
	 if ((er = rdd_write(ClipMemoryStore, &ro->index->file, ro->header, sizeof(NTX_HEADER), (char *) &hdr, __PROC__)))
	    return er;

	 ro->stack[0].page = root;
	 ro->stack[0].pos = 0;

	 loc_read(rd->loc, (unsigned char *) key, ro->keysize);
	 if ((er = _ntx_add_node(ClipMemoryStore, rd, ro, key, recno, child, __PROC__)))
	    return er;
      }
   else
      {
	 --(ro->level);
	 loc_read(rd->loc, (unsigned char *) key, ro->keysize);
	 if ((er = _ntx_add_node(ClipMemoryStore, rd, ro, key, recno, child, __PROC__)))
	    return er;
      }

   free(pbuf);
   return 0;
}

static int
_ntx_put_node(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_ORDER * ro,
	      NTX_PAGE * page, unsigned int pageoffs, char *key, unsigned int recno, unsigned int child, int buckno, int isright, int hostcs, const char *__PROC__)
{
   int count = _rdd_ushort(page->nkeys);
   NTX_BUCKET *buck;
   char *basep = (char *) page;
   int offs = 2 + buckno * 2;
   int boffs;

   if (isright)
      {
	 boffs = _rdd_ushort(page->offs + count * 2);
	 buck = (NTX_BUCKET *) (basep + boffs);
	 _rdd_put_uint(buck->left, child);
	 _rdd_put_uint(buck->recno, recno);
	 memcpy(buck->key, key, ro->keysize);
	 if (hostcs)
	    loc_write(rd->loc, buck->key, ro->keysize);
      }
   else
      {
	 boffs = _rdd_ushort(page->offs + (count + 1) * 2);
	 memmove(basep + offs + 2, basep + offs, (count + 1 - buckno) * 2);
	 _rdd_put_ushort((unsigned char *) (basep + offs), boffs);

	 buck = (NTX_BUCKET *) (basep + boffs);
	 _rdd_put_uint(buck->left, child);
	 _rdd_put_uint(buck->recno, recno);
	 memcpy(buck->key, key, ro->keysize);
	 if (hostcs)
	    loc_write(rd->loc, buck->key, ro->keysize);

	 _rdd_put_ushort(page->nkeys, count + 1);
      }
   return rdd_write(ClipMemoryStore, &ro->index->file, pageoffs, sizeof(NTX_PAGE), (char *) page, __PROC__);
}

static int
_ntx_add_node(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_ORDER * ro, char *key, int recno, int child, const char *__PROC__)
{
   NTX_PAGE page;
   int no, count, er;
   unsigned int offs = ro->stack[ro->level].page;

   _ntx_page(ro, offs, &page);
   count = _rdd_ushort(page.nkeys);
   no = ro->stack[ro->level].pos;

   if (count < ro->fullpage)
      {
	 return _ntx_put_node(ClipMemoryStore, rd, ro, &page, offs, key, recno, child, no, 0, 1, __PROC__);
      }
   else
      {
	 return _ntx_split_node(ClipMemoryStore, rd, ro, key, recno, child, __PROC__);
      }
}

static int
ntx_addkey(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_ORDER * ro, ClipVar * v, const char *__PROC__)
{
   NTX_HEADER hdr;
   NTX_BUCKET *bp;
   NTX_PAGE page;
   int r, er;

   if (ro->bforexpr.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
      {
	 ClipVar vv, *vp;

	 if ((er = rdd_calc(ClipMemoryStore, rd->area, &ro->bforexpr, &vv, 0)))
	    return er;
	 vp = _clip_vptr(&vv);
	 if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
	    return rdd_err(ClipMemoryStore, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_badforexpr);
	 if (!vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
	    {
	       _clip_destroy(ClipMemoryStore, &vv);
	       return 0;
	    }
	 _clip_destroy(ClipMemoryStore, &vv);
      }

   _ntx_header(ro, &hdr);
   ro->stack[0].page = _rdd_uint(hdr.root);
   ro->level = 0;

   if (v)
      {
	 if ((er = ntx_formatkey(ClipMemoryStore, ro, v, ro->key, __PROC__)))
	    return er;
      }
   else
      {
	 if ((er = _ntx_calc_key(ClipMemoryStore, rd, ro, __PROC__)))
	    return er;
      }

   if ((er = _ntx_search_tree(ClipMemoryStore, rd, ro, ro->key, ro->keysize, rd->recno, &r, 1, __PROC__)))
      return er;

   _ntx_page(ro, ro->stack[ro->level].page, &page);
   bp = _ntx_buck(&page, _rdd_ushort(page.nkeys));

   ro->valid_stack = 0;

   if (!r && ro->unique)
      return 0;
   else if ((er = _ntx_add_node(ClipMemoryStore, rd, ro, ro->key, rd->recno, _rdd_uint(bp->left), __PROC__)))
      return er;
   rd->eof = 0;
   ro->curoffs = 0;
   return 0;
}

static int
_ntx_remove_node(ClipMachine * ClipMemoryStore, RDD_ORDER * ro, unsigned int page, NTX_PAGE * pp, int no, const char *__PROC__)
{
   int count = _rdd_ushort(pp->nkeys);
   char *basep = (char *) pp;
   int boffs = _rdd_ushort(pp->offs + no * 2);
   int offs = 2 + no * 2;
   NTX_BUCKET *bp;

   bp = _ntx_buck(pp, no);
   memset(bp->key, 0, ro->keysize);
   _rdd_put_uint(bp->left, 0);
   _rdd_put_uint(bp->recno, 0);

   memmove(basep + offs, basep + offs + 2, (count - no) * 2);
   _rdd_put_ushort((unsigned char *) (basep + 2 + count * 2), boffs);
   _rdd_put_ushort(pp->nkeys, count - 1);
   return rdd_write(ClipMemoryStore, &ro->index->file, page, sizeof(NTX_PAGE), basep, __PROC__);
}

static int
_ntx_put_free_page(ClipMachine * ClipMemoryStore, RDD_ORDER * ro, unsigned int page, const char *__PROC__)
{
   NTX_FREEPAGE fp;
   NTX_HEADER hdr;
   unsigned int freepage;
   int er;

   _ntx_header(ro, &hdr);
   freepage = _rdd_uint(hdr.fuu);

   _ntx_page(ro, page, (NTX_PAGE *) & fp);
   _rdd_put_backuint(fp.next, freepage);
   _rdd_put_uint(hdr.fuu, page);

   if ((er = rdd_write(ClipMemoryStore, &ro->index->file, page, sizeof(NTX_FREEPAGE), (char *) &fp, __PROC__)))
      return er;
   if ((er = rdd_write(ClipMemoryStore, &ro->index->file, ro->header, sizeof(NTX_HEADER), (char *) &hdr, __PROC__)))
      return er;

   return 0;
}

static int
_ntx_balance(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__)
{
   NTX_PAGE pp, upp, rpp, tpp;
   int count, uno, ucount, rno, rcount, tno;
   unsigned int page, upage, rpage, right, tpage;
   NTX_BUCKET *bp;
   NTX_BUCKET *ubp;
   NTX_BUCKET *rbp;
   NTX_BUCKET *tbp;
   int er;

   page = ro->stack[ro->level].page;
   _ntx_page(ro, page, &pp);
   count = _rdd_ushort(pp.nkeys);

   if (ro->level < 1)
      return 0;
   if (count >= ro->halfpage)
      return 0;

   upage = ro->stack[ro->level - 1].page;
   uno = ro->stack[ro->level - 1].pos;
   _ntx_page(ro, upage, &upp);
   ucount = _rdd_ushort(upp.nkeys);
   ubp = _ntx_buck(&upp, ucount);
   right = _rdd_uint(ubp->left);

   ubp = _ntx_buck(&upp, uno);

   if (ucount < 1 && !right)
      {
	 int i;

	 _rdd_put_uint(ubp->left, 0);
	 for (i = 0; i < count; ++i)
	    {
	       char key[1024];
	       unsigned int recno;

	       bp = _ntx_buck(&pp, i);
	       memcpy(key, bp->key, ro->keysize);
	       key[ro->keysize] = 0;
	       recno = _rdd_uint(bp->recno);

	       if ((er = _ntx_put_node(ClipMemoryStore, rd, ro, &upp, upage, key, recno, 0, i, 0, 0, __PROC__)))
		  return er;
	    }
	 if ((er = _ntx_put_free_page(ClipMemoryStore, ro, page, __PROC__)))
	    return er;
	 return 0;
      }

   if (uno > 0)
      rno = uno - 1;
   else if (uno < ucount - 1)
      rno = uno + 1;
   else if ((uno < ucount) && right)
      rno = ucount;
   else
      return rdd_err(ClipMemoryStore, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_corruption);

   rbp = _ntx_buck(&upp, rno);
   rpage = _rdd_uint(rbp->left);

   _ntx_page(ro, rpage, &rpp);
   rcount = _rdd_ushort(rpp.nkeys);

   if (uno > rno)
      {
	 tpage = rpage;
	 rpage = page;
	 page = tpage;

	 memcpy((void *) &tpp, (void *) &rpp, sizeof(NTX_PAGE));
	 memcpy((void *) &rpp, (void *) &pp, sizeof(NTX_PAGE));
	 memcpy((void *) &pp, (void *) &tpp, sizeof(NTX_PAGE));

	 tno = rno;
	 rno = uno;
	 uno = tno;

	 tno = rcount;
	 rcount = count;
	 count = tno;

	 tbp = rbp;
	 rbp = ubp;
	 ubp = tbp;
      }

   bp = _ntx_buck(&pp, count);
   if (count + rcount < ro->fullpage)
      {
	/* join pages */
	 int i, j, sum;
	 char key[1024];
	 unsigned int recno;
	 unsigned int child;

	 sum = count + rcount + 1;

	 _rdd_put_uint(rbp->left, page);
	 if ((er = rdd_write(ClipMemoryStore, &ro->index->file, upage, sizeof(NTX_PAGE), (char *) &upp, __PROC__)))
	    return er;

	 memcpy(key, ubp->key, ro->keysize);
	 key[ro->keysize] = 0;
	 recno = _rdd_uint(ubp->recno);
	 child = _rdd_uint(bp->left);

	 if ((er = _ntx_put_node(ClipMemoryStore, rd, ro, &pp, page, key, recno, child, count, 0, 0, __PROC__)))
	    return er;

	 for (i = count + 1, j = 0; i <= sum; i++, j++)
	    {
	       bp = _ntx_buck(&rpp, j);
	       memcpy(key, bp->key, ro->keysize);
	       key[ro->keysize] = 0;
	       recno = _rdd_uint(bp->recno);
	       child = _rdd_uint(bp->left);

	       if ((er = _ntx_put_node(ClipMemoryStore, rd, ro, &pp, page, key, recno, child, i, i == sum, 0, __PROC__)))
		  return er;
	    }

	 if ((er = _ntx_remove_node(ClipMemoryStore, ro, upage, &upp, uno, __PROC__)))
	    return er;
	 if ((er = _ntx_put_free_page(ClipMemoryStore, ro, rpage, __PROC__)))
	    return er;
	 if ((ro->level == 1) && (ucount == 1))
	    {
	      /* New root */
	       NTX_HEADER hdr;
	       _ntx_header(ro, &hdr);
	       _rdd_put_uint(hdr.root, page);
	       if ((er = rdd_write(ClipMemoryStore, &ro->index->file, ro->header, sizeof(NTX_HEADER), (char *) &hdr, __PROC__)))
		  return er;
	    }
      }
   else
      {
	/* balance pages */
	 int i, j, sum, mediane;
	 char key[1024];
	 unsigned int recno;
	 unsigned int child;

	 sum = count + rcount;
	 mediane = sum / 2;

	 if (count > mediane)
	    {
	      /* pp -> rpp */
	       bp = _ntx_buck(&pp, count);
	       memcpy(key, ubp->key, ro->keysize);
	       key[ro->keysize] = 0;
	       recno = _rdd_uint(ubp->recno);
	       child = _rdd_uint(bp->left);
	       if ((er = _ntx_put_node(ClipMemoryStore, rd, ro, &rpp, rpage, key, recno, child, 0, 0, 0, __PROC__)))
		  return er;

	       for (i = count - 1; i > mediane; i--)
		  {
		     bp = _ntx_buck(&pp, i);
		     memcpy(key, bp->key, ro->keysize);
		     key[ro->keysize] = 0;
		     recno = _rdd_uint(bp->recno);
		     child = _rdd_uint(bp->left);
		     if ((er = _ntx_put_node(ClipMemoryStore, rd, ro, &rpp, rpage, key, recno, child, 0, 0, 0, __PROC__)))
			return er;
		  }

	       bp = _ntx_buck(&pp, mediane);
	       memcpy(ubp->key, bp->key, ro->keysize);
	       _rdd_put_uint(ubp->recno, _rdd_uint(bp->recno));
	       if ((er = rdd_write(ClipMemoryStore, &ro->index->file, upage, sizeof(NTX_PAGE), (char *) &upp, __PROC__)))
		  return er;

	       for (i = count; i > mediane; i--)
		  if ((er = _ntx_remove_node(ClipMemoryStore, ro, page, &pp, i, __PROC__)))
		     return er;
	       _rdd_put_uint(bp->recno, 0);
	       memset(bp->key, 0, ro->keysize);
	       if ((er = rdd_write(ClipMemoryStore, &ro->index->file, page, sizeof(NTX_PAGE), (char *) &pp, __PROC__)))
		  return er;
	    }
	 else if (count < mediane)
	    {
	      /* rpp -> pp */
	       bp = _ntx_buck(&pp, count);
	       memcpy(key, ubp->key, ro->keysize);
	       key[ro->keysize] = 0;
	       recno = _rdd_uint(ubp->recno);
	       child = _rdd_uint(bp->left);
	       if ((er = _ntx_put_node(ClipMemoryStore, rd, ro, &pp, page, key, recno, child, count, 0, 0, __PROC__)))
		  return er;

	       for (i = count + 1, j = 0; i < mediane; i++, j++)
		  {
		     bp = _ntx_buck(&rpp, j);
		     memcpy(key, bp->key, ro->keysize);
		     key[ro->keysize] = 0;
		     recno = _rdd_uint(bp->recno);
		     child = _rdd_uint(bp->left);

		     if ((er = _ntx_put_node(ClipMemoryStore, rd, ro, &pp, page, key, recno, child, i, 0, 0, __PROC__)))
			return er;
		  }

	       bp = _ntx_buck(&rpp, mediane - (count + 1));
	       memcpy(ubp->key, bp->key, ro->keysize);
	       _rdd_put_uint(ubp->recno, _rdd_uint(bp->recno));
	       if ((er = rdd_write(ClipMemoryStore, &ro->index->file, upage, sizeof(NTX_PAGE), (char *) &upp, __PROC__)))
		  return er;

	       bp = _ntx_buck(&rpp, j);
	       right = _rdd_uint(bp->left);
	       bp = _ntx_buck(&pp, i);
	       _rdd_put_uint(bp->left, right);
	       if ((er = rdd_write(ClipMemoryStore, &ro->index->file, page, sizeof(NTX_PAGE), (char *) &pp, __PROC__)))
		  return er;

	       for (i = count + 1; i <= mediane; i++)
		  if ((er = _ntx_remove_node(ClipMemoryStore, ro, rpage, &rpp, 0, __PROC__)))
		     return er;
	    }
      }

   --(ro->level);

   return _ntx_balance(ClipMemoryStore, rd, ro, __PROC__);
}

static int
_ntx_find_last(ClipMachine * ClipMemoryStore, RDD_ORDER * ro, const char *__PROC__)
{
   NTX_PAGE pp;
   NTX_BUCKET *bp;
   unsigned int right;
   int count, er;
   unsigned int page = ro->stack[ro->level].page;

   _ntx_page(ro, page, &pp);
   count = _rdd_ushort(pp.nkeys);
   bp = _ntx_buck(&pp, count);
   right = _rdd_uint(bp->left);

   if (right)
      {
	 ro->stack[ro->level].pos = count;
	 (ro->level)++;
	 if (ro->level >= MAX_BTREE_DEEP)
	    return rdd_err(ClipMemoryStore, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_deepexceeded);
	 ro->stack[ro->level].page = right;
	 return _ntx_find_last(ClipMemoryStore, ro, __PROC__);
      }
   ro->stack[ro->level].pos = count - 1;
   return 0;
}

static int
_ntx_del_node(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__)
{
   NTX_PAGE pp;
   int no, count, er;
   unsigned int page = ro->stack[ro->level].page;
   unsigned int child;
   NTX_BUCKET *bp;

   _ntx_page(ro, page, &pp);
   count = _rdd_ushort(pp.nkeys);
   no = ro->stack[ro->level].pos;
   bp = _ntx_buck(&pp, no);
   child = _rdd_uint(bp->left);

   if (child)
      {
	 unsigned int cpage;
	 int cno;
	 NTX_BUCKET *cbp;
	 NTX_PAGE cpp;
	 unsigned int record;

	 (ro->level)++;
	 if (ro->level >= MAX_BTREE_DEEP)
	    return rdd_err(ClipMemoryStore, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_deepexceeded);
	 ro->stack[ro->level].page = child;

	 if ((er = _ntx_find_last(ClipMemoryStore, ro, __PROC__)))
	    return er;

	 cpage = ro->stack[ro->level].page;
	 cno = ro->stack[ro->level].pos;
	 _ntx_page(ro, cpage, &cpp);
	 cbp = _ntx_buck(&cpp, cno);
	 record = _rdd_uint(cbp->recno);
	 memcpy(bp->key, cbp->key, ro->keysize);
	 _rdd_put_uint(bp->recno, record);
	 if ((er = rdd_write(ClipMemoryStore, &ro->index->file, page, sizeof(NTX_PAGE), (char *) &pp, __PROC__)))
	    return er;

	 if ((er = _ntx_remove_node(ClipMemoryStore, ro, cpage, &cpp, cno, __PROC__)))
	    return er;
      }
   else
      {
	 if ((er = _ntx_remove_node(ClipMemoryStore, ro, page, &pp, no, __PROC__)))
	    return er;
      }
   if ((er = _ntx_balance(ClipMemoryStore, rd, ro, __PROC__)))
      return er;

   return 0;
}

static int
ntx_delkey(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__)
{
   NTX_HEADER hdr;
   int r, er;

   _ntx_header(ro, &hdr);
   ro->stack[0].page = _rdd_uint(hdr.root);
   ro->level = 0;

   if ((er = _ntx_calc_key(ClipMemoryStore, rd, ro, __PROC__)))
      return er;

   if ((er = _ntx_search_tree(ClipMemoryStore, rd, ro, ro->key, ro->keysize, rd->recno, &r, 1, __PROC__)))
      return er;

   ro->valid_stack = 0;
   if (r)
      return 0;

   if ((er = _ntx_del_node(ClipMemoryStore, rd, ro, __PROC__)))
      return er;
   ro->curoffs = 0;
   return 0;
}

static int
ntx_open(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_INDEX * ri, const char *tag, const char *__PROC__)
{
   RDD_ORDER *ro = (RDD_ORDER *) malloc(sizeof(RDD_ORDER));
   NTX_HEADER hdr;
   int er;

   memset(ro, 0, sizeof(RDD_ORDER));
   ro->vtbl = ri->vtbl;
   ro->index = ri;
   ro->canadd = 1;

   rd->ords_opened++;
   rd->orders = (RDD_ORDER **) realloc(rd->orders, sizeof(RDD_ORDER *) * rd->ords_opened);
   rd->orders[rd->ords_opened - 1] = ro;

   ro->orderno = rd->ords_opened - 1;

   if ((er = rdd_read(ClipMemoryStore, &ri->file, 0, sizeof(NTX_HEADER), (char *) &hdr, __PROC__)))
      return er;

   ro->expr = (char *) malloc(strlen(hdr.key) + 1);
   strcpy(ro->expr, hdr.key);
   loc_read(rd->loc, (unsigned char *) ro->expr, strlen(ro->expr));
   if ((ro->simpfno = _rdd_fieldno(rd, _clip_casehashword(ro->expr, strlen(ro->expr)))) != -1)
      ro->simpexpr = 1;

   {
      int r = _clip_parni(ClipMemoryStore, 1);
      char expr[1024];

      memset(expr, 0, sizeof(expr));
      rdd_expandmacro(rd->area, r, ro->expr, expr);

      if ((er = _clip_eval_macro(ClipMemoryStore, expr, strlen(expr), &ro->block)))
	 return er;
   }
   ro->unique = hdr.unique;
   ro->descend = hdr.descend;
   ro->custom = hdr.custom;
   ro->keysize = _rdd_ushort(hdr.keysize);
   ro->dec = _rdd_ushort(hdr.dec);

   ro->key = (char *) malloc(ro->keysize + 1);
   ro->fullpage = _rdd_ushort(hdr.fullpage);
   ro->halfpage = _rdd_ushort(hdr.halfpage);
   if (hdr.tag[0])
      {
	 ro->name = malloc(strlen(hdr.tag) + 1);
	 strcpy(ro->name, hdr.tag);
      }
   else
      {
	 ro->name = strdup(ri->name);
      }
   ro->curpage = calloc(1, sizeof(NTX_PAGE));
   ro->curoffs = 0;

   if (hdr.forexpr[0])
      {
	 int r = _clip_parni(ClipMemoryStore, 1);
	 char expr[1024];

	 ro->cforexpr = malloc(strlen(hdr.forexpr) + 1);
	 strcpy(ro->cforexpr, hdr.forexpr);

	 memset(expr, 0, sizeof(expr));
	 rdd_expandmacro(rd->area, r, ro->cforexpr, expr);

	 if ((er = _clip_eval_macro(ClipMemoryStore, expr, strlen(expr), &ro->bforexpr)))
	    return er;
      }

   ri->orders = (RDD_ORDER **) malloc(sizeof(RDD_ORDER *));
   ri->orders[0] = ro;
   ri->norders = 1;

   return 0;
}

static int
ntx_close(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_INDEX * ri, const char *__PROC__)
{
   destroy_rdd_order(ClipMemoryStore, ri->orders[0]);
   destroy_rdd_index(ri);
   return 0;
}

static int
_ntx_comp(void *op, void *k1, void *k2, int *uniqfound)
{
   RDD_ORDER *ro = op;
   int r = 0;

   if (ro->type == 'C')
      {
	 unsigned char *ss1, *ss2;
	 unsigned char *e = k1 + sizeof(unsigned int) + ro->keysize;

	 for (ss1 = k1 + sizeof(unsigned int), ss2 = k2 + sizeof(unsigned int); ss1 < e && !r; ss1++, ss2++)

	    r = _clip_cmptbl[*ss1] - _clip_cmptbl[*ss2];
      }
   else
      {
	 r = memcmp(k1 + sizeof(unsigned int), k2 + sizeof(unsigned int), ro->keysize);
      }
   if (uniqfound)
      *uniqfound = r;
   if (!r)
      {
	 if (*(unsigned int *) k1 < *(unsigned int *) k2)
	    r = -1;
	 else if (*(unsigned int *) k1 > *(unsigned int *) k2)
	    r = 1;
      }
   return r;
}

static int
_ntx_savebtree(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_ORDER * ro, BTREE * bt, const char *__PROC__)
{
   NTX_HEADER hdr;
   NTX_PAGE page;
   int eof;
   char *key;
   unsigned int poffs;
   unsigned int oldrecno = rd->recno;
   int f = 1, er;

   _ntx_header(ro, &hdr);
   poffs = _rdd_uint(hdr.root);
   _ntx_page(ro, poffs, &page);
   eof = bt_first(bt);
   while (!eof)
      {
	 if (_rdd_ushort(page.nkeys) == ro->fullpage)
	    {
	       NTX_BUCKET *buck;

	       if ((er = rdd_write(ClipMemoryStore, &ro->index->file, poffs, sizeof(NTX_PAGE), &page, __PROC__)))
		  return er;
	       if ((er = rd->vtbl->rawgo(ClipMemoryStore, rd, *((unsigned int *) bt_key(bt)), 0, __PROC__)))
		  return er;
	       rd->eof = 0;
	       if ((er = ntx_addkey(ClipMemoryStore, rd, ro, NULL, __PROC__)))
		  return er;
	       if (bt_next(bt))
		  {
		     f = 0;
		     break;
		  }
	       if ((er = rdd_read(ClipMemoryStore, &ro->index->file, ro->header + 4, sizeof(unsigned int), &poffs, __PROC__)))
		  return er;
	       _ntx_page(ro, poffs, &page);
	       buck = _ntx_buck(&page, _rdd_ushort(page.nkeys));
	       while (_rdd_uint(buck->left))
		  {
		     poffs = _rdd_uint(buck->left);
		     _ntx_page(ro, _rdd_uint(buck->left), &page);
		     buck = _ntx_buck(&page, _rdd_ushort(page.nkeys));
		  }
	       _ntx_page(ro, poffs, &page);
	    }
	 key = (char *) (&page) + _rdd_ushort(&page.offs[_rdd_ushort(page.nkeys) * sizeof(unsigned short)]) + 4;
	 memcpy(key, (char *) bt_key(bt), ro->keysize + 4);
	 if (ro->type == 'C')
	    loc_write(ro->index->loc, (unsigned char *) (key + sizeof(unsigned int)), ro->keysize);
	 eof = bt_next(bt);
	 _rdd_put_ushort(page.nkeys, _rdd_ushort(page.nkeys) + 1);
      }
   if (f)
      {
	 if ((er = rdd_write(ClipMemoryStore, &ro->index->file, poffs, sizeof(NTX_PAGE), &page, __PROC__)))
	    return er;
      }
   return rd->vtbl->rawgo(ClipMemoryStore, rd, oldrecno, 0, __PROC__);
}

static int
ntx_create(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_INDEX * ri,
	   RDD_ORDER ** rop, const char *tag, const char *expr, ClipVar * block, int unique, int first, unsigned int header, const char *__PROC__)
{
   RDD_ORDER *ro = (RDD_ORDER *) malloc(sizeof(RDD_ORDER));
   NTX_HEADER hdr;
   NTX_PAGE page;
   ClipVar vv, *vp;
   int lastrec;
   int s, e, i, er;
   BTREE *bt;
   void *buf;

   memset(ro, 0, sizeof(RDD_ORDER));
   ro->vtbl = ri->vtbl;
   ro->index = ri;
   ro->canadd = 1;
   *rop = ro;

   ro->header = header;
   ro->name = (char *) calloc(1, strlen(tag ? tag : ri->name) + 1);
   strcpy(ro->name, tag ? tag : ri->name);

   ro->expr = (char *) malloc(strlen(expr) + 1);
   strcpy(ro->expr, expr);
   if ((ro->simpfno = _rdd_fieldno(rd, _clip_casehashword(ro->expr, strlen(ro->expr)))) != -1)
      ro->simpexpr = 1;

   if (!block || block->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType)
      {
	 int r = _clip_parni(ClipMemoryStore, 1);
	 char expr[1024];

	 memset(expr, 0, sizeof(expr));

	 rdd_expandmacro(rd->area, r, ro->expr, expr);

	 if ((er = _clip_eval_macro(ClipMemoryStore, expr, strlen(expr), &ro->block)))
	    return er;
      }
   else
      {
	 _clip_clone(ClipMemoryStore, &ro->block, block);
      }
   if ((er = rdd_calc(ClipMemoryStore, rd->area, &ro->block, &vv, 0)))
      return er;
   vp = _clip_vptr(&vv);

   switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
      {
      case CHARACTER_type_of_ClipVarType:
	 ro->keysize = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
	 ro->dec = 0;
	 ro->type = 'C';
	 break;
      case NUMERIC_type_of_ClipVarType:
	 ro->keysize = (vp->ClipType_t_of_ClipVar.len_of_ClipType ? vp->ClipType_t_of_ClipVar.len_of_ClipType : 10);
	 ro->dec = vp->ClipType_t_of_ClipVar.dec_of_ClipType;
	 ro->type = 'N';
	 break;
      case LOGICAL_type_of_ClipVarType:
	 ro->keysize = 1;
	 ro->dec = 0;
	 ro->type = 'L';
	 break;
      case DATE_type_of_ClipVarType:
	 ro->keysize = 8;
	 ro->dec = 0;
	 ro->type = 'D';
	 break;
      default:
	 return rdd_err(ClipMemoryStore, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_baddata);
      }
   _clip_destroy(ClipMemoryStore, &vv);

   ro->unique = unique;
   ro->descend = rd->os.lDescend;
   ro->custom = rd->os.lCustom;

   ro->key = (char *) malloc(ro->keysize + 1);
   ro->fullpage = ((((1024 - 2) / (ro->keysize + 2 * 4 + 2)) - 1) / 2) * 2;
   if (ro->fullpage < 4)
      return rdd_err(ClipMemoryStore, EG_CREATE, 0, __FILE__, __LINE__, __PROC__, er_longkey);
   ro->halfpage = ro->fullpage / 2;
   if (rd->os.bForCondition.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
      {
	 ro->cforexpr = malloc(strlen(rd->os.cForCondition) + 1);
	 strcpy(ro->cforexpr, rd->os.cForCondition);
	 _clip_clone(ClipMemoryStore, &ro->bforexpr, &rd->os.bForCondition);
      }
   ro->curpage = calloc(1, sizeof(NTX_PAGE));
   ro->curoffs = 0;

   ri->orders = (RDD_ORDER **) realloc(ri->orders, (ri->norders + 1) * sizeof(RDD_ORDER *));
   ri->orders[ri->norders] = ro;
   ri->norders++;

   memset(&hdr, 0, sizeof(NTX_HEADER));
   _rdd_put_ushort(hdr.sig, 6);
   _rdd_put_ushort(hdr.ver, 1);
   _rdd_put_uint(hdr.fuu, 0);
   _rdd_put_ushort(hdr.itemsize, ro->keysize + 8);
   _rdd_put_ushort(hdr.keysize, ro->keysize);
   _rdd_put_ushort(hdr.dec, ro->dec);
   _rdd_put_ushort(hdr.fullpage, ro->fullpage);
   _rdd_put_ushort(hdr.halfpage, ro->halfpage);
   strncpy(hdr.key, ro->expr, sizeof(hdr.key));
   loc_write(rd->loc, (unsigned char *) hdr.key, strlen(hdr.key));
   if (rd->os.cForCondition)
      strcpy(hdr.forexpr, rd->os.cForCondition);
   hdr.unique = ro->unique;
   hdr.descend = ro->descend;
   hdr.custom = ro->custom;
   if (tag)
      strcpy(hdr.tag, tag);

   if ((er = rdd_write(ClipMemoryStore, &ri->file, ro->header, sizeof(NTX_HEADER), (char *) &hdr, __PROC__)))
      return er;

   if (ri->file.md == (caddr_t) - 1)
      {
	 struct stat st;
	 fstat(ri->file.fd, &st);
	 _rdd_put_uint(hdr.root, st.st_size);
      }
   else
      {
	 _rdd_put_uint(hdr.root, ri->file.mapsize);
      }
   if ((er = rdd_write(ClipMemoryStore, &ri->file, ro->header, sizeof(NTX_HEADER), (char *) &hdr, __PROC__)))
      return er;

   if ((er = _ntx_init_page(ClipMemoryStore, ro, &page, 0, __PROC__)))
      return er;
   if ((er = rdd_write(ClipMemoryStore, &ri->file, _rdd_uint(hdr.root), sizeof(NTX_PAGE), (char *) &page, __PROC__)))
      return er;

   if (ro->custom)
      return 0;

   if ((er = rd->vtbl->lastrec(ClipMemoryStore, rd, &lastrec, __PROC__)))
      return er;

   s = rd->os.nStart;
   e = min(lastrec, rd->os.nStart + rd->os.nNext - 1);
   if (rd->os.nStart == 0 || !rd->os.lRest)
      s = 1;
   if (rd->os.nNext == 0)
      e = lastrec;
   if (rd->os.nRecord)
      s = e = min(lastrec, rd->os.nRecord);
   if (rd->os.lAll || s < 1 || e < 1)
      {
	 s = 1;
	 e = lastrec;
      }
   i = max(1, rd->os.nInterval);

   if (rd->os.bWhileCondition.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
      rd->os.lCurrent = 1;
   if (rd->os.lAll)
      {
	 if ((er = rdd_gotop(ClipMemoryStore, rd, __PROC__)))
	    return er;
	 rd->os.lCurrent = 1;
      }

   rd->tagindexing = ro->name;
   if (ro->descend)
      bt = NULL;
   else
      bt = bt_create(ro->unique, lastrec, ro->keysize + sizeof(unsigned int) + 1, _ntx_comp);
   buf = malloc(ro->keysize + sizeof(unsigned int) + 1);
   if (!rd->os.lCurrent)
      {
	 for (rd->recno = s; rd->recno <= e; rd->recno++)
	    {
	       rd->eof = 0;
	       if ((er = rd->vtbl->rawgo(ClipMemoryStore, rd, rd->recno, 0, __PROC__)))
		  return er;
	      /* check WHILE condition */
	      /* no need to check WHILE, because USECURRENT is ON with WHILE
	         clause. See above...
	         if(rd->os.bWhileCondition.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType){
	         if((er = rdd_calc(ClipMemoryStore,rd->area,&rd->os.bWhileCondition,&vv,0)))
	         return er;
	         vp = _clip_vptr(&vv);
	         if(vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType!=LOGICAL_t)
	         return rdd_err(ClipMemoryStore,EG_DATATYPE,0,__FILE__,__LINE__,__PROC__,
	         er_badwhileexpr);
	         if(!vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar){
	         _clip_destroy(ClipMemoryStore,&vv);
	         break;
	         }
	         _clip_destroy(ClipMemoryStore,&vv);
	         }
	       */
	      /* bEval every nInterval */
	       if (rd->keysincluded % i == 0 && rd->os.bEval.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
		  {
		     if ((er = rdd_calc(ClipMemoryStore, rd->area, &rd->os.bEval, &vv, 0)))
			return er;
		     vp = _clip_vptr(&vv);

		     if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
			return rdd_err(ClipMemoryStore, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_badevalexpr);
		     if (!vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
			{
			   _clip_destroy(ClipMemoryStore, &vv);
			   break;
			}
		     _clip_destroy(ClipMemoryStore, &vv);
		  }
	       if (ro->bforexpr.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
		  {
		     if ((er = rdd_calc(ClipMemoryStore, rd->area, &ro->bforexpr, &vv, 0)))
			return er;
		     vp = _clip_vptr(&vv);
		     if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
			return rdd_err(ClipMemoryStore, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_badforexpr);
		     if (!vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
			{
			   _clip_destroy(ClipMemoryStore, &vv);
			   continue;
			}
		     _clip_destroy(ClipMemoryStore, &vv);
		  }
	       if (bt)
		  {
		     if (ro->simpexpr)
			{
			   if ((er = rd->vtbl->getvalue(ClipMemoryStore, rd, ro->simpfno, &vv, __PROC__)))
			      return er;
			   vp = &vv;
			}
		     else
			{
			   if ((er = rdd_calc(ClipMemoryStore, rd->area, &ro->block, &vv, 0)))
			      return er;
			   vp = _clip_vptr(&vv);
			}
		     memcpy(buf, &rd->recno, sizeof(unsigned int));
		     if ((er = ntx_formatkey(ClipMemoryStore, ro, vp, buf + sizeof(unsigned int), __PROC__)))
			return er;
		     _clip_destroy(ClipMemoryStore, &vv);
		     bt_add(bt, ro, buf);
		  }
	       else
		  {
		     if ((er = ntx_addkey(ClipMemoryStore, rd, ro, NULL, __PROC__)))
			return er;
		  }
	       rd->keysincluded++;
	    }
	 rd->keysincluded = 0;
      }
   else
      {
	 while (!rd->eof)
	    {
	       rd->eof = 0;
	      /* check WHILE condition */
	       if (rd->os.bWhileCondition.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
		  {
		     if ((er = rdd_calc(ClipMemoryStore, rd->area, &rd->os.bWhileCondition, &vv, 0)))
			return er;
		     vp = _clip_vptr(&vv);

		     if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
			return rdd_err(ClipMemoryStore, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_badwhileexpr);
		     if (!vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
			{
			   _clip_destroy(ClipMemoryStore, &vv);
			   break;
			}
		     _clip_destroy(ClipMemoryStore, &vv);
		  }
	      /* bEval every nInterval */
	       if (rd->keysincluded % i == 0 && rd->os.bEval.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
		  {
		     if ((er = rdd_calc(ClipMemoryStore, rd->area, &rd->os.bEval, &vv, 0)))
			return er;
		     vp = _clip_vptr(&vv);

		     if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
			return rdd_err(ClipMemoryStore, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_badevalexpr);
		     if (!vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
			{
			   _clip_destroy(ClipMemoryStore, &vv);
			   break;
			}
		     _clip_destroy(ClipMemoryStore, &vv);
		  }
	       if (ro->bforexpr.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
		  {
		     if ((er = rdd_calc(ClipMemoryStore, rd->area, &ro->bforexpr, &vv, 0)))
			return er;
		     vp = _clip_vptr(&vv);
		     if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
			return rdd_err(ClipMemoryStore, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_badforexpr);
		     if (!vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
			{
			   _clip_destroy(ClipMemoryStore, &vv);
			   if ((er = rdd_skip(ClipMemoryStore, rd, 1, __PROC__)))
			      return er;
			   continue;
			}
		     _clip_destroy(ClipMemoryStore, &vv);
		  }
	       if (bt)
		  {
		     if (ro->simpexpr)
			{
			   if ((er = rd->vtbl->getvalue(ClipMemoryStore, rd, ro->simpfno, &vv, __PROC__)))
			      return er;
			   vp = &vv;
			}
		     else
			{
			   if ((er = rdd_calc(ClipMemoryStore, rd->area, &ro->block, &vv, 0)))
			      return er;
			   vp = _clip_vptr(&vv);
			}
		     memcpy(buf, &rd->recno, sizeof(unsigned int));
		     if ((er = ntx_formatkey(ClipMemoryStore, ro, vp, buf + sizeof(unsigned int), __PROC__)))
			return er;
		     _clip_destroy(ClipMemoryStore, &vv);
		     bt_add(bt, ro, buf);
		  }
	       else
		  {
		     if ((er = ntx_addkey(ClipMemoryStore, rd, ro, NULL, __PROC__)))
			return er;
		  }
	       rd->keysincluded++;
	       if ((er = rdd_skip(ClipMemoryStore, rd, 1, __PROC__)))
		  return er;
	    }
	 rd->keysincluded = 0;
      }
   if (bt)
      {
	 if ((er = _ntx_savebtree(ClipMemoryStore, rd, ro, bt, __PROC__)))
	    {
	       bt_destroy(bt);
	       return er;
	    }
	 bt_destroy(bt);
      }
   free(buf);
   rd->tagindexing = NULL;
   return 0;
}

static int
_ntx_reindex(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__)
{
   int lastrec, i, er;
   ClipVar vv, *vp;

   if (ro->custom)
      return 0;

   rd->tagindexing = ro->name;
   if ((er = rd->vtbl->lastrec(ClipMemoryStore, rd, &lastrec, __PROC__)))
      return er;
   i = max(1, rd->os.nInterval);
   for (rd->recno = 1; rd->recno <= lastrec; rd->recno++)
      {
	 rd->eof = 0;
	 if ((er = rd->vtbl->rawgo(ClipMemoryStore, rd, rd->recno, 0, __PROC__)))
	    return er;
	 if ((er = ntx_addkey(ClipMemoryStore, rd, ro, NULL, __PROC__)))
	    return er;
	/* bEval every nInterval */
	 if (rd->recno % i == 0 && rd->os.bEval.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
	    {
	       if ((er = rdd_calc(ClipMemoryStore, rd->area, &rd->os.bEval, &vv, 0)))
		  return er;
	       vp = _clip_vptr(&vv);

	       if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
		  return rdd_err(ClipMemoryStore, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_badevalexpr);
	       if (!vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
		  {
		     _clip_destroy(ClipMemoryStore, &vv);
		     break;
		  }
	       _clip_destroy(ClipMemoryStore, &vv);
	    }
      }
   rd->tagindexing = NULL;
   return 0;
}

static int
ntx_reindex(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_INDEX * ri, const char *__PROC__)
{
   RDD_ORDER *ro = ri->orders[0];
   int er;

   if ((er = ntx_zap(ClipMemoryStore, rd, ri, __PROC__)))
      return er;
   if ((er = _ntx_reindex(ClipMemoryStore, rd, ro, __PROC__)))
      return er;
   return 0;
}

static int
ntx_zap(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_INDEX * ri, const char *__PROC__)
{
   NTX_HEADER hdr;
   NTX_PAGE page;
   int er;

   if ((er = rdd_trunc(ClipMemoryStore, &ri->file, sizeof(NTX_HEADER) + sizeof(NTX_PAGE), __PROC__)))
      return er;
   _ntx_header(ri->orders[0], &hdr);
   _rdd_put_uint(hdr.fuu, 0);
   _rdd_put_uint(hdr.root, sizeof(NTX_HEADER));
   if ((er = rdd_write(ClipMemoryStore, &ri->file, 0, sizeof(NTX_HEADER), (char *) &hdr, __PROC__)))
      return er;

   if ((er = _ntx_init_page(ClipMemoryStore, ri->orders[0], &page, 0, __PROC__)))
      return er;
   if ((er = rdd_write(ClipMemoryStore, &ri->file, sizeof(NTX_HEADER), sizeof(NTX_PAGE), (char *) &page, __PROC__)))
      return er;
   rd->bof = rd->v_bof = rd->eof = 1;
   return rd->vtbl->rawgo(ClipMemoryStore, rd, 1, 0, __PROC__);
}

static int
_ntx_first(ClipMachine * ClipMemoryStore, RDD_ORDER * ro, unsigned int root, unsigned int *recno, int *out, const char *__PROC__)
{
   NTX_PAGE page;
   NTX_BUCKET *buck;
   int er;

   _ntx_page(ro, root, &page);
   buck = _ntx_buck(&page, 0);
   ro->stack[ro->level].page = root;
   ro->stack[ro->level].pos = 0;
   if (out)
      {
	 *out = 0;
	 if (_rdd_ushort(page.nkeys) == 0)
	    {
	       *out = 1;
	       return 0;
	    }
      }
   while (_rdd_uint(buck->left))
      {
	 ro->level++;
	 ro->stack[ro->level].page = _rdd_uint(buck->left);
	 ro->stack[ro->level].pos = 0;
	 _ntx_page(ro, _rdd_uint(buck->left), &page);
	 buck = _ntx_buck(&page, 0);
      }
   *recno = _rdd_uint(buck->recno);
   return 0;
}

static int
_ntx_last(ClipMachine * ClipMemoryStore, RDD_ORDER * ro, unsigned int root, unsigned int *recno, int *out, const char *__PROC__)
{
   NTX_PAGE page;
   NTX_BUCKET *buck;
   int er;

   _ntx_page(ro, root, &page);
   buck = _ntx_buck(&page, _rdd_ushort(page.nkeys));
   ro->stack[ro->level].page = root;
   ro->stack[ro->level].pos = _rdd_ushort(page.nkeys);
   if (out)
      {
	 *out = 0;
	 if (_rdd_ushort(page.nkeys) == 0)
	    {
	       *out = 1;
	       return 0;
	    }
      }
   while (_rdd_uint(buck->left))
      {
	 ro->level++;
	 ro->stack[ro->level].page = _rdd_uint(buck->left);
	 _ntx_page(ro, _rdd_uint(buck->left), &page);
	 ro->stack[ro->level].pos = _rdd_ushort(page.nkeys);
	 buck = _ntx_buck(&page, _rdd_ushort(page.nkeys));
      }
   ro->stack[ro->level].pos = _rdd_ushort(page.nkeys) - 1;
   buck = _ntx_buck(&page, _rdd_ushort(page.nkeys) - 1);
   *recno = _rdd_uint(buck->recno);
   return 0;
}

static int
ntx_gotop(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__)
{
   NTX_HEADER hdr;
   unsigned int recno, lastrec;
   int res, sok, out, er;

   _ntx_header(ro, &hdr);
/*	printf("go top\n? 'go top',recno(),bof()\n");*/
   ro->level = 0;
   if ((er = _ntx_first(ClipMemoryStore, ro, _rdd_uint(hdr.root), &recno, &out, __PROC__)))
      return er;
   if (out)
      {
	 if ((er = rd->vtbl->lastrec(ClipMemoryStore, rd, (int *) (&lastrec), __PROC__)))
	    return er;
	 rd->bof = rd->v_bof = rd->eof = 1;
	 if ((er = rd->vtbl->rawgo(ClipMemoryStore, rd, lastrec + 1, 0, __PROC__)))
	    return er;
	 return 0;
      }
   rd->bof = rd->v_bof = rd->eof = 0;
   if ((er = rd->vtbl->rawgo(ClipMemoryStore, rd, recno, 1, __PROC__)))
      return er;
   if ((er = rdd_checkfilter(ClipMemoryStore, rd, &res, __PROC__)))
      return er;
   if ((er = _ntx_checkscope(ClipMemoryStore, rd, ro, &sok, __PROC__)))
      return er;
   if (!res || !sok)
      if ((er = ntx_next(ClipMemoryStore, rd, ro, __PROC__)))
	 return er;
   return 0;
}

static int
ntx_gobottom(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__)
{
   NTX_HEADER hdr;
   unsigned int recno, lastrec;
   int res, sok, out, er;

   _ntx_header(ro, &hdr);
/*	printf("go bottom\n? 'go bottom',recno(),bof()\n");*/
   ro->level = 0;
   if ((er = _ntx_last(ClipMemoryStore, ro, _rdd_uint(hdr.root), &recno, &out, __PROC__)))
      return er;
   if (out)
      {
	 if ((er = rd->vtbl->lastrec(ClipMemoryStore, rd, (int *) (&lastrec), __PROC__)))
	    return er;
	 rd->bof = rd->v_bof = rd->eof = 1;
	 if ((er = rd->vtbl->rawgo(ClipMemoryStore, rd, lastrec + 1, 0, __PROC__)))
	    return er;
	 return 0;
      }
   rd->bof = rd->v_bof = rd->eof = 0;
   if ((er = rd->vtbl->rawgo(ClipMemoryStore, rd, recno, 1, __PROC__)))
      return er;
   if ((er = rdd_checkfilter(ClipMemoryStore, rd, &res, __PROC__)))
      return er;
   if ((er = _ntx_checkscope(ClipMemoryStore, rd, ro, &sok, __PROC__)))
      return er;
   if (!res || !sok)
      if ((er = ntx_prev(ClipMemoryStore, rd, ro, __PROC__)))
	 return er;
   return 0;
}

static int
ntx_next(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__)
{
   int res = 0, sok, er;
   unsigned int lastrec;

   rd->bof = rd->v_bof = 0;
   if (rd->eof)
      return 0;
   rd->bof = rd->v_bof = rd->eof = 0;
/*	printf("skip 1\n? 'skip 1',recno(),bof()\n");*/
   if (rd->shared || !ro->valid_stack)
      {
	 char cnt[2];
	 int r;

	 er = rdd_read(ClipMemoryStore, &ro->index->file, 2, 2, cnt, __PROC__);
	 if (!ro->valid_stack || ro->cnt != _rdd_ushort((unsigned char *) cnt))
	    {
	       NTX_HEADER hdr;
	       _ntx_header(ro, &hdr);
	       ro->stack[0].page = _rdd_uint(hdr.root);
	       ro->level = 0;
	       ro->curoffs = 0;

	       if ((er = _ntx_calc_key(ClipMemoryStore, rd, ro, __PROC__)))
		  return er;

	       if ((er = _ntx_search_tree(ClipMemoryStore, rd, ro, ro->key, ro->keysize, rd->recno, &r, 0, __PROC__)))
		  return er;
	       if (r)
		  {
		     if ((er = rd->vtbl->lastrec(ClipMemoryStore, rd, (int *) (&lastrec), __PROC__)))
			return er;
		     if ((er = rd->vtbl->rawgo(ClipMemoryStore, rd, lastrec + 1, 0, __PROC__)))
			return er;
		     rd->eof = 1;
		     ro->valid_stack = 0;
		     return 0;
		  }
	       ro->cnt = _rdd_ushort((unsigned char *) cnt);
	    }
      }

   while (1)
      {
	 int out;

	 if ((er = __ntx_next(ClipMemoryStore, rd, ro, &out, __PROC__)))
	    return er;
	 if (out)
	    {
	       if ((er = rd->vtbl->lastrec(ClipMemoryStore, rd, (int *) (&lastrec), __PROC__)))
		  return er;
	       if ((er = rd->vtbl->rawgo(ClipMemoryStore, rd, lastrec + 1, 0, __PROC__)))
		  return er;
	       rd->eof = 1;
	       ro->valid_stack = 0;
	       break;
	    }
	 if ((er = rdd_checkfilter(ClipMemoryStore, rd, &res, __PROC__)))
	    return er;
	 if ((er = _ntx_checkscope(ClipMemoryStore, rd, ro, &sok, __PROC__)))
	    return er;
	 if (res && sok)
	    {
	       ro->valid_stack = 1;
	       break;
	    }
      }
   return 0;
}

static int
ntx_prev(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__)
{
   int res = 0, sok, out, er;
   int oldrecno = rd->recno;
   int oldeof = rd->eof;
   int lastrec;

   if (rd->bof)
      return 0;
/*	printf("skip -1\n? 'skip -1',recno(),bof()\n");*/
   if (rd->eof)
      {
	 rd->eof = 0;
	 if ((er = ro->vtbl->gobottom(ClipMemoryStore, rd, ro, __PROC__)))
	    return er;
	 rd->eof = rd->bof;
	 if (rd->eof)
	    {
	       if ((er = rd->vtbl->lastrec(ClipMemoryStore, rd, &lastrec, __PROC__)))
		  return er;
	       if ((er = rd->vtbl->rawgo(ClipMemoryStore, rd, lastrec + 1, 0, __PROC__)))
		  return er;
	       ro->valid_stack = 0;
	    }
	 else
	    ro->valid_stack = 1;
	 return 0;
      }
   rd->bof = rd->v_bof = rd->eof = 0;

   if (rd->shared || !ro->valid_stack)
      {
	 char cnt[2];
	 int r;

	 er = rdd_read(ClipMemoryStore, &ro->index->file, 2, 2, cnt, __PROC__);
	 if (!ro->valid_stack || ro->cnt != _rdd_ushort((unsigned char *) cnt))
	    {
	       NTX_HEADER hdr;
	       _ntx_header(ro, &hdr);
	       ro->stack[0].page = _rdd_uint(hdr.root);
	       ro->level = 0;
	       ro->curoffs = 0;

	       if ((er = _ntx_calc_key(ClipMemoryStore, rd, ro, __PROC__)))
		  return er;

	       if ((er = _ntx_search_tree(ClipMemoryStore, rd, ro, ro->key, ro->keysize, rd->recno, &r, 0, __PROC__)))
		  return er;
	       if (r)
		  {
		     if ((er = ntx_gotop(ClipMemoryStore, rd, ro, __PROC__)))
			return er;
		     rd->bof = 1;
		     return 0;
		  }
	       ro->cnt = _rdd_ushort((unsigned char *) cnt);
	    }
      }

   if ((er = rdd_checkfilter(ClipMemoryStore, rd, &res, __PROC__)))
      return er;
   if (!res)
      {
	 if ((er = ntx_next(ClipMemoryStore, rd, ro, __PROC__)))
	    return er;
	 oldrecno = rd->recno;
	 oldeof = rd->eof;
      }

   while (1)
      {
	 rd->eof = 0;
	 if ((er = __ntx_prev(ClipMemoryStore, rd, ro, &out, __PROC__)))
	    return er;
	 if (out)
	    {
	       if ((er = rd->vtbl->rawgo(ClipMemoryStore, rd, oldrecno, 0, __PROC__)))
		  return er;
	       rd->bof = 1;
	       rd->eof = oldeof;
	       ro->valid_stack = 0;
	       break;
	    }
	 if ((er = rdd_checkfilter(ClipMemoryStore, rd, &res, __PROC__)))
	    return er;
	 if ((er = _ntx_checkscope(ClipMemoryStore, rd, ro, &sok, __PROC__)))
	    return er;
	 if (res && sok)
	    {
	       ro->valid_stack = 1;
	       break;
	    }
      }
   return 0;
}

static int
ntx_seek(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_ORDER * ro, ClipVar * v, int soft, int last, int *found, const char *__PROC__)
{
   NTX_HEADER hdr;
   NTX_BUCKET *buck;
   NTX_PAGE page;
   char expr[1024];
   int res, sok, lastrec, oldrecno, er;
   int len = ro->keysize;

   rd->bof = rd->v_bof = rd->eof = 0;
   if ((er = ntx_formatkey(ClipMemoryStore, ro, v, expr, __PROC__)))
      return er;
   if (v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
      len = v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;

   _ntx_header(ro, &hdr);
   ro->stack[0].page = _rdd_uint(hdr.root);
   ro->level = 0;

   if ((er = _ntx_search_tree(ClipMemoryStore, rd, ro, expr, len, 0, &res, 0, __PROC__)))
      return er;

   _ntx_page(ro, ro->stack[ro->level].page, &page);
   buck = _ntx_buck(&page, ro->stack[ro->level].pos);
   while (!_rdd_uint(buck->recno) && (ro->level > 0))
      {
	 ro->level--;
	 _ntx_page(ro, ro->stack[ro->level].page, &page);
	 buck = _ntx_buck(&page, ro->stack[ro->level].pos);
      }
   if ((er = rd->vtbl->rawgo(ClipMemoryStore, rd, _rdd_uint(buck->recno), 1, __PROC__)))
      return er;
   *found = !res;

   if (*found)
      {
	 if (!ro->unique)
	    {
	       if (last)
		  {
		     while (1)
			{
			   RDD_STACK stack[MAX_BTREE_DEEP];
			   int level = ro->level;
			   int i;

			   for (i = 0; i <= level; i++)
			      stack[i] = ro->stack[i];
			   oldrecno = rd->recno;
			   if ((er = ntx_next(ClipMemoryStore, rd, ro, __PROC__)))
			      return er;
			   if (rd->eof)
			      {
				 if (rd->bof)
				    {
				       *found = 0;
				       break;
				    }
				 rd->eof = 0;
				 if ((er = rd->vtbl->rawgo(ClipMemoryStore, rd, oldrecno, 0, __PROC__)))
				    return er;
				 for (i = 0; i <= level; i++)
				    ro->stack[i] = stack[i];
				 ro->level = level;
				 if ((er = rdd_checkfilter(ClipMemoryStore, rd, &res, __PROC__)))
				    return er;
				 if ((er = _ntx_checkscope(ClipMemoryStore, rd, ro, &sok, __PROC__)))
				    return er;
				 if (!res || !sok)
				    {
				       if ((er = ntx_prev(ClipMemoryStore, rd, ro, __PROC__)))
					  return er;
				       if ((er = _ntx_calc_key(ClipMemoryStore, rd, ro, __PROC__)))
					  return er;
				       loc_write(rd->loc, (unsigned char *) ro->key, ro->keysize);
				       *found = !_ntx_compare(rd, 0, ro->descend, ro->key, ro->keysize, 0, expr, len, 0);
				    }
				 break;
			      }
			   if ((er = _ntx_calc_key(ClipMemoryStore, rd, ro, __PROC__)))
			      return er;
			   loc_write(rd->loc, (unsigned char *) ro->key, ro->keysize);
			   if (_ntx_compare(rd, 0, ro->descend, ro->key, ro->keysize, 0, expr, len, 0))
			      {
				 if ((er = ntx_prev(ClipMemoryStore, rd, ro, __PROC__)))
				    return er;
				 if ((er = _ntx_calc_key(ClipMemoryStore, rd, ro, __PROC__)))
				    return er;
				 loc_write(rd->loc, (unsigned char *) ro->key, ro->keysize);
				 *found = !_ntx_compare(rd, 0, ro->descend, ro->key, ro->keysize, 0, expr, len, 0);
				 break;
			      }
			}
		  }
	       else
		  {
		     while (1)
			{
			   RDD_STACK stack[MAX_BTREE_DEEP];
			   int level = ro->level;
			   int i;

			   for (i = 0; i <= level; i++)
			      stack[i] = ro->stack[i];
			   oldrecno = rd->recno;
			   if ((er = ntx_prev(ClipMemoryStore, rd, ro, __PROC__)))
			      return er;
			   if (rd->bof)
			      {
				 if (rd->eof)
				    {
				       *found = 0;
				       break;
				    }
				 rd->bof = rd->v_bof = 0;
				 if ((er = rd->vtbl->rawgo(ClipMemoryStore, rd, oldrecno, 0, __PROC__)))
				    return er;
				 for (i = 0; i <= level; i++)
				    ro->stack[i] = stack[i];
				 ro->level = level;
				 if ((er = rdd_checkfilter(ClipMemoryStore, rd, &res, __PROC__)))
				    return er;
				 if ((er = _ntx_checkscope(ClipMemoryStore, rd, ro, &sok, __PROC__)))
				    return er;
				 if (!res || !sok)
				    {
				       if ((er = ntx_next(ClipMemoryStore, rd, ro, __PROC__)))
					  return er;
				       if ((er = _ntx_calc_key(ClipMemoryStore, rd, ro, __PROC__)))
					  return er;
				       loc_write(rd->loc, (unsigned char *) ro->key, ro->keysize);
				       *found = !_ntx_compare(rd, 0, ro->descend, ro->key, ro->keysize, 0, expr, len, 0);
				    }
				 break;
			      }
			   if ((er = _ntx_calc_key(ClipMemoryStore, rd, ro, __PROC__)))
			      return er;
			   loc_write(rd->loc, (unsigned char *) ro->key, ro->keysize);
			   if (_ntx_compare(rd, 0, ro->descend, ro->key, ro->keysize, 0, expr, len, 0))
			      {
				 if ((er = ntx_next(ClipMemoryStore, rd, ro, __PROC__)))
				    return er;
				 if ((er = _ntx_calc_key(ClipMemoryStore, rd, ro, __PROC__)))
				    return er;
				 loc_write(rd->loc, (unsigned char *) ro->key, ro->keysize);
				 *found = !_ntx_compare(rd, 0, ro->descend, ro->key, ro->keysize, 0, expr, len, 0);
				 break;
			      }
			}
		  }
	    }
	 else
	    {
	       if ((er = rdd_checkfilter(ClipMemoryStore, rd, &res, __PROC__)))
		  return er;
	       if ((er = _ntx_checkscope(ClipMemoryStore, rd, ro, &sok, __PROC__)))
		  return er;
	       if (!res || !sok)
		  {
		     if ((er = ntx_next(ClipMemoryStore, rd, ro, __PROC__)))
			return er;
		     *found = 0;
		  }
	    }
      }

   if (*found)
      {
	 ro->valid_stack = 1;
	 if (rd->area != -1)
	    ((DBWorkArea *) (ClipMemoryStore->areas->items_of_ClipVect[rd->area]))->found = 1;
	 return 0;
      }
   else if (rd->area != -1)
      {
	 ((DBWorkArea *) (ClipMemoryStore->areas->items_of_ClipVect[rd->area]))->found = 0;
      }

   if (!soft || (rd->recno == 0))
      {
	 if ((er = rd->vtbl->lastrec(ClipMemoryStore, rd, &lastrec, __PROC__)))
	    return er;
	 rd->eof = 1;
	 if ((er = rd->vtbl->rawgo(ClipMemoryStore, rd, lastrec + 1, 0, __PROC__)))
	    return er;
      }
   else
      {
	 if ((er = rdd_checkfilter(ClipMemoryStore, rd, &res, __PROC__)))
	    return er;
	 if ((er = _ntx_checkscope(ClipMemoryStore, rd, ro, &sok, __PROC__)))
	    return er;
	 if (!res || !sok)
	    {
	       if ((er = ntx_next(ClipMemoryStore, rd, ro, __PROC__)))
		  return er;
	    }
      }
   return 0;
}

static int
_ntx_leftkeys(ClipMachine * ClipMemoryStore, RDD_ORDER * ro, unsigned int pageoffs, int *keyno, const char *__PROC__)
{
   NTX_PAGE page;
   NTX_BUCKET *buck;
   int i, er;

   if (pageoffs)
      {
	 _ntx_page(ro, pageoffs, &page);
	 *keyno += _rdd_ushort(page.nkeys);
	 for (i = 0; i <= _rdd_ushort(page.nkeys); i++)
	    {
	       buck = _ntx_buck(&page, i);
	       if (_rdd_uint(buck->left))
		  if ((er = _ntx_leftkeys(ClipMemoryStore, ro, _rdd_uint(buck->left), keyno, __PROC__)))
		     return er;
	    }
      }
   return 0;
}

static int
ntx_keyno(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_ORDER * ro, int *keyno, const char *__PROC__)
{
   NTX_HEADER hdr;
   NTX_PAGE page;
   NTX_BUCKET *buck;
   int r, i, j, er;

   *keyno = 0;
   if (rd->eof)
      return 0;

   _ntx_header(ro, &hdr);
   ro->stack[0].page = _rdd_uint(hdr.root);
   ro->level = 0;

   if ((er = _ntx_calc_key(ClipMemoryStore, rd, ro, __PROC__)))
      return er;

   if ((er = _ntx_search_tree(ClipMemoryStore, rd, ro, ro->key, ro->keysize, rd->recno, &r, 1, __PROC__)))
      return er;
   if (r)
      return 0;

   _ntx_page(ro, ro->stack[ro->level].page, &page);

   buck = _ntx_buck(&page, ro->stack[ro->level].pos);
   if (rd->recno != _rdd_uint(buck->recno))
      return 0;

   *keyno += ro->stack[ro->level].pos + 1;
   for (i = 0; i <= ro->stack[ro->level].pos; i++)
      {
	 buck = _ntx_buck(&page, i);
	 if ((er = _ntx_leftkeys(ClipMemoryStore, ro, _rdd_uint(buck->left), keyno, __PROC__)))
	    return er;
      }
   for (j = 0; j < ro->level; j++)
      {
	 _ntx_page(ro, ro->stack[j].page, &page);
	 *keyno += ro->stack[j].pos;
	 for (i = 0; i < ro->stack[j].pos; i++)
	    {
	       buck = _ntx_buck(&page, i);
	       if ((er = _ntx_leftkeys(ClipMemoryStore, ro, _rdd_uint(buck->left), keyno, __PROC__)))
		  return er;
	    }
      }
   return 0;
}

static int
ntx_lastkey(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_ORDER * ro, int *lastkey, const char *__PROC__)
{
   NTX_HEADER hdr;
   int er;

   _ntx_header(ro, &hdr);
   *lastkey = 0;
   return _ntx_leftkeys(ClipMemoryStore, ro, _rdd_uint(hdr.root), lastkey, __PROC__);
}

static int
ntx_gotokey(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_ORDER * ro, unsigned int keyno, int *ok, const char *__PROC__)
{
   NTX_HEADER hdr;
   NTX_PAGE page;
   NTX_BUCKET *buck;
   int left, oneleft, i, lastkey, er;

   *ok = 1;
   if ((er = ntx_lastkey(ClipMemoryStore, rd, ro, &lastkey, __PROC__)))
      return er;
   if (!lastkey)
      {
	 rd->bof = rd->v_bof = rd->eof = 1;
	 if ((er = rd->vtbl->rawgo(ClipMemoryStore, rd, 1, 0, __PROC__)))
	    return er;
	 return 0;
      }

   _ntx_header(ro, &hdr);
   _ntx_page(ro, _rdd_uint(hdr.root), &page);
   left = 0;
   while (1)
      {
	 for (i = 0; i <= _rdd_ushort(page.nkeys); i++)
	    {
	       buck = _ntx_buck(&page, i);
	       oneleft = 0;
	       if ((er = _ntx_leftkeys(ClipMemoryStore, ro, _rdd_uint(buck->left), &oneleft, __PROC__)))
		  return er;
	       if (left + oneleft + 1 == keyno)
		  {
		     goto ok;
		  }
	       else if (left + oneleft + 1 > keyno)
		  {
		     _ntx_page(ro, _rdd_uint(buck->left), &page);
		     break;
		  }
	       left += oneleft + 1;
	    }
      }
 ok:
   return rd->vtbl->rawgo(ClipMemoryStore, rd, _rdd_uint(buck->recno), 1, __PROC__);
}

static int
ntx_descend(ClipMachine * ClipMemoryStore, RDD_ORDER * ro, int descend, int *res, const char *__PROC__)
{
   *res = ro->descend;
   return 0;
}

static int
ntx_keyvalue(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_ORDER * ro, ClipVar * v, const char *__PROC__)
{
   NTX_HEADER hdr;
   NTX_PAGE page;
   NTX_BUCKET *buck;
   int r, er;
   ClipVar vv, *vp;

   if (rd->eof)
      return 0;

   _ntx_header(ro, &hdr);
   ro->stack[0].page = _rdd_uint(hdr.root);
   ro->level = 0;

   if ((er = _ntx_calc_key(ClipMemoryStore, rd, ro, __PROC__)))
      return er;

   if ((er = _ntx_search_tree(ClipMemoryStore, rd, ro, ro->key, ro->keysize, rd->recno, &r, 1, __PROC__)))
      return er;
   if (r)
      return 0;

   _ntx_page(ro, ro->stack[ro->level].page, &page);

   buck = _ntx_buck(&page, ro->stack[ro->level].pos);
   if (rd->recno != _rdd_uint(buck->recno))
      return 0;

   if ((er = rdd_calc(ClipMemoryStore, rd->area, &ro->block, &vv, 0)))
      return er;
   vp = _clip_vptr(&vv);
   _clip_clone(ClipMemoryStore, v, vp);
   _clip_destroy(ClipMemoryStore, &vv);
   return 0;
}

static int
ntx_info(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_ORDER * ro, int cmd, const char *__PROC__)
{
   switch (cmd)
      {
      case DBOI_CONDITION:
	 {
	    _clip_retc(ClipMemoryStore, "");
	    break;
	 }
      case DBOI_CUSTOM:
	 {
	    _clip_retl(ClipMemoryStore, 0);
	    break;
	 }
      case DBOI_HPLOCKING:
	 {
	    _clip_retl(ClipMemoryStore, 0);
	    break;
	 }
      case DBOI_ISCOND:
	 {
	    _clip_retl(ClipMemoryStore, 0);
	    break;
	 }
      case DBOI_ISDESC:
	 {
	    _clip_retl(ClipMemoryStore, 0);
	    break;
	 }
      case DBOI_ORDERCOUNT:
	 {
	    _clip_retni(ClipMemoryStore, 1);
	    break;
	 }
      case DBOI_SCOPEBOTTOM:
	 {
	    _clip_clone(ClipMemoryStore, RETPTR(ClipMemoryStore), &ro->scopebottomvar);
	    break;
	 }
      case DBOI_SCOPETOP:
	 {
	    _clip_clone(ClipMemoryStore, RETPTR(ClipMemoryStore), &ro->scopetopvar);
	    break;
	 }
      case DBOI_SKIPUNIQUE:
	 {
	    _clip_retl(ClipMemoryStore, 1);
	    break;
	 }
      }
   return 0;
}

static int
ntx_destroy(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__)
{
   return 0;
}

static int
ntx_buildtree(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__)
{
   NTX_HEADER hdr;
   int r, er;

   if (!rd->shared && !rd->eof)
      {
	 _ntx_header(ro, &hdr);
	 ro->stack[0].page = _rdd_uint(hdr.root);
	 ro->level = 0;

	 if ((er = _ntx_calc_key(ClipMemoryStore, rd, ro, __PROC__)))
	    return er;

	 if ((er = _ntx_search_tree(ClipMemoryStore, rd, ro, ro->key, ro->keysize, rd->recno, &r, 1, __PROC__)))
	    return er;
      }
   ro->valid_stack = 1;
   return 0;
}

static RDD_INDEX_VTBL *
ntx_vtbl()
{
   RDD_INDEX_VTBL *vtbl = (RDD_INDEX_VTBL *) malloc(sizeof(RDD_INDEX_VTBL));

   memset(vtbl, 0, sizeof(RDD_INDEX_VTBL));
   strcpy(vtbl->id, "NTX");
   strcpy(vtbl->suff, ".ntx");
   strcpy(vtbl->sing_suff, ".ntx");
   strcpy(vtbl->desc, "Generic NTX index files driver v0.0.1 (c) 2001 Copyright ITK Ltd.");
   vtbl->ismulti = 0;		/* one order per file */

   vtbl->open = ntx_open;
   vtbl->close = ntx_close;
   vtbl->create = ntx_create;
   vtbl->zap = ntx_zap;
   vtbl->reindex = ntx_reindex;
   vtbl->gotop = ntx_gotop;
   vtbl->gobottom = ntx_gobottom;
   vtbl->next = ntx_next;
   vtbl->prev = ntx_prev;
   vtbl->seek = ntx_seek;
   vtbl->addkey = ntx_addkey;
   vtbl->delkey = ntx_delkey;
   vtbl->destroy = ntx_destroy;
   vtbl->keyno = ntx_keyno;
   vtbl->lastkey = ntx_lastkey;
   vtbl->gotokey = ntx_gotokey;
   vtbl->descend = ntx_descend;
   vtbl->buildtree = ntx_buildtree;
   vtbl->info = ntx_info;
   vtbl->keyvalue = ntx_keyvalue;
  /*vtbl->setscope        = ntx_setscope; */
   vtbl->formatkey = ntx_formatkey;
   vtbl->_rlock = ntx__rlock;
   vtbl->_wlock = ntx__wlock;
   vtbl->_ulock = ntx__ulock;
   return vtbl;
}

static int
_ctx_openorder(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_INDEX * ri, CTX_TAG * tag, RDD_ORDER ** pro, const char *__PROC__)
{
   RDD_ORDER *ro;
   NTX_HEADER hdr;
   int er;

   ro = *pro = (RDD_ORDER *) malloc(sizeof(RDD_ORDER));
   memset(ro, 0, sizeof(RDD_ORDER));
   ro->vtbl = ri->vtbl;
   ro->index = ri;
   ro->canadd = 1;

   rd->ords_opened++;
   rd->orders = (RDD_ORDER **) realloc(rd->orders, sizeof(RDD_ORDER *) * rd->ords_opened);
   rd->orders[rd->ords_opened - 1] = ro;

   ro->name = (char *) malloc(strlen((const char *) tag->tagname) + 1);
   strcpy(ro->name, (const char *) tag->tagname);
   ro->orderno = rd->ords_opened - 1;
   ro->header = _rdd_uint(tag->header);

   if ((er = rdd_read(ClipMemoryStore, &ri->file, ro->header, sizeof(NTX_HEADER), &hdr, __PROC__)))
      return er;

   ro->expr = (char *) malloc(strlen(hdr.key) + 1);
   strcpy(ro->expr, hdr.key);
   loc_read(rd->loc, (unsigned char *) ro->expr, strlen(ro->expr));
   if ((ro->simpfno = _rdd_fieldno(rd, _clip_casehashword(ro->expr, strlen(ro->expr)))) != -1)
      ro->simpexpr = 1;
   {
      int r = _clip_parni(ClipMemoryStore, 1);
      char expr[1024];

      memset(expr, 0, sizeof(expr));
      rdd_expandmacro(rd->area, r, ro->expr, expr);

      if ((er = _clip_eval_macro(ClipMemoryStore, expr, strlen(expr), &ro->block)))
	 return er;
   }
   ro->unique = hdr.unique;
   ro->descend = hdr.descend;
   ro->custom = hdr.custom;
   ro->keysize = _rdd_ushort(hdr.keysize);
   ro->dec = _rdd_ushort(hdr.dec);

   ro->key = (char *) malloc(ro->keysize + 1);
   ro->fullpage = _rdd_ushort(hdr.fullpage);
   ro->halfpage = _rdd_ushort(hdr.halfpage);

   ro->curpage = calloc(1, sizeof(NTX_PAGE));
   ro->curoffs = 0;

   if (hdr.forexpr[0])
      {
	 int r = _clip_parni(ClipMemoryStore, 1);
	 char expr[1024];

	 ro->cforexpr = malloc(strlen(hdr.forexpr) + 1);
	 strcpy(ro->cforexpr, hdr.forexpr);

	 memset(expr, 0, sizeof(expr));
	 rdd_expandmacro(rd->area, r, ro->cforexpr, expr);

	 if ((er = _clip_eval_macro(ClipMemoryStore, expr, strlen(expr), &ro->bforexpr)))
	    return er;
      }
   ri->norders++;
   return 0;
}

static int
ctx_open(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_INDEX * ri, const char *tag, const char *__PROC__)
{
   CTX_HEADER hdr;
   int i, er;

   if ((er = rdd_read(ClipMemoryStore, &ri->file, 0, sizeof(CTX_HEADER), (char *) &hdr, __PROC__)))
      return er;
   if (_rdd_ushort(hdr.sig) != (unsigned short int) 0x9591)
      {
	 char err[1024];

	 snprintf(err, sizeof(err), _clip_gettext("Not CTX file (bad signature): %s"), ri->path);
	 return rdd_err(ClipMemoryStore, EG_OPEN, 0, __FILE__, __LINE__, __PROC__, err);
      }
   if (!tag)
      {
	 ri->orders = (RDD_ORDER **) malloc(sizeof(RDD_ORDER *) * hdr.ntags);
	 for (i = 0; i < hdr.ntags; i++)
	    if ((er = _ctx_openorder(ClipMemoryStore, rd, ri, &hdr.tags[i], &ri->orders[i], __PROC__)))
	       return er;
      }
   else
      {
	 ri->orders = (RDD_ORDER **) malloc(sizeof(RDD_ORDER *));
	 for (i = 0; i < hdr.ntags; i++)
	    if (strcasecmp(tag, (const char *) hdr.tags[i].tagname) == 0)
	       {
		  if ((er = _ctx_openorder(ClipMemoryStore, rd, ri, &hdr.tags[i], &ri->orders[i], __PROC__)))
		     return er;
		  break;
	       }
      }
   return 0;
}

static int
ctx_close(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_INDEX * ri, const char *__PROC__)
{
   int i;

   for (i = 0; i < ri->norders; i++)
      destroy_rdd_order(ClipMemoryStore, ri->orders[i]);
   destroy_rdd_index(ri);
   return 0;
}

static int
ctx_zap(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_INDEX * ri, const char *__PROC__)
{
   CTX_HEADER chdr;
   NTX_HEADER *hdr;
   NTX_PAGE page;
   NTX_BUCKET *buck;
   int i, j, offs, er;

   if ((er = rdd_read(ClipMemoryStore, &ri->file, 0, sizeof(CTX_HEADER), (char *) &chdr, __PROC__)))
      return er;

   hdr = (NTX_HEADER *) malloc(chdr.ntags * sizeof(NTX_HEADER));
   for (i = 0; i < chdr.ntags; i++)
      {
	 if ((er = rdd_read(ClipMemoryStore, &ri->file, _rdd_uint(chdr.tags[i].header), sizeof(NTX_HEADER), &hdr[i], __PROC__)))
	    return er;
      }

   if ((er = rdd_trunc(ClipMemoryStore, &ri->file, sizeof(CTX_HEADER) + chdr.ntags * (sizeof(NTX_HEADER) + sizeof(NTX_PAGE)), __PROC__)))
      return er;

   for (i = 0; i < chdr.ntags; i++)
      {
	 _rdd_put_uint(chdr.tags[i].header, sizeof(CTX_HEADER) + i * sizeof(NTX_HEADER));
	 ri->orders[i]->header = _rdd_uint(chdr.tags[i].header);
	 _rdd_put_uint(hdr[i].fuu, 0);
	 _rdd_put_uint(hdr[i].root, sizeof(CTX_HEADER) + chdr.ntags * sizeof(NTX_HEADER) + i * sizeof(NTX_PAGE));
	 if ((er = rdd_write(ClipMemoryStore, &ri->file, sizeof(CTX_HEADER) + i * sizeof(NTX_HEADER), sizeof(NTX_HEADER), &hdr[i], __PROC__)))
	    return er;

	 memset(&page, 0, sizeof(NTX_PAGE));
	 offs = 2 * (_rdd_ushort(hdr[i].fullpage) + 2);
	 _rdd_put_ushort(page.nkeys, 0);

	 buck = (NTX_BUCKET *) (((char *) &page) + offs);
	 _rdd_put_uint(buck->left, 0);
	 _rdd_put_uint(buck->recno, 0);

	 for (j = 0; j <= _rdd_ushort(hdr[i].fullpage); ++j, offs += _rdd_ushort(hdr[i].itemsize))
	    _rdd_put_ushort(page.offs + j * 2, offs);

	 if ((er = rdd_write(ClipMemoryStore, &ri->file, sizeof(CTX_HEADER) + chdr.ntags * sizeof(NTX_HEADER) + i * sizeof(NTX_PAGE), sizeof(NTX_PAGE), &page, __PROC__)))
	    return er;

	 for (j = 0; j < ri->norders; j++)
	    {
	       if (strcasecmp((const char *) chdr.tags[i].tagname, ri->orders[j]->name) == 0)
		  {
		     ri->orders[j]->header = sizeof(CTX_HEADER) + i * sizeof(NTX_HEADER);
		     break;
		  }
	    }
      }
   if ((er = rdd_write(ClipMemoryStore, &ri->file, 0, sizeof(CTX_HEADER), &chdr, __PROC__)))
      return er;

   rd->bof = rd->v_bof = rd->eof = 1;
   if ((er = rd->vtbl->rawgo(ClipMemoryStore, rd, 1, 0, __PROC__)))
      return er;
   return 0;
}

static int
ctx_reindex(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_INDEX * ri, const char *__PROC__)
{
   RDD_ORDER *ro;
   int i, er;

   if ((er = ctx_zap(ClipMemoryStore, rd, ri, __PROC__)))
      return er;
   for (i = 0; i < ri->norders; i++)
      {
	 ro = ri->orders[i];
	 if ((er = _ntx_reindex(ClipMemoryStore, rd, ro, __PROC__)))
	    return er;
      }
   return 0;
}

static int
ctx_create(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_INDEX * ri,
	   RDD_ORDER ** rop, const char *tag, const char *expr, ClipVar * block, int unique, int first, unsigned int h, const char *__PROC__)
{
   CTX_HEADER chdr;
   int i, er;
   unsigned int header = 0;

   if (first)
      {
	 memset(&chdr, 0, sizeof(CTX_HEADER));
	 _rdd_put_ushort(chdr.sig, (unsigned short int) 0x9591);
	 if ((er = rdd_write(ClipMemoryStore, &ri->file, 0, sizeof(CTX_HEADER), &chdr, __PROC__)))
	    return er;
	 if (strcmp(rd->name, ri->name) == 0)
	    ri->structural = 1;
      }
   else
      {
	 if ((er = rdd_read(ClipMemoryStore, &ri->file, 0, sizeof(CTX_HEADER), &chdr, __PROC__)))
	    return er;
      }
   for (i = 0; i < chdr.ntags; i++)
      {
	 if (strcasecmp((const char *) chdr.tags[i].tagname, tag) == 0)
	    {
	       if (ri->file.md == (caddr_t) - 1)
		  {
		     struct stat st;
		     fstat(ri->file.fd, &st);
		     _rdd_put_uint(chdr.tags[i].header, st.st_size);
		  }
	       else
		  {
		     _rdd_put_uint(chdr.tags[i].header, ri->file.mapsize);
		     header = _rdd_uint(chdr.tags[i].header);
		  }
	       break;
	    }
      }
   if (i == chdr.ntags)
      {
	 if (i == 62)
	    return rdd_err(ClipMemoryStore, EG_CREATE, 0, __FILE__, __LINE__, __PROC__, "Too many tags");
	 strncpy((char *) chdr.tags[i].tagname, tag, MAX_TAG_LEN);
	 chdr.tags[i].tagname[MAX_TAG_LEN] = 0;
	 if (ri->file.md == (caddr_t) - 1)
	    {
	       struct stat st;
	       fstat(ri->file.fd, &st);
	       _rdd_put_uint(chdr.tags[i].header, st.st_size);
	    }
	 else
	    {
	       _rdd_put_uint(chdr.tags[i].header, ri->file.mapsize);
	    }
	 header = _rdd_uint(chdr.tags[i].header);
	 chdr.ntags = i + 1;
      }
   if ((er = rdd_write(ClipMemoryStore, &ri->file, 0, sizeof(CTX_HEADER), &chdr, __PROC__)))
      return er;
   if ((er = ntx_create(ClipMemoryStore, rd, ri, rop, tag, expr, block, unique, 0, header, __PROC__)))
      return er;
   return 0;
}

static int
ctx_destroy(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__)
{
   CTX_HEADER chdr;
   int i, er;

   if ((er = rdd_read(ClipMemoryStore, &ro->index->file, 0, sizeof(CTX_HEADER), &chdr, __PROC__)))
      return er;
   for (i = 0; i < chdr.ntags; i++)
      {
	 if (strcasecmp((const char *) chdr.tags[i].tagname, ro->name) == 0)
	    {
	       for (i++; i < chdr.ntags; i++)
		  {
		     memcpy(&chdr.tags[i - 1], &chdr.tags[i], sizeof(CTX_TAG));
		  }
	       memset(&chdr.tags[i - 1], 0, sizeof(CTX_TAG));
	       chdr.ntags--;
	       if ((er = rdd_write(ClipMemoryStore, &ro->index->file, 0, sizeof(CTX_HEADER), &chdr, __PROC__)))
		  return er;
	       break;
	    }
      }
   return 0;
}

static RDD_INDEX_VTBL *
ctx_vtbl()
{
   RDD_INDEX_VTBL *vtbl = (RDD_INDEX_VTBL *) malloc(sizeof(RDD_INDEX_VTBL));

   memset(vtbl, 0, sizeof(RDD_INDEX_VTBL));
   strcpy(vtbl->id, "CTX");
   strcpy(vtbl->suff, ".ctx");
   strcpy(vtbl->sing_suff, ".ntx");
   strcpy(vtbl->desc, "Compound NTX (CTX) index files driver v0.0.1 (c) 2001 Copyright ITK Ltd.");
   vtbl->ismulti = 1;

   vtbl->open = ctx_open;
   vtbl->close = ctx_close;
   vtbl->create = ctx_create;
   vtbl->zap = ctx_zap;
   vtbl->reindex = ctx_reindex;
   vtbl->gotop = ntx_gotop;
   vtbl->gobottom = ntx_gobottom;
   vtbl->next = ntx_next;
   vtbl->prev = ntx_prev;
   vtbl->seek = ntx_seek;
   vtbl->addkey = ntx_addkey;
   vtbl->delkey = ntx_delkey;
   vtbl->destroy = ctx_destroy;
   vtbl->keyno = ntx_keyno;
   vtbl->lastkey = ntx_lastkey;
   vtbl->gotokey = ntx_gotokey;
   vtbl->descend = ntx_descend;
   vtbl->buildtree = ntx_buildtree;
   vtbl->info = ntx_info;
   vtbl->keyvalue = ntx_keyvalue;
  /*vtbl->setscope        = ntx_setscope; */
   vtbl->formatkey = ntx_formatkey;
   vtbl->_rlock = ntx__rlock;
   vtbl->_wlock = ntx__wlock;
   vtbl->_ulock = ntx__ulock;
   return vtbl;
}
