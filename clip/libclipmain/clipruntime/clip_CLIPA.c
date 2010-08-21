int
clip_CLIPA(ClipMachine * ClipMachineMemory)
{
   char *funcname = _clip_parc(ClipMachineMemory, 1);

   int parcount;

   ClipVar *params, *retVal = RETPTR(ClipMachineMemory);

   ClipVar *ap = _clip_par(ClipMachineMemory, 2);

   if (!funcname || !ap || ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != ARRAY_type_of_ClipVarType)
   {
      int r;

      r = _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "CLIPA");
      return _clip_call_errblock(ClipMachineMemory, r);
   }

   parcount = ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
   params = ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar;

   _clip_clip(ClipMachineMemory, funcname, parcount, params, retVal);

   return 0;
}
