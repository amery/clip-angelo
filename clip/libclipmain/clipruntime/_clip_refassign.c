CLIP_DLLEXPORT int
_clip_refassign(ClipMachine * ClipMachineMemory, void *Lval, int isTop)
{
   ClipVar *rval = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - 1;

   ClipVar *lval = (ClipVar *) Lval;

   _clip_destroy(ClipMachineMemory, lval);

   if (!(rval->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags))
      return 1;
   *lval = *rval;
   if (isTop)
      ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame--;
   else
   {
      rval->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar->ClipType_t_of_ClipVar.count_of_ClipType++;
   }

   return 0;
}
