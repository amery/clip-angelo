/*
	Copyright (C) 2001  ITK
	Author  : Rushad Nabiullin (rust@itk.ru)
	Licence : (GPL) http://www.itk.ru/clipper/licence.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

*/
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include "ci_rdd.h"
#include "ci_error.ch"
#include "ci_btree.h"
#include "ci_clipcfg.h"
#ifdef HAVE_REGEX_H
#include <regex.h>
#endif

#define D20070109		/* spped up after "custom manage" */

extern int type_weight(int type);

static RDD_INDEX_VTBL *cdx_vtbl();

static RDD_INDEX_VTBL *idx_vtbl();

typedef struct _CDX_HEADER_
{
   unsigned char root[4];
   unsigned char fuu[4];
   unsigned char reserved1[4];
   unsigned char keysize[2];
   unsigned char options;
   unsigned char sign;
   unsigned char reserved2[483];
   unsigned char ic;
   unsigned char autonom[2];
   unsigned char sort[2];
   unsigned char totlen[2];
   unsigned char forlen[2];
   unsigned char reserved3[2];
   unsigned char explen[2];
   unsigned char keyforexpr[512];
} CDX_HEADER;

typedef struct _CDX_PAGE_
{
   unsigned char attrs[2];
   unsigned char nkeys[2];
   unsigned char left[4];
   unsigned char right[4];
   unsigned char res1[500];
} CDX_PAGE;

typedef struct _CDX_LEAF_
{
   unsigned char attrs[2];
   unsigned char nkeys[2];
   unsigned char left[4];
   unsigned char right[4];
   unsigned char free[2];
   unsigned char recmask[4];
   unsigned char dupmask;
   unsigned char trailmask;
   unsigned char recbits;
   unsigned char dupbits;
   unsigned char trailbits;
   unsigned char bytes;
   unsigned char keys[488];
} CDX_LEAF;

typedef struct _CDX_BRANCH_
{
   unsigned char attrs[2];
   unsigned char nkeys[2];
   unsigned char left[4];
   unsigned char right[4];
   unsigned char keys[500];
} CDX_BRANCH;

#define _CDX_HEADER(ro,hdr) if((er = rdd_read(ClipMachineMemory,&ro->index->file,ro->header,sizeof(CDX_HEADER),hdr,__PROC__))) return er
#define _CDX_PAGE(ro,pageoffs,page)	if((er = rdd_read(ClipMachineMemory,&ro->index->file,pageoffs,sizeof(CDX_PAGE),page,__PROC__))) return er; \
		leaf = (_rdd_ushort((page)->attrs)&2)?(CDX_LEAF*)(page):NULL; \
		branch = (_rdd_ushort((page)->attrs)&2)?NULL:(CDX_BRANCH*)(page)
#define _CDX_LEAF_ITEM(item,rec,dup,trail) \
		rec = (unsigned int)((item) & _rdd_uint(leaf->recmask)); \
		(item) = (item) >> leaf->recbits; \
		dup = (unsigned char)((item) & leaf->dupmask); \
		(item) = (item) >> leaf->dupbits; \
		trail = (unsigned char)((item) & leaf->trailmask)

static int cdx_open(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, const char *tag, const char *__PROC__);

static int cdx_close(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, const char *__PROC__);

static int cdx_create(ClipMachine * ClipMachineMemory, RDD_DATA * rd,
		      RDD_INDEX * ri, RDD_ORDER ** rop, const char *tag,
		      const char *expr, ClipVar * block, int unique, int first, unsigned int header, const char *__PROC__);
static int cdx_reindex(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, const char *__PROC__);

static int cdx_zap(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, const char *__PROC__);

static int cdx_gotop(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__);

static int cdx_gobottom(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__);

static int cdx_next(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__);

static int cdx_prev(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__);

static int cdx_seek(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro,
		    ClipVar * v, int soft, int last, int *found, const char *__PROC__);
static int cdx_gotokey(ClipMachine * ClipMachineMemory, RDD_DATA * rd,
		       RDD_ORDER * ro, unsigned int keyno, int *ok, const char *__PROC__);
static int cdx_addkey(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, ClipVar * v, const char *__PROC__);

static int cdx_formatkey(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, ClipVar * var, void *res, const char *__PROC__);

static int cdx_keyno(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, int *keyno, const char *__PROC__);

static int cdx_lastkey(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, int *lastkey, const char *__PROC__);

static int cdx_info(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, int cmd, const char *__PROC__);

static int cdx_keyvalue(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, ClipVar * v, const char *__PROC__);

static int cdx_setscope(ClipMachine * ClipMachineMemory, RDD_DATA * rd,
			RDD_ORDER * ro, ClipVar * top, ClipVar * bottom,
			unsigned int *map, int bytes, int exact, const char *__PROC__);
static int cdx_wildskip(ClipMachine * ClipMachineMemory, RDD_DATA * rd,
			RDD_ORDER * ro, const char *pattern, int regular,
			const char *s, ClipVar * block, int *found, const char *__PROC__);

static int _cdx_addkey(ClipMachine * ClipMachineMemory, int area, DbfLocale * loc,
		       ClipVar * v, RDD_ORDER * ro, void *key, int len, unsigned int recno, const char *__PROC__);
static int __cdx_addkey(ClipMachine * ClipMachineMemory, int area, RDD_ORDER * ro,
			ClipVar * v, DbfLocale * loc, unsigned int recno, const char *__PROC__);

static int
cdx__rlock(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, const char *__PROC__)
{
   return 0;
}

static int
cdx__wlock(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, const char *__PROC__)
{
   return 0;
}

static int
cdx__ulock(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, const char *__PROC__)
{
   return 0;
}

static int _cdx_prev_rm(ClipMachine * ClipMachineMemory, RDD_ORDER * ro,
			CDX_LEAF * leaf, int *out, int cache, const char *__PROC__);
static int _cdx_next_rm(ClipMachine * ClipMachineMemory, RDD_ORDER * ro,
			CDX_LEAF * leaf, int *out, int cache, const char *__PROC__);

static int idx_open(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, const char *tag, const char *__PROC__);

static int idx_create(ClipMachine * ClipMachineMemory, RDD_DATA * rd,
		      RDD_INDEX * ri, RDD_ORDER ** rop, const char *tag,
		      const char *expr, ClipVar * block, int unique, int first, unsigned int header, const char *__PROC__);
static int idx_zap(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, const char *__PROC__);

static int idx_reindex(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, const char *__PROC__);

const unsigned int _cdx_bits[] = {
   0x00000001, 0x00000003, 0x00000007, 0x0000000f,
   0x0000001f, 0x0000003f, 0x0000007f, 0x000000ff,
   0x000001ff, 0x000003ff, 0x000007ff, 0x00000fff,
   0x00001fff, 0x00003fff, 0x00007fff, 0x0000ffff,
   0x0001ffff, 0x0003ffff, 0x0007ffff, 0x000fffff,
   0x001fffff, 0x003fffff, 0x007fffff, 0x00ffffff,
   0x01ffffff, 0x03ffffff, 0x07ffffff, 0x0fffffff,
   0x1fffffff, 0x3fffffff, 0x7fffffff, 0xffffffff
};

#define er_deepexceeded     _clip_gettext("MAX_BTREE_DEEP exceeded")
#define er_badroot          _clip_gettext("CDX internal error: bad root page")
#define er_badleaf          _clip_gettext("CDX internal error: bad leaf page")
#define er_nokeys           _clip_gettext("CDX internal error: no keys on the page")
#define er_badforexpr       _clip_gettext("Bad FOR expression")
#define er_badheader        _clip_gettext("Bad header")
#define er_notag            _clip_gettext("No such tag")
#define er_baddata          _clip_gettext("Bad data")
#define er_badwhileexpr     _clip_gettext("Bad WHILE expression")
#define er_badevalexpr      _clip_gettext("Bad EVAL expression")

int
clip_INIT_CDX(ClipMachine * ClipMachineMemory)
{
   rdd_registerindexdriver(ClipMachineMemory, cdx_vtbl());
   return 0;
}

int
clip_INIT_IDX(ClipMachine * ClipMachineMemory)
{
   rdd_registerindexdriver(ClipMachineMemory, idx_vtbl());
   return 0;
}

static int
_cdx_loadpage(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, unsigned int offs, const char *__PROC__)
{
   if (offs == ro->curoffs)
      return 0;
   ro->curoffs = offs;
   return rdd_read(ClipMachineMemory, &ro->index->file, offs, sizeof(CDX_PAGE), (char *) ro->curpage, __PROC__);
}

static int
_cdx_getfreepage(ClipMachine * ClipMachineMemory, RDD_INDEX * ri, unsigned int *page, const char *__PROC__)
{
   unsigned int fsize;

   char      fuu[4];

   int       er;

   if ((er = rdd_read(ClipMachineMemory, &ri->file, 4, 4, fuu, __PROC__)))
      return er;
   *page = _rdd_uint((unsigned char *) fuu);
   if (!*page)
    {
       int       delta;

       if (ri->file.md == (caddr_t) - 1)
	{
	   struct stat st;

	   fstat(ri->file.fd, &st);
	   fsize = st.st_size;
	}
       else
	  fsize = ri->file.mapsize;

       delta = (fsize / sizeof(CDX_PAGE)) / 5;
       if (delta < 1)
	  delta = 1;
       *page = fsize;
       if ((er = rdd_write(ClipMachineMemory, &ri->file, *page + delta * sizeof(CDX_PAGE), 0, "", __PROC__)))
	  return er;
       if (delta > 1)
	{
	   int       i;

	   _rdd_put_uint((unsigned char *) fuu, *page + sizeof(CDX_PAGE));
	   if ((er = rdd_write(ClipMachineMemory, &ri->file, 4, 4, fuu, __PROC__)))
	      return er;
	   for (i = 1; i < delta; i++)
	    {
	       if (i < delta - 1)
		  _rdd_put_uint((unsigned char *) fuu, *page + (i + 1) * sizeof(CDX_PAGE));
	       else
		  _rdd_put_uint((unsigned char *) fuu, 0);
	       if ((er = rdd_write(ClipMachineMemory, &ri->file, *page + i * sizeof(CDX_PAGE), 4, fuu, __PROC__)))
		  return er;
	    }
	}
    }
   else
    {
       if ((er = rdd_read(ClipMachineMemory, &ri->file, *page, 4, fuu, __PROC__)))
	  return er;
       if ((er = rdd_write(ClipMachineMemory, &ri->file, 4, 4, fuu, __PROC__)))
	  return er;
    }
   return 0;
}

static int
_cdx_putfreepage(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, unsigned int page, const char *__PROC__)
{
   char      fuu[4];

   CDX_PAGE  fp;

   int       er;

   memset(&fp, 0, sizeof(CDX_PAGE));
   if ((er = rdd_read(ClipMachineMemory, &ro->index->file, 4, 4, fuu, __PROC__)))
      return er;
   if ((er = rdd_write(ClipMachineMemory, &ro->index->file, 4, 4, &page, __PROC__)))
      return er;
   _rdd_put_uint((unsigned char *) &fp, _rdd_uint((unsigned char *) fuu));
   if ((er = rdd_write(ClipMachineMemory, &ro->index->file, page, sizeof(CDX_PAGE), &fp, __PROC__)))
      return er;
   return 0;
}

static int
_cdx_init_order(ClipMachine * ClipMachineMemory, int area, RDD_INDEX * ri,
		unsigned int header, RDD_ORDER * ro, const char *__PROC__)
{
   CDX_HEADER hdr;

   int       er;

   if ((er = rdd_read(ClipMachineMemory, &ri->file, header, sizeof(CDX_HEADER), &hdr, __PROC__)))
      return er;
   memset(ro, 0, sizeof(RDD_ORDER));
   ro->expr = malloc(_rdd_ushort(hdr.explen) + 1);
   memcpy(ro->expr, hdr.keyforexpr, _rdd_ushort(hdr.explen));
   ro->expr[_rdd_ushort(hdr.explen)] = 0;
   if (ri->rd)
    {
       if ((ro->simpfno = _rdd_fieldno(ri->rd, _clip_casehashword(ro->expr, strlen(ro->expr)))) != -1)
	{
	   ro->simpexpr = 1;
	   if (ri->rd->fields[ro->simpfno].binary)
	      ro->binary = 1;
	   ro->type = ri->rd->fields[ro->simpfno].type;
	}
    }
   if (!ro->binary)
      loc_read(ri->loc, (unsigned char *) ro->expr, strlen(ro->expr));
   ro->cforexpr = malloc(_rdd_ushort(hdr.forlen) + 1);
   memcpy(ro->cforexpr, hdr.keyforexpr + _rdd_ushort(hdr.explen), _rdd_ushort(hdr.forlen));
   ro->cforexpr[_rdd_ushort(hdr.forlen)] = 0;
   if (!ro->binary)
      loc_read(ri->loc, (unsigned char *) ro->cforexpr, strlen(ro->cforexpr));
   ro->header = header;

   ro->ic = hdr.ic;
   ro->unique = (hdr.options & 0x0001 ? 1 : 0);
   ro->custom = (hdr.options & 0x0004 ? 1 : 0);
   ro->descend = (_rdd_ushort(hdr.sort) ? 1 : 0);
   ro->keysize = _rdd_ushort(hdr.keysize);
   ro->curpage = calloc(1, sizeof(CDX_PAGE));
   ro->curoffs = 0;
   if (strlen(ro->expr) == 0)
    {				/* bag order */
       ro->type = 'C';
       ro->bufsize = 10;
    }
   else
    {				/* calc expr and determine key type */
       int       r = _clip_parni(ClipMachineMemory, 1);

       char      expr[1024];

       ClipVar   v, *vp;

       memset(expr, 0, sizeof(expr));
       rdd_expandmacro(area, r, ro->expr, expr);

       if ((er = _clip_eval_macro(ClipMachineMemory, expr, strlen(expr), &ro->block)))
	  return er;

       if (ro->type == 'X')
	{
	   ro->bufsize = ri->rd->fields[ro->simpfno].len - 1;
	}
       else
	{
	   if ((er = rdd_calc(ClipMachineMemory, area, &ro->block, &v, 0)))
	      return er;
	   vp = _clip_vptr(&v);
	   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
	    {
	       ro->type = 'C';
	       ro->bufsize = ro->keysize;
	    }
	   else if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
	    {
	       ro->type = 'N';
	       ro->bufsize = sizeof(double);
	    }
	   else if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == DATE_type_of_ClipVarType)
	    {
	       ro->type = 'D';
	       ro->bufsize = sizeof(double);
	    }
	   else if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == DATETIME_type_of_ClipVarType)
	    {
	       ro->type = 'T';
	       ro->bufsize = 8;
	    }
	   else if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == LOGICAL_type_of_ClipVarType)
	    {
	       ro->type = 'L';
	       ro->bufsize = 1;
	    }
	   else
	      ro->type = 'U';
	   _clip_destroy(ClipMachineMemory, &v);
	}
    }
   ro->key = malloc(ro->bufsize);
   ro->index = ri;
   ro->vtbl = ri->vtbl;
   return 0;
}

static int
_cdx_init_rootleaf(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, unsigned int page, const char *__PROC__)
{
   CDX_LEAF  leaf;

   int       i, bits = 1, er;

   unsigned int mask;

   _rdd_put_ushort(leaf.attrs, 3);
   _rdd_put_ushort(leaf.nkeys, 0);
   _rdd_put_uint(leaf.left, 0xffffffff);
   _rdd_put_uint(leaf.right, 0xffffffff);
   _rdd_put_ushort(leaf.free, sizeof(leaf.keys));

   for (i = 1; bits < ro->bufsize; bits = (bits << 1) + 1, i++);
   leaf.dupbits = leaf.trailbits = i;
   leaf.dupmask = leaf.trailmask = bits;

   leaf.bytes = 3;
   leaf.recbits = leaf.bytes * 8 - (leaf.dupbits + leaf.trailbits);
   mask = 1;
   for (i = 1; i < leaf.recbits; i++)
      mask = (mask << 1) + 1;
   _rdd_put_uint(leaf.recmask, mask);

   memset(leaf.keys, 0, sizeof(leaf.keys));

   if ((er = rdd_write(ClipMachineMemory, &ro->index->file, page, sizeof(CDX_LEAF), &leaf, __PROC__)))
      return er;
   if ((er = rdd_write(ClipMachineMemory, &ro->index->file, ro->header, 4, &page, __PROC__)))
      return er;
   return 0;
}

static void
_cdx_dup_trail_rec(CDX_LEAF * leaf, int pos, int *dup, int *trail, int *rec)
{
   unsigned long long item;

   unsigned int _rec;

   int       _dup, _trail;

   memcpy(&item, leaf->keys + pos * leaf->bytes, leaf->bytes);
   _CDX_LEAF_ITEM(item, _rec, _dup, _trail);
   *dup = _dup;
   *trail = _trail;
   if (rec)
      *rec = _rec;
}

static int
_cdx_recno(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, int level, unsigned int *recno, const char *__PROC__)
{
   CDX_PAGE  page;

   CDX_LEAF *leaf;

   CDX_BRANCH *branch;

   int       er;

   _CDX_PAGE(ro, ro->stack[level].page, &page);
   if (leaf)
    {
       unsigned long long item;

       unsigned int rec;

       int       dup, trail;

       memcpy(&item, leaf->keys + (ro->stack[level].pos) * leaf->bytes, leaf->bytes);
       _CDX_LEAF_ITEM(item, rec, dup, trail);
       *recno = rec;
    }
   else
    {
       *recno = _rdd_backuint(branch->keys + (ro->stack[level].pos + 1) * (ro->bufsize + 8) - 4);
    }
   return 0;
}

static void
_cdx_leaf_item(RDD_ORDER * ro, CDX_LEAF * leaf, int pos, unsigned int *recno, void *keyval)
{
   unsigned long long item;

   unsigned int rec = 0;

   int       dup, trail;

   char     *p = (char *) leaf + sizeof(CDX_LEAF);

   int       i;

   for (i = 0; i <= pos; i++)
    {
       memcpy(&item, leaf->keys + i * leaf->bytes, leaf->bytes);
       _CDX_LEAF_ITEM(item, rec, dup, trail);
       p -= ro->bufsize - (dup + trail);
       memcpy(keyval + dup, p, ro->bufsize - (dup + trail));
       if (ro->type == 'C' && !ro->binary)
	{
	   memset(keyval + dup + ro->bufsize - (dup + trail), 0x20, trail);
	}
       else
	{
	   memset(keyval + dup + ro->bufsize - (dup + trail), 0x00, trail);
	}
    }
   *recno = rec;
}

static int
_cdx_recno_keyval(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, int level,
		  unsigned int *recno, void *keyval, const char *__PROC__)
{
   CDX_PAGE  page;

   CDX_LEAF *leaf;

   CDX_BRANCH *branch;

   int       er;

   _CDX_PAGE(ro, ro->stack[level].page, &page);
   if (leaf)
    {
       _cdx_leaf_item(ro, leaf, ro->stack[level].pos, recno, keyval);
    }
   else
    {
    }
   return 0;
}

static int
_cdx_search_leaf_recno(RDD_ORDER * ro, CDX_LEAF * leaf, unsigned int recno, int *pos)
{
   for ((*pos) = 0; *pos < _rdd_ushort(leaf->nkeys); (*pos)++)
    {
       if (((*(unsigned int *) (leaf->keys + (*pos) * leaf->bytes)) & _rdd_uint(leaf->recmask)) == recno)
	{
	   return 1;
	}
    }
   return 0;
}

static int
_cdx_cmp(RDD_ORDER * ro, unsigned char *s1, unsigned char *s2, int len)
{
   int       r;

   if (ro->type == 'C' && !ro->binary)
    {
       r = loc_cmp(ro->index->loc, s1, s2, len);
    }
   else if (ro->type != 'X')
    {
       r = memcmp(s1, s2, ro->bufsize);
    }
   else
    {
       r = s1[0] - s2[0];
       if (r == 0)
	{
	   if (s1[0] == type_weight(CHARACTER_type_of_ClipVarType) && !ro->binary)
	    {
	       r = loc_cmp(ro->index->loc, s1 + 1, s2 + 1, len - 1);
	    }
	   else
	    {
	       r = memcmp(s1 + 1, s2 + 1, len - 1);
	    }
	}
    }
   return r;
}

static int
_cdx_search_leaf(RDD_ORDER * ro, CDX_LEAF * leaf, void *key, int len, unsigned int recno, int *pos, int back)
{
   void     *val = malloc(ro->bufsize);

   unsigned long long item;

   unsigned int rec;

   int       dup, trail;

   char     *p = (char *) leaf + sizeof(CDX_LEAF);

   int       ret = 0;

   int       f = 0;

   if (_rdd_ushort(leaf->nkeys) == 0)
    {
       *pos = 0;
    }
   else
    {
       for ((*pos) = 0; *pos < _rdd_ushort(leaf->nkeys); (*pos)++)
	{
	   int       r;

	   memcpy(&item, leaf->keys + (*pos) * leaf->bytes, leaf->bytes);
	   _CDX_LEAF_ITEM(item, rec, dup, trail);
	   p -= ro->bufsize - (dup + trail);
	   memset(val + dup, 0, ro->bufsize - dup);
	   memcpy(val + dup, p, ro->bufsize - (dup + trail));
	   if (ro->type == 'C' && !ro->binary)
	      memset(val + dup + ro->bufsize - (dup + trail), 0x20, trail);
	   r = _cdx_cmp(ro, val, key, len);
	   if (r == 0)
	    {
	       memcpy(ro->key, val, ro->keysize);
	       if (recno)
		{
		   if (rec == recno)
		    {
		       ret = 1;
		       goto e;
		    }
		   else if (rec > recno)
		    {
		       goto e;
		    }
		}
	       else
		{
		   if (back && (*pos < _rdd_ushort(leaf->nkeys) - 1))
		    {
		       f = 1;
		       continue;
		    }
		   ret = 1;
		   goto e;
		}
	    }
	   else if (r > 0)
	    {
	       if (back)
		  (*pos)--;
	       ret = f;
	       goto e;
	    }
	   else
	    {
	       memcpy(ro->key, val, ro->keysize);
	    }
	}
    }
 e:
   free(val);
   return ret;
}

static int
_cdx_search_tree(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, void *key,
		 int len, unsigned int recno, int *found, int *out, const char *__PROC__)
{
   CDX_PAGE  page;

   CDX_LEAF *leaf;

   CDX_BRANCH *branch;

   int       er;

   *found = 0;
   if (out)
      *out = 0;
   _CDX_PAGE(ro, ro->stack[ro->level].page, &page);
   while (!leaf)
    {
       int       i;

       for (i = 0; i < _rdd_ushort(branch->nkeys); i++)
	{
	   unsigned int recno1 = _rdd_backuint(branch->keys + i * (ro->bufsize + 8) + ro->bufsize);

	   int       r;

	   r = _cdx_cmp(ro, branch->keys + i * (ro->bufsize + 8), key, len);
	   if ((r > 0) || ((r == 0) && (recno1 >= recno)) || (i == _rdd_ushort(branch->nkeys) - 1))
	    {

	       ro->stack[ro->level].pos = i;
	       (ro->level)++;
	       if ((ro->level) >= MAX_BTREE_DEEP)
		  return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_deepexceeded);
	       if ((er = _cdx_recno(ClipMachineMemory, ro, (ro->level) - 1, &ro->stack[ro->level].page, __PROC__)))
		  return er;
	       _CDX_PAGE(ro, ro->stack[ro->level].page, &page);
	       break;
	    }
	}
    }
   *found = _cdx_search_leaf(ro, leaf, key, len, recno, &(ro->stack[ro->level].pos), 0);
   if (ro->stack[ro->level].pos < 0)
      if (out)
	 *out = -1;
   if (ro->stack[ro->level].pos == _rdd_ushort(leaf->nkeys))
      if (out)
	 *out = 1;
   return 0;
}

static int
_cdx_back_search_tree(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, void *key,
		      int len, unsigned int recno, int *found, int *out, const char *__PROC__)
{
   CDX_PAGE  page;

   CDX_LEAF *leaf;

   CDX_BRANCH *branch;

   int       er, i, ll = 0;

   *found = 0;
   if (out)
      *out = 0;
   _CDX_PAGE(ro, ro->stack[ro->level].page, &page);
   while (!leaf)
    {
       for (i = _rdd_ushort(branch->nkeys) - 1; i >= 0; i--)
	{
	   unsigned int recno1;

	   int       s;

	   recno1 = _rdd_backuint(branch->keys + i * (ro->bufsize + 8) + ro->bufsize);
	   s = _cdx_cmp(ro, branch->keys + i * (ro->bufsize + 8), key, len);
	   if (recno && !s)
	      s = recno1 - recno;
	   if ((s <= 0) || (i == 0))
	    {
	       ro->stack[ro->level].pos = i;
	       if ((s <= 0) && (i < _rdd_ushort(branch->nkeys) - 1))
		{
		   ro->stack[ro->level].pos++;
		   (ro->level)++;
		   if ((ro->level) >= MAX_BTREE_DEEP)
		      return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_deepexceeded);
		   if ((er = _cdx_recno(ClipMachineMemory, ro, (ro->level) - 1, &ro->stack[ro->level].page, __PROC__)))
		      return er;
		   _CDX_PAGE(ro, ro->stack[ro->level].page, &page);
		   break;
/*					if(leaf){
						void* val = calloc(1,ro->bufsize);
						unsigned long long item;
						unsigned int rec;
						int dup,trail;
						char* p = (char*)leaf+sizeof(CDX_LEAF);

						memcpy(&item,leaf->keys,leaf->bytes);
						_CDX_LEAF_ITEM(item,rec,dup,trail);
						p -= ro->bufsize-(dup+trail);
						memcpy(val,p,ro->bufsize-trail);
						s = _cdx_cmp(ro,val,key,len);
						free(val);
						if(recno && !s)
							s = rec-recno;
						if(s > 0){
							ro->level--;
							ro->stack[ro->level].pos = i;
							ll = 1;
						} else {
							break;
						}
					} else {
						break;
					}*/
		}
	       (ro->level)++;
	       if ((ro->level) >= MAX_BTREE_DEEP)
		  return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_deepexceeded);
	       if ((er = _cdx_recno(ClipMachineMemory, ro, (ro->level) - 1, &ro->stack[ro->level].page, __PROC__)))
		  return er;
	       _CDX_PAGE(ro, ro->stack[ro->level].page, &page);
	       break;
	    }
	}
    }
   if (recno)
    {
       *found = _cdx_search_leaf_recno(ro, leaf, recno, &(ro->stack[ro->level].pos));
    }
   else
    {
       int       o;

       *found = _cdx_search_leaf(ro, leaf, key, len, 0, &(ro->stack[ro->level].pos), 1);
       if (ro->stack[ro->level].pos < 0)
	{
	   ro->stack[ro->level].pos = 0;
	   if ((er = _cdx_prev_rm(ClipMachineMemory, ro, leaf, &o, 0, __PROC__)))
	      return er;
	   if (out && o)
	    {
	       *out = -1;
	    }
	   else
	    {
	       void     *val = calloc(1, ro->bufsize);

	       unsigned int recno;

	       int       s;

	       _cdx_leaf_item(ro, leaf, _rdd_ushort(leaf->nkeys) - 1, &recno, val);
	       s = _cdx_cmp(ro, val, key, len);
	       free(val);
	       if (!s)
		  *found = 1;
/*
				void* val = calloc(1,ro->bufsize);
				unsigned long long item;
				unsigned int rec;
				int dup,trail,s;
				char* p = (char*)leaf+sizeof(CDX_LEAF);

				memcpy(&item,leaf->keys,leaf->bytes);
				_CDX_LEAF_ITEM(item,rec,dup,trail);
				p -= ro->bufsize-(dup+trail);
				memcpy(val,p,ro->bufsize-trail);
				s = _cdx_cmp(ro,val,key,len);
				free(val);
				if(recno && !s)
					s = rec-recno;
				if(!s)
					*found = 1;
*/
	    }
	}
       if ((ro->stack[ro->level].pos >= _rdd_ushort(leaf->nkeys)) && !ll)
	{
	   ro->stack[ro->level].pos = _rdd_ushort(leaf->nkeys);
	   if ((er = _cdx_next_rm(ClipMachineMemory, ro, leaf, &o, 0, __PROC__)))
	      return er;
	   if (out && o)
	    {
	       *out = 1;
	    }
	   else
	    {
	       void     *val = calloc(1, ro->bufsize);

	       unsigned long long item;

	       unsigned int rec;

	       int       dup, trail, s;

	       char     *p = (char *) leaf + sizeof(CDX_LEAF);

	       memcpy(&item, leaf->keys, leaf->bytes);
	       _CDX_LEAF_ITEM(item, rec, dup, trail);
	       p -= ro->bufsize - (dup + trail);
	       memcpy(val, p, ro->bufsize - trail);
	       s = _cdx_cmp(ro, val, key, len);
	       free(val);
	       if (recno && !s)
		  s = rec - recno;
	       if (!s)
		  *found = 1;
	    }
	}
    }
   return 0;
}

