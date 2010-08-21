CLIP_DLLEXPORT int
_clip_madd(ClipMachine * ClipMachineMemory, ClipVar * ap, long no, ClipVar * vp)
{
   int ind, c;

   ClipVarEl *vlp;

   ap = _clip_vptr(ap);

   if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != MAP_type_of_ClipVarType)
   {
      _clip_trap_printf(ClipMachineMemory, __file__, __LINE__, "madd for non-map object");
      return _clip_call_errblock(ClipMachineMemory, 1);
   }

   c = ap->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar;

   if (search_map(ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar, c, no, &ind))
   {
      vlp = ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar + ind;
      _clip_destroy(ClipMachineMemory, &vlp->ClipVar_var_of_ClipVarEl);
   }
   else
   {
      ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar = (ClipVarEl *) realloc(ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar, (c + 1) * sizeof(ClipVarEl));
      if (ind < c)
	 memmove(ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar + ind + 1, ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar + ind, (c - ind /* -1 */ ) * sizeof(ClipVarEl));
      ap->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar++;
      vlp = ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar + ind;
      memset(vlp, 0, sizeof(ClipVarEl));
      vlp->no_of_ClipVarEl = no;
   }
   _clip_mclone(ClipMachineMemory, &vlp->ClipVar_var_of_ClipVarEl, vp);

   return 0;
}
