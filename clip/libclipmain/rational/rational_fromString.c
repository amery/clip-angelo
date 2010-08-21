/*********************************************/
rational *
rational_fromString(char *s)
{
   int i, lenstr;

   int base = 10;

   integer *tmp = NULL;

   rational *ret = (rational *) calloc(sizeof(rational), 1);

   ret->den = integer_long_init(1L);
   lenstr = strlen(s);
   for (; lenstr > 0 && s[lenstr] == ' '; lenstr--);
   for (i = 0; i < lenstr && s[i] != '/'; i++);
   if (i < lenstr)
   {
      ret->num = integer_fromString(s);
      integer_destroy(ret->den);
      ret->den = integer_fromString(s + i + 1);
      rational_normalize(ret);
      return ret;
   }
   for (i = 0; i < lenstr && (s[i] != ' ' || s[i] == 0); i++);
   switch (s[i])
   {				// ���� �� � ���������� �������, � � ������� 0x1234...
   case 'b':
      base = 2;
      break;
   case 'o':
      base = 8;
      break;
   case 'd':
      base = 10;
      break;
   case 'x':
      base = 16;
      break;
   default:
      base = 10;
   }
   for (i = 0; i < lenstr && s[i] != '.'; i++);
   if (i < lenstr)
   {
      integer_destroy(ret->den);
      ret->den = integer_long_init(base);
      integer_powa(ret->den, lenstr - i - 1);
      ret->num = integer_fromString(s);
      integer_mula(ret->num, ret->den);

      tmp = integer_fromString(s + i + 1);
      integer_adda(ret->num, tmp);
      integer_destroy(tmp);
      rational_normalize(ret);
      return ret;
   }
   ret->num = integer_fromString(s);
   return ret;
}
