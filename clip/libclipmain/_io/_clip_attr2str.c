int
_clip_attr2str(int attr, char *buf, int buflen, int num_format)
{
   int l = 0;

   if (num_format)
   {
      snprintf(buf, buflen, "%d/%d", attr & 0xf, (attr >> 4) & 0xf);
      l = strlen(buf);
      return l;
   }

   if (attr & COLOR_HI)
   {
      buf[l] = '+';
      ++l;
   }

   if (l >= buflen)
      return l;

   if ((attr >> 4) & COLOR_HI)
   {
      buf[l] = '*';
      ++l;
   }

   if (l >= buflen)
      return l;

   switch (attr & 0x7)
   {
   case COLOR_BLACK:
      buf[l] = 'N';
      break;
   case COLOR_WHITE:
      buf[l] = 'W';
      break;
   case COLOR_BLUE:
   case COLOR_CYAN:
      buf[l] = 'B';
      break;
   case COLOR_GREEN:
   case COLOR_YELLOW:
      buf[l] = 'G';
      break;
   case COLOR_RED:
   case COLOR_MAGENTA:
      buf[l] = 'R';
      break;
   }

   ++l;
   if (l >= buflen)
      return l;

   switch (attr & 0x7)
   {
   case COLOR_CYAN:
      buf[l] = 'G';
      ++l;
      break;
   case COLOR_MAGENTA:
      buf[l] = 'B';
      ++l;
      break;
   case COLOR_YELLOW:
      buf[l] = 'R';
      ++l;
      break;
   }

   if (l >= buflen)
      return l;

   buf[l] = '/';
   ++l;

   if (l >= buflen)
      return l;

   switch ((attr >> 4) & 0x7)
   {
   case COLOR_BLACK:
      buf[l] = 'N';
      break;
   case COLOR_WHITE:
      buf[l] = 'W';
      break;
   case COLOR_BLUE:
   case COLOR_CYAN:
      buf[l] = 'B';
      break;
   case COLOR_GREEN:
   case COLOR_YELLOW:
      buf[l] = 'G';
      break;
   case COLOR_RED:
   case COLOR_MAGENTA:
      buf[l] = 'R';
      break;
   }

   ++l;
   if (l >= buflen)
      return l;

   switch ((attr >> 4) & 0x7)
   {
   case COLOR_CYAN:
      buf[l] = 'G';
      ++l;
      break;
   case COLOR_MAGENTA:
      buf[l] = 'B';
      ++l;
      break;
   case COLOR_YELLOW:
      buf[l] = 'R';
      ++l;
      break;
   }

   return l;
}
