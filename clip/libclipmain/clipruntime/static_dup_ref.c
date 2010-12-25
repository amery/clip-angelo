static void
dup_ref(ClipVar * dest, ClipVar * src)
{
   if (dest == src || src->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar == dest)
      return;
   CLEAR_CLIPVAR(dest);
   dest->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = src->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType;
   dest->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = src->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType;
   dest->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = src->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
   dest->ClipType_t_of_ClipVar.field_of_ClipType = src->ClipType_t_of_ClipVar.field_of_ClipType;
   dest->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar = src->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar;
   ++(src->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar->ClipType_t_of_ClipVar.count_of_ClipType);
}
