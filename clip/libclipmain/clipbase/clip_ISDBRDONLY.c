int
clip_ISDBRDONLY(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ISDBRDONLY";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   CHECKWA(wa);
   _clip_retl(ClipMachineMemory, wa->rd->readonly);
   _clip_retl(ClipMachineMemory, wa->rd->memo ? 1 : 0);
   return 0;
}
