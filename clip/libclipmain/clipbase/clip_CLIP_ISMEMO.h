int
clip_CLIP_ISMEMO(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "CLIP_ISMEMO";

   DBWorkArea *wa = cur_area(ClipMachineMemory);


   CHECKWA(wa);
   _clip_retl(ClipMachineMemory, wa->rd->memo ? 1 : 0);
   return 0;
}
