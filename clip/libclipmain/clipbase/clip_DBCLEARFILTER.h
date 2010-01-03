int
clip_DBCLEARFILTER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBCLEARFILTER";

   DBWorkArea *wa = cur_area(ClipMachineMemory);


   CHECKWA(wa);

   return rdd_clearfilter(ClipMachineMemory, wa->rd, __PROC__);
}
