static int
hs_add(ClipMachine * ClipMachineMemory, HIPER * hs, const char *str, int len, unsigned int *rno, const char *__PROC__)
{
   unsigned int rec;

   unsigned char c4[4];

   int er;

   if ((er = rdd_read(ClipMachineMemory, &hs->file, 10, 4, c4, __PROC__)))
      return er;
   rec = _rdd_uint(c4) + 1;
   if ((er = _hs_add(ClipMachineMemory, hs, str, len, rec, __PROC__)))
      return er;
   _rdd_put_uint(c4, rec);
   if ((er = rdd_write(ClipMachineMemory, &hs->file, 10, 4, c4, __PROC__)))
      return er;
   hs->lastrec = rec;
   *rno = rec;
   return 0;
}
