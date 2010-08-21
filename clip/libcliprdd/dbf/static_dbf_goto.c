static int
dbf_goto(ClipMachine * ClipMachineMemory, RDD_DATA * rd, unsigned int rec, const char *__PROC__)
{
   int lastrec;

   int er;

/*	printf("goto %d\n? 'goto %d',recno(),bof()\n",rec,rec);*/
   if ((er = dbf_lastrec(ClipMachineMemory, rd, &lastrec, __PROC__)))
      return er;

   rd->recno = rec;
   rd->bof = rd->v_bof = rd->eof = 0;
   if ((rec < 1) || (rec > lastrec))
   {
      rd->v_bof = rd->eof = 1;
      rd->recno = lastrec + 1;
   }
   return dbf_rawgoto(ClipMachineMemory, rd, rd->recno, 0, __PROC__);
}
