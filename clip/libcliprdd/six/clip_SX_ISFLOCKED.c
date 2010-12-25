int
clip_SX_ISFLOCKED(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_ISFLOCKED";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipMachineMemory->m6_error = 0;
   CHECKWA(wa);
   _clip_retl(ClipMachineMemory, wa->rd->flocked);
   return 0;
}
