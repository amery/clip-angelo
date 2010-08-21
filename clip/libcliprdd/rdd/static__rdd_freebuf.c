static void
_rdd_freebuf(ClipMachine * ClipMachineMemory, RDD_DATA * rd)
{
   int i;

   for (i = 0; i < rd->nfields; i++)
   {
      if (rd->data[i])
      {
	 _clip_destroy(ClipMachineMemory, rd->data[i]);
	 free(rd->data[i]);
	 rd->data[i] = NULL;
      }
   }
}
