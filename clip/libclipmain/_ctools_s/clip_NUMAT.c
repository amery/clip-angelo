int
clip_NUMAT(ClipMachine * ClipMachineMemory)
{
   int l1, l2, ret = 0, atlike0, atlike1;

   unsigned char *e1, *e2, *s, *beg, *end, *send;

   unsigned char *sstr = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l1);

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 2, &l2);

   int ignore = _clip_parni(ClipMachineMemory, 3);

   char *atlike = _clip_fetch_item(ClipMachineMemory, HASH_setatlike);

   int cset = (*(unsigned char *) _clip_fetch_item(ClipMachineMemory, HASH_csetatmupa)) == 't';

   atlike0 = atlike[0];
   atlike1 = atlike[1];

   if (sstr == NULL || str == NULL)
   {
      _clip_retni(ClipMachineMemory, 0);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "NUMAT");
   }
   end = str + l2 - l1 + 1;
   beg = str + l2;
   send = sstr + l1;
   if (ignore < 0)
      ignore = 0;
   for (e1 = str + ignore; e1 < end; e1++)
   {
      for (s = sstr, e2 = e1; s < send && (*s == *e2 || (atlike0 && *s == atlike1)); s++, e2++);
      if (s != send)
	 continue;
      ret++;
      if (!cset)
	 e1 = e2 - 1;
   }
   _clip_retni(ClipMachineMemory, ret);
   return 0;
}
