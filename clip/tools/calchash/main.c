int
main(int argc, char **argv)
{
   char buf[256], *beg, *end;

   int dec_flag = 0, i;

   for (i = 1; i < argc; i++)
   {
      char *ap;

      ap = argv[i];

      if (!strcmp(ap, "-d"))
      {
	 dec_flag = 1;
      }
      else
      {
	 fprintf(stderr, "usage: %s [-d] < inuput_stream >output_stream\n", argv[0]);
	 fprintf(stderr, " 	-d for deciman output (default is hex)\n");
	 exit(1);
      }
   }

   while (fgets(buf, sizeof(buf), stdin))
   {
      for (beg = buf; *beg && (beg - buf) < sizeof(buf) && (*beg == ' ' || *beg == '\t'); ++beg)
	 ;
      for (end = buf + strlen(buf) - 1; end >= buf && (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n'); --end)
	 ;
      if (end >= beg)
      {
	 fputc('\'', stdout);
	 fwrite(beg, end - beg + 1, 1, stdout);
	 fputc('\'', stdout);
	 if (dec_flag)
	    fprintf(stdout, ": %ld\n", _clip_casehashbytes(0, beg, end - beg + 1));
	 else
	    fprintf(stdout, ": 0x%lx\n", _clip_casehashbytes(0, beg, end - beg + 1));
      }
   }
   return 0;
}
