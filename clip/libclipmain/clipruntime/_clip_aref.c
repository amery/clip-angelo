CLIP_DLLEXPORT ClipVar *
_clip_aref(ClipMachine * ClipMachineMemory, ClipVar * ap, int ndim, long *dims)
{
   int c;

   long ind;

   ClipVar *app = 0;

   ap = _clip_vptr(ap);

   if ((ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != ARRAY_type_of_ClipVarType && ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != MAP_type_of_ClipVarType) || !ndim)
   {
      _clip_trap_printf(ClipMachineMemory, __file__, __LINE__, "aget for non-array object");
      return 0;
   }

   if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType)
   {
      app = _clip_mget(ClipMachineMemory, ap, dims[0]);
   }
   else
   {
      c = ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
      ind = GETLONG(dims);

      if (ind < 0 || ind >= c)
      {
	 _clip_trap_printf(ClipMachineMemory, __file__, __LINE__, "index for aget is out-of range: ind=%d, count=%d", ind + 1, c);
	 return 0;
      }

      app = ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + ind;
   }

   if (ndim > 1)
   {
      return _clip_aref(ClipMachineMemory, app, ndim - 1, dims + 1);
   }

   return app;
}
