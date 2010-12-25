static int
make_pa_file(char *arname, int argc, char **argv)
{
   Coll coll;

   Coll offs;

   Coll inits, exits;

   Coll readed;

   int i;

   long npub, nstat, modpos;

   short ninit, nexit;

   FILE *file;

   time_t ts;

   long mod_len, mod_beg, func_Offs, fp, end;

   init_Coll(&coll, free, NULL);
   init_Coll(&offs, NULL, cmp_OffsEntry);
   init_Coll(&inits, NULL, NULL);
   init_Coll(&exits, NULL, NULL);
   init_Coll(&readed, NULL, strcmp);
   for (i = 0; i < argc; ++i)
      read_file(argv[i], &coll, &readed);

   npub = 0;
   ninit = 0;
   nexit = 0;
   nstat = 0;
   modpos = 0;
	for (i = 0; i < coll.count_of_Coll; ++i)
   {
		char *modp = (char *) coll.items_of_Coll[i];

      char *modbeg = M_OFFS(modp, 2, 0);

      long modlen = *(long *) M_OFFS(modp, 1, 0);

      long funcOffs = *(long *) M_OFFS(modp, 7, 4 /*6, 5 */ );

      long *f_p = (long *) (modbeg + funcOffs);

      long funcnum = *(long *) M_OFFS(modp, 4, 2);

      /*long allnum = *(long *) M_OFFS(modp, 5, 2); */
      short initnum = *(short *) M_OFFS(modp, 8, 4 /*7, 5 */ );

      short exitnum = *(short *) M_OFFS(modp, 8, 5 /*7, 6 */ );

      long statnum = *(long *) M_OFFS(modp, 3, 0);

      long *statOffs = (long *) M_OFFS(modp, 2, 0);

      int j;

      *statOffs = nstat;
      nstat += statnum;
      modpos += 8 + 2 * sizeof(long);

      for (j = 0; j < funcnum; ++j)
      {
	 OffsEntry *op = (OffsEntry *) malloc(sizeof(OffsEntry));

	 op->hash_of_OffsEntry = *f_p;
	 ++f_p;
	 op->offs_of_OffsEntry = *f_p + modpos;
	 ++f_p;
	 insert_Coll(&offs, op);
      }
      for (j = 0; j < initnum; ++j)
      {
	 OffsEntry *op = (OffsEntry *) malloc(sizeof(OffsEntry));

	 op->hash_of_OffsEntry = *f_p;
	 ++f_p;
	 op->offs_of_OffsEntry = *f_p + modpos;
	 ++f_p;
	 insert_Coll(&inits, op);
      }
      for (j = 0; j < exitnum; ++j)
      {
	 OffsEntry *op = (OffsEntry *) malloc(sizeof(OffsEntry));

	 op->hash_of_OffsEntry = *f_p;
	 ++f_p;
	 op->offs_of_OffsEntry = *f_p + modpos;
	 ++f_p;
	 insert_Coll(&exits, op);
      }

      modpos += modlen;
   }
   npub = offs.count_of_Coll;
	ninit = inits.count_of_Coll;
	nexit = exits.count_of_Coll;

   file = fopen(arname, "wb");
   if (!file)
      error("cannot open file '%s': %s", arname, strerror(errno));

   time(&ts);
   v_printf(1, "write file '%s', %d func\n", arname, npub);

   write_buf(file, "!<pobj>\n", 8);
   write_long(file, ts);
   mod_len = ftell(file);
   write_long(file, 0);
   mod_beg = ftell(file);
   write_long(file, 0);
   write_long(file, nstat);
   write_short(file, 0);
   write_short(file, 0);
   write_long(file, npub);
   write_long(file, npub + ninit + nexit);
   write_short(file, 0);
   write_short(file, 0);
   write_long(file, 0);
   func_Offs = ftell(file);
   write_long(file, 0);
   write_short(file, ninit);
   write_short(file, nexit);
   write_str(file, arname);
   /* func    */

   fp = ftell(file);

   modpos = fp - mod_beg + (npub + ninit + nexit) * sizeof(long) * 2;

	for (i = 0; i < offs.count_of_Coll; ++i)
   {
		OffsEntry *op = (OffsEntry *) offs.items_of_Coll[i];

		write_long(file, op->hash_of_OffsEntry);
		write_long(file, op->offs_of_OffsEntry + modpos);
   }
   for (i = 0; i < inits.count_of_Coll; ++i)
   {
		OffsEntry *op = (OffsEntry *) inits.items_of_Coll[i];

		write_long(file, op->hash_of_OffsEntry);
		write_long(file, op->offs_of_OffsEntry + modpos);
   }
   for (i = 0; i < exits.count_of_Coll; ++i)
   {
		OffsEntry *op = (OffsEntry *) exits.items_of_Coll[i];

		write_long(file, op->hash_of_OffsEntry);
		write_long(file, op->offs_of_OffsEntry + modpos);
   }

   end = ftell(file);
   fseek(file, mod_len, SEEK_SET);
   write_long(file, end - mod_beg);
   fseek(file, func_Offs, SEEK_SET);
   write_long(file, fp - mod_beg);
   fseek(file, end, SEEK_SET);

	for (i = 0; i < coll.count_of_Coll; ++i)
   {
		char *modp = (char *) coll.items_of_Coll[i];

      long modlen = *(long *) M_OFFS(modp, 1, 0);

      modlen += 8 + 2 * sizeof(long);

      write_buf(file, modp, modlen);
   }

   end = ftell(file);
   /*
      fseek(file, mod_len, SEEK_SET);
      write_long(file, end-mod_beg);
    */
   v_printf(1, "done, %ld bytes\n", end);
   fclose(file);
   return 0;
}
