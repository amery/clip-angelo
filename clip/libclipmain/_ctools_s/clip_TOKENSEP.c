int
clip_TOKENSEP(ClipMachine * ClipMachineMemory)
{
   int flag = _clip_parl(ClipMachineMemory, 1);

   void *atsep = _clip_fetch_item(ClipMachineMemory, HASH_token_atsep);

   atsep += 3 * sizeof(int);

   if (flag)
      atsep += 2;
   _clip_retcn(ClipMachineMemory, atsep, 1);
   return 0;
}
