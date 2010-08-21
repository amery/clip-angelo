int
clip_RESTTOKEN(ClipMachine * ClipMachineMemory)
{
   int l, len_str, tmp1 = 3 * sizeof(int) + 4, tmp2 = 5 * sizeof(int) + 4;

   void *atsep, *del;

   unsigned char *str;

   unsigned char *s = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l);

   if (s == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "RESTTOKEN");
   }
   _clip_free_item(ClipMachineMemory, HASH_token_atsep);
   _clip_free_item(ClipMachineMemory, HASH_token_string);
   _clip_free_item(ClipMachineMemory, HASH_token_delimiters);
   atsep = calloc(tmp2, 1);
   memcpy(atsep, s, tmp2);
   _clip_store_item(ClipMachineMemory, HASH_token_atsep, atsep);
   len_str = *((int *) (s + tmp1));
   str = malloc(len_str + 1);
   memcpy(str, s + tmp2, len_str);
   str[len_str] = 0;
   _clip_store_item(ClipMachineMemory, HASH_token_string, str);
   del = malloc(256);
   memcpy(del, s + tmp2 + len_str, 256);
   _clip_store_item(ClipMachineMemory, HASH_token_delimiters, del);

   _clip_retc(ClipMachineMemory, "");
   return 0;
}
