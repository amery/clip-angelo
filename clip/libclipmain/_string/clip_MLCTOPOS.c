int
clip_MLCTOPOS(ClipMachine * ClipMachineMemory)
{
   int vl = 0, i, j, len, nstr, ntab, npos;

   char *e;

   char *vp = _clip_parcl(ClipMachineMemory, 1, &vl);

   if (vp == NULL)
   {
      _clip_retni(ClipMachineMemory, 0);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "MLTOPOS");
   }
   len = _clip_parni(ClipMachineMemory, 2);
   if (len <= 4)
      len = 79;
   nstr = _clip_parni(ClipMachineMemory, 3);
   if (nstr <= 1)
      nstr = 1;
   npos = _clip_parni(ClipMachineMemory, 4);
   if (npos <= 1)
      npos = 0;
   npos++;
   ntab = _clip_parni(ClipMachineMemory, 5);
   if (ntab < 1)
      ntab = 1;
   if (ntab >= len)
      ntab = len - 1;

   for (i = 1, j = 1, e = vp; e < vp + vl; e++, j++)
   {
      if (j >= len || *e == '\n')
      {
	 i++;
	 j = 0;
      }
      if (*e == '\t')
	 j += ntab - 1;
      if (*e == '\r')
	 j--;
      if (i >= nstr && j >= npos)
	 break;
   }
   /*printf("%d,%d,%d,%d...",i,j,e,vp); */
   _clip_retni(ClipMachineMemory, e - vp + 1);
   return 0;
}
