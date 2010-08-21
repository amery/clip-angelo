int
clip_MEMVARGET(ClipMachine * ClipMachineMemory)
{
   int l;

   char *name = _clip_parcl(ClipMachineMemory, 1, &l);

   ClipVar *var;

   ClipVar *ret = RETPTR(ClipMachineMemory);

   var = _clip_ref_memvar_noadd(ClipMachineMemory, _clip_casehashbytes(0, name, l));
   if (var)
      _clip_clone(ClipMachineMemory, ret, var);
   return 0;
}
