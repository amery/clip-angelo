int
clip_USED(ClipMachine * ClipMachineMemory)
{
   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int ret = 0;

   if (wa)
      ret = wa->used;

   _clip_retl(ClipMachineMemory, ret);
   return 0;
}
