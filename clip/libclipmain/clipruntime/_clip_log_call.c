CLIP_DLLEXPORT void
_clip_log_call(ClipMachine * ClipMachineMemory)
{
   ClipFrame *fp = ClipMachineMemory->fp;

   if (_clip_profiler)
      _clip_start_profiler(ClipMachineMemory);

   if (log_level >= 5)
   {
      _clip_logg(5, "C call: proc '%s' file '%s' line %d", fp->procname_of_ClipFrame ? fp->procname_of_ClipFrame : "unknown", fp->filename_of_ClipFrame, fp->line_of_ClipFrame);
   }
}
