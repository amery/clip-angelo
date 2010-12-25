CLIP_DLLEXPORT int
_clip_ains(ClipMachine * ClipMachineMemory, ClipVar * ap, int ndim, long *dims)
{
   int c;

   long ind;

   ap = _clip_vptr(ap);

   if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != ARRAY_type_of_ClipVarType || !ndim)
   {
      _clip_trap_printf(ClipMachineMemory, __file__, __LINE__, "ains for non-array object");
      return _clip_call_errblock(ClipMachineMemory, 1);
   }

   c = ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
   ind = GETLONG(dims);
   if (ind < 0 || ind >= c)
   {
      _clip_trap_printf(ClipMachineMemory, __file__, __LINE__, "index for ains is out-of range: ind=%d, count=%d", ind + 1, c);
      return _clip_call_errblock(ClipMachineMemory, 1);
   }
   if (ndim > 1)
   {
      ClipVar *app;

      app = ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + ind;
      return _clip_ains(ClipMachineMemory, app, ndim - 1, dims + 1);
   }

   if (c > 0)
   {
      _clip_destroy(ClipMachineMemory, &ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar[c - 1]);
      memmove(ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + ind + 1, ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + ind, (c - ind - 1) * sizeof(ClipVar));
   }
   memset(ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + ind, 0, sizeof(ClipVar));

   return 0;
}
