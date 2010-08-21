unsigned char *
_clip_attoken(unsigned char *str, int l1, unsigned char *str2, int l2, int count)
{
   unsigned char *e1, *e2, *s, *dstr, *end, *send, *ret, *ret1;

   int sovp = 1;

   if (str2 == NULL)
   {
      dstr = (unsigned char *) CLIP_TOKEN_CHARACTERS;
      l2 = CLIP_TOKEN_CHARS_NUM;
   }
   else
      dstr = str2;
   end = str + l1;
   send = dstr + l2;
   ret = str;
   ret1 = NULL;
   for (e1 = str; e1 <= end - l2; e1++)
   {
      for (e2 = e1, s = dstr; s <= send && e2 <= end && *s != *e2; s++, e2++);
      if (*e2 != *s)
	 continue;
      if (sovp == count)
	 break;
      if (e1 != str)
	 sovp++;
      ret = e1 + l2;
      if (e2 >= end - l2)
	 break;
      if (ret < end)
	 ret1 = ret;
      /*
         printf("\ncount=%d,l1=%d,e2=%d,ret1=%d,ret=%d",count,l1,e2-end,ret1-str,ret-str);
       */

   }
   /*
      printf("\ncount=%d,ret1=%d,ret=%d\n",count,ret1-str,ret-str);
    */
   if (count == 0xFFFF)
      ret = ret1;
   else if (sovp != count)
      ret = NULL;
   return ret;
}
