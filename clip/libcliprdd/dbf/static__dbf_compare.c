static int
_dbf_compare(void *op, void *lp, void *rp, int *uniq)
{
   if ((unsigned long) lp < (unsigned long) rp)
      return -1;
   if ((unsigned long) lp > (unsigned long) rp)
      return 1;
   return 0;
}
