static VarEntry *
add_private(ClipMachine * ClipMachineMemory, long hash)
{
   long *p;

   VarEntry *vp, *np;

   ClipFrame *fp;

   if (ClipMachineMemory->inMacro)
      fp = ClipMachineMemory->inMacro;
   else
      fp = ClipMachineMemory->fp;

   if (fp)
   {
      p = fp->privates_of_ClipFrame;
      if (p)
      {
	 int n;

	 long c;

	 for (n = *p, p++; n >= 0; n--, p++)
	    if (hash == GETLONG(p))
	    {
	       vp = (VarEntry *) HashTable_fetch(ClipMachineMemory->privates, hash);
	       if (vp)
		  return vp;
	    }
	 p = fp->privates_of_ClipFrame;
	 c = GETLONG(p);
	 p = (long *) realloc(p, (c + 2) * sizeof(long));

	 SETLONG(p, c + 1);
	 SETLONG(p + c + 1, hash);
	 fp->privates_of_ClipFrame = p;
      }
      else
      {
	 p = fp->privates_of_ClipFrame = (long *) malloc(sizeof(long) * 2);

	 SETLONG(p, 1);
	 SETLONG(p + 1, hash);
      }
   }

   vp = (VarEntry *) HashTable_fetch(ClipMachineMemory->privates, hash);
   np = new_VarEntry(hash);
   if (!vp)
      HashTable_insert(ClipMachineMemory->privates, np, hash);
   else
   {
      np->VarEntry_next_of_VarEntry = vp;
      HashTable_remove(ClipMachineMemory->privates, hash);
      HashTable_store(ClipMachineMemory->privates, np, hash);
   }
   return np;
}
