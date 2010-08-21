int
po_write_header(FILE * out)
{
   time_t now;

   struct tm local_time;

   char tz_sign;

   long tz_min;

   fprintf(out, "# SOME DESCRIPTIVE TITLE.\n\
# FIRST AUTHOR <EMAIL@ADDRESS>, YEAR.\n#\n#, fuzzy\n");

   time(&now);
   local_time = *localtime(&now);
   tz_sign = '+';
   tz_min = difftm(&local_time, gmtime(&now)) / 60;
   if (tz_min < 0)
   {
      tz_min = -tz_min;
      tz_sign = '-';
   }

#if 1
   fprintf(out, "msgid \"\"\nmsgstr \"\"\n\"\
Project-Id-Version: PACKAGE VERSION\\n\"\n\
\"PO-Revision-Date: YEAR-MO-DA HO:MI+ZONE\\n\"\n\
\"Last-Translator: FULL NAME <EMAIL@ADDRESS>\\n\"\n\
\"Language-Team: LANGUAGE <LL@li.org>\\n\"\n\
\"MIME-Version: 1.0\\n\"\n\
\"Content-Type: text/plain; charset=iso-8859-1\\n\"\n\
\"Content-Transfer-Encoding: 8-bit\"\n");
#else
   fprintf(out, "msgid \"\"\nmsgstr \"\"\n\"\
Project-Id-Version: PACKAGE VERSION\\n\"\n\
\"POT-Creation-Date: %d-%02d-%02d %02d:%02d%c%02d%02d\\n\"\n\
\"PO-Revision-Date: YEAR-MO-DA HO:MI+ZONE\\n\"\n\
\"Last-Translator: FULL NAME <EMAIL@ADDRESS>\\n\"\n\
\"Language-Team: LANGUAGE <LL@li.org>\\n\"\n\
\"MIME-Version: 1.0\\n\"\n\
\"Content-Type: text/plain; charset=iso-8859-1\\n\"\n\
\"Content-Transfer-Encoding: 8-bit\"\n", local_time.tm_year + TM_YEAR_ORIGIN, local_time.tm_mon + 1, local_time.tm_mday, local_time.tm_hour, local_time.tm_min, tz_sign, (int) (tz_min / 60), (int) (tz_min % 60));
#endif
   return 0;
}