static int
_cdx_first(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, int *out, const char *__PROC__)
{
   CDX_HEADER hdr;

   CDX_PAGE  page;

   CDX_LEAF *leaf;

   CDX_BRANCH *branch;

   int       er;

   ro->level = 0;
   *out = 0;
   _CDX_HEADER(ro, &hdr);
   ro->stack[0].page = _rdd_uint(hdr.root);

   _CDX_PAGE(ro, ro->stack[ro->level].page, &page);
   if (!(_rdd_ushort(page.attrs) & 0x1))	/* not root page */
      return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_badroot);

   if (_rdd_ushort(page.nkeys) == 0)
    {
       *out = 1;
       return 0;
    }

   ro->stack[0].pos = 0;
   while (!leaf)
    {
       (ro->level)++;
       if ((ro->level) >= MAX_BTREE_DEEP)
	  return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_deepexceeded);
       if ((er = _cdx_recno(ClipMachineMemory, ro, (ro->level) - 1, &ro->stack[ro->level].page, __PROC__)))
	  return er;
       ro->stack[ro->level].pos = 0;
       _CDX_PAGE(ro, ro->stack[ro->level].page, &page);
    }
#ifndef D20070109
   if (*out != 1)
    {
       int       recno;

       _cdx_leaf_item(ro, leaf, ro->stack[ro->level].pos, &recno, ro->key);
    }
#endif
   return 0;
}

static int
_cdx_last(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, int *out, const char *__PROC__)
{
   CDX_HEADER hdr;

   CDX_PAGE  page;

   CDX_LEAF *leaf;

   CDX_BRANCH *branch;

   int       er;

   ro->level = 0;
   *out = 0;
   _CDX_HEADER(ro, &hdr);
   ro->stack[0].page = _rdd_uint(hdr.root);

   _CDX_PAGE(ro, ro->stack[ro->level].page, &page);
   if (!(_rdd_ushort(page.attrs) & 0x1))	/* not root page */
      return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_badroot);

   if (_rdd_ushort(page.nkeys) == 0)
    {
       *out = 1;
       return 0;
    }

   ro->stack[0].pos = _rdd_ushort(page.nkeys) - 1;
   while (!leaf)
    {
       (ro->level)++;
       if ((ro->level) >= MAX_BTREE_DEEP)
	  return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_deepexceeded);
       if ((er = _cdx_recno(ClipMachineMemory, ro, (ro->level) - 1, &ro->stack[ro->level].page, __PROC__)))
	  return er;
       _CDX_PAGE(ro, ro->stack[ro->level].page, &page);
       ro->stack[ro->level].pos = _rdd_ushort(page.nkeys) - 1;
    }
#ifndef D20070109
   if (*out != 1)
    {
       int       recno;

       _cdx_leaf_item(ro, leaf, ro->stack[ro->level].pos, &recno, ro->key);
    }
#endif
   return 0;
}

static int
_cdx_next_rm(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, CDX_LEAF * leaf, int *out, int cache, const char *__PROC__)
{
   int       er;

   *out = 0;
   if (!leaf)
      return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_badleaf);

   ro->stack[ro->level].pos++;
   if (ro->stack[ro->level].pos >= _rdd_ushort(leaf->nkeys))
    {
       ro->stack[ro->level].pos = _rdd_ushort(leaf->nkeys);
       if (_rdd_uint(leaf->right) != 0xffffffff)
	{
	   ro->stack[ro->level].page = _rdd_uint(leaf->right);
	   if (cache)
	    {
	       if ((er = _cdx_loadpage(ClipMachineMemory, ro, ro->stack[ro->level].page, __PROC__)))
		  return er;
	    }
	   else
	    {
	       if ((er =
		    rdd_read(ClipMachineMemory, &ro->index->file, ro->stack[ro->level].page, sizeof(CDX_PAGE), leaf, __PROC__)))
		  return er;
	    }

	   if (_rdd_ushort(leaf->nkeys) == 0)
	      return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_nokeys);
	   ro->stack[ro->level].pos = 0;
	}
       else
	{
	   *out = 1;
	}
    }
#ifndef D20070109
   if (*out != 1)
    {
       int       recno;

       _cdx_leaf_item(ro, leaf, ro->stack[ro->level].pos, &recno, ro->key);
    }
#endif
   return 0;
}

static int
_cdx_next(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, int *out, unsigned int *rec, const char *__PROC__)
{
   CDX_LEAF *leaf = ro->curpage;

   unsigned int recno;

   int       er;

   *out = 0;
   if ((er = _cdx_loadpage(ClipMachineMemory, ro, ro->stack[ro->level].page, __PROC__)))
      return er;
   if (!(_rdd_ushort(leaf->attrs) & 2))
      return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_badleaf);

   if ((er = _cdx_next_rm(ClipMachineMemory, ro, leaf, out, 1, __PROC__)))
      return er;
   if (rd->filter && rd->filter->rmap)
    {
       recno = *(unsigned int *) (leaf->keys + (ro->stack[ro->level].pos) * leaf->bytes) & _rdd_uint(leaf->recmask);
       while (!(*out) && !_rm_getbit(rd->filter->rmap, rd->filter->size, recno))
	{
	   if ((er = _cdx_next_rm(ClipMachineMemory, ro, leaf, out, 1, __PROC__)))
	      return er;
	   recno = *(unsigned int *) (leaf->keys + (ro->stack[ro->level].pos) * leaf->bytes) & _rdd_uint(leaf->recmask);
	}
    }
   if (rec)
      *rec = *(unsigned int *) (leaf->keys + (ro->stack[ro->level].pos) * leaf->bytes) & _rdd_uint(leaf->recmask);
   return 0;
}

static int
_cdx_prev_rm(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, CDX_LEAF * leaf, int *out, int cache, const char *__PROC__)
{
   int       er;

   *out = 0;
   if (!leaf)
      return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_badleaf);

   ro->stack[ro->level].pos--;
   if (ro->stack[ro->level].pos < 0)
    {
       if (_rdd_uint(leaf->left) != 0xffffffff)
	{
	   ro->stack[ro->level].page = _rdd_uint(leaf->left);
	   if (cache)
	    {
	       if ((er = _cdx_loadpage(ClipMachineMemory, ro, ro->stack[ro->level].page, __PROC__)))
		  return er;
	    }
	   else
	    {
	       if ((er =
		    rdd_read(ClipMachineMemory, &ro->index->file, ro->stack[ro->level].page, sizeof(CDX_PAGE), leaf, __PROC__)))
		  return er;
	    }

	   if (_rdd_ushort(leaf->nkeys) == 0)
	      return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_nokeys);
	   ro->stack[ro->level].pos = _rdd_ushort(leaf->nkeys) - 1;
	}
       else
	{
	   *out = 1;
	   ro->stack[ro->level].pos = 0;
	}
    }
#ifndef D20070109
   if (*out != 1)
    {
       int       recno;

       _cdx_leaf_item(ro, leaf, ro->stack[ro->level].pos, &recno, ro->key);
    }
#endif
   return 0;
}

static int
_cdx_prev(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, int *out, unsigned int *rec, const char *__PROC__)
{
   CDX_LEAF *leaf = ro->curpage;

   unsigned int recno;

   int       er;

   *out = 0;
   if ((er = _cdx_loadpage(ClipMachineMemory, ro, ro->stack[ro->level].page, __PROC__)))
      return er;
   if (!(_rdd_ushort(leaf->attrs) & 2))
      return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_badleaf);

   if ((er = _cdx_prev_rm(ClipMachineMemory, ro, leaf, out, 1, __PROC__)))
      return er;
   if (rd->filter && rd->filter->rmap)
    {
       recno = *(unsigned int *) (leaf->keys + (ro->stack[ro->level].pos) * leaf->bytes) & _rdd_uint(leaf->recmask);
       while (!(*out) && !_rm_getbit(rd->filter->rmap, rd->filter->size, recno))
	{
	   if ((er = _cdx_prev_rm(ClipMachineMemory, ro, leaf, out, 1, __PROC__)))
	      return er;
	   recno = *(unsigned int *) (leaf->keys + (ro->stack[ro->level].pos) * leaf->bytes) & _rdd_uint(leaf->recmask);
	}
    }
   if (rec)
      *rec = *(unsigned int *) (leaf->keys + (ro->stack[ro->level].pos) * leaf->bytes) & _rdd_uint(leaf->recmask);
   return 0;
}

static int
_cdx_checkscope(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, void *key, int *ok, const char *__PROC__)
{
   int       lok = 1, hok = 1;

   *ok = 0;
   if (!ro->scopetop && !ro->scopebottom)
      return 0;

   if (ro->scopetop)
    {
       int       r;

       r = _cdx_cmp(ro, key, (unsigned char *) ro->scopetop, ro->stoplen);
       lok = (ro->descend ? (r <= 0) : (r >= 0));
    }
   if (ro->scopebottom)
    {
       int       r;

       r = _cdx_cmp(ro, key, (unsigned char *) ro->scopebottom, ro->sbotlen);
       hok = (ro->descend ? (r >= 0) : (r <= 0));
    }

   if (lok && hok)
      *ok = 0;
   else if (!lok)
      *ok = -1;
   else
      *ok = 1;

   return 0;
}

static int
_cdx_keyno(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, unsigned int npage,
	   int pos, unsigned int *keyno, const char *__PROC__)
{
   CDX_PAGE  page;

   int       er;

   if (!ro->descend)
    {
       if ((er = rdd_read(ClipMachineMemory, &ro->index->file, npage, 12, &page, __PROC__)))
	  return er;
       npage = _rdd_uint(page.left);
       *keyno = pos + 1;
       while (npage != 0xffffffff)
	{
	   if ((er = rdd_read(ClipMachineMemory, &ro->index->file, npage, 12, &page, __PROC__)))
	      return er;
	   npage = _rdd_uint(page.left);
	   *keyno += _rdd_ushort(page.nkeys);
	}
    }
   else
    {
       if ((er = rdd_read(ClipMachineMemory, &ro->index->file, npage, 12, &page, __PROC__)))
	  return er;
       npage = _rdd_uint(page.right);
       *keyno = _rdd_ushort(page.nkeys) - pos;
       while (npage != 0xffffffff)
	{
	   if ((er = rdd_read(ClipMachineMemory, &ro->index->file, npage, 12, &page, __PROC__)))
	      return er;
	   npage = _rdd_uint(page.right);
	   *keyno += _rdd_ushort(page.nkeys);
	}
    }
   return 0;
}

static int
_cdx_dupbytes(void *key1, void *key2, int len)
{
   int       i;

   int      *k1, *k2;

   int       l = len / sizeof(int);

   int       o = len % sizeof(int);

   for (i = 0, k1 = key1, k2 = key2; i < l && *k1 == *k2 && *k1; i++, k1++, k2++)
      ;
   o += (l - i) * sizeof(int);
   if (o || i < l)
    {
       i = i << 2;
       if (o == 1)
	{
	   i += ((*(char *) k1 == *(char *) k2) && (*(char *) k1));
	   return i;
	}
       if ((*(short *) k1 == *(short *) k2) && (*(short *) k1))
	{
	   i += 2;
	   if (o > 2)
	      i += (*(((char *) k1) + 2) == *(((char *) k2) + 2) && (*(char *) k1));
	}
       else
	{
	   i += ((*(char *) k1 == *(char *) k2) && (*(char *) k1));
	}
       return i;
    }
   return len;
}

static int
_cdx_leaf2split(RDD_ORDER * ro, CDX_LEAF * leaf, void *keys, int pos, void *key, int len, unsigned int recno, int *addbytes)
{
   int       free = _rdd_ushort(leaf->free);

   int       dup, trail = 0;

   *addbytes = 0;
   free -= leaf->bytes;
   if ((recno & _rdd_uint(leaf->recmask)) != recno)
    {
       int       i = 0;

       while (_cdx_bits[i++] < recno);
       i -= leaf->recbits;
       if (i % 8)
	  i = i / 8 + 1;
       else
	  i = i / 8;
       free -= (_rdd_ushort(leaf->nkeys) + 1) * i;
       *addbytes = i;
    }
   if (free < 0)
      return free;

  /* free += */
   if (pos < _rdd_ushort(leaf->nkeys))
    {
       _cdx_dup_trail_rec(leaf, pos, &dup, &trail, NULL);
       free += ro->bufsize - (dup + trail);
    }
  /* free -= */
   if (pos > 0)
      free -= len - _cdx_dupbytes(keys + (pos - 1) * ro->bufsize, key, len);
   else
      free -= len;
   if (pos < _rdd_ushort(leaf->nkeys))
      free -= (ro->bufsize - trail) - _cdx_dupbytes(keys + pos * ro->bufsize, key, len);

   return free;
}

static void
_cdx_loadkeys(RDD_ORDER * ro, CDX_LEAF * leaf, void *keys)
{
   unsigned long long item;

   unsigned int rec;

   int       dup, trail;

   char     *p = (char *) leaf + sizeof(CDX_LEAF);

   int       i;

   for (i = 0; i < _rdd_ushort(leaf->nkeys); i++)
    {
       memcpy(&item, leaf->keys + i * leaf->bytes, leaf->bytes);
       _CDX_LEAF_ITEM(item, rec, dup, trail);
       p -= ro->bufsize - (dup + trail);
       if (i)
	  memcpy(keys + i * ro->bufsize, keys + (i - 1) * ro->bufsize, ro->bufsize);
       memcpy(keys + i * ro->bufsize + dup, p, ro->bufsize - (dup + trail));
       memset(keys + i * ro->bufsize + dup + ro->bufsize - (dup + trail), 0x0, trail);
    }
}

static void
_cdx_addnode(RDD_ORDER * ro, CDX_LEAF * leaf, void *keys, int pos, void *key, int len, unsigned int recno, int addbytes, int fr)
{
   int       i;

   int       dup, trail;

   unsigned long long item;

  /*if(addbytes) */
   {
      int      *lens = malloc((_rdd_ushort(leaf->nkeys) + 1) * sizeof(int));

      unsigned int *recnos = malloc((_rdd_ushort(leaf->nkeys) + 1) * sizeof(int));

      int      *dups = malloc((_rdd_ushort(leaf->nkeys) + 1) * sizeof(int));

      void     *p;

      for (i = 0; i < pos; i++)
       {
	  memcpy(&item, leaf->keys + i * leaf->bytes, leaf->bytes);
	  _CDX_LEAF_ITEM(item, recnos[i], dup, trail);
	  lens[i] = ro->bufsize - trail;
	  dups[i] = dup;
       }
      recnos[i] = recno;
      lens[i] = len;
      dups[i] = 0;
      for (i = pos; i < _rdd_ushort(leaf->nkeys); i++)
       {
	  memcpy(&item, leaf->keys + i * leaf->bytes, leaf->bytes);
	  _CDX_LEAF_ITEM(item, recnos[i + 1], dup, trail);
	  lens[i + 1] = ro->bufsize - trail;
	  dups[i + 1] = dup;
       }
      memmove(keys + (pos + 1) * ro->bufsize, keys + pos * ro->bufsize, (_rdd_ushort(leaf->nkeys) - pos) * ro->bufsize);
      memcpy(keys + pos * ro->bufsize, key, len);
      memset(keys + pos * ro->bufsize + len, 0, ro->bufsize - len);

      if (addbytes)
       {
	  unsigned int bits = 1;

	  leaf->bytes += addbytes;
	  leaf->recbits += addbytes * 8;
	  for (i = 1; i < leaf->recbits; i++)
	     bits = (bits << 1) + 1;
	  _rdd_put_uint(leaf->recmask, bits);
       }

      memset(leaf->keys, 0, sizeof(leaf->keys));
      _rdd_put_ushort(leaf->nkeys, _rdd_ushort(leaf->nkeys) + 1);
      _rdd_put_ushort(leaf->free, fr);
      p = (void *) leaf + sizeof(CDX_PAGE);
      for (i = 0; i < _rdd_ushort(leaf->nkeys); i++)
       {
	  trail = ro->bufsize - lens[i];
	  item = trail;
	  item = item << leaf->dupbits;
	  if (i == 0)
	   {
	      dup = 0;
	   }
	  else
	   {
	      if (i == pos || i == pos + 1)
		 dup = _cdx_dupbytes(keys + i * ro->bufsize, keys + (i - 1) * ro->bufsize, min(lens[i - 1], lens[i]));
	      else
		 dup = dups[i];
	      item += dup;
	   }
	  item = item << leaf->recbits;
	  item += recnos[i];
	  memcpy(leaf->keys + i * leaf->bytes, &item, leaf->bytes);
	  p -= ro->bufsize - (dup + trail);
	  memcpy(p, keys + i * ro->bufsize + dup, ro->bufsize - (dup + trail));
       }
      free(lens);
      free(recnos);
      free(dups);
   }				/* else {
				   unsigned int rec;
				   int l,db;
				   void* val = malloc(ro->bufsize);
				   void* be = (void*)leaf+sizeof(CDX_PAGE);
				   void* ab;

				   for(i=0;i<pos;i++){
				   item = *(unsigned long long*)(leaf->keys+i*leaf->bytes);
				   _CDX_LEAF_ITEM(item,rec,dup,trail);
				   l = ro->bufsize - (dup+trail);
				   be -= l;
				   memcpy(val+dup,be,l);
				   }
				   item = *(unsigned long long*)(leaf->keys+i*leaf->bytes);
				   _CDX_LEAF_ITEM(item,rec,dup,trail);
				   ab = pos==_rdd_ushort(leaf->nkeys)?0:be-(ro->bufsize - (dup+trail));
				   memmove(leaf->keys+(pos+1)*leaf->bytes,leaf->keys+pos*leaf->bytes,
				   (_rdd_ushort(leaf->nkeys)-pos)*leaf->bytes);
				   db = _cdx_dupbytes(key,val,min(len,ro->bufsize-trail));
				   if(ab)
				   memmove(be-(len-db),ab,
				   (int)(ab-(void*)leaf->keys)-
				   (_rdd_ushort(leaf->nkeys)+1)*leaf->bytes);
				   memcpy(be-(len-db),key+db,len-db);
				   item = ro->bufsize-len;
				   item = item << leaf->dupbits;
				   item += db;
				   item = item << leaf->recbits;
				   item += recno;
				   memcpy(leaf->keys+pos*leaf->bytes,&item,leaf->bytes);
				   _rdd_put_ushort(leaf->nkeys,_rdd_ushort(leaf->nkeys)+1);
				   _rdd_put_ushort(leaf->free,fr);
				   free(val);
				   } */
}

static int
_cdx_splitleaf(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, CDX_LEAF * old,
	       CDX_LEAF * new, unsigned int oldpage, unsigned int newpage, void *keys, const char *__PROC__)
{
   int      *lens = malloc(_rdd_ushort(old->nkeys) * sizeof(int));

   unsigned int *recnos = malloc(_rdd_ushort(old->nkeys) * sizeof(unsigned int));

   int       dup, trail, i, er, fr;

   unsigned long long item;

   void     *p;

   int       c1 = _rdd_ushort(old->nkeys) / 2;

   int       c2 = _rdd_ushort(old->nkeys) - c1;

   for (i = 0; i < _rdd_ushort(old->nkeys); i++)
    {
       _cdx_dup_trail_rec(old, i, &dup, &trail, (int *) (recnos + i));
       lens[i] = ro->bufsize - trail;
    }

   _rdd_put_ushort(old->attrs, 2);
   _rdd_put_ushort(new->attrs, 2);
   _rdd_put_uint(new->left, oldpage);
   _rdd_put_uint(new->right, _rdd_uint(old->right));
   _rdd_put_uint(old->right, newpage);
   if (_rdd_uint(new->right) != 0xffffffff)
    {
       if ((er = rdd_write(ClipMachineMemory, &ro->index->file, _rdd_uint(new->right) + 4, 4, &newpage, __PROC__)))
	  goto err;
    }
   _rdd_put_ushort(old->nkeys, c1);
   _rdd_put_ushort(new->nkeys, c2);
   memcpy(new->recmask, old->recmask, 10);	/* masks, bits, bytes ... */

   memset(old->keys, 0, sizeof(old->keys));
   fr = sizeof(old->keys);
   p = (void *) old + sizeof(CDX_PAGE);
   for (i = 0; i < c1; i++)
    {
       trail = ro->bufsize - lens[i];
       item = trail;
       item = item << old->dupbits;
       if (i == 0)
	{
	   dup = 0;
	}
       else
	{
	   dup = _cdx_dupbytes(keys + i * ro->bufsize, keys + (i - 1) * ro->bufsize, lens[i - 1]);
	   item += dup;
	}
       item = item << old->recbits;
       item += recnos[i];
       memcpy(old->keys + i * old->bytes, &item, old->bytes);
       p -= ro->bufsize - (dup + trail);
       memcpy(p, keys + i * ro->bufsize + dup, ro->bufsize - (dup + trail));
       fr -= old->bytes + ro->bufsize - (dup + trail);
    }
   _rdd_put_ushort(old->free, fr);

   memset(new->keys, 0, sizeof(new->keys));
   fr = sizeof(new->keys);
   p = (void *) new + sizeof(CDX_PAGE);
   for (; i < c1 + c2; i++)
    {
       trail = ro->bufsize - lens[i];
       item = trail;
       item = item << new->dupbits;
       if (i == c1)
	{
	   dup = 0;
	}
       else
	{
	   dup = _cdx_dupbytes(keys + i * ro->bufsize, keys + (i - 1) * ro->bufsize, lens[i - 1]);
	   item += dup;
	}
       item = item << new->recbits;
       item += recnos[i];
       memcpy(new->keys + (i - c1) * new->bytes, &item, new->bytes);
       p -= ro->bufsize - (dup + trail);
       memcpy(p, keys + i * ro->bufsize + dup, ro->bufsize - (dup + trail));
       fr -= new->bytes + ro->bufsize - (dup + trail);
    }
   _rdd_put_ushort(new->free, fr);
   free(lens);
   free(recnos);
   return 0;
 err:
   free(lens);
   free(recnos);
   return er;
}

static int
_cdx_addroot(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, unsigned int brpage,
	     void *key1, unsigned int rec1, unsigned int page1, void *key2,
	     unsigned int rec2, unsigned int page2, const char *__PROC__)
{
   CDX_BRANCH root;

   char      b[4];

   int       er;

   memset(&root, 0, sizeof(CDX_BRANCH));

   _rdd_put_ushort(root.attrs, 1);
   _rdd_put_ushort(root.nkeys, 2);
   _rdd_put_uint(root.left, 0xffffffff);
   _rdd_put_uint(root.right, 0xffffffff);

   memcpy(root.keys, key1, ro->bufsize);
   _rdd_put_backuint(root.keys + ro->bufsize, rec1);
   _rdd_put_backuint(root.keys + ro->bufsize + 4, page1);

   memcpy(root.keys + ro->bufsize + 8, key2, ro->bufsize);
   _rdd_put_backuint(root.keys + (ro->bufsize + 8) + ro->bufsize, rec2);
   _rdd_put_backuint(root.keys + (ro->bufsize + 8) + ro->bufsize + 4, page2);

   if ((er = rdd_write(ClipMachineMemory, &ro->index->file, brpage, sizeof(CDX_BRANCH), &root, __PROC__)))
      return er;

   _rdd_put_uint((unsigned char *) b, brpage);
   if ((er = rdd_write(ClipMachineMemory, &ro->index->file, ro->header, sizeof(unsigned int), b, __PROC__)))
      return er;
   return 0;
}

