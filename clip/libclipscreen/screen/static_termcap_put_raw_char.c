static int
termcap_put_raw_char(int c, void *par)
{
   ScreenData *dp = (ScreenData *) par;

   if (dp->outptr >= dp->outbuf + sizeof(dp->outbuf))
      termcap_flush(dp);
   *dp->outptr++ = c;
   return 0;
}
