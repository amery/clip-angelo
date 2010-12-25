int
clip_MLCOUNT(ClipMachine * ClipMachineMemory)
{
   int vl = 0, i, j, len, ntab, wrap;

   char *e, *wrappos = NULL;

   char *vp = _clip_parcl(ClipMachineMemory, 1, &vl);

   if (vp == NULL)
   {
      _clip_retni(ClipMachineMemory, 0);
      return 0;
   }
   len = _clip_parni(ClipMachineMemory, 2);
   if (len <= 4)
      len = 79;
   ntab = _clip_parni(ClipMachineMemory, 3);
   if (ntab < 1)
      ntab = 1;
   if (ntab >= len)
      ntab = len - 1;
   if (_clip_parinfo(ClipMachineMemory, 5) == LOGICAL_type_of_ClipVarType)
      wrap = _clip_parl(ClipMachineMemory, 5);
   else
      wrap = 1;

   for (i = 0, j = 1, e = vp; e < vp + vl; e++, j++)
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
	    {
	       e = wrappos;
	       wrappos = NULL;
	    }
	 }
	 i++;
	 j = 0;
      }
      if (*e == '\t')
	 j += ntab - 1;
      if (*e == '\r')
	 j--;
   }
   _clip_retni(ClipMachineMemory, i + (j > 1));
   return 0;
}
