CLIP_DLLEXPORT int
_clip_mind(ClipMachine * ClipMachineMemory, ClipVar * ap, long no, int *ind)
{
   ap = _clip_vptr(ap);
   if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != MAP_type_of_ClipVarType)
   {
      _clip_trap_printf(ClipMachineMemory, __file__, __LINE__, "mind for non-map object");
      return _clip_call_errblock(ClipMachineMemory, 1);
   }

   return !search_map(ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar, ap->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar, no, ind);
}
