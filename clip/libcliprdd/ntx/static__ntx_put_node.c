static int
_ntx_put_node(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro,
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
   return rdd_write(ClipMachineMemory, &ro->index->file, pageoffs, sizeof(NTX_PAGE), (char *) page, __PROC__);
}
