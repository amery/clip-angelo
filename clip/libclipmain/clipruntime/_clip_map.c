CLIP_DLLEXPORT int
_clip_map(ClipMachine * ClipMachineMemory, ClipVar * vp)
{
   ClipVar *ap;

   _clip_destroy(ClipMachineMemory, vp);
   ap = NEW(ClipVar);
   vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = MAP_type_of_ClipVarType;
   vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MPTR_ClipFlags;
   vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = ap;
   ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = MAP_type_of_ClipVarType;
   ap->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   ap->ClipType_t_of_ClipVar.count_of_ClipType = 1;
   ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar = 0;
   ap->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar = 0;

   return 0;
}
