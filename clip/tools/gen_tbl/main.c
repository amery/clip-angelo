int
main(int argc, char **argv)
{
   FILE *file;

   cons_CharsetEntry *cp = 0;

   int len = 0, i, mb = 0;

   char buf[4096];

   UniRecord *rp;

   /* unicode-order */
   Coll uni;

   /* charset-order */
   Coll cs;

   Coll out;

   Coll cmp;

   char *fname;

   if (argc < 2)
   {
      fprintf(stderr, "usage: %s charset_file [cs_file1...] < unicode_data\n", argv[0]);
      exit(1);
   }

   for (i = 1; i < argc; i++)
   {
      fname = argv[i];
      file = fopen(fname, "r");
      if (!file)
      {
	 fprintf(stderr, "cannot open %s: %s\n", fname, strerror(errno));
	 exit(2);
      }

      fprintf(stderr, "load charset '%s'\n", fname);
      if (load_charset(file, &cp, &len))
      {
	 fprintf(stderr, "cannot read %s: %s\n", fname, strerror(errno));
	 exit(3);
      }
      fprintf(stderr, "loaded %d entries\n", len);
      fclose(file);
#ifdef DBG
      fprintf(stderr, "readed %d charset entries\n", len);
#endif
   }

   init_Coll(&uni, 0, cmp_UniRecord);
   init_Coll(&cs, 0, cmp_CSRecord);
   init_Coll(&out, 0, 0);
   init_Coll(&cmp, 0, cmp_UniRecord);

   while (fgets(buf, sizeof(buf), stdin))
   {
      int l;

      char *s;

      rp = (UniRecord *) calloc(1, sizeof(*rp));
      l = strlen(buf);
      if (l > 0 && buf[--l] == '\n')
	 buf[l] = 0;
      if (!l)
	 break;

      s = strdup(buf);
		rp->mem_of_UniRecord = s;

		rp->hex_of_UniRecord = s;
		rp->no_of_UniRecord = strtoul(s, 0, 16);
      l = strcspn(s, ";");
      s += l;
      *s++ = 0;

		rp->name_of_UniRecord = s;
      l = strcspn(s, ";");
      s += l;
      *s++ = 0;

		rp->cat_of_UniRecord = s;
      l = strcspn(s, ";");
      s += l;
      *s++ = 0;

		rp->comb_of_UniRecord = s;
      l = strcspn(s, ";");
      s += l;
      *s++ = 0;

		rp->bidir_of_UniRecord = s;
      l = strcspn(s, ";");
      s += l;
      *s++ = 0;

		rp->decomp_of_UniRecord = s;
      l = strcspn(s, ";");
      s += l;
      *s++ = 0;

		rp->dec_of_UniRecord = s;
      l = strcspn(s, ";");
      s += l;
      *s++ = 0;

		rp->dig_of_UniRecord = s;
      l = strcspn(s, ";");
      s += l;
      *s++ = 0;

		rp->num_of_UniRecord = s;
      l = strcspn(s, ";");
      s += l;
      *s++ = 0;

		rp->mirror_of_UniRecord = s;
      l = strcspn(s, ";");
      s += l;
      *s++ = 0;

		rp->name1_of_UniRecord = s;
      l = strcspn(s, ";");
      s += l;
      *s++ = 0;

		rp->comment_of_UniRecord = s;
      l = strcspn(s, ";");
      s += l;
      *s++ = 0;

		rp->upper_of_UniRecord = s;
      l = strcspn(s, ";");
      s += l;
      *s++ = 0;

		rp->lower_of_UniRecord = s;
      l = strcspn(s, ";");
      s += l;
      *s++ = 0;

		rp->title_of_UniRecord = s;
      l = strcspn(s, ";");
      s += l;
      *s++ = 0;

		rp->cp_of_UniRecord = in_map(cp, len, rp->no_of_UniRecord);
		if (rp->cp_of_UniRecord)
      {
			remove_char(&uni, rp->cp_of_UniRecord->ch_of_cons_CharsetEntry);
			remove_char(&cs, rp->cp_of_UniRecord->ch_of_cons_CharsetEntry);
	 insert_Coll(&uni, rp);
	 insert_Coll(&cs, rp);
      }
      else
      {
			free(rp->mem_of_UniRecord);
	 free(rp);
      }
   }
   fprintf(stderr, "appended %d uni %d chars\n", uni.count_of_Coll, cs.count_of_Coll);

#ifdef DBG
fprintf(stderr, "uni: %d\n", uni.count_of_Coll);
for (i = 0; i < uni.count_of_Coll; i++)
   {
      rp = (UniRecord *) uni.items[i];
		fprintf(stderr, "hex; 	'%s'\n", rp->hex_of_UniRecord);
		fprintf(stderr, "name;   '%s'\n", rp->name_of_UniRecord);
		fprintf(stderr, "cat;    '%s'\n", rp->cat_of_UniRecord);
		fprintf(stderr, "comb;   '%s'\n", rp->comb_of_UniRecord);
		fprintf(stderr, "bidir;  '%s'\n", rp->bidir_of_UniRecord);
		fprintf(stderr, "decomp; '%s'\n", rp->decomp_of_UniRecord);
		fprintf(stderr, "dec;    '%s'\n", rp->dec_of_UniRecord);
		fprintf(stderr, "dig;    '%s'\n", rp->dig_of_UniRecord);
		fprintf(stderr, "num;    '%s'\n", rp->num_of_UniRecord);
		fprintf(stderr, "mirror; '%s'\n", rp->mirror_of_UniRecord);
		fprintf(stderr, "name1;  '%s'\n", rp->name1_of_UniRecord);
		fprintf(stderr, "comment;'%s'\n", rp->comment_of_UniRecord);
		fprintf(stderr, "upper;  '%s'\n", rp->upper_of_UniRecord);
		fprintf(stderr, "lower;  '%s'\n", rp->lower_of_UniRecord);
		fprintf(stderr, "title;  '%s'\n", rp->title_of_UniRecord);
      fprintf(stderr, "\n");
   }
#endif
#ifdef DBG1
fprintf(stderr, "cs: %d\n", cs.count_of_Coll);
for (i = 0; i < cs.count_of_Coll; i++)
   {
      rp = (UniRecord *) cs.items[i];
		fprintf(stderr, "ch=%d\n", rp->cp_of_UniRecord->ch_of_cons_CharsetEntry);
		fprintf(stderr, "hex; 	'%s'\n", rp->hex_of_UniRecord);
		fprintf(stderr, "name;   '%s'\n", rp->name_of_UniRecord);
		fprintf(stderr, "cat;    '%s'\n", rp->cat_of_UniRecord);
		fprintf(stderr, "comb;   '%s'\n", rp->comb_of_UniRecord);
		fprintf(stderr, "bidir;  '%s'\n", rp->bidir_of_UniRecord);
		fprintf(stderr, "decomp; '%s'\n", rp->decomp_of_UniRecord);
		fprintf(stderr, "dec;    '%s'\n", rp->dec_of_UniRecord);
		fprintf(stderr, "dig;    '%s'\n", rp->dig_of_UniRecord);
		fprintf(stderr, "num;    '%s'\n", rp->num_of_UniRecord);
		fprintf(stderr, "mirror; '%s'\n", rp->mirror_of_UniRecord);
		fprintf(stderr, "name1;  '%s'\n", rp->name1_of_UniRecord);
		fprintf(stderr, "comment;'%s'\n", rp->comment_of_UniRecord);
		fprintf(stderr, "upper;  '%s'\n", rp->upper_of_UniRecord);
		fprintf(stderr, "lower;  '%s'\n", rp->lower_of_UniRecord);
		fprintf(stderr, "title;  '%s'\n", rp->title_of_UniRecord);
      fprintf(stderr, "\n");
   }
#endif

   for (i = 0; i < 256; i++)
   {
      UniRecord *rp = find_ch(&cs, i);

      if (!rp)
      {
	 cons_CharsetEntry *cp = (cons_CharsetEntry *) calloc(sizeof(cons_CharsetEntry *), 1);

	 rp = (UniRecord *) calloc(sizeof(UniRecord), 1);
	 rp->no_of_UniRecord = i;
	 rp->ch_of_UniRecord = i;
	 cp->ch_of_cons_CharsetEntry = i;
	 cp->unich_of_cons_CharsetEntry = i;
	 rp->cp_of_UniRecord = cp;
	 rp->hex_of_UniRecord = "";
	 rp->name_of_UniRecord = "";
	 rp->cat_of_UniRecord = "";
	 rp->comb_of_UniRecord = "";
	 rp->bidir_of_UniRecord = "";
	 rp->decomp_of_UniRecord = "";
	 rp->dec_of_UniRecord = "";
	 rp->dig_of_UniRecord = "";
	 rp->num_of_UniRecord = "";
	 rp->mirror_of_UniRecord = "";
	 rp->name1_of_UniRecord = "";
	 rp->comment_of_UniRecord = "";
	 rp->upper_of_UniRecord = "";
	 rp->lower_of_UniRecord = "";
	 rp->title_of_UniRecord = "";
      }
            if (rp->cp_of_UniRecord->ch_of_cons_CharsetEntry < 256)
      {
	 append_Coll(&out, rp);
	 insert_Coll(&cmp, rp);
      }
   }

#ifdef DBG3
fprintf(stderr, "cmp: %d\n", cmp.count_of_Coll);
for (i = 0; i < cmp.count_of_Coll; i++)
   {
		rp = (UniRecord *) cmp.items_of_Coll[i];
		fprintf(stderr, "ch=%d\n", rp->cp_of_UniRecord->ch_of_cons_CharsetEntry);
		fprintf(stderr, "hex; 	'%s'\n", rp->hex_of_UniRecord);
		fprintf(stderr, "name;   '%s'\n", rp->name_of_UniRecord);
		fprintf(stderr, "cat;    '%s'\n", rp->cat_of_UniRecord);
		fprintf(stderr, "comb;   '%s'\n", rp->comb_of_UniRecord);
		fprintf(stderr, "bidir;  '%s'\n", rp->bidir_of_UniRecord);
		fprintf(stderr, "decomp; '%s'\n", rp->decomp_of_UniRecord);
		fprintf(stderr, "dec;    '%s'\n", rp->dec_of_UniRecord);
		fprintf(stderr, "dig;    '%s'\n", rp->dig_of_UniRecord);
		fprintf(stderr, "num;    '%s'\n", rp->num_of_UniRecord);
		fprintf(stderr, "mirror; '%s'\n", rp->mirror_of_UniRecord);
		fprintf(stderr, "name1;  '%s'\n", rp->name1_of_UniRecord);
		fprintf(stderr, "comment;'%s'\n", rp->comment_of_UniRecord);
		fprintf(stderr, "upper;  '%s'\n", rp->upper_of_UniRecord);
		fprintf(stderr, "lower;  '%s'\n", rp->lower_of_UniRecord);
		fprintf(stderr, "title;  '%s'\n", rp->title_of_UniRecord);
      fprintf(stderr, "\n");
   }
#endif

   printf("# generated by gen_tbl from sources");
   for (i = 1; i < argc; i++)
      printf(" %s", argv[i]);
   printf("\n# cmptbl\n");
   for (i = 0; i < 256; i++)
   {
      int ch;

      ch = find_cmp(&cmp, i);
      printf("%d\n", ch);
   }
   printf("# uptbl\n");
	for (i = 0; i < out.count_of_Coll; i++)
   {
      UniRecord *rp1;

      int ch;

		rp = (UniRecord *) out.items_of_Coll[i];
      if (!rp)
      {
	 printf("%d\n", i);
	 continue;
      }
      if (!strcasecmp(rp->cat_of_UniRecord, "Ll") && rp->upper_of_UniRecord)
      {
			unsigned long unich = strtoul(rp->upper_of_UniRecord, 0, 16);

	 rp1 = find_uni(&uni, unich);
	 if (rp1)
		 ch = rp1->cp_of_UniRecord->ch_of_cons_CharsetEntry;
	 else
		 ch = rp->cp_of_UniRecord->ch_of_cons_CharsetEntry;
      }
      else
			ch = rp->cp_of_UniRecord->ch_of_cons_CharsetEntry;

      printf("%d\n", ch);
   }

   printf("# lowtbl\n");
	for (i = 0; i < out.count_of_Coll; i++)
   {
      UniRecord *rp1;

      int ch;

		rp = (UniRecord *) out.items_of_Coll[i];
      if (!rp)
      {
	 printf("%d\n", i);
	 continue;
      }
      if (!strcasecmp(rp->cat_of_UniRecord, "Lu") && rp->lower_of_UniRecord)
      {
			unsigned long unich = strtoul(rp->lower_of_UniRecord, 0, 16);

	 rp1 = find_uni(&uni, unich);
	 if (rp1)
		 ch = rp1->cp_of_UniRecord->ch_of_cons_CharsetEntry;
	 else
		 ch = rp->cp_of_UniRecord->ch_of_cons_CharsetEntry;
      }
      else
			ch = rp->cp_of_UniRecord->ch_of_cons_CharsetEntry;

      printf("%d\n", ch);
   }

   printf("# isalpha\n");
	for (i = 0; i < out.count_of_Coll; i++)
   {
      int r = 0;

      char *s;

		rp = (UniRecord *) out.items_of_Coll[i];
      if (!rp)
      {
	 printf("0\n");
	 continue;
      }
      s = rp->cat_of_UniRecord;
      if (s && s[0] == 'L')
	 r = 1;
      printf("%d\n", r);
   }

   printf("# pgtbl\n");
	for (i = 0; i < out.count_of_Coll; i++)
   {
      int r = 0;

      char *s;

		rp = (UniRecord *) out.items_of_Coll[i];
      if (!rp)
      {
	 printf("0\n");
	 continue;
      }
      s = rp->cat_of_UniRecord;
		if (!strcmp(s, "So") && rp->no_of_UniRecord >= 0x2500 && rp->no_of_UniRecord < 0x25ff)
			r = pg_chars[rp->no_of_UniRecord - 0x2500];
      printf("%d\n", r);
   }

   printf("# multibyte\n%d\n", mb ? 1 : 0);

   return 0;
}
