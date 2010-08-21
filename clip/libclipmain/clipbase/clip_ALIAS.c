int
clip_ALIAS(ClipMachine * ClipMachineMemory)
{
   int nwa = _clip_parni(ClipMachineMemory, 1);

   DBWorkArea *wa;

   if (nwa > 0)
      wa = get_area(ClipMachineMemory, nwa, 0, 0);
   else
      wa = cur_area(ClipMachineMemory);

   if (!wa)
   {
      _clip_retc(ClipMachineMemory, "");
      return 0;
   }

   _clip_retc(ClipMachineMemory, wa->alias);
   return 0;
}
