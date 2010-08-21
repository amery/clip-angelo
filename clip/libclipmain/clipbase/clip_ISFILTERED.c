int
clip_ISFILTERED(ClipMachine * ClipMachineMemory)
{
   DBWorkArea *wa = cur_area(ClipMachineMemory);

   _clip_retl(ClipMachineMemory, 0);
   if (!wa)
      return 0;

   _clip_retl(ClipMachineMemory, wa->rd->filter != NULL);
   return 0;
}
