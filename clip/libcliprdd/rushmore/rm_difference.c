int
rm_difference(ClipMachine * ClipMachineMemory, RDD_FILTER * f1, RDD_FILTER * f2, const char *__PROC__)
{
   int i;

   unsigned int bytes = ((f1->size + 1) >> 5) + 1;

   if (f1->rmap)
   {
      for (i = 0; i < bytes; i++)
      {
	 f1->rmap[i] ^= f2->rmap[i];
      }
   }
   return 0;
}
