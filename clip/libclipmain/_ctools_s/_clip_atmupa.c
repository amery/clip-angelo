unsigned char *
_clip_atmupa(ClipMachine * ClipMachineMemory, unsigned char *sstr, int l1, unsigned char *str, int l2, int ignore, int count, int flag)
{
   unsigned char *e1, *e2, *s, *beg, *end, *send;

   int cset = (*(unsigned char *) _clip_fetch_item(ClipMachineMemory, HASH_csetatmupa)) == 't';

   char *atlike = _clip_fetch_item(ClipMachineMemory, HASH_setatlike);

   int atlike0, atlike1;

   int sovp = 0;

   atlike0 = atlike[0];
   atlike1 = atlike[1];
   end = str + l2 - l1 + 1;
   beg = str + l2;
   if (l1 == 0)
      return beg;
   send = sstr + l1;
   if (ignore < 0)
      ignore = 0;
   for (e1 = str + ignore; e1 < end; e1++)
   {
      if (*e1 != *sstr)
	 continue;
      for (s = sstr, e2 = e1; s < send && (*s == *e2 || (atlike0 && *s == atlike1)); s++, e2++);
      if (s != send)
	 continue;
      if (flag)
	 beg = e1;
      if (!cset)
	 e1 = e2 - 1;
      if (!flag)
      {
	 if (count != 0 || (cset && count == 0))
	    beg = e2;
	 else
	    beg = e1;
      }
      sovp++;
      if (count != 0 && sovp >= count)
	 break;
   }
   if ((count != 0 && sovp < count) || sovp == 0)
      beg = str + l2;
   return beg;
}
