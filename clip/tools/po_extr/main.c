int
main(int argc, char **argv)
{
   int r = 0;

   char *start = "\137(";

   char *stop = ")\137";

   int noheader = 0;

   int i;

   int fnum = 0;

   FILE *out;

   EntryData par;

   prgname = strrchr(argv[0], '/');
   if (prgname)
      ++prgname;
   else
      prgname = argv[0];

   out = stdout;
	init_Coll(&par.ids_of_EntryData, delete_PoEntry, cmp_PoEntry);
	init_Coll(&par.uids_of_EntryData, 0, 0);

   for (++argv, --argc; argc > 0;)
   {
      char *ap;

      FILE *in;

      ap = argv[0];

      if (ap[0] == '-')
      {
	 switch (ap[1])
	 {
	 default:
	    fflush(stdout);
	    fprintf(stderr, "unknown option: %s\n", ap);
	    fflush(stderr);
	 case 'h':
	 case 'H':
	    goto help;
	  err:
	    fflush(stdout);
	    fprintf(stderr, "invalid command string\n");
	    fflush(stderr);
	  help:
	    fprintf(stdout, "usage: %s [options] [file1 ...  fileN]\n", prgname);
	    fprintf(stdout, "\t-o <out_filename>\toutput filename (default is stdout)\n");
	    fprintf(stdout, "\t-s <start_string>\tstart entry string (default is \"_(\")\n");
	    fprintf(stdout, "\t-p <stop_string>\tstop entry string (default is \")_\")\n");
	    fprintf(stdout, "\t-n don't append default header/footer\n");
	    fprintf(stdout, "\t-d duplicate msgid to msgstr\n");
	    fprintf(stdout, "\t-v<level> set verbose level\n");
	    return 1;
	 case 'o':
	    if (argc < 2 || argv[1][0] == '-')
	       goto err;
	    if (out != stdout)
	       fclose(out);
	    out = fopen(argv[1], "w");
	    if (!out)
	    {
	       fprintf(stderr, "cannot open output file '%s': %s\n", argv[1], strerror(errno));
	       return 1;
	    }
	    debug(1, "open output file %s", argv[1]);
	    argc--;
	    argv++;
	    break;
	 case 's':
	    if (argc < 2 || argv[1][0] == '-')
	       goto err;
	    start = argv[1];
	    debug(1, "set start string to '%s'", start);
	    argc--;
	    argv++;
	    break;
	 case 'p':
	    if (argc < 2 || argv[1][0] == '-')
	       goto err;
	    stop = argv[1];
	    debug(1, "set stop string to '%s'", stop);
	    argc--;
	    argv++;
	    break;
	 case 'n':
	    noheader = 1;
	    break;
	 case 'd':
	    dupflag = 1;
	    break;
	 case 'v':
	    if (ap[2])
	       verbose = atoi(ap + 2);
	    else
	       ++verbose;
	    break;
	 }

	 ++argv, --argc;
      }

      if (argc < 1)
	 break;

      if (argv[0][0] == '-')
	 continue;

      in = fopen(argv[0], "r");
      if (!in)
      {
	 fprintf(stderr, "cannot open input file '%s': %s\n", argv[0], strerror(errno));
	 return 1;
      }
      filename = argv[0];
      debug(1, "process file %s", filename);
      ++fnum;
      r = po_parse_template(in, filename, start, stop, &par, entry_handler, 0);
      if (r)
	 return r;
      fclose(in);
      ++argv, --argc;
   }

   if (!fnum)
   {
      r = po_parse_template(stdin, "stdin", start, stop, &par, entry_handler, 0);
      if (r)
	 return r;
   }

   if (!noheader)
   {
      r = po_write_header(out);
      if (r)
	 return r;
   }

   debug(1, "%d files processed", fnum);
	debug(1, "output %d entries", par.uids_of_EntryData.count_of_Coll);

	for (i = 0; i < par.uids_of_EntryData.count_of_Coll; ++i)
   {
      PoEntry *ep;

		ep = (PoEntry *) par.uids_of_EntryData.items_of_Coll[i];
      po_write_entry(out, ep, dupflag);
   }

   if (out != stdout)
      fclose(out);

	destroy_Coll(&par.ids_of_EntryData);
	destroy_Coll(&par.uids_of_EntryData);

   return r;
}
