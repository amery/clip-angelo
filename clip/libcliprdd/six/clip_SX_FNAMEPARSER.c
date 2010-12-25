int
clip_SX_FNAMEPARSER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_FNAMEPARSER";

   const char *fname = _clip_parc(ClipMachineMemory, 1);

   int lpath = _clip_parl(ClipMachineMemory, 2);

   int lext = _clip_parl(ClipMachineMemory, 3);

   const char *s, *e;

   char *r;

   int er;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, CHARACTER_type_of_ClipVarType);
   CHECKOPT1(2, LOGICAL_type_of_ClipVarType);
   CHECKOPT1(3, LOGICAL_type_of_ClipVarType);

   s = fname;
   e = fname + strlen(fname);
   if (!lpath && !lext)
   {
      s = max(s, strrchr(fname, '/'));
      s = max(s, strrchr(fname, '\\'));
      if (s > fname)
	 s++;
      e = strrchr(fname, '.');
      if (!(e && *(e + 1) != '/' && *(e + 1) != '\\'))
	 e = NULL;
      if (e < s)
	 e = fname + strlen(fname);
   }
   else if (!lpath && lext)
   {
      s = max(s, strrchr(fname, '/'));
      s = max(s, strrchr(fname, '\\'));
      if (s > fname)
	 s++;
   }
   else if (lpath && !lext)
   {
      e = strrchr(fname, '.');
      if (!(e && *(e + 1) != '/' && *(e + 1) != '\\'))
	 e = NULL;
      if (e < s)
	 e = fname + strlen(fname);
   }
   r = calloc(1, e - s + 1);
   memcpy(r, s, e - s);
   _clip_retcn_m(ClipMachineMemory, r, e - s);
   return 0;
 err:
   return er;
}
