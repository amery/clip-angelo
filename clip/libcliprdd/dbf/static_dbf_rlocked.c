static int
dbf_rlocked(ClipMachine * ClipMachineMemory, RDD_DATA * rd, unsigned int rec, int *r, const char *__PROC__)
{
   int i;

   *r = 1;
   if (!rd->shared)
      return 0;
   for (i = 0; i < rd->nlocks; i++)
   {
      if (rd->locks[i] == rec)
	 break;
   }
   if (i == rd->nlocks)
      *r = 0;
   return 0;
}
