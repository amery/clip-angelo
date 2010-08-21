int
load_charset_tables(char *bname, unsigned char *cmptbl, unsigned char *uptbl, unsigned char *lowtbl, unsigned char *alphatbl, unsigned char *pgtbl)
{
   char buf[256];

   FILE *file;

   int i;

   int r = 0;

   int val;

   char *name, *s;

   name = strdup(bname);
   for (s = name; *s; s++)
      *s = tolower(*s);

   for (i = 0; i < 256; i++)
   {
      if (cmptbl)
	 cmptbl[i] = i;
      if (uptbl)
	 uptbl[i] = toupper(i);
      if (lowtbl)
	 lowtbl[i] = tolower(i);
      if (alphatbl)
	 alphatbl[i] = (unsigned char) isalpha(i);
      if (pgtbl)
	 pgtbl[i] = 0;
   }

   snprintf(buf, sizeof(buf), "%s/charsets/%s.tbl", CLIPROOT, name);
   file = fopen(buf, "r");

   if (!file)
   {
      r = -1;
      goto err;
   }

   for (i = 0; i < 256; i++)
   {
      if (get_str(file, buf, sizeof(buf)))
	 goto err;
      if (sscanf(buf, "%d", &val) != 1)
	 goto err;
      if (cmptbl)
	 cmptbl[i] = val;
   }

   for (i = 0; i < 256; i++)
   {
      if (get_str(file, buf, sizeof(buf)))
	 goto err;
      if (sscanf(buf, "%d", &val) != 1)
	 goto err;
      if (uptbl)
	 uptbl[i] = val;
   }

   for (i = 0; i < 256; i++)
   {
      if (get_str(file, buf, sizeof(buf)))
	 goto err;
      if (sscanf(buf, "%d", &val) != 1)
	 goto err;
      if (lowtbl)
	 lowtbl[i] = val;
   }

   for (i = 0; i < 256; i++)
   {
      if (get_str(file, buf, sizeof(buf)))
	 goto err;
      if (sscanf(buf, "%d", &val) != 1)
	 goto err;
      if (alphatbl)
	 alphatbl[i] = (unsigned char) val;
   }

   for (i = 0; i < 256; i++)
   {
      if (get_str(file, buf, sizeof(buf)))
	 goto err;
      if (sscanf(buf, "%d", &val) != 1)
	 goto err;
      if (pgtbl)
	 pgtbl[i] = val;
   }

   goto ret;
 err:
   r = -1;
 ret:
   if (pgtbl)
   {
      if (!pgtbl[PGCH_HLINE])
	 pgtbl[PGCH_HLINE] = PG_HLINE;
      if (!pgtbl[PGCH_LLCORNER])
	 pgtbl[PGCH_LLCORNER] = PG_LLCORNER;
      if (!pgtbl[PGCH_LRCORNER])
	 pgtbl[PGCH_LRCORNER] = PG_LRCORNER;
      if (!pgtbl[PGCH_TTEE])
	 pgtbl[PGCH_TTEE] = PG_TTEE;
      if (!pgtbl[PGCH_RTEE])
	 pgtbl[PGCH_RTEE] = PG_RTEE;
      if (!pgtbl[PGCH_LTEE])
	 pgtbl[PGCH_LTEE] = PG_LTEE;
      if (!pgtbl[PGCH_BTEE])
	 pgtbl[PGCH_BTEE] = PG_BTEE;
      if (!pgtbl[PGCH_ULCORNER])
	 pgtbl[PGCH_ULCORNER] = PG_ULCORNER;
      if (!pgtbl[PGCH_URCORNER])
	 pgtbl[PGCH_URCORNER] = PG_URCORNER;
      if (!pgtbl[PGCH_VLINE])
	 pgtbl[PGCH_VLINE] = PG_VLINE;

      if (!pgtbl[PGCH_HLINE2])
	 pgtbl[PGCH_HLINE2] = PG_HLINE2;
      if (!pgtbl[PGCH_LLCORNER2])
	 pgtbl[PGCH_LLCORNER2] = PG_LLCORNER2;
      if (!pgtbl[PGCH_LRCORNER2])
	 pgtbl[PGCH_LRCORNER2] = PG_LRCORNER2;
      if (!pgtbl[PGCH_TTEE2])
	 pgtbl[PGCH_TTEE2] = PG_TTEE2;
      if (!pgtbl[PGCH_RTEE2])
	 pgtbl[PGCH_RTEE2] = PG_RTEE2;
      if (!pgtbl[PGCH_LTEE2])
	 pgtbl[PGCH_LTEE2] = PG_LTEE2;
      if (!pgtbl[PGCH_BTEE2])
	 pgtbl[PGCH_BTEE2] = PG_BTEE2;
      if (!pgtbl[PGCH_ULCORNER2])
	 pgtbl[PGCH_ULCORNER2] = PG_ULCORNER2;
      if (!pgtbl[PGCH_URCORNER2])
	 pgtbl[PGCH_URCORNER2] = PG_URCORNER2;
      if (!pgtbl[PGCH_VLINE2])
	 pgtbl[PGCH_VLINE2] = PG_VLINE2;

      if (!pgtbl[PGCH_PLUS])
	 pgtbl[PGCH_PLUS] = PG_PLUS;
      if (!pgtbl[PGCH_PLUS2])
	 pgtbl[PGCH_PLUS2] = PG_PLUS2;
      if (!pgtbl[PGCH_BLOCK])
	 pgtbl[PGCH_BLOCK] = PG_BLOCK;

      if (!pgtbl[PGCH_LARROW])
	 pgtbl[PGCH_LARROW] = PG_LARROW;
      if (!pgtbl[PGCH_RARROW])
	 pgtbl[PGCH_RARROW] = PG_RARROW;
   }
   if (file)
      fclose(file);
   free(name);
   return r;
}
