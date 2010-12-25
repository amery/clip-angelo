int
clip_NUMCOUNT(ClipMachine * ClipMachineMemory)
{
   long *count = (long *) _clip_fetch_item(ClipMachineMemory, HASH_numcount);

   int par = _clip_parinfo(ClipMachineMemory, 0);

   if (par == 1)
      *count += _clip_parnl(ClipMachineMemory, 1);
   if (par == 2)
      *count = _clip_parnl(ClipMachineMemory, 1);
   _clip_retnl(ClipMachineMemory, *count);
   return 0;
}
