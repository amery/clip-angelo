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
