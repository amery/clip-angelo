/****************************************************/
char *
integer_toString(integer * data, int base)
{
   integer *z = integer_copy(data);

   char *ret, *buf, *sret;

   int lenstr = 0, blen = 0;

   INTEGER_HALF_TYPE i = INTEGER_MAXNUM / base;

   INTEGER_HALF_TYPE rem, rem2, db = base, power = 1;

	lenstr = (data->len_of_integer * INTEGER_HALF) * 2 + 1;
   buf = calloc(25, 1);
   buf[0] = ' ';
   blen = 1;
   ret = malloc(lenstr + 1);
   for (i = 0; i < lenstr; i++)
      ret[i] = ' ';
   ret[lenstr] = 0;

   while (db < i)
   {
      db *= base;
      power++;
   }
   if (!integer_empty(data))
   {
      lenstr--;
      while (!integer_empty(z))
      {
	 integer_sunscale(z, db, &rem2);
	 for (i = 0; i < power; i++)
	 {
	    rem = (rem2 % base);
	    rem += (rem < 10 ? INTEGER_SYM_NULL : (INTEGER_SYM_A - 10));
	    rem2 /= base;
	    ret[lenstr] = (char) rem;
	    lenstr--;
	 }
      }
   }
   lenstr = strlen(ret);
   for (i = 0; i < lenstr && (ret[i] == ' ' || ret[i] == '0'); i++);
	if (data->sign_of_integer)
      buf[0] = '-';
   switch (base)
   {
   case 2:
      buf[blen] = '0';
      buf[blen + 1] = 'b';
      blen += 2;
      break;
   case 8:
      buf[blen] = '0';
      buf[blen + 1] = 'o';
      blen += 2;
      break;
   case 16:
      buf[blen] = '0';
      buf[blen + 1] = 'x';
      blen += 2;
      break;
   }
   if ((lenstr - i) == 0)
   {
      buf[blen] = '0';
      blen++;
   }
      sret = malloc(lenstr + blen + data->sign_of_integer + 1);
		sprintf(sret, "%s%s", buf + 1 - data->sign_of_integer, ret + i);
		sret[lenstr + blen + data->sign_of_integer] = 0;
   free(buf);
   free(ret);
   integer_destroy(z);
   return sret;
}
