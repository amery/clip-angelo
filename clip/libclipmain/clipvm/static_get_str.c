static void
get_str(char *modbeg, int no, char **sp, int *len)
{
   short strOffs = GETSHORT(F_OFFS(modbeg, 4, 3, 0));

   char *bp = (modbeg + strOffs);

   bp += 2 * no * sizeof(long);

   SETINT(len, GET_LONG(bp));
   *sp = modbeg + GETLONG(bp);
}