static int
_cdx_adjust_tree(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, int level, void *key, unsigned int rec, const char *__PROC__)
{
   CDX_PAGE  page;

   CDX_LEAF *leaf;

   CDX_BRANCH *branch;

   int       s, er;

   _CDX_PAGE(ro, ro->stack[level].page, &page);
   s = ro->stack[level].pos * (ro->bufsize + 8);
   memcpy(branch->keys + s, key, ro->bufsize);
   _rdd_put_backuint(branch->keys + s + ro->bufsize, rec);
   if ((er = rdd_write(ClipMachineMemory, &ro->index->file, ro->stack[level].page, sizeof(CDX_BRANCH), branch, __PROC__)))
      return er;
   if (level && (ro->stack[level].pos == _rdd_ushort(branch->nkeys) - 1))
    {
       if ((er = _cdx_adjust_tree(ClipMachineMemory, ro, level - 1,
				  branch->keys +
				  ro->stack[level].pos * (ro->bufsize + 8),
				  _rdd_backuint(branch->keys +
						ro->stack[level].pos * (ro->bufsize + 8) + ro->bufsize), __PROC__)))
	  return er;
    }
   return 0;
}

static int
_cdx_addstick(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, int level,
	      void *key1, unsigned int rec1, unsigned int page1, void *key2,
	      unsigned int rec2, unsigned int page2, const char *__PROC__)
{
   CDX_PAGE  page;

   CDX_LEAF *leaf;

   CDX_BRANCH *branch;

   int       s = (ro->bufsize + 8) * ro->stack[level].pos;

   int       fr, er;

   int       i = ro->stack[level].pos + 1;

   _CDX_PAGE(ro, ro->stack[level].page, &page);
   memcpy(branch->keys + s, key1, ro->bufsize);
   _rdd_put_backuint(branch->keys + s + ro->bufsize, rec1);
   _rdd_put_backuint(branch->keys + s + ro->bufsize + 4, page1);

   fr = sizeof(branch->keys) - (ro->bufsize + 8) * _rdd_ushort(branch->nkeys);

   if (fr > ro->bufsize + 8)
    {
      /* add */
       memmove(branch->keys + (ro->bufsize + 8) * (i + 1),
	       branch->keys + (ro->bufsize + 8) * i, (_rdd_ushort(branch->nkeys) - i) * (ro->bufsize + 8));
       memcpy(branch->keys + (ro->bufsize + 8) * i, key2, ro->bufsize);
       _rdd_put_backuint(branch->keys + (ro->bufsize + 8) * i + ro->bufsize, rec2);
       _rdd_put_backuint(branch->keys + (ro->bufsize + 8) * i + ro->bufsize + 4, page2);
       _rdd_put_ushort(branch->nkeys, _rdd_ushort(branch->nkeys) + 1);
       if ((er = rdd_write(ClipMachineMemory, &ro->index->file, ro->stack[level].page, sizeof(CDX_BRANCH), branch, __PROC__)))
	  return er;
       if (level && (i == _rdd_ushort(branch->nkeys) - 1))
	{
	   if ((er = _cdx_adjust_tree(ClipMachineMemory, ro, level - 1, key2, rec2, __PROC__)))
	      return er;
	}
    }
   else
    {
       CDX_BRANCH new;

       unsigned int newpage;

       void     *keys = malloc((_rdd_ushort(branch->nkeys) + 1) * ro->bufsize);

       unsigned int *recnos = malloc((_rdd_ushort(branch->nkeys) + 1) * sizeof(unsigned int));

       unsigned int *pages = malloc((_rdd_ushort(branch->nkeys) + 1) * sizeof(unsigned int));

       int       j;

       int       c1 = (_rdd_ushort(branch->nkeys) + 1) / 2;

       int       c2 = (_rdd_ushort(branch->nkeys) + 1) - c1;

       if ((er = _cdx_getfreepage(ClipMachineMemory, ro->index, &newpage, __PROC__)))
	  goto err1;

       for (j = 0; j < _rdd_ushort(branch->nkeys); j++)
	{
	   s = j * (ro->bufsize + 8);
	   memcpy(keys + j * ro->bufsize, branch->keys + s, ro->bufsize);
	   recnos[j] = _rdd_backuint(branch->keys + s + ro->bufsize);
	   pages[j] = _rdd_backuint(branch->keys + s + ro->bufsize + 4);
	}
       memmove(keys + (i + 1) * ro->bufsize, keys + i * ro->bufsize, (_rdd_ushort(branch->nkeys) - i) * ro->bufsize);
       memmove(&recnos[i + 1], &recnos[i], (_rdd_ushort(branch->nkeys) - i) * sizeof(unsigned int));
       memmove(&pages[i + 1], &pages[i], (_rdd_ushort(branch->nkeys) - i) * sizeof(unsigned int));

       memcpy(keys + i * ro->bufsize, key2, ro->bufsize);
       recnos[i] = rec2;
       pages[i] = page2;

       memset(branch->keys, 0, sizeof(branch->keys));
       memset(new.keys, 0, sizeof(new.keys));
       _rdd_put_ushort(branch->attrs, 0);
       _rdd_put_ushort(new.attrs, 0);
       _rdd_put_ushort(branch->nkeys, c1);
       _rdd_put_ushort(new.nkeys, c2);
       _rdd_put_uint(new.right, _rdd_uint(branch->right));
       _rdd_put_uint(branch->right, newpage);
       _rdd_put_uint(new.left, ro->stack[level].page);
       if (_rdd_uint(new.right) != 0xffffffff)
	{
	   if ((er = rdd_write(ClipMachineMemory, &ro->index->file, _rdd_uint(new.right) + 4, 4, &newpage, __PROC__)))
	      goto err1;
	}
       for (j = 0; j < c1; j++)
	{
	   s = j * (ro->bufsize + 8);
	   memcpy(branch->keys + s, keys + j * ro->bufsize, ro->bufsize);
	   _rdd_put_backuint(branch->keys + s + ro->bufsize, recnos[j]);
	   _rdd_put_backuint(branch->keys + s + ro->bufsize + 4, pages[j]);
	}
       for (; j < c1 + c2; j++)
	{
	   s = (j - c1) * (ro->bufsize + 8);
	   memcpy(new.keys + s, keys + j * ro->bufsize, ro->bufsize);
	   _rdd_put_backuint(new.keys + s + ro->bufsize, recnos[j]);
	   _rdd_put_backuint(new.keys + s + ro->bufsize + 4, pages[j]);
	}
       if ((er = rdd_write(ClipMachineMemory, &ro->index->file, ro->stack[level].page, sizeof(CDX_BRANCH), branch, __PROC__)))
	  goto err1;
       if ((er = rdd_write(ClipMachineMemory, &ro->index->file, newpage, sizeof(CDX_BRANCH), &new, __PROC__)))
	  goto err1;

       if (level == 0)
	{
	   unsigned int brpage;

	   if ((er = _cdx_getfreepage(ClipMachineMemory, ro->index, &brpage, __PROC__)))
	      goto err1;

	   if ((er = _cdx_addroot(ClipMachineMemory, ro, brpage,
				  branch->keys + (c1 - 1) * (ro->bufsize + 8),
				  _rdd_backuint(branch->keys +
						(c1 - 1) * (ro->bufsize +
							    8) +
						ro->bufsize),
				  ro->stack[level].page,
				  new.keys + (c2 - 1) * (ro->bufsize + 8),
				  _rdd_backuint(new.keys + (c2 - 1) * (ro->bufsize + 8) + ro->bufsize), newpage, __PROC__)))
	      goto err1;
	}
       else
	{
	   if ((er = _cdx_addstick(ClipMachineMemory, ro, level - 1,
				   branch->keys + (c1 - 1) * (ro->bufsize +
							      8),
				   _rdd_backuint(branch->keys +
						 (c1 - 1) * (ro->bufsize +
							     8) +
						 ro->bufsize),
				   ro->stack[level].page,
				   new.keys + (c2 - 1) * (ro->bufsize + 8),
				   _rdd_backuint(new.keys + (c2 - 1) * (ro->bufsize + 8) + ro->bufsize), newpage, __PROC__)))
	      goto err1;
	}
       free(keys);
       free(recnos);
       free(pages);
       goto cont;
     err1:
       free(keys);
       free(recnos);
       free(pages);
       return er;
    }
 cont:
   return 0;
}

static int
__cdx_addkey(ClipMachine * ClipMachineMemory, int area, RDD_ORDER * ro,
	     ClipVar * v, DbfLocale * loc, unsigned int recno, const char *__PROC__)
{
   CDX_HEADER hdr;

   int       found;

   int       len = ro->bufsize;

   int       er;

   char     *newkey;

   ro->level = 0;
   if (ro->bforexpr.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
    {
       ClipVar   vv, *vp;

       if ((er = rdd_calc(ClipMachineMemory, area, &ro->bforexpr, &vv, 0)))
	  return er;
       vp = _clip_vptr(&vv);
       if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
	  return rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_badforexpr);
       if (!vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
	{
	   _clip_destroy(ClipMachineMemory, &vv);
	   return 0;
	}
       _clip_destroy(ClipMachineMemory, &vv);
    }
   newkey = malloc(ro->keysize);
   if (v)
    {
       if ((er = cdx_formatkey(ClipMachineMemory, ro, v, newkey, __PROC__)))
	  return er;
       if (v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
	{
	   len =
	    min(v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, ro->bufsize - (ro->type == 'X' ? 2 : 0));
	}
       else if (ro->type != 'L')
	{
	   char     *s = newkey + ro->bufsize - 1;

	   while ((s >= newkey) && (*s == 0))
	      s--;
	   len = s - (char *) newkey + 1;
	}
    }
   else
    {
       ClipVar   vv, *vp;

       if ((er = rdd_calc(ClipMachineMemory, area, &ro->block, &vv, 0)))
	  goto err1;
       vp = _clip_vptr(&vv);
       if ((er = cdx_formatkey(ClipMachineMemory, ro, vp, newkey, __PROC__)))
	  goto err1;
       _clip_destroy(ClipMachineMemory, &vv);
    }

   _CDX_HEADER(ro, &hdr);
   ro->stack[0].page = _rdd_uint(hdr.root);

  /* Note: _cdx_back_search_tree()'s up ro->levels of ro->stack may be incorrect */
   if (ro->unique)
    {
       if ((er = _cdx_search_tree(ClipMachineMemory, ro, newkey, ro->bufsize, 0, &found, NULL, __PROC__)))
	  goto err1;
       if (found)
	  goto cont;
    }
   ro->level = 0;
   if ((er = _cdx_search_tree(ClipMachineMemory, ro, newkey, ro->bufsize, recno, &found, NULL, __PROC__)))
      goto err1;
   if (found)
      goto cont;
   if (!v)
    {
       if (ro->type == 'C')
	{
	   char     *s = newkey + ro->bufsize - 1;

	   if (ro->binary)
	      while (*s == 0)
		 (s--, len--);
	   else
	      while (*s == ' ')
		 (s--, len--);
	   if (s != newkey + ro->bufsize - 1)
	      *(s + 1) = 0;
	}
       else if (ro->type != 'L')
	{
	   char     *s = newkey + ro->bufsize - 1;

	   while ((s >= newkey) && (*s == 0))
	      s--;
	   len = s - (char *) newkey + 1;
	}
    }
   if ((ro->type == 'C' || (ro->type == 'X' && newkey[0] == type_weight(CHARACTER_type_of_ClipVarType))) && !ro->binary)
      loc_write(loc, (unsigned char *) newkey, len);
   er = _cdx_addkey(ClipMachineMemory, area, loc, v, ro, newkey, len, recno, __PROC__);
 err1:
   free(newkey);
   return er;
 cont:
   free(newkey);
   return 0;
}

static int
_cdx_addkey(ClipMachine * ClipMachineMemory, int area, DbfLocale * loc,
	    ClipVar * v, RDD_ORDER * ro, void *key, int len, unsigned int recno, const char *__PROC__)
{
   CDX_PAGE  page;

   CDX_LEAF *leaf;

   CDX_BRANCH *branch;

   int       addbytes;

   void     *keys = NULL;

   int       fr, er;

   _CDX_PAGE(ro, ro->stack[ro->level].page, &page);
   keys = malloc(ro->bufsize * (_rdd_ushort(leaf->nkeys) + 1));
   _cdx_loadkeys(ro, leaf, keys);
   if ((fr = _cdx_leaf2split(ro, leaf, keys, ro->stack[ro->level].pos, key, len, recno, &addbytes)) <= 0)
    {

       CDX_LEAF  newleaf;

       unsigned int newpage;

       unsigned int brpage;

       void     *key1 = malloc(ro->bufsize);

       void     *key2 = malloc(ro->bufsize);

       unsigned int rec1, rec2;

       if ((er = _cdx_getfreepage(ClipMachineMemory, ro->index, &newpage, __PROC__)))
	  goto err;

       if ((er = _cdx_splitleaf(ClipMachineMemory, ro, leaf, &newleaf, ro->stack[ro->level].page, newpage, keys, __PROC__)))
	  goto err;
       if ((er = rdd_write(ClipMachineMemory, &ro->index->file, ro->stack[ro->level].page, sizeof(CDX_LEAF), leaf, __PROC__)))
	  goto err;
       if ((er = rdd_write(ClipMachineMemory, &ro->index->file, newpage, sizeof(CDX_LEAF), &newleaf, __PROC__)))
	  goto err;

       _cdx_leaf_item(ro, leaf, _rdd_ushort(leaf->nkeys) - 1, &rec1, key1);
       _cdx_leaf_item(ro, &newleaf, _rdd_ushort(newleaf.nkeys) - 1, &rec2, key2);
       if (ro->level == 0)
	{
	   if ((er = _cdx_getfreepage(ClipMachineMemory, ro->index, &brpage, __PROC__)))
	      goto err1;
	   if ((er =
		_cdx_addroot(ClipMachineMemory, ro, brpage, key1, rec1,
			     ro->stack[ro->level].page, key2, rec2, newpage, __PROC__)))
	      goto err1;
	}
       else
	{
	   brpage = ro->stack[ro->level - 1].page;

	   if ((er =
		_cdx_addstick(ClipMachineMemory, ro, ro->level - 1, key1, rec1,
			      ro->stack[ro->level].page, key2, rec2, newpage, __PROC__)))
	      goto err1;
	}
       free(key1);
       free(key2);
       if ((er = __cdx_addkey(ClipMachineMemory, area, ro, v, loc, recno, __PROC__)))
	  goto err;
       goto cont;
     err1:
       free(key1);
       free(key2);
       goto err;
    }
   else
    {
       _cdx_addnode(ro, leaf, keys, ro->stack[ro->level].pos, key, len, recno, addbytes, fr);
       if ((er = rdd_write(ClipMachineMemory, &ro->index->file, ro->stack[ro->level].page, sizeof(CDX_LEAF), leaf, __PROC__)))
	  goto err;

       if (ro->level && (ro->stack[ro->level].pos == _rdd_ushort(leaf->nkeys) - 1))
	{
	   void     *key = malloc(ro->bufsize);

	   unsigned int rec;

	   _cdx_leaf_item(ro, leaf, _rdd_ushort(leaf->nkeys) - 1, &rec, key);
	   if ((er = _cdx_adjust_tree(ClipMachineMemory, ro, ro->level - 1, key, rec, __PROC__)))
	    {
	       free(key);
	       goto err;
	    }
	   free(key);
	}
    }
 cont:
   free(keys);
   return 0;
 err:
   if (keys)
      free(keys);
   return er;
}

static void
_cdx_delnode(RDD_ORDER * ro, CDX_LEAF * leaf, int pos)
{
   void     *keys = malloc(_rdd_ushort(leaf->nkeys) * ro->bufsize);

   unsigned int *recnos = malloc(_rdd_ushort(leaf->nkeys) * sizeof(unsigned int));

   unsigned int *lens = malloc(_rdd_ushort(leaf->nkeys) * sizeof(unsigned int));

   int       dup, trail;

   unsigned long long item;

   void     *p;

   int       i, fr;

   _cdx_loadkeys(ro, leaf, keys);

   for (i = 0; i < _rdd_ushort(leaf->nkeys); i++)
    {
       _cdx_dup_trail_rec(leaf, i, &dup, &trail, (int *) (recnos + i));
       lens[i] = ro->bufsize - trail;
    }
   memmove(keys + pos * ro->bufsize, keys + (pos + 1) * ro->bufsize, (_rdd_ushort(leaf->nkeys) - pos - 1) * ro->bufsize);
   memmove(&recnos[pos], &recnos[pos + 1], (_rdd_ushort(leaf->nkeys) - pos - 1) * sizeof(unsigned int));
   memmove(&lens[pos], &lens[pos + 1], (_rdd_ushort(leaf->nkeys) - pos - 1) * sizeof(unsigned int));

   _rdd_put_ushort(leaf->nkeys, _rdd_ushort(leaf->nkeys) - 1);
   memset(leaf->keys, 0, sizeof(leaf->keys));
   fr = sizeof(leaf->keys);
   p = (void *) leaf + sizeof(CDX_PAGE);
   for (i = 0; i < _rdd_ushort(leaf->nkeys); i++)
    {
       trail = ro->bufsize - lens[i];
       item = trail;
       item = item << leaf->dupbits;
       if (i == 0)
	{
	   dup = 0;
	}
       else
	{
	   dup = _cdx_dupbytes(keys + i * ro->bufsize, keys + (i - 1) * ro->bufsize, lens[i - 1]);
	   item += dup;
	}
       item = item << leaf->recbits;
       item += recnos[i];
       memcpy(leaf->keys + i * leaf->bytes, &item, leaf->bytes);
       p -= ro->bufsize - (dup + trail);
       memcpy(p, keys + i * ro->bufsize + dup, ro->bufsize - (dup + trail));
       fr -= leaf->bytes + ro->bufsize - (dup + trail);
    }
   _rdd_put_ushort(leaf->free, fr);
   free(keys);
   free(recnos);
   free(lens);
}

static int
_cdx_delstick(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, int level, const char *__PROC__)
{
   CDX_PAGE  page;

   CDX_LEAF *leaf;

   CDX_BRANCH *branch;

   int       er;

   _CDX_PAGE(ro, ro->stack[level].page, &page);
   if (_rdd_ushort(branch->nkeys) == 1)
    {
       if (_rdd_uint(branch->left) != 0xffffffff)
	{
	   if ((er = rdd_write(ClipMachineMemory, &ro->index->file, _rdd_uint(branch->left) + 8, 4, branch->right, __PROC__)))
	      return er;
	}
       if (_rdd_uint(branch->right) != 0xffffffff)
	{
	   if ((er = rdd_write(ClipMachineMemory, &ro->index->file, _rdd_uint(branch->right) + 4, 4, branch->left, __PROC__)))
	      return er;
	}
       if (level > 0)
	{
	   if ((er = _cdx_putfreepage(ClipMachineMemory, ro, ro->stack[level].page, __PROC__)))
	      return er;
	   if ((er = _cdx_delstick(ClipMachineMemory, ro, level - 1, __PROC__)))
	      return er;
	}
       else
	{
	   if ((er = _cdx_init_rootleaf(ClipMachineMemory, ro, ro->stack[level].page, __PROC__)))
	      return er;
	}
    }
   else
    {
       memmove(branch->keys + ro->stack[level].pos * (ro->bufsize + 8),
	       branch->keys + (ro->stack[level].pos + 1) * (ro->bufsize + 8),
	       (_rdd_ushort(branch->nkeys) - ro->stack[level].pos - 1) * (ro->bufsize + 8));
       memset(branch->keys + (_rdd_ushort(branch->nkeys) - 1) * (ro->bufsize + 8), 0, ro->bufsize + 8);
       _rdd_put_ushort(branch->nkeys, _rdd_ushort(branch->nkeys) - 1);
       if ((er = rdd_write(ClipMachineMemory, &ro->index->file, ro->stack[level].page, sizeof(CDX_BRANCH), branch, __PROC__)))
	  return er;
       if (level && (ro->stack[level].pos == _rdd_ushort(branch->nkeys)))
	{
	   int       s = (_rdd_ushort(branch->nkeys) - 1) * (ro->bufsize + 8);

	   unsigned int rec = _rdd_backuint(branch->keys + s + ro->bufsize);

	   if ((er = _cdx_adjust_tree(ClipMachineMemory, ro, level - 1, branch->keys + s, rec, __PROC__)))
	      return er;
	}
    }
   return 0;
}

static int
_cdx_delkey(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, const char *__PROC__)
{
   CDX_PAGE  page;

   CDX_LEAF *leaf;

   CDX_BRANCH *branch;

   int       er;

   _CDX_PAGE(ro, ro->stack[ro->level].page, &page);
   if ((_rdd_ushort(leaf->nkeys) == 1) && (ro->level > 0))
    {
       if (_rdd_uint(leaf->left) != 0xffffffff)
	{
	   if ((er = rdd_write(ClipMachineMemory, &ro->index->file, _rdd_uint(leaf->left) + 8, 4, leaf->right, __PROC__)))
	      return er;
	}
       if (_rdd_uint(leaf->right) != 0xffffffff)
	{
	   if ((er = rdd_write(ClipMachineMemory, &ro->index->file, _rdd_uint(leaf->right) + 4, 4, leaf->left, __PROC__)))
	      return er;
	}
       if ((er = _cdx_putfreepage(ClipMachineMemory, ro, ro->stack[ro->level].page, __PROC__)))
	  return er;
       if ((er = _cdx_delstick(ClipMachineMemory, ro, ro->level - 1, __PROC__)))
	  return er;
    }
   else
    {
       _cdx_delnode(ro, leaf, ro->stack[ro->level].pos);
       if ((er = rdd_write(ClipMachineMemory, &ro->index->file, ro->stack[ro->level].page, sizeof(CDX_LEAF), leaf, __PROC__)))
	  return er;
       if (ro->level && (ro->stack[ro->level].pos == _rdd_ushort(leaf->nkeys)))
	{
	   void     *key = malloc(ro->bufsize);

	   unsigned int rec;

	   _cdx_leaf_item(ro, leaf, _rdd_ushort(leaf->nkeys) - 1, &rec, key);
	   if ((er = _cdx_adjust_tree(ClipMachineMemory, ro, ro->level - 1, key, rec, __PROC__)))
	    {
	       free(key);
	       return er;
	    }
	   free(key);
	}
    }
   return 0;
}

static int
cdx_open(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, const char *tag, const char *__PROC__)
{
   RDD_ORDER *tagbag = (RDD_ORDER *) malloc(sizeof(RDD_ORDER));

   RDD_ORDER *ro = NULL;

   char      key[11];

   CDX_HEADER hdr;

   int       f, i, stord, er;

   RDD_ORDER *h;

   char      cntcdx[4];

   if ((er = rdd_read(ClipMachineMemory, &ri->file, 0, sizeof(CDX_HEADER), &hdr, __PROC__)))
      goto err;
   if (hdr.options != 0xe0)
    {
       free(tagbag);
       return idx_open(ClipMachineMemory, rd, ri, tag, __PROC__);
    }

   if ((er = _cdx_init_order(ClipMachineMemory, rd->area, ri, 0, tagbag, __PROC__)))
      goto err;

   tagbag->stack[0].page = _rdd_uint(hdr.root);
   tagbag->level = 0;

   ri->loc = rd->loc;
   if (tag)
    {
       int       found;

       unsigned int newheader;

       char     *e = key + 10;

       char      cntcdx[4];

       memset(key, 0, sizeof(key));
       strncpy(key, tag, 10);
       while (!(*(--e)))
	  *e = ' ';
       _clip_upstr(key, 10);
       if ((er = _cdx_search_tree(ClipMachineMemory, tagbag, key, 10, 0, &found, NULL, __PROC__)))
	  goto err;
       if (!found)
	{
	   er = rdd_err(ClipMachineMemory, EG_OPEN, 0, __FILE__, __LINE__, __PROC__, er_notag);
	   goto err;
	}
       if ((er = _cdx_recno(ClipMachineMemory, tagbag, tagbag->level, &newheader, __PROC__)))
	  goto err;
       ro = (RDD_ORDER *) malloc(sizeof(RDD_ORDER));
       if ((er = _cdx_init_order(ClipMachineMemory, rd->area, ri, newheader, ro, __PROC__)))
	  goto err;

       rd->ords_opened++;
       rd->orders = (RDD_ORDER **) realloc(rd->orders, sizeof(RDD_ORDER *) * rd->ords_opened);
       rd->orders[rd->ords_opened - 1] = ro;
       ro->name = (char *) malloc(strlen(tag) + 1);
       strcpy(ro->name, tag);
       ro->orderno = rd->ords_opened - 1;
       ri->norders++;
       ri->orders = (RDD_ORDER **) realloc(ri->orders, ri->norders * sizeof(RDD_ORDER *));
       ri->orders[ri->norders - 1] = ro;
       ro->canadd = 1;
       if ((er = rdd_read(ClipMachineMemory, &ro->index->file, 8, 4, cntcdx, __PROC__)))
	  goto err;
       ro->cntcdx = _rdd_backuint((unsigned char *) cntcdx);
    }
   else
    {
       int       out;

       unsigned int header;

       char      tagname[11];

       char     *s;

       if ((er = _cdx_first(ClipMachineMemory, tagbag, &out, __PROC__)))
	  goto err;
       while (!out)
	{
	   if ((er = _cdx_recno_keyval(ClipMachineMemory, tagbag, tagbag->level, &header, tagname, __PROC__)))
	      goto err;
	   ro = (RDD_ORDER *) malloc(sizeof(RDD_ORDER));
	   if ((er = _cdx_init_order(ClipMachineMemory, rd->area, ri, header, ro, __PROC__)))
	      goto err;

	   ro->canadd = 1;
	   rd->ords_opened++;
	   rd->orders = (RDD_ORDER **) realloc(rd->orders, sizeof(RDD_ORDER *) * rd->ords_opened);
	   rd->orders[rd->ords_opened - 1] = ro;
	   tagname[sizeof(tagname) - 1] = 0;
	   s = strchr(tagname, ' ');
	   if (s)
	      *s = 0;
	   ro->name = (char *) malloc(strlen(tagname) + 1);
	   strcpy(ro->name, tagname);
	   ro->orderno = rd->ords_opened - 1;
	   ri->norders++;
	   ri->orders = (RDD_ORDER **) realloc(ri->orders, ri->norders * sizeof(RDD_ORDER *));
	   ri->orders[ri->norders - 1] = ro;
	   if ((er = _cdx_next(ClipMachineMemory, rd, tagbag, &out, NULL, __PROC__)))
	      goto err;
	   if ((er = rdd_read(ClipMachineMemory, &ro->index->file, 8, 4, cntcdx, __PROC__)))
	      goto err;
	   ro->cntcdx = _rdd_backuint((unsigned char *) cntcdx);
	}
       stord = ri->orders[0]->orderno;
       do
	{
	   f = 1;
	   for (i = 0; i < ri->norders - 1; i++)
	    {
	       if (rd->orders[stord + i]->header > rd->orders[stord + i + 1]->header)
		{
		   h = rd->orders[stord + i];
		   rd->orders[stord + i] = rd->orders[stord + i + 1];
		   rd->orders[stord + i + 1] = h;
		   rd->orders[stord + i]->orderno = stord + i;
		   rd->orders[stord + i + 1]->orderno = stord + i + 1;
		   f = 0;
		}
	    }
	}
       while (!f);
       do
	{
	   f = 1;
	   for (i = 0; i < ri->norders - 1; i++)
	    {
	       if (ri->orders[i]->header > ri->orders[i + 1]->header)
		{
		   h = ri->orders[i];
		   ri->orders[i] = ri->orders[i + 1];
		   ri->orders[i + 1] = h;
		   f = 0;
		}
	    }
	}
       while (!f);
    }
   destroy_rdd_order(ClipMachineMemory, tagbag);
   return 0;
 err:
   if (ro)
      free(ro);
   destroy_rdd_order(ClipMachineMemory, tagbag);
   return er;
}

static int
cdx_close(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, const char *__PROC__)
{
   int       i;

   for (i = 0; i < ri->norders; i++)
      destroy_rdd_order(ClipMachineMemory, ri->orders[i]);
   destroy_rdd_index(ri);
   return 0;
}

static void
_cdx_init_leaf(RDD_ORDER * ro, CDX_LEAF * leaf, unsigned int lastrec, unsigned int left)
{
   int       i, bits = 1;

   unsigned int mask;

   _rdd_put_ushort(leaf->attrs, (left == 0xffffffff) ? 3 : 2);
   _rdd_put_ushort(leaf->nkeys, 0);
   _rdd_put_uint(leaf->left, left);
   _rdd_put_uint(leaf->right, 0xffffffff);
   _rdd_put_ushort(leaf->free, sizeof(leaf->keys));

   for (i = 1; bits < ro->bufsize; bits = (bits << 1) + 1, i++);
   leaf->dupbits = leaf->trailbits = i;
   leaf->dupmask = leaf->trailmask = bits;
   i = 0;
   lastrec = max(lastrec, 256);
   while (_cdx_bits[i++] < lastrec);
   i += leaf->dupbits + leaf->trailbits;
   if (i % 8)
      i = i / 8 + 1;
   else
      i = i / 8;

   leaf->bytes = i;
   leaf->recbits = leaf->bytes * 8 - (leaf->dupbits + leaf->trailbits);
   mask = 1;
   for (i = 1; i < leaf->recbits; i++)
      mask = (mask << 1) + 1;
   _rdd_put_uint(leaf->recmask, mask);

   memset(leaf->keys, 0, sizeof(leaf->keys));
}

static void
_cdx_init_branch(RDD_ORDER * ro, CDX_BRANCH * branch, unsigned int left)
{
   memset(branch, 0, sizeof(CDX_BRANCH));
   _rdd_put_ushort(branch->attrs, (left == 0xffffffff) ? 1 : 0);
   _rdd_put_uint(branch->left, left);
   _rdd_put_uint(branch->right, 0xffffffff);
}

static int
_cdx_add(RDD_ORDER * ro, CDX_LEAF * leaf, void *key, void **pos, void *buf)
{
   int       dup, trail;

   int       len = ro->bufsize;

   unsigned long long item;

   if (ro->type == 'C' || (ro->type == 'X' && ((char *) key)[4] == type_weight(CHARACTER_type_of_ClipVarType)))
    {
       if (ro->binary)
	{
	   while ((len > 0) && (((char *) key)[len + sizeof(unsigned int) - 1] == 0))
	      len--;
	}
       else
	{
	   loc_write(ro->index->loc, key + sizeof(unsigned int), ro->bufsize);
	   while ((len > 0) && (((char *) key)[len + sizeof(unsigned int) - 1] == (ro->type == 'X' ? 0 : ' ')))
	      len--;
	}
    }
   else				/*if(ro->type == 'N' || ro->type == 'D' || ro->type == 'T') */
    {
       char     *s = key + sizeof(unsigned int) + ro->bufsize - 1;

       while ((s >= (char *) key + sizeof(unsigned int)) && (*s == 0))
	  s--;
       len = s - ((char *) key + sizeof(unsigned int)) + 1;
    }
   if (_rdd_ushort(leaf->nkeys) == 0)
    {
       dup = 0;
       trail = ro->bufsize - len;
    }
   else
    {
       unsigned int _rec;

       int       _dup, _trail;

       memcpy(&item, leaf->keys + (_rdd_ushort(leaf->nkeys) - 1) * leaf->bytes, leaf->bytes);
       _CDX_LEAF_ITEM(item, _rec, _dup, _trail);
       dup = _cdx_dupbytes(buf, key + sizeof(unsigned int), min(ro->bufsize - _trail, len));
       trail = ro->bufsize - len;
    }
   memcpy(buf + dup, key + sizeof(unsigned int) + dup, len - dup);
   *pos -= len - dup;
   memcpy(*pos, buf + dup, len - dup);

   item = trail;
   item = item << leaf->dupbits;
   item += dup;
   item = item << leaf->recbits;
   item += _rdd_uint(key);

   memcpy(leaf->keys + _rdd_ushort(leaf->nkeys) * leaf->bytes, &item, leaf->bytes);
   _rdd_put_ushort(leaf->nkeys, _rdd_ushort(leaf->nkeys) + 1);
   _rdd_put_ushort(leaf->free, _rdd_ushort(leaf->free) - (leaf->bytes + len - dup));
   return _rdd_ushort(leaf->free);
}

static int
__cdx_sets(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, int level, void *key, const char *__PROC__)
{
   CDX_BRANCH branch;

   int       er;

   if (level < 0)
      return 0;
   if ((er = rdd_read(ClipMachineMemory, &ro->index->file, ro->stack[level].page, sizeof(CDX_BRANCH), &branch, __PROC__)))
      return er;
   memcpy(branch.keys + (_rdd_ushort(branch.nkeys) - 1) * (ro->bufsize + 8), key + 4, ro->bufsize);
   _rdd_put_backuint(branch.keys + (_rdd_ushort(branch.nkeys) - 1) * (ro->bufsize + 8) + ro->bufsize, _rdd_uint(key));
   _rdd_put_backuint(branch.keys +
		     (_rdd_ushort(branch.nkeys) - 1) * (ro->bufsize + 8) + ro->bufsize + 4, ro->stack[level + 1].page);
   if ((er = rdd_write(ClipMachineMemory, &ro->index->file, ro->stack[level].page, sizeof(CDX_BRANCH), &branch, __PROC__)))
      return er;
   return __cdx_sets(ClipMachineMemory, ro, level - 1, key, __PROC__);
}

static int
__cdx_adds(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, int level, void *key, unsigned int *newroot, const char *__PROC__)
{
   CDX_BRANCH branch;

   int       er;

   if (level < 0)
    {
       CDX_BRANCH b;

       if ((er = _cdx_getfreepage(ClipMachineMemory, ro->index, newroot, __PROC__)))
	  return er;
       _cdx_init_branch(ro, &branch, 0xffffffff);

       if ((er = rdd_read(ClipMachineMemory, &ro->index->file, ro->stack[0].page, sizeof(CDX_BRANCH), &b, __PROC__)))
	  return er;
       if (!(_rdd_ushort(b.attrs) & 0x02))
	{
	   unsigned int page = _rdd_uint(b.left);

	   if ((er = rdd_read(ClipMachineMemory, &ro->index->file, page, sizeof(CDX_BRANCH), &b, __PROC__)))
	      return er;
	   memcpy(branch.keys, b.keys + (_rdd_ushort(b.nkeys) - 1) * (ro->bufsize + 8), ro->bufsize + 4);
	   _rdd_put_backuint(branch.keys + ro->bufsize + 4, page);
	   _rdd_put_ushort(branch.nkeys, 1);
	}
    }
   else
    {
       unsigned int newbranch;

       if ((er = rdd_read(ClipMachineMemory, &ro->index->file, ro->stack[level].page, sizeof(CDX_BRANCH), &branch, __PROC__)))
	  return er;
       if ((_rdd_ushort(branch.nkeys) + 1) * (ro->bufsize + 8) > sizeof(branch.keys))
	{
	   if ((er = _cdx_getfreepage(ClipMachineMemory, ro->index, &newbranch, __PROC__)))
	      return er;
	   if ((er = rdd_write(ClipMachineMemory, &ro->index->file, ro->stack[level].page + 8, 4, &newbranch, __PROC__)))
	      return er;
	   _cdx_init_branch(ro, &branch, ro->stack[level].page);
	   ro->stack[level].page = newbranch;
	   if ((er =
		rdd_write(ClipMachineMemory, &ro->index->file, ro->stack[level].page, sizeof(CDX_BRANCH), &branch, __PROC__)))
	      return er;
	   if ((er = __cdx_adds(ClipMachineMemory, ro, level - 1, key, newroot, __PROC__)))
	      return er;
	}
    }
   memcpy(branch.keys + _rdd_ushort(branch.nkeys) * (ro->bufsize + 8), key + 4, ro->bufsize);
   _rdd_put_backuint(branch.keys + _rdd_ushort(branch.nkeys) * (ro->bufsize + 8) + ro->bufsize, _rdd_uint(key));
   _rdd_put_backuint(branch.keys + _rdd_ushort(branch.nkeys) * (ro->bufsize + 8) + ro->bufsize + 4, ro->stack[level + 1].page);
   if ((er = __cdx_sets(ClipMachineMemory, ro, level - 1, key, __PROC__)))
      return er;
   _rdd_put_ushort(branch.nkeys, _rdd_ushort(branch.nkeys) + 1);
   if ((er =
	rdd_write(ClipMachineMemory, &ro->index->file,
		  (level < 0) ? *newroot : ro->stack[level].page, sizeof(CDX_BRANCH), &branch, __PROC__)))
      return er;
   return 0;
}

static int
_cdx_adds(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, void *key, const char *__PROC__)
{
   unsigned int newroot = 0;

   int       er;

   if ((er = __cdx_adds(ClipMachineMemory, ro, ro->level - 1, key, &newroot, __PROC__)))
      return er;
   if (newroot)
    {
       int       i;

       for (i = MAX_BTREE_DEEP - 1; i > 0; i--)
	  ro->stack[i] = ro->stack[i - 1];
       ro->stack[0].page = newroot;
       ro->level++;
       if ((er = rdd_write(ClipMachineMemory, &ro->index->file, ro->header, sizeof(unsigned int), &newroot, __PROC__)))
	  return er;
    }
   return 0;
}

static int
_cdx_savebtree(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, BTREE1 * bt, int lastrec, const char *__PROC__)
{
   CDX_HEADER hdr;

   CDX_PAGE  page;

   CDX_LEAF *leaf = (CDX_LEAF *) & page;

   int       eof, er;

   void     *pos = (void *) leaf + sizeof(CDX_LEAF);

   void     *buf = calloc(1, ro->bufsize);

   ro->level = 0;
   _CDX_HEADER(ro, &hdr);
   ro->stack[ro->level].page = _rdd_uint(hdr.root);
   eof = bt1_first(bt);
   _cdx_init_leaf(ro, leaf, lastrec, 0xffffffff);
   while (!eof)
    {
       while (_cdx_add(ro, leaf, bt1_key(bt), &pos, buf) > ro->bufsize + leaf->bytes)
	{
	   eof = bt1_next(bt);
	   if (eof)
	      break;
	}
       if (_rdd_ushort(leaf->attrs) == 3 && eof)
	{
	   if ((er =
		rdd_write(ClipMachineMemory, &ro->index->file, ro->stack[ro->level].page, sizeof(CDX_LEAF), leaf, __PROC__)))
	      goto err;
	   break;
	}
       _rdd_put_ushort(leaf->attrs, 2);
       if (eof)
	  bt1_last(bt);
       pos = (void *) leaf + sizeof(CDX_LEAF);
       if (_rdd_uint(leaf->left) != 0xffffffff)
	{
	   if ((er =
		rdd_write(ClipMachineMemory, &ro->index->file,
			  _rdd_uint(leaf->left) + 8, 4, &ro->stack[ro->level].page, __PROC__)))
	      goto err;
	}
       if ((er = rdd_write(ClipMachineMemory, &ro->index->file, ro->stack[ro->level].page, sizeof(CDX_LEAF), leaf, __PROC__)))
	  goto err;
       _cdx_init_leaf(ro, leaf, lastrec, ro->stack[ro->level].page);
       if ((er = _cdx_adds(ClipMachineMemory, ro, bt1_key(bt), __PROC__)))
	  goto err;
       eof = bt1_next(bt);
       if (eof)
	  break;
       if ((er = _cdx_getfreepage(ClipMachineMemory, ro->index, &ro->stack[ro->level].page, __PROC__)))
	  goto err;
    }
   free(buf);
   return 0;
 err:
   free(buf);
   return er;
}

static int
_cdx_compare(void *op, void *k1, void *k2, int *uniqfound)
{
   RDD_ORDER *ro = op;

   int       r = 0;

   if (ro->type == 'C' && !ro->binary)
    {
       unsigned char *ss1, *ss2;

       unsigned char *e = k1 + sizeof(unsigned int) + ro->bufsize;

       for (ss1 = k1 + sizeof(unsigned int), ss2 = k2 + sizeof(unsigned int); ss1 < e && !r; ss1++, ss2++)

	  r = _clip_cmptbl[*ss1] - _clip_cmptbl[*ss2];
    }
   else if (ro->type != 'X')
    {
       r = memcmp(k1 + sizeof(unsigned int), k2 + sizeof(unsigned int), ro->bufsize);
    }
   else
    {				/* 'X' */
       r = ((char *) k1)[4] - ((char *) k2)[4];
       if (r == 0)
	{
	   if (((char *) k1)[4] == type_weight(CHARACTER_type_of_ClipVarType) && !ro->binary)
	    {
	       unsigned char *ss1, *ss2;

	       unsigned char *e = k1 + sizeof(unsigned int) + ro->bufsize;

	       for (ss1 = k1 + sizeof(unsigned int) + 1, ss2 = k2 + sizeof(unsigned int) + 1; ss1 < e && !r; ss1++, ss2++)

		  r = _clip_cmptbl[*ss1] - _clip_cmptbl[*ss2];
	    }
	   else
	    {
	       r = memcmp(k1 + sizeof(unsigned int) + 1, k2 + sizeof(unsigned int) + 1, ro->bufsize - 1);
	    }
	}
    }
   if (uniqfound)
      *uniqfound = r;
   if (!r)
      r = *(int *) k1 - *(int *) k2;
   return r;
}

#define D20070119
static int
_cdx_create(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri,
	    RDD_ORDER ** rop, const char *tag, const char *expr,
	    ClipVar * block, int unique, unsigned int header, const char *__PROC__)
{
   RDD_ORDER *ro = NULL;

   CDX_HEADER hdr;

   ClipVar   vv, *vp;

   int       lastrec, s, e, i, er;

   unsigned int newroot;

   BTREE1   *bt;

   void     *buf;

   for (i = 0; i < ri->norders; i++)
      if (strcasecmp(tag, ri->orders[i]->name) == 0)
       {
	  destroy_rdd_order(ClipMachineMemory, ro);
	  ro = ri->orders[i];
	  memset(ro, 0, sizeof(RDD_ORDER));
	  break;
       }
   if (!ro)
    {
       ro = (RDD_ORDER *) calloc(1, sizeof(RDD_ORDER));
       ri->orders = (RDD_ORDER **) realloc(ri->orders, (ri->norders + 1) * sizeof(RDD_ORDER *));
       ri->orders[ri->norders] = ro;
       ri->norders++;
    }
   ro->vtbl = ri->vtbl;
   ro->index = ri;
   *rop = ro;

   ro->header = header;
   ro->name = (char *) malloc(strlen(tag ? tag : ri->name) + 1);
   strcpy(ro->name, tag ? tag : ri->name);

   ro->expr = (char *) malloc(strlen(expr) + 1);
   strcpy(ro->expr, expr);
   if ((ro->simpfno = _rdd_fieldno(rd, _clip_casehashword(ro->expr, strlen(ro->expr)))) != -1)
    {
       ro->simpexpr = 1;
       if (ri->rd->fields[ro->simpfno].binary)
	  ro->binary = 1;
       ro->type = ri->rd->fields[ro->simpfno].type;
       ro->bufsize = ri->rd->fields[ro->simpfno].len - 1;
    }

   if (!block || block->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType)
    {
       int       r = _clip_parni(ClipMachineMemory, 1);

       char      expr[1024];

       memset(expr, 0, sizeof(expr));

       rdd_expandmacro(rd->area, r, ro->expr, expr);

       if ((er = _clip_eval_macro(ClipMachineMemory, expr, strlen(expr), &ro->block)))
	  return er;
    }
   else
    {
       _clip_clone(ClipMachineMemory, &ro->block, block);
    }

   if (ro->type == 'X')
    {
       ro->keysize = ro->bufsize = ro->index->rd->fields[ro->simpfno].len - 1;
    }
   else
    {
       if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->block, &vv, 0)))
	  return er;
       vp = _clip_vptr(&vv);

       switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
	{
	case CHARACTER_type_of_ClipVarType:
	   ro->bufsize = ro->keysize = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
	   ro->dec = 0;
	   ro->type = 'C';
	   break;
	case NUMERIC_type_of_ClipVarType:
	   ro->keysize = (vp->ClipType_t_of_ClipVar.len_of_ClipType ? vv.ClipType_t_of_ClipVar.len_of_ClipType : 10);
	   ro->dec = vp->ClipType_t_of_ClipVar.dec_of_ClipType;
	   ro->bufsize = 8;
	   ro->type = 'N';
	   break;
	case LOGICAL_type_of_ClipVarType:
	   ro->keysize = 1;
	   ro->dec = 0;
	   ro->bufsize = 1;
	   ro->type = 'L';
	   break;
	case DATE_type_of_ClipVarType:
	   ro->keysize = 8;
	   ro->dec = 0;
	   ro->bufsize = 8;
	   ro->type = 'D';
	   break;
	case DATETIME_type_of_ClipVarType:
	   ro->keysize = 8;
	   ro->dec = 0;
	   ro->bufsize = 8;
	   ro->type = 'T';
	   break;
	default:
#ifdef D20070119
	   _clip_destroy(ClipMachineMemory, &vv);
#endif
	   return rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_baddata);
	}
