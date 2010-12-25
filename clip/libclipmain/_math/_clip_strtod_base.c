double
_clip_strtod_base(char *str, int base)
{
   int i, val = 0;

   double ret = 0, b;

   if (base <= 0 || base > 36)
      base = 10;
   for (i = 0; str[i] != 0; i++);
   for (i--, b = 1; i >= 0; i--, b *= base)
   {
      switch (str[i])
      {
      case '0'...'9':
	 val = str[i] - '0';
	 break;
      case 'a'...'z':
	 val = str[i] - 'a' + 10;
	 break;
      case 'A'...'Z':
	 val = str[i] - 'A' + 10;
	 break;
      default:
	 val = 0;
      }
      if (val < base)
	 ret += val * b;
      else
	 break;
   }
   return ret;
}
