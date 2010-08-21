char *
_clip_ttoc(ClipMachine * ClipMachineMemory, long julian, long time, int *retlen, char *date_format, int hours, int seconds)
{
   char *r = _clip_date_to_str(julian, date_format);

   int len = strlen(r);

   int h, m, s;

   long t;

   int pm = 0;

   r = realloc(r, len + 13);
   r[len] = ' ';
   h = time / (60 * 60 * 1000);
   t = time % (60 * 60 * 1000);
   m = t / (60 * 1000);
   t = t % (60 * 1000);
   s = t / 1000;
   if (!hours)
   {
      if (h >= 12)
	 pm = 1;
      if (h > 12)
	 h -= 12;
   }

   if (seconds)
   {
      if (h == 0 && m == 0 && s == 0)
	 snprintf(r + len + 1, 11, "  :  :  AM");
      else if (hours)
	 snprintf(r + len + 1, 9, "%02d:%02d:%02d", h, m, s);
      else if (pm)
	 snprintf(r + len + 1, 11, "%02d:%02d:%02dPM", h, m, s);
      else
	 snprintf(r + len + 1, 11, "%02d:%02d:%02dPM", h, m, s);
   }
   else
   {
      if (h == 0 && m == 0)
	 snprintf(r + len + 1, 8, "  :  AM");
      else if (hours)
	 snprintf(r + len + 1, 6, "%02d:%02d", h, m);
      else if (pm)
	 snprintf(r + len + 1, 8, "%02d:%02dPM", h, m);
      else
	 snprintf(r + len + 1, 8, "%02d:%02dAM", h, m);
   }
   if (retlen)
      *retlen = len + 6 + (seconds ? 3 : 0) + (hours ? 0 : 2);
   return r;
}