#ifdef D20070119
       _clip_destroy(ClipMachineMemory, &vv);
#endif
    }
   ro->key = malloc(ro->bufsize);
#ifndef D20070119
   _clip_destroy(ClipMachineMemory, &vv);
#endif

   ro->unique = unique;
   ro->descend = rd->os.lDescend;
   ro->custom = rd->os.lCustom;
   ro->canadd = 1;
   ro->curpage = calloc(1, sizeof(CDX_PAGE));
   ro->curoffs = 0;
   ro->ic = rd->os.lIgnoreCase;
   if (rd->os.bForCondition.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
    {
       ro->cforexpr = malloc(strlen(rd->os.cForCondition) + 1);
       strcpy(ro->cforexpr, rd->os.cForCondition);
       _clip_clone(ClipMachineMemory, &ro->bforexpr, &rd->os.bForCondition);
    }

   memset(&hdr, 0, sizeof(CDX_HEADER));
   if ((er = rdd_write(ClipMachineMemory, &ri->file, ro->header, sizeof(CDX_HEADER), (char *) &hdr, __PROC__)))
      return er;
   if ((er = _cdx_getfreepage(ClipMachineMemory, ri, &newroot, __PROC__)))
      return er;
   _rdd_put_uint(hdr.root, newroot);
   _rdd_put_uint(hdr.keysize, ro->bufsize);
   hdr.options = (strcmp(ri->vtbl->id, "CDX") ? 0x20 : 0x62)
    | (unique ? 0x01 : 0x00)
    | (ro->custom ? 0x04 : 0x00)
    | (rd->os.bForCondition.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType ? 0x08 : 0x00);
   hdr.sign = 1;
   hdr.ic = ro->ic;
   _rdd_put_ushort(hdr.sort, ro->descend);
   _rdd_put_ushort(hdr.explen, strlen(ro->expr) + 1);
   _rdd_put_ushort(hdr.totlen, _rdd_ushort(hdr.explen));
   strcpy((char *) hdr.keyforexpr, ro->expr);
   if (!ro->binary)
      loc_write(ri->loc, hdr.keyforexpr, strlen((const char *) hdr.keyforexpr));
   if (rd->os.cForCondition)
    {
       _rdd_put_ushort(hdr.forlen, strlen(rd->os.cForCondition) + 1);
       _rdd_put_ushort(hdr.totlen, _rdd_ushort(hdr.totlen) + _rdd_ushort(hdr.forlen));
       strcpy((char *) (hdr.keyforexpr + _rdd_ushort(hdr.explen)), rd->os.cForCondition);
       if (!ro->binary)
	  loc_write(ri->loc, hdr.keyforexpr + _rdd_ushort(hdr.explen),
		    strlen((const char *) (hdr.keyforexpr + _rdd_ushort(hdr.explen))));
    }
   else
    {
       _rdd_put_ushort(hdr.forlen, 1);
    }

   if ((er = rdd_write(ClipMachineMemory, &ri->file, ro->header, sizeof(CDX_HEADER), (char *) &hdr, __PROC__)))
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
   if ((er = rdd_write(ClipMachineMemory, &ri->file, ro->header, sizeof(CDX_HEADER), (char *) &hdr, __PROC__)))
      return er;

   if ((er = _cdx_init_rootleaf(ClipMachineMemory, ro, newroot, __PROC__)))
      return er;

   if (ro->custom)
      return 0;

   if ((er = rd->vtbl->lastrec(ClipMachineMemory, rd, &lastrec, __PROC__)))
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
       if ((er = rdd_gotop(ClipMachineMemory, rd, __PROC__)))
	  return er;
       rd->os.lCurrent = 1;
    }

   rd->tagindexing = ro->name;
   bt = bt1_create(ro->unique, ro->bufsize + sizeof(unsigned int), _cdx_compare, ClipMachineMemory->index_buffer_limit);
//      bt = NULL;
   buf = malloc(ro->bufsize + sizeof(unsigned int));
   if (!rd->os.lCurrent)
    {
       for (rd->recno = s; rd->recno <= e; rd->recno++)
	{
	   rd->eof = 0;
	   if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, rd->recno, 0, __PROC__)))
	      return er;
	  /* check WHILE condition */
	  /* no need to check WHILE, because USECURRENT is ON with WHILE
	     clause. See above...
	     if(rd->os.bWhileCondition.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType){
	     if((er = rdd_calc(ClipMachineMemory,rd->area,&rd->os.bWhileCondition,&vv,0)))
	     return er;
	     vp = _clip_vptr(&vv);
	     if(vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType!=LOGICAL_type_of_ClipVarType)
	     return rdd_err(ClipMachineMemory,EG_DATATYPE,0,__FILE__,__LINE__,__PROC__,
	     er_badwhileexpr);
	     if(!vp->ClipLogVar_l_of_ClipVar.val){
	     _clip_destroy(ClipMachineMemory,&vv);
	     break;
	     }
	     _clip_destroy(ClipMachineMemory,&vv);
	     }
	   */
	  /* bEval every nInterval */
	   if (rd->keysincluded % i == 0
	       && rd->os.bEval.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
	    {
	       if ((er = rdd_calc(ClipMachineMemory, rd->area, &rd->os.bEval, &vv, 0)))
		  return er;
	       vp = _clip_vptr(&vv);

	       if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
		  return rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_badevalexpr);
	       if (!vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
		{
		   _clip_destroy(ClipMachineMemory, &vv);
		   break;
		}
	       _clip_destroy(ClipMachineMemory, &vv);
	    }
	   if (ro->bforexpr.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
	    {
	       if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->bforexpr, &vv, 0)))
		  return er;
	       vp = _clip_vptr(&vv);
	       if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
		  return rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_badforexpr);
	       if (!vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
		{
		   _clip_destroy(ClipMachineMemory, &vv);
		   continue;
		}
	       _clip_destroy(ClipMachineMemory, &vv);
	    }
	   if (bt)
	    {
	       if (ro->simpexpr)
		{
		   if ((er = rd->vtbl->getvalue(ClipMachineMemory, rd, ro->simpfno, &vv, __PROC__)))
		      return er;
		   vp = &vv;
		}
	       else
		{
		   if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->block, &vv, 0)))
		      return er;
		   vp = _clip_vptr(&vv);
		}
	       memcpy(buf, &rd->recno, sizeof(unsigned int));
	       if ((er = cdx_formatkey(ClipMachineMemory, ro, vp, buf + sizeof(unsigned int), __PROC__)))
		  return er;
	       _clip_destroy(ClipMachineMemory, &vv);
	       if (bt1_add(bt, ro, buf))
		{
		   if ((er = _cdx_savebtree(ClipMachineMemory, ro, bt, lastrec, __PROC__)))
		    {
		       bt1_destroy(bt);
		       return er;
		    }
		   bt1_destroy(bt);
		   bt = NULL;
		   if ((er = cdx_addkey(ClipMachineMemory, rd, ro, NULL, __PROC__)))
		      return er;
		}
	    }
	   else
	    {
	       if ((er = cdx_addkey(ClipMachineMemory, rd, ro, NULL, __PROC__)))
		  return er;
	    }
	   rd->keysincluded++;
	}
       rd->keysincluded = 0;
    }
   else
    {
/*		if((er = rdd_gotop(ClipMachineMemory,rd,__PROC__))) return er;*/
       while (!rd->eof)
	{
	   rd->eof = 0;
	  /* check WHILE condition */
	   if (rd->os.bWhileCondition.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
	    {
	       if ((er = rdd_calc(ClipMachineMemory, rd->area, &rd->os.bWhileCondition, &vv, 0)))
		  return er;
	       vp = _clip_vptr(&vv);
	       if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
		  return rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_badwhileexpr);
	       if (!vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
		{
		   _clip_destroy(ClipMachineMemory, &vv);
		   break;
		}
	       _clip_destroy(ClipMachineMemory, &vv);
	    }
	  /* bEval every nInterval */
	   if (rd->keysincluded % i == 0
	       && rd->os.bEval.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
	    {
	       if ((er = rdd_calc(ClipMachineMemory, rd->area, &rd->os.bEval, &vv, 0)))
		  return er;
	       vp = _clip_vptr(&vv);
	       if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
		  return rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_badevalexpr);
	       if (!vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
		{
		   _clip_destroy(ClipMachineMemory, &vv);
		   break;
		}
	       _clip_destroy(ClipMachineMemory, &vv);
	    }
	   if (ro->bforexpr.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
	    {
	       if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->bforexpr, &vv, 0)))
		  return er;
	       vp = _clip_vptr(&vv);
	       if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
		  return rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_badforexpr);
	       if (!vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
		{
		   _clip_destroy(ClipMachineMemory, &vv);
		   if ((er = rdd_skip(ClipMachineMemory, rd, 1, __PROC__)))
		      return er;
		   continue;
		}
	       _clip_destroy(ClipMachineMemory, &vv);
	    }
	   if (bt)
	    {
	       if (ro->simpexpr)
		{
		   if ((er = rd->vtbl->getvalue(ClipMachineMemory, rd, ro->simpfno, &vv, __PROC__)))
		      return er;
		   vp = &vv;
		}
	       else
		{
		   if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->block, &vv, 0)))
		      return er;
		   vp = _clip_vptr(&vv);
		}
	       memcpy(buf, &rd->recno, sizeof(unsigned int));
	       if ((er = cdx_formatkey(ClipMachineMemory, ro, vp, buf + sizeof(unsigned int), __PROC__)))
		  return er;
	       _clip_destroy(ClipMachineMemory, &vv);
	       if (bt1_add(bt, ro, buf))
		{
		   if ((er = _cdx_savebtree(ClipMachineMemory, ro, bt, lastrec, __PROC__)))
		    {
		       bt1_destroy(bt);
		       return er;
		    }
		   bt1_destroy(bt);
		   bt = NULL;
		   if ((er = cdx_addkey(ClipMachineMemory, rd, ro, NULL, __PROC__)))
		      return er;
		}
	    }
	   else
	    {
	       if ((er = cdx_addkey(ClipMachineMemory, rd, ro, NULL, __PROC__)))
		  return er;
	    }
	   rd->keysincluded++;
	   if ((er = rdd_skip(ClipMachineMemory, rd, 1, __PROC__)))
	      return er;
	}
       rd->keysincluded = 0;
    }
   if (bt)
    {
       if ((er = _cdx_savebtree(ClipMachineMemory, ro, bt, lastrec, __PROC__)))
	{
	   bt1_destroy(bt);
	   return er;
	}
       bt1_destroy(bt);
    }
   free(buf);
   rd->tagindexing = NULL;
   return 0;
}

