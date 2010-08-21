int
clip_ORDLISTCLEAR(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ORDLISTCLEAR";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   CHECKWA(wa);
   return rdd_clearindex(ClipMachineMemory, wa->rd, __PROC__);
}
