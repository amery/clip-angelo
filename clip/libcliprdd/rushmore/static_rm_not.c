static void
rm_not(unsigned int *lval, unsigned int *rval, int bytes)
{
   int i;

   for (i = 0; i < bytes; i++)
      lval[i] = ~rval[i];
   free(rval);
}
