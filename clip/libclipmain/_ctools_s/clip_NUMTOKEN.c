int
clip_NUMTOKEN(ClipMachine * ClipMachineMemory)
{
   int l1, l2, ret = 0, nt;

   unsigned char *e, *end, *send, *dstr, *buf;

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l1);

   unsigned char *sstr = (unsigned char *) _clip_parcl(ClipMachineMemory, 2, &l2);

   int tlen = _clip_parni(ClipMachineMemory, 3);

   if (str == NULL)
   {
      _clip_retni(ClipMachineMemory, 0);
      return 0;
      /*
         return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "NUMTOKEN");
       */
   }

   if (sstr == NULL)
   {
      dstr = (unsigned char *) CLIP_TOKEN_CHARACTERS;
      l2 = CLIP_TOKEN_CHARS_NUM;
   }
   else
      dstr = sstr;

   if (tlen <= 0)
      tlen = CLIP_TOKEN_MAXLEN;

   end = str + l1;
   send = dstr + l2;

   buf = calloc(256, 1);
   for (e = dstr; e < send; e++)
      buf[(int) (*e)] = 1;

   for (e = str; e < end && buf[(int) (*e)]; e++);
   for (; e < end;)
   {
      for (; e < end && !buf[(int) (*e)]; e++);
      for (nt = 0; nt < tlen && e < end && buf[(int) (*e)]; e++, nt++);
      ret++;

   }
   free(buf);
   _clip_retni(ClipMachineMemory, ret);
   return 0;
}
