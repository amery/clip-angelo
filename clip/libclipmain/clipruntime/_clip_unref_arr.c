CLIP_DLLEXPORT void
_clip_unref_arr(ClipMachine * ClipMachineMemory)
{
   ClipVar *sp = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - 1, *ap;

   int type = _clip_type(sp);

   if ((type != ARRAY_type_of_ClipVarType && type != MAP_type_of_ClipVarType)
       || (sp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType != F_MPTR_ClipFlags && sp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType != F_MREF_ClipFlags))
      return;
   ap = _clip_vptr(sp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar);

   _clip_dup(ClipMachineMemory, sp, ap);
}
