int
clip_PROCNAME(ClipMachine * ClipMachineMemory)
{
   int level = _clip_parni(ClipMachineMemory, 1), l;

   ClipFrame *fp;

   for (fp = ClipMachineMemory->fp, l = 0; fp && l < level; fp = fp->ClipFrame_up_of_ClipFrame, l++)
      ;

   if (!fp)
   {
#if 1
      _clip_retc(ClipMachineMemory, "");
      return 0;
#else
      _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "PROCNAME: invalid call level '%d'", level);
      return 1;
#endif
   }

   _clip_retc(ClipMachineMemory, (char *) fp->procname_of_ClipFrame);

   return 0;
}
