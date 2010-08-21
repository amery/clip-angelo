int
clip_CLIP_MEMOTYPE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "CLIP_MEMOTYPE";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   char *type = NULL;

   CHECKWA(wa);
   switch (wa->rd->sig)
   {
   case 0x83:
      type = "DBT";
      break;
   case 0xF5:
   case 0x30:
      type = "FPT";
      break;
   default:
      break;
   }
   _clip_retc(ClipMachineMemory, type);
   return 0;
}
