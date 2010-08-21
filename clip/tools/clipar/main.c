int
main(int argc, char **argv)
{
   int ch, i;

   char *arname;

   tmp_file[0] = 0;
   while ((ch = getopt(argc, argv, "tchVv")) != EOF)
      switch (ch)
      {
      case 'V':
	 printf("0.1\n");
	 exit(0);
      case 'v':
	 verbose_level++;
	 break;
      case 't':
	 table_flag = 1;
	 break;
      case 'c':
	 c_flag = 1;
	 break;
      case 'h':
       usage:
      default:
	 printf("usage: clipar [-tcvV] <file>.pa [<file>.po|<file>.pa...]\n");
	 printf("\t-t print table of archive\n");
	 printf("\t-c make C file\n");
	 printf("\t-v increase verbose level\n");
	 printf("\t-V print version and exit\n");
	 exit(1);
      }
   argc -= optind;
   argv += optind;

   if (table_flag && argc < 1)
      goto usage;

   if (table_flag)
   {
      for (i = 0; i < argc; ++i)
      {
	 if (argc > 1)
	    v_printf(0, "file '%s' table:\n", argv[i]);
	 print_table(argv[i]);
      }
      return 0;
   }

   arname = argv[0];
   argv++;
   argc--;

   if (argc < 1)
      goto usage;

   if (c_flag)
      return make_c_file(arname, argc, argv);

   return make_pa_file(arname, argc, argv);
}
