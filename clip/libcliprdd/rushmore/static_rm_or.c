static void
rm_or(unsigned int *lval, unsigned int *rval, int *lopt, int ropt, int bytes)
{
   int i;

   for (i = 0; i < bytes; i++)
      lval[i] = lval[i] | rval[i];
   free(rval);
   if (*lopt == 2 && ropt == 2)
      *lopt = 2;
   else if ((*lopt == 2 && ropt == 1) || (*lopt == 1 && ropt == 2))
      *lopt = 1;
   else if (*lopt == 1 && ropt == 1)
      *lopt = 1;
   else
      *lopt = 0;
}
