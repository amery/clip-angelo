int
clip_SX_VERSION(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_VERSION";

   int type = _clip_parni(ClipMachineMemory, 1);

   int er;

   ClipMachineMemory->m6_error = 0;
   CHECKOPT1(1, NUMERIC_type_of_ClipVarType);
   switch (type)
   {
   case 1:
      _clip_retdc(ClipMachineMemory, 2002, 10, 14);
      break;
   case 2:
      _clip_retc(ClipMachineMemory, "00:00a");
      break;
   case 3:
      _clip_retc(ClipMachineMemory, "SIx 3.* by (c) SuccessWare, Inc. for CLIP, 1.0b, 10/14/2002, 00:00a");
      break;
   default:
      _clip_retc(ClipMachineMemory, "1.0b");
   }
   return 0;
 err:
   return er;
}
