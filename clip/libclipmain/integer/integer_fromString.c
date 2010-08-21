/**********************************************/
integer *
integer_fromString(const char *s)
{
   integer *ret = integer_long_init(0L);

   integer *a = integer_init();

   int base = 10, k, ch, i, sgn = 0, lenstr = strlen(s);

   INTEGER_HALF_TYPE db, j, power;

   long sum;

   for (i = 0; i < lenstr && (s[i] == ' ' || s[i] == '\t'); i++);

   ch = s[i];
   if (ch == '-' /*INTEGER_SYM_MINUS */ )
   {
      sgn = 1;
      i++;
   }
   if (ch == INTEGER_SYM_PLUS)
      i++;
   while (s[i] == '0')
      i++;

   switch (s[i])
   {
   case 'b':
   case 'B':
      base = 2;
      break;
   case 'o':
   case 'O':
      base = 8;
      break;
   case 'd':
   case 'D':
      base = 10;
      break;
   case 'x':
   case 'X':
      base = 16;
      break;
   default:
      i--;
      break;
   }
   i++;
   db = INTEGER_MAXNUM / base;
   j = base;
   for (power = 1; j < db; power++)
      j *= base;
   power *= 2;

   while (1)
   {
      sum = 0;
      for (k = 0; k < power; i++, k++)
      {
	 ch = s[i];
	 if (ch >= '0' && ch <= '9')
	    j = ch - '0';
	 else if (ch >= 'a' && ch <= 'z')
	    j = ch - 'a' + 10;
	 else if (ch >= 'A' && ch <= 'Z')
	    j = ch - 'A' + 10;
	 else
	    break;
	 if (j >= base)
	    break;
	 sum *= base;
	 sum += j;
      }
      if (k == 0)
	 break;
      integer_destroy(a);
      a = integer_long_init((long) pow(base, k));
      integer_mula(ret, a);
      integer_destroy(a);
      a = integer_long_init(sum);
      integer_sadd(ret, a);
   }
   integer_destroy(a);
	ret->sign_of_integer = sgn;
   return ret;
}
