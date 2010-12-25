int
clip_EVAL(ClipMachine * ClipMachineMemory)
{
   ClipVar *bp = _clip_par(ClipMachineMemory, 1);

   if (!bp || (bp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CCODE_type_of_ClipVarType && bp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != PCODE_type_of_ClipVarType))
      return 0;

   return _clip_eval(ClipMachineMemory, bp, ClipMachineMemory->argc - 1, ARGPTR(ClipMachineMemory, 2), RETPTR(ClipMachineMemory));
}
