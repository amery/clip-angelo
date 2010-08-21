int
clip_SX_TABLETYPE(ClipMachine * ClipMachineMemory)
{
   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipMachineMemory->m6_error = 0;
   _clip_retni(ClipMachineMemory, 0);
   if (!wa)
      return 0;
   _clip_retni(ClipMachineMemory, 1);
   return 0;
}
