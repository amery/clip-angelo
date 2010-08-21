CLIP_DLLEXPORT int
_clip_storl(ClipMachine * ClipMachineMemory, int val, int num, int ind)
{
   ClipVar *vp = _clip_par(ClipMachineMemory, num);

   if (!vp)
      return 0;

   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
   {
      if (ind < 0 || ind >= vp->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar)
	 return 0;
      vp = _clip_vptr(vp->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + ind);
   }

   if (vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType == F_MSTAT_ClipFlags)
      return 0;

   _clip_destroy(ClipMachineMemory, vp);
   _clip_var_log(val, vp);

   return 1;
}
