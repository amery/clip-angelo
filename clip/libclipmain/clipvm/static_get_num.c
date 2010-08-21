static void
get_num(char *modbeg, int no, double *sp, int *len, int *dec)
{
   short numOffs = GETSHORT(F_OFFS(modbeg, 4, 2, 0));

   short numNum = GETSHORT(F_OFFS(modbeg, 2, 0, 0));

   double *bp = (double *) (modbeg + numOffs);

   unsigned char *lens = (unsigned char *) (bp + numNum);

   unsigned char *decs = lens + numNum;

   memcpy(sp, bp + no, sizeof(*sp));
   memcpy(len, lens + no, sizeof(*len));
   memcpy(dec, decs + no, sizeof(*dec));
}
