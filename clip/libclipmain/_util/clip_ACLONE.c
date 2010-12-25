int
clip_ACLONE(ClipMachine * ClipMachineMemory)
{
   ClipVar *ap = _clip_par(ClipMachineMemory, 1);

   ClipVar *rp;

   if (!ap || ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != ARRAY_type_of_ClipVarType)
      return 0;

   rp = RETPTR(ClipMachineMemory);

   return _clip_dup(ClipMachineMemory, rp, ap);
}
