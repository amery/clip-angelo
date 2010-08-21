int
main(int argc, char **argv)
{
   int r = 0, setout = 0;

   char *start = "\137(";

   char *stop = ")\137";

   int fnum = 0;

   FILE *in, *out;

   ParseData par;

   in = stdin;
   out = stdout;

   prgname = strrchr(argv[0], '/');
   if (prgname)
      ++prgname;
   else
      prgname = argv[0];

   init_Coll(&po_files, 0, 0);

   for (++argv, --argc; argc > 0;)
   {
      char *ap;

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
	    fprintf(stdout, "usage: %s [options] \n", prgname);
	    fprintf(stdout, "\t-i <po_filename> filename of input translation, may be mutltiple times\n");
	    fprintf(stdout, "\t-o <in_filename> <out_filename> template/result pair (stdin/stdout) \n");
	    fprintf(stdout, "\t-s <start_string> start entry string (default is \"_(\")\n");
	    fprintf(stdout, "\t-p <stop_string> stop entry string (default is \")_\")\n");
	    fprintf(stdout, "\t-v<level> set verbose level\n");
	    fprintf(stdout, "\t-u recode to utf-8\n");
	    fprintf(stdout, "\t-e <encoding> recode to encoding\n");
	    return 1;
	 case 'u':
	    encoding = "utf-8";
	    break;
	 case 'e':
	    if (argc < 2 || argv[1][0] == '-')
	       goto err;
	    encoding = argv[1];
	    debug(1, "set output encoding to '%s'", encoding);
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
	 case 'v':
	    if (ap[2])
	       verbose = atoi(ap + 2);
	    else
	       ++verbose;
	    break;
	 case 'i':
	    if (argc < 2 || argv[1][0] == '-')
	       goto err;
	    debug(1, "read po file %s", argv[1]);
	    if (read_po_file(argv[1]))
	    {
	       fprintf(stderr, "error reading po file '%s': %s\n", argv[1], strerror(errno));
	       return 1;
	    }
	    argc--;
	    argv++;
	    break;
	 case 'o':
	    setout++;
	    if (argc < 2 || argv[1][0] == '-')
	       goto err;
	    if (in != stdin)
	       fclose(in);
	    filename = argv[1];
	    in = fopen(argv[1], "r");
	    if (!in)
	    {
	       fprintf(stderr, "cannot open input file '%s': %s\n", argv[1], strerror(errno));
	       return 1;
	    }
	    debug(1, "open input file %s", argv[1]);
	    argc--;
	    argv++;
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
	 }

	 ++argv, --argc;
      }

      if (!setout)
	 continue;

    parse:

if (!po_files.count_of_Coll)
	 goto err;
      fnum++;
		par.out_of_ParseData = out;
      r = po_parse_template(in, filename, start, stop, &par, entry_handler, norm_handler);
      if (r)
	 return r;

      ++argv, --argc;
   }

   if (!fnum)
      goto parse;

   return 0;
}
