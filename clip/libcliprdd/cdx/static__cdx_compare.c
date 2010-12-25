static int
_cdx_compare(void *op, void *k1, void *k2, int *uniqfound)
{
   RDD_ORDER *ro = op;

   int r = 0;

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
