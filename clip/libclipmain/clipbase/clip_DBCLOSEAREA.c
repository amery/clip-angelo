int
clip_DBCLOSEAREA(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBCLOSEAREA";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   if (!wa)
      return 0;

   return close_area(ClipMachineMemory, wa, __PROC__);
}
