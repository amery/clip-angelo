int
clip_ISDBEXCL(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ISDBEXCL";

   DBWorkArea *wa = cur_area(ClipMachineMemory);


   CHECKWA(wa);
   _clip_retl(ClipMachineMemory, !wa->rd->shared);
   return 0;
}
