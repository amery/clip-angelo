int
main(int argc, char **argv)
{
   int i;

   char errbuf[128];

   FILE *file;

   if (argc < 2)
   {
      printf("usage: %s keymap\n", argv[0]);
      return 1;
   }

   file = fopen(argv[1], "rt");
   if (!file)
   {
      printf("cannot open file %s\n", argv[1]);
      return 2;
   }

   if (load_keymap(file, errbuf, sizeof(errbuf)))
   {
      printf("load_keymap error: %s\n", errbuf);
      exit(1);
   }
   fclose(file);

   tcgetattr(0, &ts);
   ts0 = ts;
   cfmakeraw(&ts);
   tcsetattr(0, TCSANOW, &ts);

   atexit(exit_f);
   signal(SIGINT, sig_f);
   signal(SIGTERM, sig_f);
   signal(SIGHUP, sig_f);

/* switch into scanmode */
   printf("\033[S");

   printf("\r\n");

   for (i = 0; i < 100; i++)
   {
      unsigned char b;

      long key;

      int state;

      state = scan_state();
      printf("\r\nstate: %d\r\n", state);

      if (!read(0, &b, 1))
	 break;

      /*printf("read: %x\r\n", (int)b); */

      key = scan_key(b);
      if (key)
      {
	 state = scan_state();
	 printf("scan_key: %ld (%c), state=%d\r\n", key, (int) ((key > 32 && key < 256) ? key : 32), state);
	 if (key == 'q')
	    exit(0);
      }
   }

   return 0;
}
