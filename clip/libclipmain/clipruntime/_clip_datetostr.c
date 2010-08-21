CLIP_DLLEXPORT void
_clip_datetostr(long date, char *buf)
{
   int dd, mm, yy, ww;

   char *sss = "0123456789";

   buf[8] = 0;
   if (date == 0)
   {
      memset(buf, ' ', 8);
      return;
   }
   _clip_cdate(date, &dd, &mm, &yy, &ww);
   buf[7] = sss[dd % 10];
   buf[6] = sss[dd / 10];
   buf[5] = sss[mm % 10];
   buf[4] = sss[mm / 10];

   buf[3] = sss[yy % 10];
   yy /= 10;
   buf[2] = sss[yy % 10];
   yy /= 10;
   buf[1] = sss[yy % 10];
   yy /= 10;
   buf[0] = sss[yy];
}
