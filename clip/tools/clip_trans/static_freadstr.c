static int
freadstr(FILE * f, char *buf, int maxlen)
{
   int ret = 0;

   int ch;

   while (!feof(f) && ret < maxlen)
   {
      ch = fgetc(f);
      if (ch == '\n')
	 break;
      buf[ret] = ch;
      ret++;
   }
   if (ret > 0 && buf[ret - 1] == '\r')
      ret--;
   if (ret >= 0)
      buf[ret] = 0;
   return ret;
}
