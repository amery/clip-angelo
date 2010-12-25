static int
_ntx_remove_node(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, unsigned int page, NTX_PAGE * pp, int no, const char *__PROC__)
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
   return rdd_write(ClipMachineMemory, &ro->index->file, page, sizeof(NTX_PAGE), basep, __PROC__);
}
