static int
print_table(char *arname)
{
   FILE *file = fopen(arname, "rb");

   char *modp;

   if (!file)
      error("cannot open file '%s': %s", arname, strerror(errno));

   while ((modp = read_module(file)))
   {
      char *modbeg = M_OFFS(modp, 2, 0);

      int j;

      int modlen = *(long *) M_OFFS(modp, 1, 0);

      int funcnum = *(long *) M_OFFS(modp, 5, 2);

      int pubnum = *(long *) M_OFFS(modp, 4, 2);

      int initnum = *(short *) M_OFFS(modp, 8, 4 /*7, 5 */ );

      int exitnum = *(short *) M_OFFS(modp, 8, 5 /*7, 6 */ );

      time_t ts = *(long *) M_OFFS(modp, 0, 0);

      char *filename = M_OFFS(modp, 8, 6 /*7, 7 */ );

      long funcOffs = *(long *) M_OFFS(modp, 7, 4 /*6, 5 */ );

      long *fp = (long *) (modbeg + funcOffs);

      v_printf(0, "%s: %d byte %d func %d pub %d inits %d exits %s", filename, modlen + 8 + 2 * sizeof(long), funcnum, pubnum, initnum, exitnum, ctime(&ts));

      for (j = 0; j < funcnum; ++j, fp += 2)
      {
	 v_printf(1, "\t%d:\thash=0x%x\toffs=%ld\n", j, fp[0], fp[1]);
      }

      free(modp);
   }

   fclose(file);
   return 0;
}
