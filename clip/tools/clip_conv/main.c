int
main(int argc, char **argv)
{
   char *p1, *p2;

   cons_CharsetEntry *cs1 = 0, *cs2 = 0;

   int len1 = 0, len2 = 0;

   unsigned char tbl[256];

   int ch;

   char *s;

   s = getenv("CLIPROOT");
   if (s && *s)
      CLIPROOT = s;

   if (argc < 3)
   {
      fprintf(stderr, "usage: %s source_charset target_charset\n", argv[0]);
      return 1;
   }

   p1 = argv[1];
   p2 = argv[2];

   if (load_charset_name(p1, &cs1, &len1))
   {
      fprintf(stderr, "cannot load charset file '%s': %s", p1, strerror(errno));
      return 2;
   }

   if (!strcasecmp(p2, "utf-8"))
   {
      unsigned short *utbl = (unsigned short *) malloc(sizeof(unsigned short) * 256);

      make_utbl(utbl, cs1, len1);
      while ((ch = getchar()) != EOF)
	 put_utf8(utbl[ch & 0xff]);

      return 0;
   }

   if (!strcasecmp(p2, "ucs-2"))
   {
      unsigned short *utbl = (unsigned short *) malloc(sizeof(unsigned short) * 256);

      make_utbl(utbl, cs1, len1);
      while ((ch = getchar()) != EOF)
	 put_ucs2(utbl[ch & 0xff]);

      return 0;
   }

   if (load_charset_name(p2, &cs2, &len2))
   {
      fprintf(stderr, "cannot load charset file '%s': %s", p2, strerror(errno));
      return 3;
   }

   make_translation(cs1, len1, cs2, len2, tbl);

   while ((ch = getchar()) != EOF)
   {
      putchar(tbl[ch & 0xff]);
   }

   return 0;
}
