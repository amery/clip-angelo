int
clip_DBCLEARINDEX(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBCLEARINDEX";

   DBWorkArea *wa = cur_area(ClipMachineMemory);


   CHECKWA(wa);

   return rdd_clearindex(ClipMachineMemory, wa->rd, __PROC__);
}
