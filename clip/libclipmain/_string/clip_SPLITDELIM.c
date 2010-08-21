int
clip_SPLITDELIM(ClipMachine * ClipMachineMemory)
{
   ClipVar *ap = RETPTR(ClipMachineMemory);

   char *str = _clip_parc(ClipMachineMemory, 1);

   char *fdelim = _clip_parc(ClipMachineMemory, 2);

   char *rdelim = _clip_parc(ClipMachineMemory, 3);

   char *b, *p, f, r;
   long dims[1] = { 0 };
   ClipVar v;

   _clip_array(ClipMachineMemory, ap, 1, dims);

   if (!str || !str[0])
      return 0;
   if (!fdelim)
      f = '\"';
   else
      f = fdelim[0];
   if (!rdelim)
      r = ',';
   else
      r = rdelim[0];

   p = str;
   while (*p)
   {
      while (*p && *p == ' ')
	 p++;
      if (!(*p))
	 continue;
      b = p;
      if (*p == f)
      {
	 while (*p && *(++p) != f);
	 if (!(*p))
	    continue;
	 _clip_var_str(b + 1, p - b - 1, &v);
	 _clip_aadd(ClipMachineMemory, ap, &v);
	 while (*p && *(p++) != r);
	 continue;
      }
      while (*p && *p != r)
	 p++;
      _clip_var_str(b, p - b, &v);
      _clip_aadd(ClipMachineMemory, ap, &v);
      if (*p)
	 p++;
   }
   return 0;
}
