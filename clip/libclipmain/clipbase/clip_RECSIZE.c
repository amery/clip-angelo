int
clip_RECSIZE(ClipMachine * ClipMachineMemory)
{
   DBWorkArea *wa = cur_area(ClipMachineMemory);

   _clip_retni(ClipMachineMemory, 0);
   if (!wa)
      return 0;

   _clip_retni(ClipMachineMemory, wa->rd->recsize);
   return 0;
}