static int
cdx_create(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri,
	   RDD_ORDER ** rop, const char *tag, const char *expr,
	   ClipVar * block, int unique, int first, unsigned int header, const char *__PROC__)
{
   CDX_HEADER chdr;

   RDD_ORDER *tagbag = malloc(sizeof(RDD_ORDER));

   unsigned int root;

   ClipVar   v;

   unsigned int newheader, dummy;

   char      tagname[11];

   char      tn[11];

   unsigned int oldrn;

   int       found, er;

   char     *e;

   tagbag->level = 0;
   memset(&v, 0, sizeof(ClipVar));
   ri->loc = rd->loc;
   if (first)
    {
       root = sizeof(CDX_HEADER);
       memset(&chdr, 0, sizeof(CDX_HEADER));
       _rdd_put_uint(chdr.root, sizeof(CDX_HEADER));
       _rdd_put_ushort(chdr.keysize, 10);
       chdr.options = 0xE0;
       chdr.sign = 1;
       _rdd_put_ushort(chdr.totlen, 1);
       _rdd_put_ushort(chdr.forlen, 1);
       _rdd_put_ushort(chdr.explen, 1);
       if ((er = rdd_write(ClipMachineMemory, &ri->file, 0, sizeof(CDX_HEADER), &chdr, __PROC__)))
	  goto err;
       if ((er = _cdx_getfreepage(ClipMachineMemory, ri, &root, __PROC__)))
	  goto err;
       if ((er = _cdx_init_order(ClipMachineMemory, rd->area, ri, 0, tagbag, __PROC__)))
	  goto err;
       if ((er = _cdx_init_rootleaf(ClipMachineMemory, tagbag, root, __PROC__)))
	  goto err;
       if (strcmp(rd->name, ri->name) == 0)
	  ri->structural = 1;
    }
   else
    {
       if ((er = rdd_read(ClipMachineMemory, &ri->file, 0, sizeof(CDX_HEADER), &chdr, __PROC__)))
	  goto err;
       if ((er = _cdx_init_order(ClipMachineMemory, rd->area, ri, 0, tagbag, __PROC__)))
	  goto err;
    }

   tagbag->unique = 1;
   if (tag)
      strncpy(tagname, tag, 10);
   else
      strncpy(tagname, ri->name, 10);
   tagname[10] = 0;
   _clip_upstr(tagname, strlen(tagname));

   e = tagname + strlen(tagname);
   while (*(--e) == ' ');
   *(e + 1) = 0;
   v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   v.ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = tagname;
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = strlen(tagname);
   if ((er = _cdx_getfreepage(ClipMachineMemory, ri, &newheader, __PROC__)))
      goto err;
   if ((er = _cdx_getfreepage(ClipMachineMemory, ri, &dummy, __PROC__)))
      goto err;

   strcpy(tn, tagname);
   memset(tn + strlen(tn), 0x20, 10 - strlen(tn));
   tn[10] = 0;
   tagbag->stack[0].page = _rdd_uint(chdr.root);
   if ((er = _cdx_search_tree(ClipMachineMemory, tagbag, tn, 10, 0, &found, NULL, __PROC__)))
      goto err;
   if (found)
    {
       if ((er = _cdx_delkey(ClipMachineMemory, tagbag, __PROC__)))
	  goto err;
    }
   oldrn = rd->recno;
   rd->recno = newheader;
   if ((er = cdx_addkey(ClipMachineMemory, rd, tagbag, &v, __PROC__)))
      goto err;
   rd->recno = oldrn;
   if ((er = _cdx_create(ClipMachineMemory, rd, ri, rop, tagname, expr, block, unique, newheader, __PROC__)))
      goto err;
   rd->recno = oldrn;

   destroy_rdd_order(ClipMachineMemory, tagbag);
   return 0;
 err:
   destroy_rdd_order(ClipMachineMemory, tagbag);
   return er;
}

static int
cdx_reindex(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, const char *__PROC__)
{
   RDD_ORDER *ro;

   int       lastrec, i;

   BTREE1   *bt;

   void     *buf;

   ClipVar   vv, *vp;

   int       interval = max(1, rd->os.nInterval);

   unsigned int oldrn = rd->recno;

   int       er;

   memset(&vv, 0, sizeof(ClipVar));

   if ((er = cdx_zap(ClipMachineMemory, rd, ri, __PROC__)))
      return er;
   if ((er = rd->vtbl->lastrec(ClipMachineMemory, rd, &lastrec, __PROC__)))
      return er;

   for (i = 0; i < ri->norders; i++)
    {
       ro = ri->orders[i];
       if (ro->custom)
	  continue;
       rd->eof = 0;
       if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, rd->recno, 0, __PROC__)))
	  return er;
       if (ro->type == 'X')
	{
	   ro->keysize = ro->bufsize = ro->index->rd->fields[ro->simpfno].len - 1;
	}
       else
	{
	   if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->block, &vv, 0)))
	      return er;
	   vp = _clip_vptr(&vv);

	   switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
	    {
	    case CHARACTER_type_of_ClipVarType:
	       ro->bufsize = ro->keysize = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
	       ro->dec = 0;
	       ro->type = 'C';
	       break;
	    case NUMERIC_type_of_ClipVarType:
	       ro->keysize = (vp->ClipType_t_of_ClipVar.len_of_ClipType ? vv.ClipType_t_of_ClipVar.len_of_ClipType : 10);
	       ro->dec = vp->ClipType_t_of_ClipVar.dec_of_ClipType;
	       ro->bufsize = 8;
	       ro->type = 'N';
	       break;
	    case LOGICAL_type_of_ClipVarType:
	       ro->keysize = 1;
	       ro->dec = 0;
	       ro->bufsize = 1;
	       ro->type = 'L';
	       break;
	    case DATE_type_of_ClipVarType:
	       ro->keysize = 8;
	       ro->dec = 0;
	       ro->bufsize = 8;
	       ro->type = 'D';
	       break;
	    case DATETIME_type_of_ClipVarType:
	       ro->keysize = 8;
	       ro->dec = 0;
	       ro->bufsize = 8;
	       ro->type = 'T';
	       break;
	    default:
	       return rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_baddata);
	    }
	}
       rd->tagindexing = ro->name;
       bt = bt1_create(ro->unique, ro->bufsize + sizeof(unsigned int), _cdx_compare, ClipMachineMemory->index_buffer_limit);
       buf = malloc(ro->bufsize + sizeof(unsigned int));
       for (rd->recno = 1; rd->recno <= lastrec; rd->recno++)
	{
	   rd->eof = 0;
	   if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, rd->recno, 0, __PROC__)))
	      return er;
	   if (ro->bforexpr.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
	    {
	       if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->bforexpr, &vv, 0)))
		  return er;
	       vp = _clip_vptr(&vv);
	       if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
		  return rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_badforexpr);
	       if (!vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
		{
		   _clip_destroy(ClipMachineMemory, &vv);
		   continue;
		}
	       _clip_destroy(ClipMachineMemory, &vv);
	    }
	   if (bt)
	    {
	       if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->block, &vv, 0)))
		  return er;
	       vp = _clip_vptr(&vv);
	       memcpy(buf, &rd->recno, sizeof(unsigned int));
	       if ((er = cdx_formatkey(ClipMachineMemory, ro, vp, buf + sizeof(unsigned int), __PROC__)))
		  return er;
	       _clip_destroy(ClipMachineMemory, &vv);
	       if (bt1_add(bt, ro, buf))
		{
		   if ((er = _cdx_savebtree(ClipMachineMemory, ro, bt, lastrec, __PROC__)))
		    {
		       bt1_destroy(bt);
		       return er;
		    }
		   bt1_destroy(bt);
		   bt = NULL;
		   if ((er = cdx_addkey(ClipMachineMemory, rd, ro, NULL, __PROC__)))
		      return er;
		}
	    }
	   else
	    {
	       if ((er = cdx_addkey(ClipMachineMemory, rd, ro, NULL, __PROC__)))
		  return er;
	    }
	  /* bEval every nInterval */
	   if (rd->recno % interval == 0
	       && rd->os.bEval.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
	    {
	       if ((er = rdd_calc(ClipMachineMemory, rd->area, &rd->os.bEval, &vv, 0)))
		  return er;
	       vp = _clip_vptr(&vv);

	       if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
		  return rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_badevalexpr);
	       if (!vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
		{
		   _clip_destroy(ClipMachineMemory, &vv);
		   break;
		}
	       _clip_destroy(ClipMachineMemory, &vv);
	    }
	}
       if (bt)
	{
	   if ((er = _cdx_savebtree(ClipMachineMemory, ro, bt, lastrec, __PROC__)))
	    {
	       bt1_destroy(bt);
	       return er;
	    }
	   bt1_destroy(bt);
	}
       free(buf);
    }
   rd->tagindexing = NULL;
   if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, oldrn, 0, __PROC__)))
      return er;
   return 0;
}

static int
cdx_zap(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, const char *__PROC__)
{
   RDD_ORDER *tagbag = malloc(sizeof(RDD_ORDER));

   int       out, i;

   CDX_HEADER *hdrs = NULL;

   char    **tags = NULL;

   int      *ros = NULL;

   unsigned int recno;

   int       ntags = 0, er;

   if ((er = _cdx_init_order(ClipMachineMemory, rd->area, ri, 0, tagbag, __PROC__)))
      goto err;

   if ((er = _cdx_first(ClipMachineMemory, tagbag, &out, __PROC__)))
      goto err;
   while (!out)
    {
       ntags++;
       ros = realloc(ros, sizeof(int) * ntags);
       hdrs = realloc(hdrs, sizeof(CDX_HEADER) * ntags);
       tags = realloc(tags, sizeof(char *) * ntags);
       tags[ntags - 1] = malloc(11);
       ros[ntags - 1] = -1;
       if ((er = _cdx_recno_keyval(ClipMachineMemory, tagbag, tagbag->level, &recno, tags[ntags - 1], __PROC__)))
	  goto err;
       for (i = 0; i < ri->norders; i++)
	{
	   if (ri->orders[i]->header == recno)
	    {
	       ros[ntags - 1] = i;
	       break;
	    }
	}
       if ((er = rdd_read(ClipMachineMemory, &ri->file, recno, sizeof(CDX_HEADER), &hdrs[ntags - 1], __PROC__)))
	  goto err;
       if ((er = _cdx_next(ClipMachineMemory, rd, tagbag, &out, NULL, __PROC__)))
	  goto err;
    }

   if ((er = rdd_trunc(ClipMachineMemory, &ri->file, sizeof(CDX_HEADER), __PROC__)))
      goto err;
   if ((er = _cdx_init_rootleaf(ClipMachineMemory, tagbag, sizeof(CDX_HEADER), __PROC__)))
      goto err;
   recno = 0;
   if ((er = rdd_write(ClipMachineMemory, &ri->file, 4, 4, &recno, __PROC__)))
      goto err;

   for (i = 0; i < ntags; i++)
    {
       ClipVar   v;

       RDD_ORDER *ro = malloc(sizeof(RDD_ORDER));

       int       j = 9;

       int       oldrecno;

       unsigned int newroot;

       unsigned int dummy;

       v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
       v.ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
       v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = tags[ros[i]];
       v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = 10;
       while (tags[ros[i]][j] == ' ')
	{
	   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf--;
	   tags[ros[i]][j--] = 0;
	}
       if ((er = _cdx_getfreepage(ClipMachineMemory, ri, &recno, __PROC__)))
	  goto err;
       if ((er = _cdx_getfreepage(ClipMachineMemory, ri, &dummy, __PROC__)))
	  goto err;
       oldrecno = rd->recno;
       rd->recno = recno;
       if ((er = cdx_addkey(ClipMachineMemory, rd, tagbag, &v, __PROC__)))
	  goto err;
       rd->recno = oldrecno;
       if ((er = rdd_write(ClipMachineMemory, &ri->file, recno, sizeof(CDX_HEADER), &hdrs[ros[i]], __PROC__)))
	  goto err;
       if ((er = _cdx_init_order(ClipMachineMemory, rd->area, ri, recno, ro, __PROC__)))
	  goto err;
       if ((er = _cdx_getfreepage(ClipMachineMemory, ri, &newroot, __PROC__)))
	  goto err;
       if ((er = _cdx_init_rootleaf(ClipMachineMemory, ro, newroot, __PROC__)))
	  goto err;
//              if(ros[i]!=-1)
       ri->orders[i]->header = recno;
       destroy_rdd_order(ClipMachineMemory, ro);
    }
   if ((er = rdd_childs(ClipMachineMemory, rd, __PROC__)))
      return er;
   if (tags)
    {
       for (i = 0; i < ntags; i++)
	  if (tags[i])
	     free(tags[i]);
       free(tags);
    }
   if (hdrs)
      free(hdrs);
   if (ros)
      free(ros);
   destroy_rdd_order(ClipMachineMemory, tagbag);
   return 0;
 err:
   if (tags)
    {
       for (i = 0; i < ntags; i++)
	  if (tags[i])
	     free(tags[i]);
       free(tags);
    }
   if (hdrs)
      free(hdrs);
   if (ros)
      free(ros);
   destroy_rdd_order(ClipMachineMemory, tagbag);
   return er;
}

static int
cdx_gotop(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__)
{
   unsigned int lastrec;

   int       out = 0, fok, deleted, er;

   ro->level = 0;
   rd->bof = rd->v_bof = rd->eof = 0;
   if (rd->filter && rd->filter->list)
    {
       if (rd->filter->listlen == 0)
	{
	   if ((er = rdd_lastrec(ClipMachineMemory, rd, (int *) (&lastrec), __PROC__)))
	      return er;
	   rd->bof = rd->v_bof = rd->eof = 1;
	   if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, lastrec + 1, 0, __PROC__)))
	      return er;
	   rd->filter->cursor = 0;
	}
       else
	{
	   rd->filter->cursor = -1;
	   while (++rd->filter->cursor < rd->filter->listlen)
	    {
	       if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, rd->filter->list[rd->filter->cursor], 0, __PROC__)))
		  return er;
	       if (!(ClipMachineMemory->flags & DELETED_FLAG))
		  return 0;
	       if ((er = rdd_deleted(ClipMachineMemory, rd, &deleted, __PROC__)))
		  return er;
	       if (!deleted)
		  return 0;
	    }
	   rd->eof = rd->bof = rd->v_bof = 1;
	   if ((er = rdd_lastrec(ClipMachineMemory, rd, (int *) &lastrec, __PROC__)))
	      return er;
	   if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, lastrec + 1, 0, __PROC__)))
	      return er;
	}
       return 0;
    }
   if (ro->scopetop)
    {
       int       found;

       int       ok = 0;

       CDX_HEADER hdr;

       void     *key = malloc(ro->bufsize);

       int       outrange;

       _CDX_HEADER(ro, &hdr);
       ro->stack[0].page = _rdd_uint(hdr.root);

       if (!ro->descend)
	{
	   if ((er = _cdx_search_tree(ClipMachineMemory, ro, ro->scopetop, ro->stoplen, 0, &found, &outrange, __PROC__)))
	      return er;
	}
       else
	{
	   if ((er = _cdx_back_search_tree(ClipMachineMemory, ro, ro->scopetop, ro->stoplen, 0, &found, &outrange, __PROC__)))
	      return er;
	   if (outrange > 0 && ro->stack[ro->level].pos > 0)
	    {
	       ro->stack[ro->level].pos--;
	       outrange = 0;
	    }
	}
       if (!outrange)
	{
	   if ((er = _cdx_recno_keyval(ClipMachineMemory, ro, ro->level, &rd->recno, key, __PROC__)))
	      return er;
	   if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, rd->recno, 1, __PROC__)))
	      return er;
	   if ((er = _cdx_checkscope(ClipMachineMemory, rd, ro, key, &ok, __PROC__)))
	      return er;
	}
       free(key);
       if (outrange || ok)
	{
	   unsigned int lastrec;

	   if ((er = rd->vtbl->lastrec(ClipMachineMemory, rd, (int *) &lastrec, __PROC__)))
	      return er;
	   rd->eof = 1;
	   if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, lastrec + 1, 0, __PROC__)))
	      return er;
	}
    }
   else
    {
       if (!ro->descend)
	{
	   if ((er = _cdx_first(ClipMachineMemory, ro, &out, __PROC__)))
	      return er;
	}
       else
	{
	   if ((er = _cdx_last(ClipMachineMemory, ro, &out, __PROC__)))
	      return er;
	}
       if (out)
	{
	   rd->bof = rd->v_bof = rd->eof = 1;
	}
       else
	{
	   if ((er = _cdx_recno(ClipMachineMemory, ro, ro->level, &rd->recno, __PROC__)))
	      return er;
	   if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, rd->recno, 1, __PROC__)))
	      return er;
	}
    }
   if ((er = rdd_checkfilter(ClipMachineMemory, rd, &fok, __PROC__)))
      return er;
   if (!fok)
    {
       if ((er = cdx_next(ClipMachineMemory, rd, ro, __PROC__)))
	  return er;
       if (rd->eof)
	{
	   rd->bof = rd->v_bof = 1;
	   return 0;
	}
    }
   return 0;
}

static int
cdx_gobottom(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__)
{
   unsigned int lastrec;

   int       out = 0, fok, deleted, er;

   ro->level = 0;
   rd->bof = rd->v_bof = rd->eof = 0;
   if (rd->filter && rd->filter->list)
    {
       if (rd->filter->listlen == 0)
	{
	   if ((er = rdd_lastrec(ClipMachineMemory, rd, (int *) &lastrec, __PROC__)))
	      return er;
	   rd->bof = rd->v_bof = rd->eof = 1;
	   if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, lastrec + 1, 0, __PROC__)))
	      return er;
	   rd->filter->cursor = 0;
	}
       else
	{
	   rd->filter->cursor = rd->filter->listlen;
	   while (rd->filter->cursor > 0)
	    {
	       if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, rd->filter->list[--rd->filter->cursor], 0, __PROC__)))
		  return er;
	       if (!(ClipMachineMemory->flags & DELETED_FLAG))
		  return 0;
	       if ((er = rdd_deleted(ClipMachineMemory, rd, &deleted, __PROC__)))
		  return er;
	       if (!deleted)
		  return 0;
	    }
	   rd->eof = rd->bof = rd->v_bof = 1;
	   if ((er = rdd_lastrec(ClipMachineMemory, rd, (int *) &lastrec, __PROC__)))
	      return er;
	   if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, lastrec + 1, 0, __PROC__)))
	      return er;
	}
       return 0;
    }
   if (ro->scopebottom)
    {
       int       found;

       int       ok = 0;

       CDX_HEADER hdr;

       void     *key = malloc(ro->bufsize);

       int       outrange;

       _CDX_HEADER(ro, &hdr);
       ro->stack[0].page = _rdd_uint(hdr.root);

       if (!ro->descend)
	{
	   if ((er =
		_cdx_back_search_tree(ClipMachineMemory, ro, ro->scopebottom, ro->sbotlen, 0, &found, &outrange, __PROC__)))
	      return er;
	   if (outrange > 0 && ro->stack[ro->level].pos > 0)
	    {
	       ro->stack[ro->level].pos--;
	       outrange = 0;
	    }
	}
       else
	{
	   if ((er = _cdx_search_tree(ClipMachineMemory, ro, ro->scopebottom, ro->sbotlen, 0, &found, &outrange, __PROC__)))
	      return er;
	}
       if (!outrange)
	{
	   if ((er = _cdx_recno_keyval(ClipMachineMemory, ro, ro->level, &rd->recno, key, __PROC__)))
	      return er;
	   if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, rd->recno, 1, __PROC__)))
	      return er;
	   if ((er = _cdx_checkscope(ClipMachineMemory, rd, ro, key, &ok, __PROC__)))
	      return er;
	}
       free(key);
       if (outrange || ok)
	{
	   unsigned int lastrec;

	   if ((er = rd->vtbl->lastrec(ClipMachineMemory, rd, (int *) &lastrec, __PROC__)))
	      return er;
	   rd->bof = rd->v_bof = rd->eof = 1;
	   if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, lastrec + 1, 0, __PROC__)))
	      return er;
	}
    }
   else
    {
       if (!ro->descend)
	{
	   if ((er = _cdx_last(ClipMachineMemory, ro, &out, __PROC__)))
	      return er;
	}
       else
	{
	   if ((er = _cdx_first(ClipMachineMemory, ro, &out, __PROC__)))
	      return er;
	}
       if (out)
	{
	   rd->bof = rd->v_bof = rd->eof = 1;
	}
       else
	{
	   if ((er = _cdx_recno(ClipMachineMemory, ro, ro->level, &rd->recno, __PROC__)))
	      return er;
	   if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, rd->recno, 1, __PROC__)))
	      return er;
	}
    }
   if ((er = rdd_checkfilter(ClipMachineMemory, rd, &fok, __PROC__)))
      return er;
   if (!fok)
    {
       if ((er = cdx_prev(ClipMachineMemory, rd, ro, __PROC__)))
	  return er;
       if (rd->bof)
	{
	   unsigned int lastrec;

	   if ((er = rd->vtbl->lastrec(ClipMachineMemory, rd, (int *) &lastrec, __PROC__)))
	      return er;
	   rd->eof = 1;
	   if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, lastrec + 1, 0, __PROC__)))
	      return er;
	   return 0;
	}
    }
   return 0;
}

static int
cdx_next(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__)
{
   CDX_HEADER hdr;

   int       out = 0, found;

   ClipVar   v, *vp;

   void     *key;

   int       ok = 1, fok = 0, deleted, lastrec, er;

   rd->bof = rd->v_bof = 0;
   if (rd->eof)
      return 0;

   rd->bof = rd->v_bof = rd->eof = 0;

   if (rd->filter && rd->filter->list)
    {
       ro->valid_stack = 0;
       while (++rd->filter->cursor < rd->filter->listlen)
	{
	   if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, rd->filter->list[rd->filter->cursor], 0, __PROC__)))
	      return er;
	   if (!(ClipMachineMemory->flags & DELETED_FLAG))
	      return 0;
	   if ((er = rdd_deleted(ClipMachineMemory, rd, &deleted, __PROC__)))
	      return er;
	   if (!deleted)
	      return 0;
	}
       if ((er = rd->vtbl->lastrec(ClipMachineMemory, rd, &lastrec, __PROC__)))
	  return er;
       rd->eof = 1;
       if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, lastrec + 1, 0, __PROC__)))
	  return er;
       if (rd->filter->listlen == 0)
	{
	   rd->bof = rd->v_bof = 1;
	   rd->filter->cursor = 0;
	}
       return 0;
    }
   else
    {
       key = malloc(ro->bufsize);
       if (rd->shared || !ro->valid_stack)
	{
	   char      cntcdx[4];

	   if ((er = rdd_read(ClipMachineMemory, &ro->index->file, 8, 4, cntcdx, __PROC__)))
	      return er;
	   if (!ro->valid_stack || ro->cntcdx != _rdd_backuint((unsigned char *) cntcdx))
	    {
	       if (ro->custom)
		{
		   memcpy(key, ro->key, ro->keysize);
		}
	       else
		{
		   if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->block, &v, 0)))
		      return er;
		   vp = _clip_vptr(&v);
		   if ((er = cdx_formatkey(ClipMachineMemory, ro, vp, key, __PROC__)))
		      return er;
		   _clip_destroy(ClipMachineMemory, &v);
		}
	       ro->curoffs = 0;

	       _CDX_HEADER(ro, &hdr);
	       ro->stack[0].page = _rdd_uint(hdr.root);
	       ro->level = 0;

	       if (!ro->descend)
		{
		   if ((er = _cdx_search_tree(ClipMachineMemory, ro, key, ro->bufsize, rd->recno, &found, NULL, __PROC__)))
		      return er;
		}
	       else
		{
		   if ((er = _cdx_back_search_tree(ClipMachineMemory, ro, key, ro->bufsize, rd->recno, &found, NULL, __PROC__)))
		      return er;
		}
	      //    _clip_destroy(ClipMachineMemory,&v);
	    }
	}

       while (ok || !fok)
	{
	   unsigned int recno;

	   if (!ro->descend)
	    {
	       if ((er = _cdx_next(ClipMachineMemory, rd, ro, &out, &recno, __PROC__)))
		  return er;
	    }
	   else
	    {
	       if ((er = _cdx_prev(ClipMachineMemory, rd, ro, &out, &recno, __PROC__)))
		  return er;
	    }
	   if (out)
	      break;
	   if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, recno, 1, __PROC__)))
	      return er;
	   if (!ro->scopetop && !ro->scopebottom && !rd->filter && !(ClipMachineMemory->flags & DELETED_FLAG))
	      break;
	   if ((er = _cdx_recno_keyval(ClipMachineMemory, ro, ro->level, &rd->recno, key, __PROC__)))
	      return er;
	   if ((er = _cdx_checkscope(ClipMachineMemory, rd, ro, key, &ok, __PROC__)))
	      return er;
	   if (ok > 0)
	    {
	       out = 1;
	       break;
	    }
	   if ((er = rdd_checkfilter(ClipMachineMemory, rd, &fok, __PROC__)))
	      return er;
	}
       free(key);
    }
   if (out)
    {
       unsigned int lastrec;

       if ((er = rd->vtbl->lastrec(ClipMachineMemory, rd, (int *) &lastrec, __PROC__)))
	  return er;
       rd->eof = 1;
       if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, lastrec + 1, 0, __PROC__)))
	  return er;
       ro->valid_stack = 0;
    }
   else
    {
       ro->valid_stack = 1;
    }
   return 0;
}

