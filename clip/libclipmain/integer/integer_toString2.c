/**********************************************/
char *
integer_toString2(integer * data)
{
   char *ret = calloc(1, 1), buf[25];

   int slen = 0, blen, i;

	for (i = 0; i < data->len_of_integer; i++)
   {
		snprintf(buf, 25, "%u", data->vec_of_integer[i]);
      blen = strlen(buf);
      ret = realloc(ret, slen + blen + 1);
      memcpy(ret + slen, buf, blen);
      slen += blen;
      ret[slen] = ':';
      slen++;
   }
   ret[slen - 1] = 0;
   return ret;
}
