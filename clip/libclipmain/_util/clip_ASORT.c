int
clip_ASORT(ClipMachine * ClipMachineMemory)
{

   ClipVar *rp;

   ClipVar *ap = _clip_par(ClipMachineMemory, 1);

   int start = _clip_parni(ClipMachineMemory, 2) - 1;

   int count = _clip_parni(ClipMachineMemory, 3);

   ClipVar *bp = _clip_par(ClipMachineMemory, 4);

   int c;

   if (!ap || ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != ARRAY_type_of_ClipVarType)
      return 0;
   if (bp && bp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CCODE_type_of_ClipVarType && bp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != PCODE_type_of_ClipVarType)
      bp = 0;

   c = ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
   if (start >= c)
      return 0;
   if (start < 0)
      start = 0;
   if (count <= 0 || count > (c - start))
      count = c - start;

   if (bp)
      swap_sort(ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + start, count, compare_var, ClipMachineMemory, bp);
   else
      q_sort(ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + start, count, compare_var, ClipMachineMemory, bp);

   rp = RETPTR(ClipMachineMemory);
   ap = ARGPTR(ClipMachineMemory, 1);
   _clip_clone(ClipMachineMemory, rp, ap);

   return 0;
}
