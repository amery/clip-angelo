long
_clip_str_to_date(char *str, char *format, int epoch)
{
   int i, j, len;

   int d = 0, m = 0, y = 0;
   int b[3] = { 0, 0, 0 };
   int mms[12] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
   char ch = 0;

   if (epoch == 0)
      epoch = CLIP_EPOCH_DEFAULT;
   if (format == NULL)
      format = CLIP_DATEFORMAT_DEFAULT;

   len = strlen(format);
   for (i = 0, j = 0; i < len; i++)
   {
      if (ch == format[i])
	 continue;
      switch (format[i])
      {
      case 'y':
      case 'Y':
	 y = j;
	 break;
      case 'm':
      case 'M':
	 m = j;
	 break;
      case 'd':
      case 'D':
	 d = j;
	 break;
      case ' ':
	 break;
      default:
	 j++;
	 break;
      }
      if (j > 2)
	 break;
      ch = format[i];
   }
   if (j < 2)
      return 0;

   len = strlen(str);

   for (i = 0; i < len && !isdigit(str[i]); i++);

   for (j = 0, b[0] = i; i < len && j < 2; i++)
   {
      ch = str[i];
      if ((ch < '0' || ch > '9') && ch != ' ')
      {
	 for (; i < len && !isdigit(str[i]); i++);
	 j++;
	 b[j] = i;
      }
   }
   if (j < 2)
      return 0;
   m = atoi(str + b[m]);
   d = atoi(str + b[d]);
   y = atoi(str + b[y]);
   if (m == 0 && d == 0 && y == 0)
      return 0;
   if (m > 12 || d > 31)
      return 0;
   /*
      printf("\nctod m=%d,d=%d,mms[m]=%d\n",m,d,mms[m]);
    */
   if (m > 0 && (d > mms[m - 1]))
      return 0;

   i = epoch % 100;
   j = epoch / 100;
   if (y < 100)
   {
      if (y < i)
	 y += (j + 1) * 100;
      else
	 y += j * 100;
   }
   return _clip_jdate(d, m, y);
}
