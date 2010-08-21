int
_clip_str2attr(char *str, int len)
{
   int bg = 0, fg = 0, *ap = &fg, done = 0;

   char *e;

   if (len == 2 && !strcmp(str, "-1"))
      return -1;

   for (e = str + len; !done && *str && str < e; ++str)
      switch (*str)
      {
      case ',':
	 done = 1;
	 break;
      case '/':
	 ap = &bg;
	 break;
      case 'U':
      case 'N':
      case 'X':
      case 'u':
      case 'n':
      case 'x':
      case ' ':
	 *ap |= COLOR_BLACK;
	 break;
      case 'B':
      case 'b':
	 *ap |= COLOR_BLUE;
	 break;
      case 'W':
      case 'w':
	 *ap |= COLOR_WHITE;
	 break;
      case 'G':
      case 'g':
	 *ap |= COLOR_GREEN;
	 break;
      case 'R':
      case 'r':
	 *ap |= COLOR_RED;
	 break;
      case '+':
	 fg |= COLOR_HI;
	 break;
      case 'I':
      case 'i':
	 *ap = ~*ap;
	 break;
      case '*':
	 bg |= COLOR_HI;
	 break;
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
	 *ap = strtol(str, &str, 10);
	 --str;
	 break;
      }
   return (((bg << 4) & 0xf0) | (fg & 0xf)) /*& 0x7f */ ;
}
