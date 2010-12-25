void
_clip_check_stack(ClipMachine * ClipMachineMemory)
{
   ClipFrame *fp = ClipMachineMemory->fp;

   if (fp && fp->ClipVar_sp_of_ClipFrame > (fp->ClipVar_stack_of_ClipFrame + fp->stklen_of_ClipFrame))
   {
      _clip_call_errblock(ClipMachineMemory, EG_MEM);
      _clip_logg(0, "stack check failed at line %d file %d", fp->line_of_ClipFrame, fp->filename_of_ClipFrame ? fp->filename_of_ClipFrame : "unknown");
      exit(1);
   }
}
