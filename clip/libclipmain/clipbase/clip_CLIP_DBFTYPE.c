int
clip_CLIP_DBFTYPE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "CLIP_DBFTYPE";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   char *type = NULL;

   CHECKWA(wa);
   switch (wa->rd->sig)
   {
   case 0x03:
   case 0x83:
      type = "DBF";
      break;
   case 0xF5:
      type = "FOX";
      break;
   case 0x30:
      type = "VFP";
      break;
   default:
      break;
   }
   _clip_retc(ClipMachineMemory, type);
   return 0;
}
