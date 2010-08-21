int
clip_TOKENEND(ClipMachine * ClipMachineMemory)
{
   int end, len;

   void *atsep = _clip_fetch_item(ClipMachineMemory, HASH_token_atsep);

   atsep += 2 * sizeof(int);

   end = *((int *) atsep);
   atsep += sizeof(int) + 4;

   len = *((int *) atsep);
   _clip_retl(ClipMachineMemory, end >= len);
   return 0;
}