static int
cdx_prev(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__)
{
   CDX_HEADER hdr;

   int       out, found;

   ClipVar   v, *vp;

   void     *key;

   int       ok = 1, fok = 0, er;

   unsigned int oldrecno = rd->recno;

   unsigned int oldpage = ro->stack[ro->level].page;

   int       oldpos = ro->stack[ro->level].pos;

   int       lastrec, deleted;

   int       oldeof = rd->eof;

   void     *oldkey;

   if (rd->bof)
      return 0;
   if (rd->eof)
    {
       rd->eof = 0;
       if ((er = cdx_gobottom(ClipMachineMemory, rd, ro, __PROC__)))
	  return er;
       rd->eof = rd->bof;
       return 0;
    }
   oldkey = malloc(ro->bufsize);
   memcpy(oldkey, ro->key, ro->keysize);
   key = malloc(ro->bufsize);
   rd->bof = rd->v_bof = rd->eof = 0;

   if (rd->filter && rd->filter->list)
    {
       int       oldcursor = rd->filter->cursor;

       while (rd->filter->cursor > 0)
	{
	   if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, rd->filter->list[--rd->filter->cursor], 0, __PROC__)))
	      goto err1;
	   if (!(ClipMachineMemory->flags & DELETED_FLAG))
	      goto cont;
	   if ((er = rdd_deleted(ClipMachineMemory, rd, &deleted, __PROC__)))
	      goto err1;
	   if (!deleted)
	      goto cont;
	}
       rd->bof = rd->v_bof = 1;
       if (rd->filter->listlen == 0)
	{
	   rd->eof = 1;
	   if ((er = rdd_lastrec(ClipMachineMemory, rd, &lastrec, __PROC__)))
	      goto err1;
	   if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, lastrec + 1, 0, __PROC__)))
	      goto err1;
	}
       else
	{
	   rd->filter->cursor = oldcursor;
	   if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, rd->filter->list[oldcursor], 0, __PROC__)))
	      goto err1;
	}
       goto cont;
    }
   if (rd->shared || !ro->valid_stack)
    {
       char      cntcdx[4];

       if ((er = rdd_read(ClipMachineMemory, &ro->index->file, 8, 4, cntcdx, __PROC__)))
	  goto err1;
       if (!ro->valid_stack || ro->cntcdx != _rdd_backuint((unsigned char *) cntcdx))
	{
	   if (ro->custom)
	    {
	       memcpy(key, ro->key, ro->keysize);
	    }
	   else
	    {
	       if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->block, &v, 0)))
		  goto err1;
	       vp = _clip_vptr(&v);
	       if ((er = cdx_formatkey(ClipMachineMemory, ro, vp, key, __PROC__)))
		  goto err1;
	       _clip_destroy(ClipMachineMemory, &v);
	    }
	   ro->curoffs = 0;

	   _CDX_HEADER(ro, &hdr);
	   ro->stack[0].page = _rdd_uint(hdr.root);
	   ro->level = 0;

	   if (!ro->descend)
	    {
	       if ((er = _cdx_search_tree(ClipMachineMemory, ro, key, ro->bufsize, rd->recno, &found, NULL, __PROC__)))
		  goto err1;
	    }
	   else
	    {
	       if ((er = _cdx_back_search_tree(ClipMachineMemory, ro, key, ro->bufsize, rd->recno, &found, NULL, __PROC__)))
		  goto err1;
	    }
	  // _clip_destroy(ClipMachineMemory,&v);
	}
    }

   if ((er = rdd_checkfilter(ClipMachineMemory, rd, &fok, __PROC__)))
      goto err1;
   if (!fok)
    {
       if ((er = cdx_next(ClipMachineMemory, rd, ro, __PROC__)))
	  goto err1;
       oldrecno = rd->recno;
       oldeof = rd->eof;
       fok = 1;
    }

   while (ok || !fok)
    {
       unsigned int recno;

       rd->eof = 0;
       if (!ro->descend)
	{
	   if ((er = _cdx_prev(ClipMachineMemory, rd, ro, &out, &recno, __PROC__)))
	      goto err1;
	}
       else
	{
	   if ((er = _cdx_next(ClipMachineMemory, rd, ro, &out, &recno, __PROC__)))
	      goto err1;
	}
       if (out)
	  break;
       if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, recno, 1, __PROC__)))
	  goto err1;
       if (!ro->scopetop && !ro->scopebottom && !rd->filter && !(ClipMachineMemory->flags & DELETED_FLAG))
	  break;
       if ((er = _cdx_recno_keyval(ClipMachineMemory, ro, ro->level, &rd->recno, key, __PROC__)))
	  goto err1;
       if ((er = _cdx_checkscope(ClipMachineMemory, rd, ro, key, &ok, __PROC__)))
	  goto err1;
       if (ok < 0)
	{
	   out = 1;
	   break;
	}
       if ((er = rdd_checkfilter(ClipMachineMemory, rd, &fok, __PROC__)))
	  goto err1;
    }

   if (out)
    {
       rd->bof = rd->v_bof = 1;
       ro->stack[ro->level].page = oldpage;
       ro->stack[ro->level].pos = oldpos;
       if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, oldrecno, 0, __PROC__)))
	  goto err1;
       rd->eof = oldeof;
       memcpy(ro->key, oldkey, ro->keysize);
       ro->valid_stack = 0;
    }
   else
    {
       ro->valid_stack = 1;
    }
 cont:
   free(key);
   free(oldkey);
   return 0;
 err1:
   free(key);
   free(oldkey);
   return er;
}

static int
cdx_seek(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro,
	 ClipVar * v, int soft, int last, int *found, const char *__PROC__)
{
   CDX_HEADER hdr;

   void     *key = malloc(ro->bufsize);

   int       ok, fok, len, outrange, er;

   ro->level = 0;
   rd->bof = rd->v_bof = rd->eof = 0;
   if ((er = cdx_formatkey(ClipMachineMemory, ro, v, key, __PROC__)))
      return er;

   if (v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
      len = min(v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, ro->bufsize - (ro->type == 'X' ? 2 : 0));
   else
      len = ro->bufsize;

   _CDX_HEADER(ro, &hdr);
   ro->stack[0].page = _rdd_uint(hdr.root);

   if ((last && !ro->descend) || (!last && ro->descend))
    {
       if ((er = _cdx_back_search_tree(ClipMachineMemory, ro, key, len, 0, found, &outrange, __PROC__)))
	  return er;
       if (last && outrange < 0)
	{
	   outrange = 0;
	   ro->stack[ro->level].pos = 0;
	}
       else if (!last && outrange > 0)
	{
	   outrange = 0;
	   ro->stack[ro->level].pos--;
	}
    }
   else
    {
       if ((er = _cdx_search_tree(ClipMachineMemory, ro, key, len, 0, found, &outrange, __PROC__)))
	  return er;
    }

   if (outrange)
    {
       unsigned int lastrec;

       if ((er = rd->vtbl->lastrec(ClipMachineMemory, rd, (int *) &lastrec, __PROC__)))
	  return er;
       rd->eof = 1;
       free(key);
       if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, lastrec + 1, 0, __PROC__)))
	  return er;
       if (rd->area != -1)
	  ((DBWorkArea *) (ClipMachineMemory->areas->items_of_ClipVect[rd->area]))->found = 0;
       return 0;
    }
   if ((er = _cdx_recno(ClipMachineMemory, ro, ro->level, &rd->recno, __PROC__)))
      return er;
   if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, rd->recno, 1, __PROC__)))
      return er;
   if ((ro->type == 'C' || (ro->type == 'X' && ((char *) key)[0] == type_weight(CHARACTER_type_of_ClipVarType))) && !ro->binary)
      loc_write(rd->loc, key, ro->bufsize);
   if ((er = _cdx_checkscope(ClipMachineMemory, rd, ro, key, &ok, __PROC__)))
      return er;
   if (!ok)
    {
       if ((er = rdd_checkfilter(ClipMachineMemory, rd, &fok, __PROC__)))
	  return er;
       if (!fok)
	{
	   ClipVar   v, *vp;

	   void     *k = malloc(ro->bufsize);

	   if ((er = cdx_next(ClipMachineMemory, rd, ro, __PROC__)))
	      return er;
	   if (ro->custom)
	    {
	       memcpy(k, ro->key, ro->keysize);
	    }
	   else
	    {
	       if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->block, &v, 0)))
		  return er;
	       vp = _clip_vptr(&v);
	       if ((er = cdx_formatkey(ClipMachineMemory, ro, vp, k, __PROC__)))
		  return er;
	       _clip_destroy(ClipMachineMemory, &v);
	    }
	   *found = !_cdx_cmp(ro, k, key, len);
	   free(k);
	}
    }
   free(key);
   if (!(*found) && (!soft || ok))
    {
       unsigned int lastrec;

       if ((er = rd->vtbl->lastrec(ClipMachineMemory, rd, (int *) &lastrec, __PROC__)))
	  return er;
       rd->eof = 1;
       if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, lastrec + 1, 0, __PROC__)))
	  return er;
       ro->valid_stack = 0;
       if (rd->area != -1)
	  ((DBWorkArea *) (ClipMachineMemory->areas->items_of_ClipVect[rd->area]))->found = 0;
    }
   else
    {
       if (rd->area != -1)
	  ((DBWorkArea *) (ClipMachineMemory->areas->items_of_ClipVect[rd->area]))->found = 1;
       ro->valid_stack = 1;
    }

   return 0;
}

static int
cdx_gotokey(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, unsigned int keyno, int *ok, const char *__PROC__)
{
   CDX_PAGE  page;

   int       out, er;

   unsigned int c = 0;

   ro->level = 0;
   rd->bof = rd->v_bof = rd->eof = 0;
   *ok = 0;
   if (ro->scopetop)
    {
       CDX_HEADER hdr;

       unsigned int topno;

       int       found;

       _CDX_HEADER(ro, &hdr);
       ro->stack[0].page = _rdd_uint(hdr.root);
       if (!ro->descend)
	{
	   if ((er = _cdx_search_tree(ClipMachineMemory, ro, ro->scopetop, ro->stoplen, 0, &found, NULL, __PROC__)))
	      return er;
	}
       else
	{
	   if ((er = _cdx_back_search_tree(ClipMachineMemory, ro, ro->scopetop, ro->stoplen, 0, &found, NULL, __PROC__)))
	      return er;
	}
       if ((er = _cdx_keyno(ClipMachineMemory, ro, ro->stack[ro->level].page, ro->stack[ro->level].pos, &topno, __PROC__)))
	  return er;
       keyno += topno - 1;
    }
   if (!ro->descend)
    {
       if ((er = _cdx_first(ClipMachineMemory, ro, &out, __PROC__)))
	  return er;
       if (out)
	{
	   rd->bof = rd->v_bof = rd->eof = 1;
	   return 0;
	}
       if ((er = rdd_read(ClipMachineMemory, &ro->index->file, ro->stack[ro->level].page, 12, &page, __PROC__)))
	  return er;
       while (c + _rdd_ushort(page.nkeys) < keyno)
	{
	   if (_rdd_uint(page.right) == 0xffffffff)
	    {
	       unsigned int lastrec;

	       if ((er = rd->vtbl->lastrec(ClipMachineMemory, rd, (int *) &lastrec, __PROC__)))
		  return er;
	       rd->eof = 1;
	       if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, lastrec + 1, 0, __PROC__)))
		  return er;
	       return 0;
	    }
	   c += _rdd_ushort(page.nkeys);
	   ro->stack[ro->level].page = _rdd_uint(page.right);
	   if ((er = rdd_read(ClipMachineMemory, &ro->index->file, _rdd_uint(page.right), 12, &page, __PROC__)))
	      return er;
	}
       ro->stack[ro->level].pos = keyno - c - 1;
       if ((er = _cdx_recno(ClipMachineMemory, ro, ro->level, &rd->recno, __PROC__)))
	  return er;
       if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, rd->recno, 1, __PROC__)))
	  return er;
    }
   else
    {
       if ((er = _cdx_last(ClipMachineMemory, ro, &out, __PROC__)))
	  return er;
       if (out)
	{
	   rd->bof = rd->v_bof = rd->eof = 1;
	   return 0;
	}
       if ((er = rdd_read(ClipMachineMemory, &ro->index->file, ro->stack[ro->level].page, 12, &page, __PROC__)))
	  return er;
       while (c + _rdd_ushort(page.nkeys) < keyno)
	{
	   if (_rdd_uint(page.left) == 0xffffffff)
	    {
	       unsigned int lastrec;

	       if ((er = rd->vtbl->lastrec(ClipMachineMemory, rd, (int *) &lastrec, __PROC__)))
		  return er;
	       rd->eof = 1;
	       if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, lastrec + 1, 0, __PROC__)))
		  return er;
	       return 0;
	    }
	   c += _rdd_ushort(page.nkeys);
	   ro->stack[ro->level].page = _rdd_uint(page.left);
	   if ((er = rdd_read(ClipMachineMemory, &ro->index->file, _rdd_uint(page.left), 12, &page, __PROC__)))
	      return er;
	}
       ro->stack[ro->level].pos = _rdd_ushort(page.nkeys) - (keyno - c);
       if ((er = _cdx_recno(ClipMachineMemory, ro, ro->level, &rd->recno, __PROC__)))
	  return er;
       if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, rd->recno, 1, __PROC__)))
	  return er;
    }
   if (ro->scopetop || ro->scopebottom)
    {
       ClipVar   v, *vp;

       void     *key = malloc(ro->bufsize);

       int       ook;

       if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->block, &v, 0)))
	  return er;
       vp = _clip_vptr(&v);
       if ((er = cdx_formatkey(ClipMachineMemory, ro, vp, key, __PROC__)))
	  return er;
       _clip_destroy(ClipMachineMemory, &v);
       if ((ro->type == 'C'
	    || (ro->type == 'X' && ((char *) key)[0] == type_weight(CHARACTER_type_of_ClipVarType))) && !ro->binary)
	  loc_write(rd->loc, key, ro->bufsize);
       if ((er = _cdx_checkscope(ClipMachineMemory, rd, ro, key, &ook, __PROC__)))
	  return er;
       free(key);
       if (ook)
	{
	   unsigned int lastrec;

	   if ((er = rd->vtbl->lastrec(ClipMachineMemory, rd, (int *) &lastrec, __PROC__)))
	      return er;
	   rd->bof = rd->v_bof = rd->eof = 1;
	   if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, lastrec + 1, 0, __PROC__)))
	      return er;
	}
       else
	{
	   *ok = 1;
	}
    }
   else
    {
       *ok = 1;
    }
   return 0;
}

static int
cdx_descend(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, int descend, int *res, const char *__PROC__)
{
   *res = ro->descend;
   if (descend != -1)
      ro->descend = descend;
   return 0;
}

static int
cdx_addkey(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, ClipVar * v, const char *__PROC__)
{
   int       r = __cdx_addkey(ClipMachineMemory, rd->area, ro, v, rd->loc, rd->recno, __PROC__);

   int       er;

   char      cntcdx[4];

   ro->valid_stack = 0;
   ro->curoffs = 0;
   if (!r)
    {
       if ((er = rdd_read(ClipMachineMemory, &ro->index->file, 8, 4, cntcdx, __PROC__)))
	  return er;
       ro->cntcdx = _rdd_backuint((unsigned char *) cntcdx) + 1;
       _rdd_put_backuint((unsigned char *) cntcdx, ro->cntcdx);
       if ((er = rdd_write(ClipMachineMemory, &ro->index->file, 8, 4, cntcdx, __PROC__)))
	  return er;
    }
   return r;
}

static int
cdx_delkey(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__)
{
   CDX_HEADER hdr;

   int       found, er;

   ClipVar   vv, *vp;

   void     *key = malloc(ro->bufsize);

   char      cntcdx[4];

   ro->level = 0;
   _CDX_HEADER(ro, &hdr);
   ro->stack[0].page = _rdd_uint(hdr.root);

   if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->block, &vv, 0)))
      goto err;
   vp = _clip_vptr(&vv);
   if ((er = cdx_formatkey(ClipMachineMemory, ro, vp, key, __PROC__)))
      goto err;
   _clip_destroy(ClipMachineMemory, &vv);

   if ((er = _cdx_search_tree(ClipMachineMemory, ro, key, ro->bufsize, rd->recno, &found, NULL, __PROC__)))
      goto err;
   if (!found)
      goto cont;

   if ((er = _cdx_delkey(ClipMachineMemory, ro, __PROC__)))
      goto err;
   ro->valid_stack = 0;

   if ((er = rdd_read(ClipMachineMemory, &ro->index->file, 8, 4, cntcdx, __PROC__)))
      goto err;
   ro->cntcdx = _rdd_backuint((unsigned char *) cntcdx) + 1;
   _rdd_put_backuint((unsigned char *) cntcdx, ro->cntcdx);
   if ((er = rdd_write(ClipMachineMemory, &ro->index->file, 8, 4, cntcdx, __PROC__)))
      goto err;
   goto cont;
 err:
   free(key);
   return er;
 cont:
   free(key);
   ro->curoffs = 0;
   return 0;
}

static unsigned long long
_cdx_longlong(double d)
{
   int       i;

   unsigned long long res;

   unsigned char *cd = (unsigned char *) &res;

   unsigned int *id = (unsigned int *) &res;

   for (i = 0; i < 8; i++)
      ((unsigned char *) &res)[i] = ((unsigned char *) &d)[7 - i];
   if (d < 0)
    {
       id[0] = ~(id[0]);
       id[1] = ~(id[1]);
    }
   else
    {
       cd[0] = cd[0] ^ 0x80;
    }
   return res;
}

static double
_cdx_double(unsigned long long l)
{
   int       i;

   double    d;

   unsigned long long ll = l;

   unsigned char *cl = (unsigned char *) &l;

   unsigned char *cd = (unsigned char *) &d;

   unsigned int *il = (unsigned int *) &l;

   cl[0] = cl[0] ^ 0x80;
   for (i = 0; i < 8; i++)
      cd[i] = cl[7 - i];
   if (ll != _cdx_longlong(d))
    {
       cl[0] = cl[0] ^ 0x80;
       il[0] = ~(il[0]);
       il[1] = ~(il[1]);
       for (i = 0; i < 8; i++)
	  cd[i] = cl[7 - i];
    }
   return d;
}

static void
_cdx_keyvar(RDD_ORDER * ro, void *key, char *lockey, ClipVar * v)
{
   switch (ro->type)
    {
    case 'C':
       v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
       v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = ro->bufsize;
       if (ro->binary)
	{
	   v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = (char *) key;
	}
       else
	{
	   memcpy(lockey, key, ro->bufsize);
	   loc_read(ro->index->loc, (unsigned char *) lockey, ro->bufsize);
	   v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = lockey;
	}
       break;
    case 'N':
       v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
       v->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = _cdx_double(*(unsigned long long *) key);
       v->ClipType_t_of_ClipVar.dec_of_ClipType = ro->dec;
       break;
    case 'D':
       v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATE_type_of_ClipVarType;
       v->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar = (long) _cdx_double(*(unsigned long long *) key);
       break;
    case 'T':
       v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATETIME_type_of_ClipVarType;
       v->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar = _rdd_backuint((unsigned char *) key);
       v->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar = _rdd_backuint((unsigned char *) key + 4);
       break;
    case 'L':
       v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
       v->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = ((*(char *) key) == 'T');
       break;
    default:
       memset(v, 0, sizeof(ClipVar));
       break;
    }
}

static int
cdx_formatkey(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, ClipVar * var, void *res, const char *__PROC__)
{
   memset(res, 0, ro->bufsize);
   switch (var->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
    {
    case CHARACTER_type_of_ClipVarType:
       {
	  int       len;

	  if (ro->type == 'X')
	   {
	      len = min(var->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, ro->bufsize - 1);
	      *(char *) res = type_weight(CHARACTER_type_of_ClipVarType);
	      memcpy(res + 1, var->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, len);
	      if (ro->ic && !ro->binary)
		 _clip_upstr(res + 1, len);
	      memset(res + len + 1, 0, ro->bufsize - 1 - len);
	   }
	  else
	   {
	      len = min(var->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, ro->bufsize);
	      memcpy(res, var->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, len);
	      if (ro->ic)
		 _clip_upstr(res, len);
	      memset(res + len, ' ', ro->bufsize - len);
	   }
	  break;
       }
    case NUMERIC_type_of_ClipVarType:
       {
	  if (ro->type == 'X')
	   {
	      char     *cres = (char *) res;

	      *(char *) res = type_weight(NUMERIC_type_of_ClipVarType);
	      *(((char *) res) + 1) = 0;
	      cres++;
	      res = (void *) cres;
	   }
	  if (var->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar == 0)
	     var->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = 0;
	  *(unsigned long long *) res = _cdx_longlong(var->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
	  break;
       }
    case DATE_type_of_ClipVarType:
       {
	  if (ro->type == 'X')
	   {
	      char     *cres = (char *) res;

	      *(char *) res = type_weight(DATE_type_of_ClipVarType);
	      cres++;
	      res = (void *) cres;
	   }
	  *(unsigned long long *) res = _cdx_longlong((double) var->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar);
	  break;
       }
    case DATETIME_type_of_ClipVarType:
       {
	  if (ro->type == 'X')
	   {
	      char     *cres = (char *) res;

	      *(char *) res = type_weight(DATETIME_type_of_ClipVarType);
	      cres++;
	      res = (void *) cres;
	   }
	  _rdd_put_backuint((unsigned char *) res, var->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar);
	  _rdd_put_backuint((unsigned char *) res + 4, var->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar);
	  break;
       }
    case LOGICAL_type_of_ClipVarType:
       {
	  if (ro->type == 'X')
	   {
	      char     *cres = (char *) res;

	      *(char *) res = type_weight(LOGICAL_type_of_ClipVarType);
	      cres++;
	      res = (void *) cres;
	   }
	  *(char *) res = var->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar ? 'T' : 'F';
	  break;
       }
    default:
       break;
    }
   return 0;
}

static int
cdx_keyno(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, int *keyno, const char *__PROC__)
{
   CDX_HEADER hdr;

   int       found, ok, er;

   ClipVar   v, *vp;

   void     *key = malloc(ro->bufsize);

   unsigned int oldrecno = rd->recno;

   ro->level = 0;
   *keyno = 0;
   if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->block, &v, 0)))
      return er;
   vp = _clip_vptr(&v);
   if ((er = cdx_formatkey(ClipMachineMemory, ro, vp, key, __PROC__)))
      return er;
   _clip_destroy(ClipMachineMemory, &v);

   _CDX_HEADER(ro, &hdr);
   ro->stack[0].page = _rdd_uint(hdr.root);

   if (!ro->descend)
    {
       if ((er = _cdx_search_tree(ClipMachineMemory, ro, key, ro->bufsize, rd->recno, &found, NULL, __PROC__)))
	  return er;
    }
   else
    {
       if ((er = _cdx_back_search_tree(ClipMachineMemory, ro, key, ro->bufsize, rd->recno, &found, NULL, __PROC__)))
	  return er;
    }
   if (!found)
      return 0;
   if ((er = _cdx_recno(ClipMachineMemory, ro, ro->level, &rd->recno, __PROC__)))
      return er;
   if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, rd->recno, 1, __PROC__)))
      return er;
   if ((ro->type == 'C' || (ro->type == 'X' && ((char *) key)[0] == type_weight(CHARACTER_type_of_ClipVarType))) && !ro->binary)
      loc_write(rd->loc, key, ro->bufsize);
   if ((er = _cdx_checkscope(ClipMachineMemory, rd, ro, key, &ok, __PROC__)))
      return er;
   free(key);
   if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, oldrecno, 0, __PROC__)))
      return er;
   if (ok)
      return 0;
   if ((er =
	_cdx_keyno(ClipMachineMemory, ro, ro->stack[ro->level].page,
		   ro->stack[ro->level].pos, (unsigned int *) keyno, __PROC__)))
      return er;
   if (ro->scopetop)
    {
       unsigned int topno;

       ro->stack[0].page = _rdd_uint(hdr.root);
       ro->level = 0;
       if (!ro->descend)
	{
	   if ((er = _cdx_search_tree(ClipMachineMemory, ro, ro->scopetop, ro->stoplen, 0, &found, NULL, __PROC__)))
	      return er;
	}
       else
	{
	   if ((er = _cdx_back_search_tree(ClipMachineMemory, ro, ro->scopetop, ro->stoplen, 0, &found, NULL, __PROC__)))
	      return er;
	}
       if ((er =
	    _cdx_keyno(ClipMachineMemory, ro, ro->stack[ro->level].page,
		       ro->stack[ro->level].pos, (unsigned int *) &topno, __PROC__)))
	  return er;
       *keyno -= topno - 1;
    }
   return 0;
}

