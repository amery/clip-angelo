/*  mapkeys(map) --> array of map keys */
int
clip_MAPKEYS(ClipMachine * ClipMachineMemory)
{
   ClipVar *vp = _clip_par(ClipMachineMemory, 1);

   ClipVar *rp, *ap;

   int size, i;

   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != MAP_type_of_ClipVarType)
      return 0;

   rp = RETPTR(ClipMachineMemory);

   CLEAR_CLIPVAR(rp);
   rp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = ARRAY_type_of_ClipVarType;
   rp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MPTR_ClipFlags;
   rp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = ap = (ClipVar *) calloc(sizeof(ClipVar), 1);
   ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = ARRAY_type_of_ClipVarType;
   ap->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   size = ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar = vp->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar;
   ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar = (ClipVar *) calloc(sizeof(ClipVar), size);
   ap->ClipType_t_of_ClipVar.count_of_ClipType = 1;

   for (i = 0; i < size; ++i)
   {
      ClipVar *vpp = ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + i;

      vpp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
      vpp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = (vp->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar + i)->no_of_ClipVarEl;
      vpp->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
   }

   return 0;
}
