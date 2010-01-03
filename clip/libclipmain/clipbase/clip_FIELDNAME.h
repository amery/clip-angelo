int
clip_FIELDNAME(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "FIELDNAME";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int       fld = _clip_parni(ClipMachineMemory, 1) - 1;


   if (!wa)
    {
       _clip_retc(ClipMachineMemory, "");
       return 0;
    }

   return rdd_fieldname(ClipMachineMemory, wa->rd, fld, __PROC__);
}
