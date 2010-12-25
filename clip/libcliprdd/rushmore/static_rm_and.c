static void
rm_and(unsigned int *lval, unsigned int *rval, int *lopt, int ropt, int bytes)
{
   //int i, b;
   unsigned int *rv = rval;

   //unsigned int *lv = lval;
   unsigned int *end = lval + bytes;

 beg_:
   if (lval >= end)
      goto end_;
   *lval &= *rv;
   lval++;
   rv++;
   goto beg_;
 end_:
   free(rval);
   *lopt += ropt;
   if (*lopt == 4)
      *lopt = 2;
   else if (*lopt > 0)
      *lopt = 1;
}
