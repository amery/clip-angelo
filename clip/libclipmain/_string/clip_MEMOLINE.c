int
clip_MEMOLINE(ClipMachine * ClipMachineMemory)
{
   int vl = 0, i, j, len, nstr, ntab, wrap;

   char *rp, *e, *e2, *wrappos = NULL;

   char *vp = _clip_parcl(ClipMachineMemory, 1, &vl);

   if (vp == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "MEMOLINE");
   }
   len = _clip_parni(ClipMachineMemory, 2);
   if (len <= 4)
      len = 79;
   nstr = _clip_parni(ClipMachineMemory, 3);
   if (nstr <= 1)
      nstr = 1;
   ntab = _clip_parni(ClipMachineMemory, 4);
   if (ntab < 1)
      ntab = 1;
   if (ntab >= len)
      ntab = len - 1;

   if (_clip_parinfo(ClipMachineMemory, 5) == LOGICAL_type_of_ClipVarType)
      wrap = _clip_parl(ClipMachineMemory, 5);
   else
      wrap = 1;
   rp = (char *) malloc(len + 1);
   memset(rp, ' ', len);
   rp[len] = 0;

   for (i = 1, j = 1, e = vp; e < vp + vl && i < nstr; e++, j++)
   {
      if (*e == ' ')
	 wrappos = e;
      if (j >= len || *e == '\n')
      {
	 if (wrap && wrappos != NULL && e < vp + vl - 1 && *e != '\n')
	 {
	    if (*(e + 1) == ' ')
	       e++;
	    else
	       e = wrappos;
	 }
	 i++;
	 j = 0;
      }
      if (*e == '\t')
	 j += ntab - 1;
      if (*e == '\r')
	 j--;
   }
   if (i < nstr)
   {
      _clip_retcn_m(ClipMachineMemory, rp, len);
      return 0;
   }
   if (i > 1 && j > len)
   {
      if (wrap && wrappos != NULL)
	 e = wrappos + 1;
      else
	 e++;
   }
   wrappos = NULL;
   e2 = rp;
   for (j = 0; e < vp + vl && j <= len && *e != '\n'; e++, j++, e2++)
   {
      if (*e == ' ')
	 wrappos = e2;
      if (*e != '\r')
	 *e2 = *e;
      else
      {
	 j--;
	 e2--;
      }
      if (*e == '\t')
	 j += ntab - 1;
   }
   if (j > len)
   {
      if (wrap && wrappos != NULL)
	 e2 = wrappos + 1;
      else
	 e2++;
   }
   e = rp + len;
   for (; e2 <= e; e2++)
      *e2 = ' ';
   rp[len] = 0;
   _clip_retcn_m(ClipMachineMemory, rp, len);
   return 0;
}
