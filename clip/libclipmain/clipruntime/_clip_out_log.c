void
_clip_out_log(char *buf, int len)
{
   _clip_log_init(NULL);
   fwrite(buf, len, 1, logg);
}
