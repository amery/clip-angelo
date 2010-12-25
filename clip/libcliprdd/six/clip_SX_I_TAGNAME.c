int
clip_SX_I_TAGNAME(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_I_TAGNAME";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipMachineMemory->m6_error = 0;
   CHECKWA(wa);
   _clip_retc(ClipMachineMemory, wa->rd->tagindexing);
   return 0;
}
