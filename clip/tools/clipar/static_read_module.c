static char *
read_module(FILE * file)
{
   char *ret = 0;

   char mag[8];

   long beg = ftell(file);

   long l;

   char *filename;

   if (fread(mag, 8, 1, file) != 1)
   {
      if (feof(file))
	 return 0;
      goto ferr;
   }
   if (memcmp(mag, "!<pobj>\n", 8))
      error("invalid magic number");
   if (fread(&l, sizeof(l), 1, file) != 1)
      goto ferr;
   if (fread(&l, sizeof(l), 1, file) != 1)
      goto ferr;
   l += 8 + 2 * sizeof(long);

   fseek(file, beg, SEEK_SET);
   ret = (char *) malloc(l);
   if (fread(ret, l, 1, file) != 1)
      goto ferr;
   filename = M_OFFS(ret, 8, 6 /*7, 7 */ );

   v_printf(2, "read module %s: %ld bytes\n", filename, l);

   return ret;
 ferr:
   error("unexpected error: %s", strerror(errno));
   return 0;
}
