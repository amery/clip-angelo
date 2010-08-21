
CLIP_DLLEXPORT void
_clip_resume(ClipMachine * ClipMachineMemory, int nlocals, int nreflocals)
{
   int i;

   ClipFrame *fp = ClipMachineMemory->fp;

   ClipVarFrame *vfp = fp->ClipVarFrame_localvars_of_ClipFrame;

   for (; fp->ClipVar_sp_of_ClipFrame > fp->ClipVar_stack_of_ClipFrame;)
   {
      --(fp->ClipVar_sp_of_ClipFrame);
      _clip_destroy(ClipMachineMemory, fp->ClipVar_sp_of_ClipFrame);
   }

/*      resume_mp(ClipMachineMemory); */

   if (vfp)
   {
      for (i = 0; i < nlocals && i < vfp->size_of_ClipVarFrame; ++i)
	 _clip_destroy(ClipMachineMemory, vfp->ClipVar_vars_of_ClipVarFrame + i);

      for (; i < nlocals + nreflocals && i < vfp->size_of_ClipVarFrame; ++i)
      {
	 ClipVar *vp = vfp->ClipVar_vars_of_ClipVarFrame + i;

/*#if 1
			if ((vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR) && calc_loopcount(ClipMachineMemory, vp, vfp, 0) == vfp->refcount_of_ClipVarFrame - 1)
			{
					_clip_destroy(ClipMachineMemory, vp);
			}
#endif
*/
#if 1
	 if ((vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags)
	     && vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar->ClipType_t_of_ClipVar.count_of_ClipType == 1 && (calc_loopcount(ClipMachineMemory, vp, vfp, 0) == vfp->refcount_of_ClipVarFrame - 1))
	    _clip_destroy(ClipMachineMemory, vp);
#else
	 if ((vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags)
	     && (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType || vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType))
	 {
	    if (vp->ClipRefVar_p_of_ClipVar.vp->ClipType_t_of_ClipVar.count_of_ClipVect > 1)
	       vp->ClipRefVar_p_of_ClipVar.vp->ClipType_t_of_ClipVar.count_of_ClipVect--;
	    else
	       _clip_destroy(ClipMachineMemory, vp);
	 }
#endif
      }

      delete_ClipVarFrame(ClipMachineMemory, vfp);
   }

   if (fp->privates_of_ClipFrame)
   {
      long *p = fp->privates_of_ClipFrame;

      int num = *p;

      ++p;
      remove_private_vect(ClipMachineMemory, num, p);
      free(fp->privates_of_ClipFrame);
   }

   if (fp->ClipVarVect_tempvars_of_ClipFrame)
   {
      ClipVarVect *vvp = fp->ClipVarVect_tempvars_of_ClipFrame;

      for (i = 0; i < vvp->count_of_ClipVarVect; i++)
	 _clip_destroy(ClipMachineMemory, vvp->ClipVar_items_of_ClipVarVect + i);

      free(vvp->ClipVar_items_of_ClipVarVect);
      free(vvp);
      fp->ClipVarVect_tempvars_of_ClipFrame = 0;
   }

   ClipMachineMemory->fp = ClipMachineMemory->fp->ClipFrame_up_of_ClipFrame;
}
