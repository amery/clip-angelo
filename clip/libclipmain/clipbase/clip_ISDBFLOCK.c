int
clip_ISDBFLOCK(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ISDBFLOCK";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   CHECKWA(wa);
   _clip_retl(ClipMachineMemory, wa->rd->flocked || !wa->rd->shared);
   return 0;
}
