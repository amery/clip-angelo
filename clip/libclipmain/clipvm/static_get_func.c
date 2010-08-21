static void
get_func(char *modbeg, int no, char **fp)
{
   long funcOffs = GETLONG(F_OFFS(modbeg, 5, 4, 0));

   long *bp = (long *) (modbeg + funcOffs);

   bp += 2 * no;
   ++bp;
   *fp = modbeg + GETLONG(bp);
}
