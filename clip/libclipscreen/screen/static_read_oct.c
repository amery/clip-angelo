static char *
read_oct(char *p)
{
   char *r, *s;

   char c;

   r = s = strdup(p);
   while ((c = *p))
   {
      if (c == '\\')
      {
	 p++;
	 c = *p;
	 if (!c)
	    break;
	 switch (c)
	 {
	 case '0':
	 case '1':
	 case '2':
	 case '3':
	 case '4':
	 case '5':
	 case '6':
	 case '7':
	    if (p[1] && p[2] && isdigit(p[1]) && isdigit(p[2]))
	    {
	       c = strtol(p, 0, 8);
	       p += 2;
	    }
	    break;
	 case 't':
	    c = '\t';
	    break;
	 case 'n':
	    c = '\n';
	    break;
	 case 'r':
	    c = '\r';
	    break;
	 case 'b':
	    c = '\b';
	    break;
	 case 'v':
	    c = '\v';
	    break;
	 case 'e':
	    c = '\033';
	    break;
	 case 'a':
	    c = '\007';
	    break;
	 }
      }

      *s = c;
      s++;

      p++;
   }
   *s = 0;
   return r;
}
