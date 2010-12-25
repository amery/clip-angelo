static ClipVar *
find_lval(ClipMachine * ClipMachineMemory, char *name, int namelen)
{
   VarEntry *vep;

   ClipVar *lval = 0;

   long hash;

   char *fname = 0;

   int fl = 0, r, ndim = 0;

   long *dim = 0;

   r = _clip_parse_name(ClipMachineMemory, name, namelen, 0, 0, &fname, &fl, &dim, &ndim);
   if (r == 2)			/* field */
      return 0;

   hash = _clip_casehashbytes(0, fname, fl);
   vep = fetch_var(ClipMachineMemory, hash);

   if (vep)
      lval = &vep->ClipVar_var_of_VarEntry;

   if (lval && r == 3)
   {
      lval = _clip_aref(ClipMachineMemory, lval, ndim, dim);
      free(dim);
   }

   return lval;
}
