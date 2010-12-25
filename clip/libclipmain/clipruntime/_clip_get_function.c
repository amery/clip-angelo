int
_clip_get_function(ClipMachine * ClipMachineMemory, long hash, ClipFunction ** fpp, ClipBlock ** bpp)
{
   ClipFunction *fp;

   FuncEntry *vp;

   vp = (FuncEntry *) HashTable_fetch(ClipMachineMemory->functions, hash);
   if (!vp)
   {
      fp = init_struct->_clip_builtin(hash);
      if (fp)
      {
	 *fpp = fp;
	 *bpp = 0;
	 return 1;
      }
      return 0;
   }

   if (vp->ClipFunction_f_of_FuncEntry)
   {
      *fpp = vp->ClipFunction_f_of_FuncEntry;
      *bpp = 0;
   }
   else
   {
      *fpp = 0;
      *bpp = &vp->ClipBlock_block_of_FuncEntry;
   }

   return 1;
}
