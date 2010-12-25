int
clip_ATAIL(ClipMachine * ClipMachineMemory)
{
   ClipVar *ap = _clip_par(ClipMachineMemory, 1);

   int c;

   if (!ap)
      return 0;

   if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType && (c = ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar) > 0)
   {
      if (c > 0)
	 _clip_clone(ClipMachineMemory, RETPTR(ClipMachineMemory), ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + c - 1);
   }

   return 0;
}
