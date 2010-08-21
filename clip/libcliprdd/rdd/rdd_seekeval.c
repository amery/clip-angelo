int
rdd_seekeval(ClipMachine * ClipMachineMemory, RDD_DATA * rd, ClipVar * block, int *found, const char *__PROC__)
{
   RDD_ORDER *ro;

   int er;

   if ((rd->ords_opened < 1) || (rd->curord == -1))
      return rdd_err(ClipMachineMemory, EG_NOORDER, 0, __FILE__, __LINE__, __PROC__, er_noorder);

   ro = rd->orders[rd->curord];
   if (!ro->vtbl->wildskip)
      return rdd_err(ClipMachineMemory, EG_UNSUPPORTED, 0, __FILE__, __LINE__, __PROC__, "Operation is not supported by current RDD");

   if ((er = rdd_checkifnew(ClipMachineMemory, rd, __PROC__)))
      return er;

   if ((er = ro->vtbl->wildskip(ClipMachineMemory, rd, ro, NULL, 0, NULL, block, found, __PROC__)))
      return er;
   if (!(*found))
   {
      unsigned int lastrec;

      if ((er = rdd_lastrec(ClipMachineMemory, rd, (int *) (&lastrec), __PROC__)))
	 return er;
      rd->eof = 1;
      if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, lastrec + 1, 0, __PROC__)))
	 return er;
   }
   return 0;
}
