int
clip_SX_ISREINDEX(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_ISREINDEX";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipMachineMemory->m6_error = 0;
   CHECKWA(wa);
   _clip_retl(ClipMachineMemory, wa->rd->reindexing);
   return 0;
}
