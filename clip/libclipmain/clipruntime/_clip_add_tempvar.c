CLIP_DLLEXPORT ClipVar *
_clip_add_tempvar(ClipMachine * ClipMachineMemory, ClipVar * vp)
{
   ClipFrame *fp;

   ClipVarVect *vvp;

   ClipVar *np;

   if (!vp)
      return 0;

   fp = ClipMachineMemory->fp;
   vvp = fp->ClipVarVect_tempvars_of_ClipFrame;
   if (!vvp)
   {
      vvp = NEW(ClipVarVect);
      fp->ClipVarVect_tempvars_of_ClipFrame = vvp;
   }

   vvp->ClipVar_items_of_ClipVarVect = (ClipVar *) realloc(vvp->ClipVar_items_of_ClipVarVect, (vvp->count_of_ClipVarVect + 1) * sizeof(ClipVar));
   np = vvp->ClipVar_items_of_ClipVarVect + vvp->count_of_ClipVarVect;
   vvp->count_of_ClipVarVect++;

   _clip_clone(ClipMachineMemory, np, vp);

   return np;
}
