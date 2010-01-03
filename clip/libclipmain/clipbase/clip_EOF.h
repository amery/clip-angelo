int
clip_EOF(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "EOF";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int       eof, er;

   _clip_retl(ClipMachineMemory, 0);
   if (!wa)
      return 0;
   if ((er = rdd_eof(ClipMachineMemory, wa->rd, &eof, __PROC__)))
      return er;
   _clip_retl(ClipMachineMemory, eof);
   return 0;
}
