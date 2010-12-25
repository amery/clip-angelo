int
clip_TOKENINIT(ClipMachine * ClipMachineMemory)
{
   int l1, l2, aaa = 3 * sizeof(int) + 4;

   unsigned char *e, *dstr, *tstr;

   int *tmp;

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l1);

   unsigned char *sstr = (unsigned char *) _clip_parcl(ClipMachineMemory, 2, &l2);

   int tlen = _clip_parni(ClipMachineMemory, 3);

   void *atsep = _clip_fetch_item(ClipMachineMemory, HASH_token_atsep);

   unsigned char *buf = _clip_fetch_item(ClipMachineMemory, HASH_token_delimiters);

   if (_clip_parinfo(ClipMachineMemory, 0) == 0)
   {
      memset(atsep, 0, aaa);
      _clip_retl(ClipMachineMemory, 1);
      return 0;
   }

   if (str == NULL)
   {
      _clip_retl(ClipMachineMemory, 0);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "TOKENINIT");
   }
   memset(atsep, 0, 5 * sizeof(int) + 4);

   _clip_free_item(ClipMachineMemory, HASH_token_string);
   tstr = malloc(l1 + 1);
   memcpy(tstr, str, l1);
   tstr[l1] = 0;
   _clip_store_item(ClipMachineMemory, HASH_token_string, tstr);
   tmp = (int *) (atsep + aaa);
   *tmp = l1;

   if (sstr == NULL)
   {
      dstr = (unsigned char *) CLIP_TOKEN_CHARACTERS;
      l2 = CLIP_TOKEN_CHARS_NUM;
   }
   else
      dstr = sstr;

   memset(buf, 0, 256);
   for (e = dstr; e < dstr + l2; e++)
      buf[(int) (*e)] = 1;
   tmp = (int *) (atsep + aaa + sizeof(int));

   *tmp = tlen;

   _clip_retl(ClipMachineMemory, 1);
   return 0;
}
