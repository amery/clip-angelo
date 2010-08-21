int
clip_DBCLEARRELATION(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBCLEARRELATION";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   if (!wa)
      return 0;
   return rdd_clearrelation(ClipMachineMemory, wa->rd, __PROC__);
}
