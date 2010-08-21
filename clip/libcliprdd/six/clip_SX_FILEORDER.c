int
clip_SX_FILEORDER(ClipMachine * ClipMachineMemory)
{
   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipMachineMemory->m6_error = 0;
   _clip_retni(ClipMachineMemory, 0);
   if (!wa)
      return 0;

   if (wa->rd->curord != -1)
      _clip_retni(ClipMachineMemory, wa->rd->orders[wa->rd->curord]->index->indexno + 1);
   return 0;
}
