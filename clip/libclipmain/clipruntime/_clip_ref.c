CLIP_DLLEXPORT int
_clip_ref(ClipMachine * ClipMachineMemory, void *Vp, int noadd)
{
   ClipVar *nv;

   ClipVar *vp = (ClipVar *) Vp;

   ClipVar *sp = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame;

   CLEAR_CLIPVAR(sp);

   if (!vp)
      return EG_NOVAR;

   if (vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags)
   {
      if (noadd)
	 *sp = *vp;
      else
      {
	 dup_ref(sp, vp);
	 sp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MREF_ClipFlags;
      }

      ++(ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame);
      CLIP_CHECK_STACK;
      return 0;
   }

   nv = NEW(ClipVar);

   if (vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MSTAT_ClipFlags)
   {
      /*vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType &= ~F_MSTAT_ClipFlags; */
      _clip_clone(ClipMachineMemory, nv, vp);
   }
   else
      *nv = *vp;

   nv->ClipType_t_of_ClipVar.count_of_ClipType = noadd ? 1 : 2;
   vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MREF_ClipFlags /*mptr?F_MPTR_ClipFlags:F_MREF_ClipFlags */ ;
   vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = nv;
   vp->ClipType_t_of_ClipVar.field_of_ClipType = 0;
   vp->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar = 0;

   sp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = nv->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType;
   sp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MREF_ClipFlags /*mptr?F_MPTR_ClipFlags:F_MREF_ClipFlags */ ;
   sp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = nv;
   sp->ClipType_t_of_ClipVar.field_of_ClipType = 0;
   sp->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar = 0;

   ++(ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame);
   CLIP_CHECK_STACK;
   return 0;
}
