int
clip_SX_INDEXCOUNT(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_INDEXCOUNT";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipMachineMemory->m6_error = 0;
   CHECKWA(wa);
   _clip_retni(ClipMachineMemory, wa->rd->idxs_opened);
   return 0;
}
