CLIP_DLLEXPORT int
_clip_aset(ClipMachine * ClipMachineMemory, ClipVar * ap, ClipVar * vp, int ndim, long *dims)
{
   int c;

   long ind;

   ClipVar *app;

   ap = _clip_vptr(ap);

   if ((ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != ARRAY_type_of_ClipVarType && ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != MAP_type_of_ClipVarType) || !ndim)
   {
      _clip_trap_printf(ClipMachineMemory, __file__, __LINE__, "aset for non-array object");
      return _clip_call_errblock(ClipMachineMemory, 1);
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
	 _clip_trap_printf(ClipMachineMemory, __file__, __LINE__, "index for aset is out-of range: ind=%d, count=%d", ind + 1, c);
	 return _clip_call_errblock(ClipMachineMemory, 1);
      }

      app = ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + ind;
   }

   if (ndim > 1)
   {
      return _clip_aset(ClipMachineMemory, app, vp, ndim - 1, dims + 1);
   }

/*_clip_destroy(ClipMachineMemory, app);*/
   return _clip_mclone(ClipMachineMemory, app, vp);
}
