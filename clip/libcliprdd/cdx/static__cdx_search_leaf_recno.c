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
