int
clip_SX_ISSHARED(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_ISSHARED";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipMachineMemory->m6_error = 0;
   CHECKWA(wa);

   _clip_retl(ClipMachineMemory, wa->rd->shared);
   return 0;
}
