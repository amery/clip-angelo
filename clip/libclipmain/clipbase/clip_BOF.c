int
clip_BOF(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "BOF";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int bof, er;

   _clip_retl(ClipMachineMemory, 0);
   if (!wa)
      return 0;
   if ((er = rdd_bof(ClipMachineMemory, wa->rd, &bof, __PROC__)))
      return er;
   _clip_retl(ClipMachineMemory, bof);
   return 0;
}
