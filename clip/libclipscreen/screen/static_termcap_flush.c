static void
termcap_flush(ScreenData * dp)
{
   if (dp->outptr > dp->outbuf)
      write(dp->fd, dp->outbuf, dp->outptr - dp->outbuf);
   dp->outptr = dp->outbuf;
}
