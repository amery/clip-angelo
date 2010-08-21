int
clip_MEMVARSET(ClipMachine * ClipMachineMemory)
{
   int l;

   char *name = _clip_parcl(ClipMachineMemory, 1, &l);

   ClipVar *var;

   ClipVar *data = _clip_par(ClipMachineMemory, 2);

   var = _clip_ref_memvar_noadd(ClipMachineMemory, _clip_casehashbytes(0, name, l));
   _clip_retl(ClipMachineMemory, 0);
   if (var)
   {
      _clip_clone(ClipMachineMemory, var, data);
      _clip_retl(ClipMachineMemory, 1);
   }
   return 0;
}
