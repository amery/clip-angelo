int
clip_DBFILTER(ClipMachine * ClipMachineMemory)
{
   DBWorkArea *wa = cur_area(ClipMachineMemory);

   if (!wa)
      return 0;

   _clip_retc(ClipMachineMemory, "");
   if (wa->rd->filter && wa->rd->filter->sfilter)
      _clip_retc(ClipMachineMemory, wa->rd->filter->sfilter);
   return 0;
}
