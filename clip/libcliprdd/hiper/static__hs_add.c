static int
_hs_add(ClipMachine * ClipMachineMemory, HIPER * hs, const char *str, int len, unsigned int rec, const char *__PROC__)
{
   unsigned char c4[4];

   const char *s = str;

   const char *e = str + len - 1;

   unsigned char pair[2];

   unsigned int offs;

   int i, er;

   if (!str)
      return 0;
   memset(hs->seen, 0, sizeof(hs->seen));

   while (s < e)
   {
      memcpy(pair, s, 2);
      if (hs->filtset == 1)
      {				/* text */
	 if (!isgraph(pair[0]) || !isgraph(pair[1]))
	 {
	    s++;
	    continue;
	 }
	 if (hs->lcase)
	 {			/* non case-sensitive text */
	    pair[0] = toupper(pair[0]);
	    pair[1] = toupper(pair[1]);
	 }
      }				/* otherwise binary */
      i = _rdd_ushort(pair);
      offs = sizeof(HS_HEADER) + i * 4;
      if ((er = rdd_read(ClipMachineMemory, &hs->file, offs, 4, c4, __PROC__)))
	 return er;
      if (!_rdd_uint(c4))
      {
	 if ((er = _hs_addpair(ClipMachineMemory, hs, offs, rec, __PROC__)))
	    return er;
      }
      else
      {
	 if (!hs->seen[i])
	 {
	    if ((er = _hs_setpair(ClipMachineMemory, hs, _rdd_uint(c4), rec, __PROC__)))
	       return er;
	 }
      }
      hs->seen[i] = 1;
      s++;
   }
   return 0;
}
