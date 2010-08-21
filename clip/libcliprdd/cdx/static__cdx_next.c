static int
_cdx_next(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, int *out, unsigned int *rec, const char *__PROC__)
{
   CDX_LEAF *leaf = ro->curpage;

   unsigned int recno;

   int er;

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
