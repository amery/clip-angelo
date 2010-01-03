int
clip_RECNO(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RECNO";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int       recno, er;


   _clip_retndp(ClipMachineMemory, 0, 7, 0);
   if (!wa)
      return 0;

   if ((er = rdd_recno(ClipMachineMemory, wa->rd, &recno, __PROC__)))
      return er;
   _clip_retndp(ClipMachineMemory, recno, 7, 0);
   return 0;
}
