int
clip_AFILL(ClipMachine * ClipMachineMemory)
{
   ClipVar *ap = _clip_par(ClipMachineMemory, 1);

   ClipVar *vp = _clip_spar(ClipMachineMemory, 2);

   int start = _clip_parni(ClipMachineMemory, 3) - 1;

   int count = _clip_parni(ClipMachineMemory, 4);

   int c, i, end, r;

   ClipVar *retp = RETPTR(ClipMachineMemory), *arrp = ARGPTR(ClipMachineMemory, 1);

   _clip_clone(ClipMachineMemory, retp, arrp);

   if (!ap || !vp || ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != ARRAY_type_of_ClipVarType)
      return 0;

   c = ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
   if (start >= c)
      return 0;

   if (start < 0)
      start = 0;
   if (_clip_parinfo(ClipMachineMemory, 0) < 4 || count > (c - start))
      count = c - start;

   for (i = start, end = start + count; i < end; ++i)
   {
      ClipVar *dp = ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + i;

      _clip_destroy(ClipMachineMemory, dp);
      r = _clip_clone(ClipMachineMemory, dp, vp);
      if (r)
	 return r;
   }
   return 0;
}
