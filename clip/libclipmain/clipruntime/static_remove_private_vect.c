static void
remove_private_vect(ClipMachine * ClipMachineMemory, int num, void *vect)
{
   long *lp = (long *) vect;

   for (; num > 0; --num, ++lp)
   {
      long hash = GETLONG(lp);

      VarEntry *vp = (VarEntry *) HashTable_fetch(ClipMachineMemory->privates, hash);

      if (!vp)
	 continue;
      if (vp->VarEntry_next_of_VarEntry)
      {
	 HashTable_remove(ClipMachineMemory->privates, hash);
	 HashTable_store(ClipMachineMemory->privates, vp->VarEntry_next_of_VarEntry, hash);
	 vp->VarEntry_next_of_VarEntry = 0;
      }
      else
      {
	 HashTable_remove(ClipMachineMemory->privates, hash);
      }
      delete_VarEntry(ClipMachineMemory, vp);
   }
}
