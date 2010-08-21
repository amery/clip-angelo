int
clip_RDD_NAME(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDD_NAME";

   int no = _clip_parni(ClipMachineMemory, 1);

   int er;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   _clip_retc(ClipMachineMemory, (*ClipMachineMemory->dbdrivers)[no - 1].id_of_DBDriver);
   return 0;
 err:
   return er;
}
