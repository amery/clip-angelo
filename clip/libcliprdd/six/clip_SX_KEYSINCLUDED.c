int
clip_SX_KEYSINCLUDED(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_KEYSINCLUDED";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipMachineMemory->m6_error = 0;
   CHECKWA(wa);

   _clip_retni(ClipMachineMemory, wa->rd->keysincluded);
   return 0;
}
