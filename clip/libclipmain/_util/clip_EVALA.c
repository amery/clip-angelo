int
clip_EVALA(ClipMachine * ClipMachineMemory)
{
   int parcount;

   ClipVar *params;

   ClipVar *bp = _clip_par(ClipMachineMemory, 1);

   ClipVar *ap = _clip_par(ClipMachineMemory, 2);

   if (!bp || (bp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CCODE_type_of_ClipVarType && bp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != PCODE_type_of_ClipVarType))
      return 0;

   if (!ap || ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != ARRAY_type_of_ClipVarType)
   {
      int r;

      r = _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "EVALA");
      return _clip_call_errblock(ClipMachineMemory, r);
   }

   parcount = ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
   params = ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar;

   //_clip_clip(ClipMachineMemory, funcname, parcount, params, retVal);
   return _clip_eval(ClipMachineMemory, bp, parcount, params, RETPTR(ClipMachineMemory));
}
