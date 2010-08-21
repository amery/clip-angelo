CLIP_DLLEXPORT int
_clip_asize(ClipMachine * ClipMachineMemory, ClipVar * ap, int ndim, long *dims)
{
   int c;

   long i, ind;

   ap = _clip_vptr(ap);

   if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != ARRAY_type_of_ClipVarType || !ndim)
   {
      _clip_trap_printf(ClipMachineMemory, __file__, __LINE__, "asize for non-array object");
      return _clip_call_errblock(ClipMachineMemory, 1);
   }

   c = ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
   ind = GETLONG(dims);

   if (ndim > 1)
   {
      ClipVar *app;

      ind = dims[0];
      if (ind < 0 || ind >= c)
      {
	 _clip_trap_printf(ClipMachineMemory, __file__, __LINE__, "index for asize is out-of range: ind=%d, count=%d", ind + 1, c);
	 return _clip_call_errblock(ClipMachineMemory, 1);
      }
      app = ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + ind;
      return _clip_asize(ClipMachineMemory, app, ndim - 1, dims + 1);
   }

   for (i = ind; i < c; ++i)
      _clip_destroy(ClipMachineMemory, ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + i);

   ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar = (ClipVar *) realloc(ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar, sizeof(ClipVar) * ind);
   ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar = ind;

   for (i = c; i < ind; ++i)
      memset(ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + i, 0, sizeof(ClipVar));

   return 0;
}
