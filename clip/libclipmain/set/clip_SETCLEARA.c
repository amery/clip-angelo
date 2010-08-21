int
clip_SETCLEARA(ClipMachine * ClipMachineMemory)
{
   int l, attr = _clip_parni(ClipMachineMemory, 1);

   char *s = _clip_parcl(ClipMachineMemory, 1, &l);

   int *set = _clip_fetch_item(ClipMachineMemory, HASH_setcleara);

   if (s != NULL)
      attr = _clip_str2attr(s, l);
   *set = attr;
   _clip_retc(ClipMachineMemory, "");
   return 0;
}
