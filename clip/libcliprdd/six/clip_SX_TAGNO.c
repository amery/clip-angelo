int
clip_SX_TAGNO(ClipMachine * ClipMachineMemory)
{
   DBWorkArea *wa = cur_area(ClipMachineMemory);

   RDD_ORDER *ro;

   RDD_INDEX *ri;

   int i;

   ClipMachineMemory->m6_error = 0;
   _clip_retni(ClipMachineMemory, 0);

   if (!wa)
      return 0;

   if (wa->rd->curord == -1)
      return 0;

   ro = wa->rd->orders[wa->rd->curord];
   ri = ro->index;
   for (i = 0; i < ri->norders; i++)
   {
      if (ro == ri->orders[i])
      {
	 _clip_retni(ClipMachineMemory, i + 1);
	 break;
      }
   }
   return 0;
}
