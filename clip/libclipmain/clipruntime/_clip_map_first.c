CLIP_DLLEXPORT int
_clip_map_first(ClipMachine * ClipMachineMemory, int keys, int *_if)
{
   ClipVar *rp = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - 1;

   ClipVar *arr = _clip_vptr(rp);

   ClipVar *vp = 0;

   long no = 0;

   *_if = 0;

   if (arr->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType)
   {
      arr->ClipMapVar_m_of_ClipVar.current_of_ClipMapVar = 0;
      if (arr->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar > 0)
      {
	 no = arr->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[0].no_of_ClipVarEl;
	 vp = &(arr->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[0].ClipVar_var_of_ClipVarEl);
	 *_if = 1;
      }
   }
   else if (arr->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
   {
      arr->ClipArrVar_a_of_ClipVar.current_of_ClipArrVar = 0;
      if (arr->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar > 0)
      {
	 no = 1;
	 vp = arr->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar;
	 *_if = 1;
      }
   }
   else
      return EG_ARG;

   if (*_if)
   {
      _clip_destroy(ClipMachineMemory, rp);
      if (keys)
	 _clip_var_num(no, rp);
      else
	 _clip_clone(ClipMachineMemory, rp, vp);
   }
   else
      _clip_pop(ClipMachineMemory);

   return 0;
}
