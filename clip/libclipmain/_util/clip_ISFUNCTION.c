int
clip_ISFUNCTION(ClipMachine * ClipMachineMemory)
{
   ClipFunction *fp;

   ClipBlock *bp;

   long hash;

   ClipVar *vp = _clip_par(ClipMachineMemory, 1);

   hash = _clip_casehash(ClipMachineMemory, vp);

   if (_clip_get_function(ClipMachineMemory, hash, &fp, &bp))
      _clip_retl(ClipMachineMemory, 1);
   else
      _clip_retl(ClipMachineMemory, 0);

   return 0;
}