static int
cdx_lastkey(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, int *lastkey, const char *__PROC__)
{
   unsigned int b = 1, e = 0;

   int       found, er;

   CDX_HEADER hdr;

   _CDX_HEADER(ro, &hdr);
   if (ro->scopetop)
    {
       ro->stack[0].page = _rdd_uint(hdr.root);
       ro->level = 0;
       if (!ro->descend)
	{
	   if ((er = _cdx_search_tree(ClipMachineMemory, ro, ro->scopetop, ro->stoplen, 0, &found, NULL, __PROC__)))
	      return er;
	}
       else
	{
	   if ((er = _cdx_back_search_tree(ClipMachineMemory, ro, ro->scopetop, ro->stoplen, 0, &found, NULL, __PROC__)))
	      return er;
	}
       if ((er = _cdx_keyno(ClipMachineMemory, ro, ro->stack[ro->level].page, ro->stack[ro->level].pos, &b, __PROC__)))
	  return er;
    }

   if (ro->scopebottom)
    {
       ro->stack[0].page = _rdd_uint(hdr.root);
       ro->level = 0;
       if (!ro->descend)
	{
	   if ((er = _cdx_back_search_tree(ClipMachineMemory, ro, ro->scopebottom, ro->sbotlen, 0, &found, NULL, __PROC__)))
	      return er;
	}
       else
	{
	   if ((er = _cdx_search_tree(ClipMachineMemory, ro, ro->scopebottom, ro->sbotlen, 0, &found, NULL, __PROC__)))
	      return er;
	}
       if ((er = _cdx_keyno(ClipMachineMemory, ro, ro->stack[ro->level].page, ro->stack[ro->level].pos, &e, __PROC__)))
	  return er;
    }
   else
    {
       int       out;

       if (!ro->descend)
	{
	   if ((er = _cdx_last(ClipMachineMemory, ro, &out, __PROC__)))
	      return er;
	}
       else
	{
	   if ((er = _cdx_first(ClipMachineMemory, ro, &out, __PROC__)))
	      return er;
	}
       if (!out)
	  if ((er = _cdx_keyno(ClipMachineMemory, ro, ro->stack[ro->level].page, ro->stack[ro->level].pos, &e, __PROC__)))
	     return er;
    }
   (*lastkey) = max(0, (int) (e - b + 1));
   return 0;
}

static int
cdx_info(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, int cmd, const char *__PROC__)
{
   switch (cmd)
    {
    case DBOI_CONDITION:
       {
	  if (ro->cforexpr)
	     _clip_retc(ClipMachineMemory, ro->cforexpr);
	  else
	     _clip_retc(ClipMachineMemory, "");
	  break;
       }
    case DBOI_CUSTOM:
       {
	  _clip_retl(ClipMachineMemory, 0);
	  break;
       }
    case DBOI_HPLOCKING:
       {
	  _clip_retl(ClipMachineMemory, 0);
	  break;
       }
    case DBOI_ISCOND:
       {
	  _clip_retl(ClipMachineMemory,
		     ro->bforexpr.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType);
	  break;
       }
    case DBOI_ISDESC:
       {
	  _clip_retl(ClipMachineMemory, ro->descend);
	  break;
       }
    case DBOI_ORDERCOUNT:
       {
	  _clip_retni(ClipMachineMemory, ro->index->norders);
	  break;
       }
    case DBOI_SCOPEBOTTOM:
       {
	  _clip_clone(ClipMachineMemory, RETPTR(ClipMachineMemory), &ro->scopebottomvar);
	  break;
       }
    case DBOI_SCOPETOP:
       {
	  _clip_clone(ClipMachineMemory, RETPTR(ClipMachineMemory), &ro->scopetopvar);
	  break;
       }
    case DBOI_SKIPUNIQUE:
       {
	  _clip_retl(ClipMachineMemory, 1);
	  break;
       }
    }
   return 0;
}

static int
cdx_keyvalue(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, ClipVar * v, const char *__PROC__)
{
   CDX_HEADER hdr;

   ClipVar   vv, *vp;

   int       found, er;

   void     *key = malloc(ro->bufsize);

   if (rd->eof)
      return 0;
   if (ro->custom)
    {
       _clip_var_str(ro->key, ro->keysize, &vv);
       vp = _clip_vptr(&vv);
    }
   else
    {
       ro->level = 0;
       _CDX_HEADER(ro, &hdr);
       ro->stack[0].page = _rdd_uint(hdr.root);

       if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->block, &vv, 0)))
	  goto err;
       vp = _clip_vptr(&vv);
       if ((er = cdx_formatkey(ClipMachineMemory, ro, vp, key, __PROC__)))
	  goto err;
       _clip_destroy(ClipMachineMemory, &vv);

       if ((er = _cdx_search_tree(ClipMachineMemory, ro, key, ro->bufsize, rd->recno, &found, NULL, __PROC__)))
	  goto err;
       if (!found)
	  return 0;
       if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->block, &vv, 0)))
	  goto err;
       vp = _clip_vptr(&vv);
    }
   _clip_clone(ClipMachineMemory, v, vp);
   _clip_destroy(ClipMachineMemory, &vv);
   free(key);
   return 0;
 err:
   free(key);
   return er;
}

static int
cdx_setscope(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro,
	     ClipVar * top, ClipVar * bottom, unsigned int *map, int bytes, int exact, const char *__PROC__)
{
   void     *t = NULL;

   void     *b = NULL;

   unsigned int firstrecno = 0, lastrecno = 0, recno;

   unsigned int lastrecno1 = -1;

   CDX_HEADER hdr;

   int       found, out = 0;

   CDX_LEAF  leaf;

   int       bout, tout;

   int       tlen = ro->bufsize;

   int       blen = ro->bufsize;

   int       er;

   if (top)
    {
       t = malloc(ro->bufsize);
       if ((er = cdx_formatkey(ClipMachineMemory, ro, top, t, __PROC__)))
	  goto err;
       if (top->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
	{
	   if (exact)
	    {
	       tlen = ro->bufsize;
	    }
	   else
	    {
	       tlen = top->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
	       if (ro->type == 'X')
		  tlen++;
	    }
	}
       else if (ro->type == 'X')
	{
	   blen = 9;
	}
    }
   if (bottom)
    {
       b = malloc(ro->bufsize);
       if ((er = cdx_formatkey(ClipMachineMemory, ro, bottom, b, __PROC__)))
	  goto err;
       if (bottom->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
	{
	   if (exact)
	    {
	       blen = ro->bufsize;
	    }
	   else
	    {
	       blen = bottom->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
	       if (ro->type == 'X')
		  blen++;
	    }
	}
       else if (ro->type == 'X')
	{
	   blen = 9;
	}
    }
   _CDX_HEADER(ro, &hdr);
   ro->stack[0].page = _rdd_uint(hdr.root);
   if (b)
    {
       ro->level = 0;
       if ((er = _cdx_back_search_tree(ClipMachineMemory, ro, b, blen, 0, &found, &bout, __PROC__)))
	  goto err;
       if (bout < 0)
	  goto e;
       if ((er = _cdx_recno(ClipMachineMemory, ro, ro->level, &lastrecno, __PROC__)))
	  goto err;
       if (!found && !bout)
	{
	   if ((er =
		rdd_read(ClipMachineMemory, &ro->index->file, ro->stack[ro->level].page, sizeof(CDX_LEAF), &leaf, __PROC__)))
	      goto err;
	   if ((er = _cdx_next_rm(ClipMachineMemory, ro, &leaf, &bout, 0, __PROC__)))
	      goto err;
	   if ((er = _cdx_recno(ClipMachineMemory, ro, ro->level, &lastrecno1, __PROC__)))
	      goto err;
	}
    }
   if (t)
    {
       ro->level = 0;
       if ((er = _cdx_search_tree(ClipMachineMemory, ro, t, tlen, 0, &found, &tout, __PROC__)))
	  goto err;
       if (tout > 0)
	  goto e;
       if ((er = _cdx_recno(ClipMachineMemory, ro, ro->level, &firstrecno, __PROC__)))
	  goto err;
    }
   else
    {
       if ((er = _cdx_first(ClipMachineMemory, ro, &out, __PROC__)))
	  goto err;
    }
   if (!bout && !tout && firstrecno == lastrecno1)
      goto e;
/* --------------------------- */
   if ((er = rdd_read(ClipMachineMemory, &ro->index->file, ro->stack[ro->level].page, sizeof(CDX_LEAF), &leaf, __PROC__)))
      goto err;
   recno = -1;
   recno = *(unsigned int *) (leaf.keys + (ro->stack[ro->level].pos) * leaf.bytes) & _rdd_uint(leaf.recmask);

   while (!out && recno != lastrecno)
    {
       _rm_setbit(map, bytes, recno);
       if ((er = _cdx_next_rm(ClipMachineMemory, ro, &leaf, &out, 0, __PROC__)))
	  goto err;
       recno = *(unsigned int *) (leaf.keys + (ro->stack[ro->level].pos) * leaf.bytes) & _rdd_uint(leaf.recmask);
    }
   if (lastrecno && (recno == lastrecno))
      _rm_setbit(map, bytes, recno);
/* --------------------------- */
 e:
   ro->valid_stack = 0;
   if (t)
      free(t);
   if (b)
      free(b);
   return 0;
 err:
   ro->valid_stack = 0;
   if (t)
      free(t);
   if (b)
      free(b);
   return er;
}

static int
cdx_destroy(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__)
{
   CDX_HEADER chdr;

   RDD_ORDER *tagbag = (RDD_ORDER *) malloc(sizeof(RDD_ORDER));

   char      tn[11];

   int       found, i, er;

   tagbag->level = 0;
   if ((er = rdd_read(ClipMachineMemory, &ro->index->file, 0, sizeof(CDX_HEADER), &chdr, __PROC__)))
      goto err;
   if ((er = _cdx_init_order(ClipMachineMemory, rd->area, ro->index, 0, tagbag, __PROC__)))
      goto err;
   strcpy(tn, ro->name);
   memset(tn + strlen(tn), 0x20, 10 - strlen(tn));
   tn[10] = 0;
   for (i = 0; i < 10; i++)
      tn[i] = toupper(tn[i]);
   tagbag->stack[0].page = _rdd_uint(chdr.root);
   if ((er = _cdx_search_tree(ClipMachineMemory, tagbag, tn, 10, 0, &found, NULL, __PROC__)))
      goto err;
   if (found)
    {
       if ((er = _cdx_delkey(ClipMachineMemory, tagbag, __PROC__)))
	  goto err;
    }
   free(tagbag);
   return 0;
 err:
   free(tagbag);
   return er;
}

static int
cdx_ii_create(ClipMachine * ClipMachineMemory, RDD_INDEX * ri, const char *__PROC__)
{
   CDX_HEADER chdr;

   RDD_ORDER *tagbag = malloc(sizeof(RDD_ORDER));

   unsigned int root;

   int       er;

   ri->loc = dbf_get_locale(ClipMachineMemory);
   root = sizeof(CDX_HEADER);
   memset(&chdr, 0, sizeof(CDX_HEADER));
   _rdd_put_uint(chdr.root, sizeof(CDX_HEADER));
   _rdd_put_ushort(chdr.keysize, 10);
   chdr.options = 0xE0;
   chdr.sign = 1;
   _rdd_put_ushort(chdr.totlen, 1);
   _rdd_put_ushort(chdr.forlen, 1);
   _rdd_put_ushort(chdr.explen, 1);
   if ((er = rdd_write(ClipMachineMemory, &ri->file, 0, sizeof(CDX_HEADER), &chdr, __PROC__)))
      goto err;
   if ((er = _cdx_getfreepage(ClipMachineMemory, ri, &root, __PROC__)))
      goto err;
   if ((er = _cdx_init_order(ClipMachineMemory, -1, ri, 0, tagbag, __PROC__)))
      goto err;
   if ((er = _cdx_init_rootleaf(ClipMachineMemory, tagbag, root, __PROC__)))
      goto err;

   destroy_rdd_order(ClipMachineMemory, tagbag);
   return 0;
 err:
   destroy_rdd_order(ClipMachineMemory, tagbag);
   return er;
}

static int
cdx_ii_createtag(ClipMachine * ClipMachineMemory, RDD_INDEX * ri, const char *tag,
		 const char *expr, RDD_ORDER ** rop, const char *__PROC__)
{
   CDX_HEADER chdr;

   RDD_ORDER *tagbag = malloc(sizeof(RDD_ORDER));

   char      tagname[11];

   char      tn[11];

   unsigned int newheader, dummy;

   ClipVar   v, vv, *vp;

   ClipVar   block;

   int       found, er;

   RDD_ORDER *ro = (RDD_ORDER *) malloc(sizeof(RDD_ORDER));

   CDX_HEADER hdr;

   unsigned int newroot;

   ro->level = 0;
   memset(&v, 0, sizeof(ClipVar));
   memset(&vv, 0, sizeof(ClipVar));
   memset(&block, 0, sizeof(ClipVar));

   if ((er = rdd_read(ClipMachineMemory, &ri->file, 0, sizeof(CDX_HEADER), &chdr, __PROC__)))
      goto err;
   if ((er = _cdx_init_order(ClipMachineMemory, -1, ri, 0, tagbag, __PROC__)))
      goto err;

   tagbag->unique = 1;
   strncpy(tagname, tag, 10);
   tagname[10] = 0;
   _clip_upstr(tagname, strlen(tagname));
   v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   v.ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = tagname;
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = strlen(tagname);
   if ((er = _cdx_getfreepage(ClipMachineMemory, ri, &newheader, __PROC__)))
      goto err;
   if ((er = _cdx_getfreepage(ClipMachineMemory, ri, &dummy, __PROC__)))
      goto err;

   strcpy(tn, tagname);
   memset(tn + strlen(tn), 0x20, 10 - strlen(tn));
   tn[10] = 0;
   tagbag->stack[0].page = _rdd_uint(chdr.root);
   if ((er = _cdx_search_tree(ClipMachineMemory, tagbag, tn, 10, 0, &found, NULL, __PROC__)))
      goto err;
   if (found)
    {
       if ((er = _cdx_delkey(ClipMachineMemory, tagbag, __PROC__)))
	  goto err;
    }
   if ((er = __cdx_addkey(ClipMachineMemory, -1, tagbag, &v, ri->loc, newheader, __PROC__)))
      goto err;

   memset(ro, 0, sizeof(RDD_ORDER));
   ro->vtbl = ri->vtbl;
   ro->index = ri;

   ro->header = newheader;
   ro->name = (char *) malloc(strlen(tag) + 1);
   strcpy(ro->name, tag);
   ro->orderno = -1;

   ro->expr = (char *) malloc(strlen(expr) + 1);
   strcpy(ro->expr, expr);
   {
      char      e[1024];

      sprintf(e, "{||%s}", ro->expr);

      if ((er = _clip_eval_macro(ClipMachineMemory, e, strlen(e), &block)))
	 return er;
   }
   if ((er = rdd_calc(ClipMachineMemory, -1, &block, &vv, 0)))
      return er;
   vp = _clip_vptr(&vv);

   switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
    {
    case CHARACTER_type_of_ClipVarType:
       ro->bufsize = ro->keysize = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
       ro->dec = 0;
       ro->type = 'C';
       break;
    case NUMERIC_type_of_ClipVarType:
       ro->keysize = (vp->ClipType_t_of_ClipVar.len_of_ClipType ? vp->ClipType_t_of_ClipVar.len_of_ClipType : 10);
       ro->dec = vp->ClipType_t_of_ClipVar.dec_of_ClipType;
       ro->bufsize = 8;
       ro->type = 'N';
       break;
    case LOGICAL_type_of_ClipVarType:
       ro->keysize = 1;
       ro->dec = 0;
       ro->bufsize = 1;
       ro->type = 'L';
       break;
    case DATE_type_of_ClipVarType:
       ro->keysize = 8;
       ro->dec = 0;
       ro->bufsize = 8;
       ro->type = 'D';
       break;
    case DATETIME_type_of_ClipVarType:
       ro->keysize = 8;
       ro->dec = 0;
       ro->bufsize = 8;
       ro->type = 'T';
       break;
    default:
       return rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_baddata);
    }
   ro->key = malloc(ro->bufsize);
   ro->iikey = malloc(ro->bufsize);

   ri->orders = (RDD_ORDER **) realloc(ri->orders, (ri->norders + 1) * sizeof(RDD_ORDER *));
   ri->orders[ri->norders] = ro;
   ri->norders++;

   memset(&hdr, 0, sizeof(CDX_HEADER));
   if ((er = rdd_write(ClipMachineMemory, &ri->file, ro->header, sizeof(CDX_HEADER), (char *) &hdr, __PROC__)))
      goto err;
   if ((er = _cdx_getfreepage(ClipMachineMemory, ri, &newroot, __PROC__)))
      goto err;
   _rdd_put_uint(hdr.root, newroot);
   _rdd_put_uint(hdr.keysize, ro->bufsize);
   hdr.options = 0x62;
   hdr.sign = 1;
   _rdd_put_ushort(hdr.sort, ro->descend);
   _rdd_put_ushort(hdr.explen, strlen(ro->expr) + 1);
   _rdd_put_ushort(hdr.totlen, _rdd_ushort(hdr.explen));
   strcpy((char *) hdr.keyforexpr, ro->expr);
   _rdd_put_ushort(hdr.forlen, 1);

   if ((er = rdd_write(ClipMachineMemory, &ri->file, ro->header, sizeof(CDX_HEADER), (char *) &hdr, __PROC__)))
      goto err;

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
   if ((er = rdd_write(ClipMachineMemory, &ri->file, ro->header, sizeof(CDX_HEADER), (char *) &hdr, __PROC__)))
      goto err;

   if ((er = _cdx_init_rootleaf(ClipMachineMemory, ro, newroot, __PROC__)))
      goto err;

   *rop = ro;

   _clip_destroy(ClipMachineMemory, &vv);
   _clip_destroy(ClipMachineMemory, &block);
   destroy_rdd_order(ClipMachineMemory, tagbag);
   return 0;
 err:
   _clip_destroy(ClipMachineMemory, &vv);
   _clip_destroy(ClipMachineMemory, &block);
   destroy_rdd_order(ClipMachineMemory, tagbag);
   return er;
}

static int
cdx_ii_open(ClipMachine * ClipMachineMemory, RDD_INDEX * ri, const char *__PROC__)
{
   ri->loc = dbf_get_locale(ClipMachineMemory);
   ri->orders = malloc(0);
   ri->norders = 0;
   return 0;
}

static int
cdx_ii_opentag(ClipMachine * ClipMachineMemory, RDD_INDEX * ri, const char *tag, RDD_ORDER ** rop, const char *__PROC__)
{
   CDX_HEADER chdr;

   CDX_PAGE  page;

   CDX_LEAF *leaf;

   CDX_BRANCH *branch;

   RDD_ORDER *tagbag = (RDD_ORDER *) calloc(1, sizeof(RDD_ORDER));

   RDD_ORDER *ro;

   char      tn[11];

   unsigned int header;

   ClipVar   v;

   int       found, er;

   tagbag->level = 0;
   if ((er = _cdx_init_order(ClipMachineMemory, -1, ri, 0, tagbag, __PROC__)))
      goto err;
   _clip_var_str(tag, strlen(tag), &v);
   _clip_upstr(v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf,
	       v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);

   _CDX_HEADER(tagbag, &chdr);
   tagbag->stack[0].page = _rdd_uint(chdr.root);
   strcpy(tn, tag);
   _clip_upstr(tn, strlen(tn));
   memset(tn + strlen(tn), 0x20, 10 - strlen(tn));
   tn[10] = 0;
   if ((er = _cdx_search_tree(ClipMachineMemory, tagbag, tn, 10, 0, &found, NULL, __PROC__)))
      goto err;
   _CDX_PAGE(tagbag, tagbag->stack[tagbag->level].page, &page);
   if (!leaf)
      return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_badleaf);

   if ((er = _cdx_recno_keyval(ClipMachineMemory, tagbag, tagbag->level, &header, tn, __PROC__)))
      return er;

   if (!found)
      return rdd_err(ClipMachineMemory, EG_OPEN, 0, __FILE__, __LINE__, __PROC__, er_notag);

   ro = (RDD_ORDER *) malloc(sizeof(RDD_ORDER));
   if ((er = _cdx_init_order(ClipMachineMemory, -1, ri, header, ro, __PROC__)))
      return er;

   _clip_destroy(ClipMachineMemory, &v);

   ro->key = malloc(ro->bufsize);
   ro->iikey = malloc(ro->bufsize);
   ro->name = (char *) malloc(strlen(tag) + 1);
   strcpy(ro->name, tag);
   ri->norders++;
   ri->orders = (RDD_ORDER **) realloc(ri->orders, ri->norders * sizeof(RDD_ORDER *));
   ri->orders[ri->norders - 1] = ro;
   destroy_rdd_order(ClipMachineMemory, tagbag);
   *rop = ro;
   return 0;
 err:
   destroy_rdd_order(ClipMachineMemory, tagbag);
   return er;
}

static int
cdx_ii_addkey(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, const char *cid, ClipVar * key, const char *__PROC__)
{
   unsigned int id = *(unsigned int *) cid;

   int       er;

   if ((er = __cdx_addkey(ClipMachineMemory, -1, ro, key, ro->index->loc, id, __PROC__)))
      return er;
   memcpy(ro->iikey, ro->key, ro->bufsize);
   if ((ro->type == 'C'
	|| (ro->type == 'X' && ((char *) ro->iikey)[0] == type_weight(CHARACTER_type_of_ClipVarType))) && !ro->binary)
      loc_read(ro->index->loc, (unsigned char *) ro->iikey, ro->bufsize);
   memcpy(ro->id, &id, sizeof(unsigned int));
   return 0;
}

static int
cdx_ii_delkey(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, const char *__PROC__)
{
   CDX_HEADER hdr;

   int       found, er;

   ro->level = 0;
   _CDX_HEADER(ro, &hdr);
   ro->stack[0].page = _rdd_uint(hdr.root);

   if ((er = _cdx_search_tree(ClipMachineMemory, ro, ro->iikey, ro->bufsize, *(unsigned int *) ro->id, &found, NULL, __PROC__)))
      return er;
   if (!found)
      return 0;

   return _cdx_delkey(ClipMachineMemory, ro, __PROC__);
}

static int
cdx_ii_gotop(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, const char *__PROC__)
{
   int       out, er;

   ro->bof = ro->eof = 0;
   if ((er = _cdx_first(ClipMachineMemory, ro, &out, __PROC__)))
      return er;
   if (out)
    {
       ro->bof = ro->eof = 1;
       return 0;
    }
   if ((er = _cdx_recno_keyval(ClipMachineMemory, ro, ro->level, (unsigned int *) ro->id, ro->iikey, __PROC__)))
      return er;
   if ((ro->type == 'C'
	|| (ro->type == 'X' && ((char *) ro->iikey)[0] == type_weight(CHARACTER_type_of_ClipVarType))) && !ro->binary)
      loc_read(ro->index->loc, (unsigned char *) ro->iikey, ro->bufsize);
   return 0;
}

static int
cdx_ii_gobottom(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, const char *__PROC__)
{
   int       out, er;

   ro->bof = ro->eof = 0;
   if ((er = _cdx_last(ClipMachineMemory, ro, &out, __PROC__)))
      return er;
   if (out)
    {
       ro->bof = ro->eof = 1;
       return 0;
    }
   if ((er = _cdx_recno_keyval(ClipMachineMemory, ro, ro->level, (unsigned int *) ro->id, ro->iikey, __PROC__)))
      return er;
   if ((ro->type == 'C'
	|| (ro->type == 'X' && ((char *) ro->iikey)[0] == type_weight(CHARACTER_type_of_ClipVarType))) && !ro->binary)
      loc_read(ro->index->loc, (unsigned char *) ro->iikey, ro->bufsize);
   return 0;
}

static int
cdx_ii_id(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, ClipVar * v, const char *__PROC__)
{
   v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   v->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = malloc(5);
   if (ro->eof)
      memset(v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, 0, 4);
   else
      memcpy(v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, ro->id, 4);
   v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = 4;
   return 0;
}

static int
cdx_ii_key(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, ClipVar * v, const char *__PROC__)
{
   memset(v, 0, sizeof(ClipVar));
   if (ro->eof)
    {
       switch (ro->type)
	{
	case 'C':
	   v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
	   v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = malloc(ro->bufsize);
	   if (ro->binary)
	      memset(v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, 0, ro->bufsize);
	   else
	      memset(v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, ' ', ro->bufsize);
	   v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = ro->bufsize;
	   break;
	case 'N':
	   v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	   v->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = 0;
	   v->ClipType_t_of_ClipVar.len_of_ClipType = ro->keysize;
	   v->ClipType_t_of_ClipVar.dec_of_ClipType = ro->dec;
	   break;
	case 'D':
	   v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATE_type_of_ClipVarType;
	   v->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar = 0;
	   break;
	case 'T':
	   v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATETIME_type_of_ClipVarType;
	   v->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar = 0;
	   v->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar = 0;
	   break;
	case 'L':
	   v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
	   v->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = 0;
	   break;
	}
    }
   else
    {
       switch (ro->type)
	{
	case 'C':
	   v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
	   v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = malloc(ro->bufsize);
	   memcpy(v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, ro->iikey, ro->bufsize);
	   v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = ro->bufsize;
	   break;
	case 'N':
	   v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	   v->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = _cdx_double(*(unsigned long long *) ro->iikey);
	   v->ClipType_t_of_ClipVar.len_of_ClipType = ro->keysize;
	   v->ClipType_t_of_ClipVar.dec_of_ClipType = ro->dec;
	   break;
	case 'D':
	   v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATE_type_of_ClipVarType;
	   v->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar = _cdx_double(*(unsigned long long *) ro->iikey);
	   break;
	case 'T':
	   v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATETIME_type_of_ClipVarType;
	   v->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar = _rdd_backuint((unsigned char *) ro->iikey);
	   v->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar = _rdd_backuint((unsigned char *) ro->iikey + 4);
	   break;
	case 'L':
	   v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
	   v->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = (ro->iikey[0] == type_weight(LOGICAL_type_of_ClipVarType)) ? 1 : 0;
	   break;
	}
    }
   return 0;
}

