int
clip_RDDNAME(ClipMachine * ClipMachineMemory)
{
#ifndef CLIPPER50_COMPATIBLE
   const char *__PROC__ = "RDDNAME";
#endif
   DBWorkArea *wa = cur_area(ClipMachineMemory);

   _clip_retc(ClipMachineMemory, "");
   CHECKWA(wa);

   _clip_retc(ClipMachineMemory, wa->driver);
   return 0;
}
