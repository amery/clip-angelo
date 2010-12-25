int
clip_SAVETOKEN(ClipMachine * ClipMachineMemory)
{
   int l, len_str, tmp = 3 * sizeof(int) + 4;

   unsigned char *ret;

   void *atsep = _clip_fetch_item(ClipMachineMemory, HASH_token_atsep);

   void *str = _clip_fetch_item(ClipMachineMemory, HASH_token_string);

   void *del = _clip_fetch_item(ClipMachineMemory, HASH_token_delimiters);

   len_str = *((int *) (atsep + tmp));
   l = tmp + 2 * sizeof(int) + len_str + 256;

   ret = malloc(l + 1);
   tmp = 5 * sizeof(int) + 4;

   memcpy(ret, atsep, tmp);
   memcpy(ret + tmp, str, len_str);
   memcpy(ret + tmp + len_str, del, 256);
   ret[l] = 0;
   _clip_retcn_m(ClipMachineMemory, (char *) ret, l);
   return 0;
}
