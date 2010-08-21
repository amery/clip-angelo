CLIP_DLLEXPORT int
_clip_stordj(ClipMachine * ClipMachineMemory, long julian, int num, int ind)
{
   ClipVar *vp = _clip_par(ClipMachineMemory, num);

   if (!vp)
      return 0;

   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
   {
      if (ind < 0 || ind >= vp->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar)
	 return 0;
      vp = _clip_vptr(vp->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + ind);
      goto assign;
   }
   else if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == DATE_type_of_ClipVarType)
   {
    assign:
      if (vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType == F_MSTAT_ClipFlags)
	 return 0;
      vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar = julian;
   }
   else
   {
      return 0;
   }

   return 1;
}