static int
cdx_ii_next(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, const char *__PROC__)
{
   CDX_PAGE  page;

   CDX_LEAF *leaf;

   CDX_BRANCH *branch;

   CDX_HEADER hdr;

   int       out, found, er;

   ro->level = 0;
   if (ro->eof)
      return cdx_ii_gobottom(ClipMachineMemory, ro, __PROC__);
   ro->bof = 0;
   _CDX_HEADER(ro, &hdr);
   ro->stack[0].page = _rdd_uint(hdr.root);

   if ((er = _cdx_search_tree(ClipMachineMemory, ro, ro->iikey, ro->bufsize, *(unsigned int *) ro->id, &found, &out, __PROC__)))
      return er;
   if (!found)
    {
       if (out)
	{
	   if ((er = cdx_ii_gobottom(ClipMachineMemory, ro, __PROC__)))
	      return er;
	   if ((er = cdx_ii_next(ClipMachineMemory, ro, __PROC__)))
	      return er;
	   return 0;
	}
       if ((er = _cdx_recno_keyval(ClipMachineMemory, ro, ro->level, (unsigned int *) ro->id, ro->iikey, __PROC__)))
	  return er;
       if ((ro->type == 'C'
	    || (ro->type == 'X' && ((char *) ro->iikey)[0] == type_weight(CHARACTER_type_of_ClipVarType))) && !ro->binary)
	  loc_read(ro->index->loc, (unsigned char *) ro->iikey, ro->bufsize);
       return 0;
    }
   _CDX_PAGE(ro, ro->stack[ro->level].page, &page);
   if (!leaf)
      return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_badleaf);

   if ((er = _cdx_next_rm(ClipMachineMemory, ro, leaf, &out, 0, __PROC__)))
      return er;
   if (!out)
    {
       if ((er = _cdx_recno_keyval(ClipMachineMemory, ro, ro->level, (unsigned int *) ro->id, ro->iikey, __PROC__)))
	  return er;
       if ((ro->type == 'C'
	    || (ro->type == 'X' && ((char *) ro->iikey)[0] == type_weight(CHARACTER_type_of_ClipVarType))) && !ro->binary)
	  loc_read(ro->index->loc, (unsigned char *) ro->iikey, ro->bufsize);
    }
   ro->eof = out;
   return 0;
}

static int
cdx_ii_prev(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, const char *__PROC__)
{
   CDX_PAGE  page;

   CDX_LEAF *leaf;

   CDX_BRANCH *branch;

   CDX_HEADER hdr;

   int       out, found, er;

   ro->level = 0;
   if (ro->bof)
      return 0;
   if (ro->eof)
    {
       ro->eof = 0;
       return 0;
    }
   _CDX_HEADER(ro, &hdr);
   ro->stack[0].page = _rdd_uint(hdr.root);

   if ((er = _cdx_search_tree(ClipMachineMemory, ro, ro->iikey, ro->bufsize, *(unsigned int *) ro->id, &found, &out, __PROC__)))
      return er;
   if (!found && out)
    {
       if ((er = cdx_ii_gotop(ClipMachineMemory, ro, __PROC__)))
	  return er;
       if ((er = cdx_ii_prev(ClipMachineMemory, ro, __PROC__)))
	  return er;
    }
   _CDX_PAGE(ro, ro->stack[ro->level].page, &page);
   if (!leaf)
      return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_badleaf);

   if ((er = _cdx_prev_rm(ClipMachineMemory, ro, leaf, &out, 0, __PROC__)))
      return er;
   if (!out)
    {
       if ((er = _cdx_recno_keyval(ClipMachineMemory, ro, ro->level, (unsigned int *) ro->id, ro->iikey, __PROC__)))
	  return er;
       if ((ro->type == 'C'
	    || (ro->type == 'X' && ((char *) ro->iikey)[0] == type_weight(CHARACTER_type_of_ClipVarType))) && !ro->binary)
	  loc_read(ro->index->loc, (unsigned char *) ro->iikey, ro->bufsize);
    }
   ro->bof = out;
   return 0;
}

int
cdx_calcfiltlist(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, RDD_FILTER * fp, const char *__PROC__)
{
   int       i, er;

   BTREE    *bt;

   void     *key = malloc(sizeof(unsigned int) + ro->bufsize);

   ClipVar   vv, *vp;

   if (fp->list)
    {
       bt = bt_create(0, fp->listlen, sizeof(unsigned int) + ro->bufsize, _cdx_compare);
       for (i = 0; i < fp->listlen; i++)
	{
	   if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, fp->list[i], 0, __PROC__)))
	      return er;
	   if (ro->simpexpr)
	    {
	       if ((er = rd->vtbl->getvalue(ClipMachineMemory, rd, ro->simpfno, &vv, __PROC__)))
		  return er;
	       vp = &vv;
	    }
	   else
	    {
	       if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->block, &vv, 0)))
		  return er;
	       vp = _clip_vptr(&vv);
	    }
	   memcpy(key, &rd->recno, sizeof(unsigned int));
	   if ((er = cdx_formatkey(ClipMachineMemory, ro, vp, key + sizeof(unsigned int), __PROC__)))
	      return er;
	   _clip_destroy(ClipMachineMemory, &vv);
	   bt_add(bt, ro, key);
	}
       free(fp->list);
    }
   else
    {
#if 1
       unsigned int bytes = ((fp->size + 1) >> 5) + 1;

       int       i, b, bb, t, tt;

       fp->listlen = 0;
       for (i = 0; i < bytes; i++)
	{
	   if (fp->rmap[i])
	    {
	       for (b = (i << 2), bb = 0; bb < 4; b++, bb++)
		{
		   if (((char *) fp->rmap)[b])
		    {
		       for (t = (b << 3) + 1, tt = 0; tt < 8; t++, tt++)
			{
			   if (_rm_getbit(fp->rmap, fp->size, t))
			      fp->listlen++;
			}
		    }
		}
	    }
	}
       if (fp->listlen > 100)
	{
	   free(key);
	   return 0;
	}
#else
       fp->listlen = 0;
       for (i = 1; i <= fp->size; i++)
	  if (_rm_getbit(fp->rmap, fp->size, i))
	     fp->listlen++;
#endif
       bt = bt_create(0, fp->listlen, sizeof(unsigned int) + ro->bufsize, _cdx_compare);
#if 1
       for (i = 0; i < bytes; i++)
	{
	   if (fp->rmap[i])
	    {
	       for (b = (i << 2), bb = 0; bb < 4; b++, bb++)
		{
		   if (((char *) fp->rmap)[b])
		    {
		       for (t = (b << 3) + 1, tt = 0; tt < 8; t++, tt++)
			{
			   if (_rm_getbit(fp->rmap, fp->size, t))
			    {
			       if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, t, 0, __PROC__)))
				  return er;
			       if (ro->simpexpr)
				{
				   if ((er = rd->vtbl->getvalue(ClipMachineMemory, rd, ro->simpfno, &vv, __PROC__)))
				      return er;
				   vp = &vv;
				}
			       else
				{
				   if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->block, &vv, 0)))
				      return er;
				   vp = _clip_vptr(&vv);
				}
			       memcpy(key, &rd->recno, sizeof(unsigned int));
			       if ((er = cdx_formatkey(ClipMachineMemory, ro, vp, key + sizeof(unsigned int), __PROC__)))
				  return er;
			       _clip_destroy(ClipMachineMemory, &vv);
			       bt_add(bt, ro, key);
			    }
			}
		    }
		}
	    }
	}
#else
       for (i = 1; i <= fp->size; i++)
	{
	   if (_rm_getbit(fp->rmap, fp->size, i))
	    {
	       if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, i, 0, __PROC__)))
		  return er;
	       if (ro->simpexpr)
		{
		   if ((er = rd->vtbl->getvalue(ClipMachineMemory, rd, ro->simpfno, &vv, __PROC__)))
		      return er;
		   vp = &vv;
		}
	       else
		{
		   if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->block, &vv, 0)))
		      return er;
		   vp = _clip_vptr(&vv);
		}
	       memcpy(key, &rd->recno, sizeof(unsigned int));
	       if ((er = cdx_formatkey(ClipMachineMemory, ro, vp, key + sizeof(unsigned int), __PROC__)))
		  return er;
	       _clip_destroy(ClipMachineMemory, &vv);
	       bt_add(bt, ro, key);
	    }
	}
#endif
    }
   fp->list = malloc(sizeof(unsigned int) * (fp->listlen + 1));
   if (!ro->descend)
    {
       bt_first(bt);
       if (bt_key(bt))
	{
	   i = 0;
	   fp->list[i] = *(unsigned int *) bt_key(bt);
	   while (!bt_next(bt))
	    {
	       i++;
	       fp->list[i] = *(unsigned int *) bt_key(bt);
	    }
	}
    }
   else
    {
       bt_last(bt);
       if (bt_key(bt))
	{
	   i = 0;
	   fp->list[i] = *(unsigned int *) bt_key(bt);
	   while (!bt_prev(bt))
	    {
	       i++;
	       fp->list[i] = *(unsigned int *) bt_key(bt);
	    }
	}
    }
   bt_destroy(bt);
   free(key);
   return 0;
}

static int
cdx_buildtree(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__)
{
   CDX_HEADER hdr;

   ClipVar   v, *vp;

   int       found, er;

   void     *key = NULL;

   if (!rd->shared && !rd->eof)
    {
       key = malloc(ro->bufsize);
       if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->block, &v, 0)))
	  goto err;
       vp = _clip_vptr(&v);
       if ((er = cdx_formatkey(ClipMachineMemory, ro, vp, key, __PROC__)))
	  goto err;
       _clip_destroy(ClipMachineMemory, &v);

       _CDX_HEADER(ro, &hdr);
       ro->stack[0].page = _rdd_uint(hdr.root);
       ro->level = 0;

       if (!ro->descend)
	{
	   if ((er = _cdx_search_tree(ClipMachineMemory, ro, key, ro->bufsize, rd->recno, &found, NULL, __PROC__)))
	      goto err;
	}
       else
	{
	   if ((er = _cdx_back_search_tree(ClipMachineMemory, ro, key, ro->bufsize, rd->recno, &found, NULL, __PROC__)))
	      goto err;
	}
       free(key);
       _clip_destroy(ClipMachineMemory, &v);
/*		if(!found){
			return rdd_err(ClipMachineMemory,EG_CORRUPTION,0,__FILE__,__LINE__,__PROC__,
				er_nokey);
		}*/
       ro->valid_stack = 1;
    }
   return 0;
 err:
   if (key)
      free(key);
   return er;
}

static RDD_INDEX_VTBL *
cdx_vtbl()
{
   RDD_INDEX_VTBL *vtbl = (RDD_INDEX_VTBL *) malloc(sizeof(RDD_INDEX_VTBL));

   memset(vtbl, 0, sizeof(RDD_INDEX_VTBL));
   strcpy(vtbl->id, "CDX");
   strcpy(vtbl->suff, ".cdx");
   strcpy(vtbl->sing_suff, ".idx");
   strcpy(vtbl->desc, "Generic CDX index files driver v0.0.1 (c) 2001 Copyright ITK Ltd.");
   vtbl->ismulti = 1;

   vtbl->open = cdx_open;
   vtbl->close = cdx_close;
   vtbl->create = cdx_create;
   vtbl->reindex = cdx_reindex;
   vtbl->zap = cdx_zap;
   vtbl->gotop = cdx_gotop;
   vtbl->gobottom = cdx_gobottom;
   vtbl->next = cdx_next;
   vtbl->prev = cdx_prev;
   vtbl->seek = cdx_seek;
   vtbl->gotokey = cdx_gotokey;
   vtbl->descend = cdx_descend;
   vtbl->buildtree = cdx_buildtree;
   vtbl->wildskip = cdx_wildskip;
   vtbl->addkey = cdx_addkey;
   vtbl->delkey = cdx_delkey;
   vtbl->destroy = cdx_destroy;
   vtbl->formatkey = cdx_formatkey;
   vtbl->keyno = cdx_keyno;
   vtbl->lastkey = cdx_lastkey;
   vtbl->info = cdx_info;
   vtbl->keyvalue = cdx_keyvalue;
   vtbl->setscope = cdx_setscope;
   vtbl->ii_create = cdx_ii_create;
   vtbl->ii_createtag = cdx_ii_createtag;
   vtbl->ii_open = cdx_ii_open;
   vtbl->ii_opentag = cdx_ii_opentag;
   vtbl->ii_addkey = cdx_ii_addkey;
   vtbl->ii_delkey = cdx_ii_delkey;
   vtbl->ii_gotop = cdx_ii_gotop;
   vtbl->ii_gobottom = cdx_ii_gobottom;
   vtbl->ii_id = cdx_ii_id;
   vtbl->ii_key = cdx_ii_key;
   vtbl->ii_next = cdx_ii_next;
   vtbl->ii_prev = cdx_ii_prev;
   vtbl->calcfiltlist = cdx_calcfiltlist;
   vtbl->_rlock = cdx__rlock;
   vtbl->_wlock = cdx__wlock;
   vtbl->_ulock = cdx__ulock;
   return vtbl;
}

static RDD_INDEX_VTBL *
idx_vtbl()
{
   RDD_INDEX_VTBL *vtbl = (RDD_INDEX_VTBL *) malloc(sizeof(RDD_INDEX_VTBL));

   memset(vtbl, 0, sizeof(RDD_INDEX_VTBL));
   strcpy(vtbl->id, "IDX");
   strcpy(vtbl->suff, ".idx");
   strcpy(vtbl->sing_suff, ".idx");
   strcpy(vtbl->desc, "Generic IDX index files driver v0.0.1 (c) 2001 Copyright ITK Ltd.");
   vtbl->ismulti = 0;

   vtbl->open = idx_open;
   vtbl->close = cdx_close;
   vtbl->create = idx_create;
   vtbl->reindex = idx_reindex;
   vtbl->zap = idx_zap;
   vtbl->gotop = cdx_gotop;
   vtbl->gobottom = cdx_gobottom;
   vtbl->next = cdx_next;
   vtbl->prev = cdx_prev;
   vtbl->seek = cdx_seek;
   vtbl->gotokey = cdx_gotokey;
   vtbl->descend = cdx_descend;
   vtbl->buildtree = cdx_buildtree;
   vtbl->wildskip = cdx_wildskip;
   vtbl->addkey = cdx_addkey;
   vtbl->delkey = cdx_delkey;
   vtbl->destroy = cdx_destroy;
   vtbl->formatkey = cdx_formatkey;
   vtbl->keyno = cdx_keyno;
   vtbl->lastkey = cdx_lastkey;
   vtbl->info = cdx_info;
   vtbl->keyvalue = cdx_keyvalue;
   vtbl->setscope = cdx_setscope;
   vtbl->calcfiltlist = cdx_calcfiltlist;
   vtbl->_rlock = cdx__rlock;
   vtbl->_wlock = cdx__wlock;
   vtbl->_ulock = cdx__ulock;
   return vtbl;
}

static int
idx_create(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri,
	   RDD_ORDER ** rop, const char *tag, const char *expr,
	   ClipVar * block, int unique, int first, unsigned int header, const char *__PROC__)
{
   int       er;

   if ((er = _cdx_create(ClipMachineMemory, rd, ri, rop, ri->name, expr, block, unique, 0, __PROC__)))
      goto err;

   return 0;
 err:
   return er;
}

static int
idx_open(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, const char *tag, const char *__PROC__)
{
   RDD_ORDER *ro = NULL;

   int       er;

   unsigned int newheader;

   ri->loc = rd->loc;
   newheader = 0;
   ro = (RDD_ORDER *) calloc(1, sizeof(RDD_ORDER));
   if ((er = _cdx_init_order(ClipMachineMemory, rd->area, ri, newheader, ro, __PROC__)))
      goto err;

   rd->ords_opened++;
   rd->orders = (RDD_ORDER **) realloc(rd->orders, sizeof(RDD_ORDER *) * rd->ords_opened);
   rd->orders[rd->ords_opened - 1] = ro;
   ro->name = strdup(ri->name);
   ro->orderno = rd->ords_opened - 1;
   ri->norders = 1;
   ri->orders = (RDD_ORDER **) realloc(ri->orders, ri->norders * sizeof(RDD_ORDER *));
   ri->orders[ri->norders - 1] = ro;
   ro->canadd = 1;
   return 0;
 err:
   if (ro)
      free(ro);
   return er;
}

static int
idx_zap(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, const char *__PROC__)
{
   CDX_HEADER chdr;

   RDD_ORDER *ro = ri->orders[0];

   int       er;

   if ((er = rdd_trunc(ClipMachineMemory, &ri->file, sizeof(CDX_HEADER), __PROC__)))
      goto err;
   if ((er = _cdx_init_rootleaf(ClipMachineMemory, ro, sizeof(CDX_HEADER), __PROC__)))
      goto err;
   if ((er = rdd_read(ClipMachineMemory, &ri->file, 0, sizeof(CDX_HEADER), &chdr, __PROC__)))
      goto err;
   _rdd_put_uint(chdr.fuu, 0);
   if ((er = rdd_write(ClipMachineMemory, &ri->file, 0, sizeof(CDX_HEADER), &chdr, __PROC__)))
      goto err;
   return 0;
 err:
   return er;
}

static int
idx_reindex(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, const char *__PROC__)
{
   RDD_ORDER *ro;

   int       lastrec;

   BTREE1   *bt;

   void     *buf;

   ClipVar   vv, *vp;

   int       interval = max(1, rd->os.nInterval);

   unsigned int oldrn = rd->recno;

   int       er;

   memset(&vv, 0, sizeof(ClipVar));

   if ((er = idx_zap(ClipMachineMemory, rd, ri, __PROC__)))
      return er;
   if ((er = rd->vtbl->lastrec(ClipMachineMemory, rd, &lastrec, __PROC__)))
      return er;

   ro = ri->orders[0];
   if (ro->custom)
      return 0;
   rd->tagindexing = ro->name;
   bt = bt1_create(ro->unique, ro->bufsize + sizeof(unsigned int), _cdx_compare, ClipMachineMemory->index_buffer_limit);
   buf = malloc(ro->bufsize + sizeof(unsigned int));
   for (rd->recno = 1; rd->recno <= lastrec; rd->recno++)
    {
       rd->eof = 0;
       if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, rd->recno, 0, __PROC__)))
	  return er;
       if (ro->bforexpr.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
	{
	   if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->bforexpr, &vv, 0)))
	      return er;
	   vp = _clip_vptr(&vv);
	   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
	      return rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_badforexpr);
	   if (!vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
	    {
	       _clip_destroy(ClipMachineMemory, &vv);
	       continue;
	    }
	   _clip_destroy(ClipMachineMemory, &vv);
	}
       if (bt)
	{
	   if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->block, &vv, 0)))
	      return er;
	   vp = _clip_vptr(&vv);
	   memcpy(buf, &rd->recno, sizeof(unsigned int));
	   if ((er = cdx_formatkey(ClipMachineMemory, ro, vp, buf + sizeof(unsigned int), __PROC__)))
	      return er;
	   _clip_destroy(ClipMachineMemory, &vv);
	   if (bt1_add(bt, ro, buf))
	    {
	       if ((er = _cdx_savebtree(ClipMachineMemory, ro, bt, lastrec, __PROC__)))
		{
		   bt1_destroy(bt);
		   return er;
		}
	       bt1_destroy(bt);
	       bt = NULL;
	       if ((er = cdx_addkey(ClipMachineMemory, rd, ro, NULL, __PROC__)))
		  return er;
	    }
	}
       else
	{
	   if ((er = cdx_addkey(ClipMachineMemory, rd, ro, NULL, __PROC__)))
	      return er;
	}
      /* bEval every nInterval */
       if (rd->recno % interval == 0
	   && rd->os.bEval.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
	{
	   if ((er = rdd_calc(ClipMachineMemory, rd->area, &rd->os.bEval, &vv, 0)))
	      return er;
	   vp = _clip_vptr(&vv);
	   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
	      return rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_badevalexpr);
	   if (!vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
	    {
	       _clip_destroy(ClipMachineMemory, &vv);
	       break;
	    }
	   _clip_destroy(ClipMachineMemory, &vv);
	}
    }
   if (bt)
    {
       if ((er = _cdx_savebtree(ClipMachineMemory, ro, bt, lastrec, __PROC__)))
	{
	   bt1_destroy(bt);
	   return er;
	}
       bt1_destroy(bt);
    }
   free(buf);

   rd->tagindexing = NULL;
   if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, oldrn, 0, __PROC__)))
      return er;
   return 0;
}

static int
cdx_wildskip(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro,
	     const char *pattern, int regular, const char *s, ClipVar * block, int *found, const char *__PROC__)
{
   CDX_HEADER hdr;

   CDX_PAGE  page;

   CDX_LEAF *leaf = (CDX_LEAF *) & page;

   ClipVar   v, *vp, r;

   char     *key = calloc(1, ro->bufsize + 1);

   char     *lockey = calloc(1, ro->bufsize + 1);

   char     *p = (char *) (leaf) + sizeof(CDX_LEAF);

   unsigned long long item;

   unsigned int rec = 0;

   int       dup = 0, trail = 0;

   int       ok, fok;

   int       fnd, out = 0, i, er;

#ifdef HAVE_REGEX_H
   regex_t   reg;
#endif

   *found = 1;
#ifdef HAVE_REGEX_H
   if (regular)
      regcomp(&reg, pattern, REG_EXTENDED | REG_NOSUB);
#else
   regular = 0;
#endif
   if (rd->shared || !ro->valid_stack)
    {
       char      cntcdx[4];

       if ((er = rdd_read(ClipMachineMemory, &ro->index->file, 8, 4, cntcdx, __PROC__)))
	  return er;
       if (!ro->valid_stack || ro->cntcdx != _rdd_backuint((unsigned char *) cntcdx))
	{
	   if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->block, &v, 0)))
	      goto err;
	   vp = _clip_vptr(&v);
	   if ((er = cdx_formatkey(ClipMachineMemory, ro, vp, key, __PROC__)))
	      goto err;
	   _clip_destroy(ClipMachineMemory, &v);
	   ro->curoffs = 0;

	   _CDX_HEADER(ro, &hdr);
	   ro->stack[0].page = _rdd_uint(hdr.root);
	   ro->level = 0;

	   if (!ro->descend)
	    {
	       if ((er = _cdx_search_tree(ClipMachineMemory, ro, key, ro->bufsize, rd->recno, &fnd, NULL, __PROC__)))
		  goto err;
	    }
	   else
	    {
	       if ((er = _cdx_back_search_tree(ClipMachineMemory, ro, key, ro->bufsize, rd->recno, &fnd, NULL, __PROC__)))
		  goto err;
	    }
	   _clip_destroy(ClipMachineMemory, &v);
	}
    }

   if ((er = rdd_read(ClipMachineMemory, &ro->index->file, ro->stack[ro->level].page, sizeof(CDX_LEAF), leaf, __PROC__)))
      goto err;

   for (i = 0; i <= ro->stack[ro->level].pos; i++)
    {
       memcpy(&item, leaf->keys + i * leaf->bytes, leaf->bytes);
       _CDX_LEAF_ITEM(item, rec, dup, trail);
       p -= ro->bufsize - (dup + trail);
       memcpy(key + dup, p, ro->bufsize - (dup + trail));
    }
   memset(key + dup + ro->bufsize - (dup + trail), (int) (block ? 0x20 : 0x00), trail);

   if (block)
      memset(&v, 0, sizeof(ClipVar));

   *found = 0;
   while (!out && ((block || regular) ? 1 : (memcmp(key, pattern, s - pattern) == 0)))
    {
       if (!block)
	{
	   if (regular)
	    {
#ifdef HAVE_REGEX_H
	       *found = !regexec(&reg, key, 0, NULL, 0);
#else
	       *found = 0;
#endif
	    }
	   else
	    {
	       *found = (_clip_glob_match(key + (s - pattern), s, 1) != -1);
	    }
	   if (*found)
	    {
	       if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, rec, 1, __PROC__)))
		  goto err;
	       if (!ro->scopetop && !ro->scopebottom && !rd->filter && !(ClipMachineMemory->flags & DELETED_FLAG))
		  break;
	       if ((er = _cdx_checkscope(ClipMachineMemory, rd, ro, key, &ok, __PROC__)))
		  goto err;
	       if (ok > 0)
		{
		   *found = 0;
		   break;
		}
	       if ((er = rdd_checkfilter(ClipMachineMemory, rd, &fok, __PROC__)))
		  goto err;
	       if (!ok && fok)
		  break;
	    }
	}
       else
	{
	   if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, rec, 1, __PROC__)))
	      goto err;
	   ok = 0;
	   fok = 1;
	   if (!(!ro->scopetop && !ro->scopebottom && !rd->filter && !(ClipMachineMemory->flags & DELETED_FLAG)))
	    {

	       if ((er = _cdx_checkscope(ClipMachineMemory, rd, ro, key, &ok, __PROC__)))
		  goto err;
	       if (ok > 0)
		{
		   *found = 0;
		   break;
		}
	       if ((er = rdd_checkfilter(ClipMachineMemory, rd, &fok, __PROC__)))
		  goto err;
	    }
	   if (!ok && fok)
	    {
	       _cdx_keyvar(ro, key, lockey, &v);
	       if ((er = _clip_eval(ClipMachineMemory, block, 1, &v, &r)))
		  goto err;
	       *found = r.ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;
	       if (*found)
		  break;
	    }
	}
       ro->stack[ro->level].pos++;
       if (ro->stack[ro->level].pos >= _rdd_ushort(leaf->nkeys))
	{
	   if (_rdd_uint(leaf->right) != 0xffffffff)
	    {
	       ro->stack[ro->level].page = _rdd_uint(leaf->right);
	       if ((er =
		    rdd_read(ClipMachineMemory, &ro->index->file, ro->stack[ro->level].page, sizeof(CDX_LEAF), leaf, __PROC__)))
		  goto err;
	       p = (char *) leaf + sizeof(CDX_LEAF);
	       memcpy(&item, leaf->keys, leaf->bytes);
	       _CDX_LEAF_ITEM(item, rec, dup, trail);
	       p -= ro->bufsize - (dup + trail);
	       memcpy(key + dup, p, ro->bufsize - (dup + trail));
	       memset(key + dup + ro->bufsize - (dup + trail), (int) (block ? 0x20 : 0x00), trail);

	       if (_rdd_ushort(leaf->nkeys) == 0)
		  return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_nokeys);
	       ro->stack[ro->level].pos = 0;
	    }
	   else
	    {
	       out = 1;
	    }
	}
       else
	{
	   memcpy(&item, leaf->keys + ro->stack[ro->level].pos * leaf->bytes, leaf->bytes);
	   _CDX_LEAF_ITEM(item, rec, dup, trail);
	   p -= ro->bufsize - (dup + trail);
	   memcpy(key + dup, p, ro->bufsize - (dup + trail));
	   memset(key + dup + ro->bufsize - (dup + trail), (int) (block ? 0x20 : 0x00), trail);
	}
       *found = 0;
    }
   if (out)
    {
       *found = 0;
       ro->valid_stack = 0;
    }
   else
    {
       ro->valid_stack = 1;
    }

#ifdef HAVE_REGEX_H
   if (regular)
      regfree(&reg);
#endif
   free(lockey);
   free(key);
   return 0;
 err:
#ifdef HAVE_REGEX_H
   if (regular)
      regfree(&reg);
#endif
   free(lockey);
   free(key);
   return er;
}
