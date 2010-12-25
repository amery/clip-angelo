static int
get_str(FILE * file, char *buf, int bufsize)
{
   char *s;

   int l, i;

 again:
   s = fgets(buf, bufsize, file);
   if (!s)
      return -1;
   line_no++;

   l = strlen(s);
   if (l <= 1 || s[0] == '\n')
      goto again;

   l--;
   buf[l] = 0;

   for (i = 0; i < l; i++)
   {
      switch (buf[i])
      {
      case ' ':
      case '\t':
	 continue;
      case '#':
	 goto again;
      }
      break;
   }

   if (i)
      memmove(buf, buf + i, l - i + 1);

   return 0;
}
