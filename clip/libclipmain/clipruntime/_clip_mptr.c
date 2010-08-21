CLIP_DLLEXPORT void
_clip_mptr(ClipMachine * ClipMachineMemory, ClipVar * refs, int num)
{
   while (num--)
   {
      ClipVar *vp = NEW(ClipVar);

      refs->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = UNDEF_type_of_ClipVarType;
      refs->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MREF_ClipFlags;
      refs->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = vp;
      refs->ClipType_t_of_ClipVar.field_of_ClipType = 0;
      refs->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar = 0;
      vp->ClipType_t_of_ClipVar.count_of_ClipType = 1;

      refs++;
   }
}
