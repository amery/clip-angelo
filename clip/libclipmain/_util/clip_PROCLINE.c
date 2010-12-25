int
clip_PROCLINE(ClipMachine * ClipMachineMemory)
{
   int level = _clip_parni(ClipMachineMemory, 1), l;

   ClipFrame *fp;

   for (fp = ClipMachineMemory->fp, l = 0; fp && l < level; fp = fp->ClipFrame_up_of_ClipFrame, l++)
      ;

   if (!fp)
   {
#if 1
      _clip_retni(ClipMachineMemory, 0);
      return 0;
#else
      _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "PROCNAME: invalid call level '%d'", level);
      return 1;
#endif
   }

   _clip_retni(ClipMachineMemory, fp->line_of_ClipFrame);
   return 0;
}
