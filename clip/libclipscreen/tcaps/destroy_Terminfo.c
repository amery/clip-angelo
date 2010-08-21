void
destroy_Terminfo(Terminfo * info)
{
   if (info->buf)
   {
      free(info->buf);
      info->buf = 0;
   }
   info->bufsize = info->bufpos = 0;
}
