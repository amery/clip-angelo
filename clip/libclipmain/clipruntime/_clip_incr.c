CLIP_DLLEXPORT int
_clip_incr(struct ClipMachine *ClipMachineMemory, void *lval, int isIncr, int isPost)
{
   int ret;

   ClipVar *vp = (ClipVar *) lval;

   if (isPost)
   {
      if ((ret = _clip_push(ClipMachineMemory, vp)))
	 return ret;
      if ((ret = incr_var(ClipMachineMemory, vp, isIncr)))
	 return ret;
   }
   else
   {
      if ((ret = incr_var(ClipMachineMemory, vp, isIncr)))
	 return ret;
      if ((ret = _clip_push(ClipMachineMemory, vp)))
	 return ret;
   }
   return 0;
}
