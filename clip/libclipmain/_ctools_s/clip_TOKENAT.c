int
clip_TOKENAT(ClipMachine * ClipMachineMemory)
{
   int flag = _clip_parl(ClipMachineMemory, 1);

   int *atsep = _clip_fetch_item(ClipMachineMemory, HASH_token_atsep);

   atsep++;
   if (flag)
      atsep++;
   _clip_retni(ClipMachineMemory, *atsep);
   return 0;
}
