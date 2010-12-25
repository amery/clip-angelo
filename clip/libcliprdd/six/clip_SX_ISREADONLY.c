int
clip_SX_ISREADONLY(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_ISREADONLY";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipMachineMemory->m6_error = 0;
   CHECKWA(wa);

   _clip_retl(ClipMachineMemory, wa->rd->readonly);
   return 0;
}
