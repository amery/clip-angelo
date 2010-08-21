static WinBuf *
new_WinBuf(int hi, int wi)
{
   WinBuf *wb;

   int size, y, x;

   char *mem, *chars, *colors, *attrs;

   size = sizeof(WinBuf) + hi * wi * 3 + sizeof(char *) * hi * 3;
   mem = (char *) calloc(size, 1);
   wb = (WinBuf *) mem;

   wb->hi = hi;
   wb->wi = wi;

   chars = (char *) (mem + sizeof(WinBuf));
   colors = chars + wi * hi;
   attrs = colors + wi * hi;
   wb->chars = (char **) (attrs + wi * hi);
   wb->colors = (char **) (attrs + wi * hi + sizeof(char *) * hi);
   wb->attrs = (char **) (attrs + wi * hi + sizeof(char *) * hi * 2);

   for (y = 0; y < hi; y++)
   {
      wb->chars[y] = chars + wi * y;
      wb->colors[y] = colors + wi * y;
      wb->attrs[y] = attrs + wi * y;

      for (x = 0; x < wi; x++)
      {
	 wb->chars[y][x] = ' ';
	 wb->colors[y][x] = 7;
	 wb->attrs[y][x] = 0;
      }
   }

   return wb;
}
