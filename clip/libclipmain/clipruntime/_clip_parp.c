CLIP_DLLEXPORT int
_clip_parp(ClipMachine * ClipMachineMemory, int num, int *len, int *dec)
{
   *len = *dec = 0;

   if (num > 0 && num <= ClipMachineMemory->argc)
   {
      ClipVar *vp = (ClipMachineMemory->bp - (ClipMachineMemory->argc - (num - 1)));

      if (vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags)
	 vp = vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
      *len = vp->ClipType_t_of_ClipVar.len_of_ClipType;
      *dec = vp->ClipType_t_of_ClipVar.dec_of_ClipType;
      return 0;
   }

   return -1;
}
