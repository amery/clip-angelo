CLIP_DLLEXPORT char *
_clip_date_to_str(long date, char *format)
{
   int i, fl;

   int dl = 1, ml = 1, yl = 1;

   int dd, mm, yy, ww;

   char ch, ch2;

   char ds[DD_BUF_LEN + 1], ms[DD_BUF_LEN + 1], ys[DD_BUF_LEN + 1];

   char *ret;

   if (format == NULL)
      format = CLIP_DATEFORMAT_DEFAULT;
   fl = strlen(format);
   ret = malloc(fl + 1);
   memset(ret, 32, fl - 1);
   ret[fl] = 0;
   memset(ds, 32, DD_BUF_LEN);
   ds[DD_BUF_LEN] = 0;
   memset(ms, 32, DD_BUF_LEN);
   ms[DD_BUF_LEN] = 0;
   memset(ys, 32, DD_BUF_LEN);
   ys[DD_BUF_LEN] = 0;

   _clip_cdate(date, &dd, &mm, &yy, &ww);
   if (date != 0)
   {
      snprintf(ds, DD_BUF_LEN + 1, "%07d", dd);
      snprintf(ms, DD_BUF_LEN + 1, "%07d", mm);
      snprintf(ys, DD_BUF_LEN + 1, "%07d", yy);
   }
   for (i = fl; i >= 0; i--)
   {
      ch = format[i];
      switch (ch)
      {
      case 'd':
      case 'D':
	 ch2 = ds[DD_BUF_LEN - dl];
	 dl++;
	 break;
      case 'm':
      case 'M':
	 ch2 = ms[DD_BUF_LEN - ml];
	 ml++;
	 break;
      case 'y':
      case 'Y':
	 ch2 = ys[DD_BUF_LEN - yl];
	 yl++;
	 break;
      default:
	 ch2 = ch;
	 break;
      }
      ret[i] = ch2;
      /*printf("%c%c%d%d%d.",ch,ch2,dl,ml,yl); */
   }
   return ret;
}
