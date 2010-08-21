CLIP_DLLEXPORT int
_clip_mdel(ClipMachine * ClipMachineMemory, ClipVar * ap, long no)
{
   int ind;

   ap = _clip_vptr(ap);

   if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != MAP_type_of_ClipVarType)
   {
      _clip_trap_printf(ClipMachineMemory, __file__, __LINE__, "mdel for non-map object");
      return _clip_call_errblock(ClipMachineMemory, 1);
   }
   if (search_map(ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar, ap->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar, no, &ind))
   {
      _clip_destroy(ClipMachineMemory, &((ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar + ind)->ClipVar_var_of_ClipVarEl));

      ap->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar--;
      if (ind < ap->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar)
	 memmove(ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar + ind,
		 ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar + ind + 1, (ap->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar - ind) * sizeof(ClipVarEl));
   }
   return 0;
}
