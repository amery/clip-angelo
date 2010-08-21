int
clip_COLORTON(ClipMachine * ClipMachineMemory)
{
   int l, attr = _clip_parni(ClipMachineMemory, 1);

   char *s = _clip_parcl(ClipMachineMemory, 1, &l);

   if (s != NULL)
      attr = _clip_str2attr(s, l);
   _clip_retni(ClipMachineMemory, attr);
   return 0;
}
