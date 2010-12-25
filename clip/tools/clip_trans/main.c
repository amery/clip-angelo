int
main(int argc, char **argv)
{
   int i, j, k, ch, len = 0, ret;

   FILE *f;

   char *buffer = NULL;

   int *tbl1;

   int *tbl2;

   int unich;

   char *e;

   tbl1 = calloc(sizeof(int), 256);
   tbl2 = calloc(sizeof(int), 256);

   if (argc < 3)
   {
      printf("translation file from SOURCE codepage to TARGET codepage\n");
      printf("Usage: clip_trans CP_source CP_target file1 .... fileN\n");
      return 1;
   }

   e = getenv("CLIPROOT");
   if (e && *e)
      CLIPROOT = e;
   if (CLIPROOT == NULL || *CLIPROOT == 0)
   {
      printf("error:CLIPROOT is empty in environment.\n");
      return -1;
   }
   printf("\nsearch info of codepages in directory %s/charsets", CLIPROOT);
   printf("\nload info of codepage %s ... ", argv[1]);

   if (load_unicode_table(argv[1], tbl1) < 0)
   {
      printf("error:%s\n", strerror(errno));
      return 1;
   }
   printf("O`k");
   //print_unicode_table(tbl1);
   printf("\nload info of codepage %s ... ", argv[2]);

   if (load_unicode_table(argv[2], tbl2) < 0)
   {
      printf("error:%s\n", strerror(errno));
      return 1;
   }
   printf("O`k");
   //print_unicode_table(tbl2);
   printf("\ntranslate %d files from %s to %s\n", argc - 3, argv[1], argv[2]);
   for (i = 4; i <= argc; i++)
   {
      printf("\n%s...", argv[i - 1]);
      f = fopen(argv[i - 1], "r+t");
      if (f == NULL)
      {
	 printf("error open for reading:%s\n", strerror(errno));
	 continue;
      }
      if (buffer != NULL)
	 free(buffer);
      if (fseek(f, 0, SEEK_END) < 0 || (len = ftell(f)) < 0 || fseek(f, 0, SEEK_SET) < 0)
      {
	 printf("error length:%d:%s\n", len, strerror(errno));
	 fclose(f);
	 continue;
      }
      buffer = malloc(len);
      if (buffer == NULL)
      {
	 printf("error allocated memory:%d:%s\n", len, strerror(errno));
	 fclose(f);
	 continue;
      }
      if ((ret = fread(buffer, 1, len, f)) != len)
      {
	 printf("error read:%d:%d:%s\n", ret, len, strerror(errno));
	 fclose(f);
	 continue;
      }
      printf("read %d bytes ...", len);
      for (j = 0; j < len; j++)
      {
	 ch = buffer[j];
	 if (ch < 0)
	    ch += 256;
	 if (ch < 32)
	    continue;
	 unich = tbl1[ch];
	 for (k = 0; k < 256; k++)
	 {
	    if (unich == tbl2[k])
	    {
	       buffer[j] = k;
	       break;
	    }
	 }
      }
      /* writing */
      fclose(f);
      f = fopen(argv[i - 1], "w+b");
      if (f == NULL)
      {
	 printf("error open for writing:%s\n", strerror(errno));
	 continue;
      }
      if ((ret = fwrite(buffer, 1, len, f)) != len)
      {
	 printf("error write:%d:%d:%s\n", ret, len, strerror(errno));
	 fclose(f);
	 continue;
      }
      printf("write %d bytes ...", len);
      printf("O`k");
      fclose(f);
   }
   printf("\nO`k\n");
   return 0;
}
