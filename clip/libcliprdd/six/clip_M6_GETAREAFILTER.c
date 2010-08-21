int
clip_M6_GETAREAFILTER(ClipMachine * ClipMachineMemory)
{
   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipMachineMemory->m6_error = 0;
   if (!wa)
   {
      _clip_retni(ClipMachineMemory, -1);
      return 0;
   }
   if (wa->rd->filter)
      _clip_retni(ClipMachineMemory, wa->rd->filter->handle);
   else
      _clip_retni(ClipMachineMemory, -1);
   return 0;
}
