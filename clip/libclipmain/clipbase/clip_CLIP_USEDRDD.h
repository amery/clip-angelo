int
clip_CLIP_USEDRDD(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "CLIP_USEDRDD";

   DBWorkArea *wa = cur_area(ClipMachineMemory);


   CHECKWA(wa);
   _clip_retc(ClipMachineMemory, wa->driver);
   return 0;
}
