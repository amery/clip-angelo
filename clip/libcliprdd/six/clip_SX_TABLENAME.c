int
clip_SX_TABLENAME(ClipMachine * ClipMachineMemory)
{
   DBWorkArea *wa = cur_area(ClipMachineMemory);

   char *s = wa->rd->path;

   ClipMachineMemory->m6_error = 0;
   _clip_retc(ClipMachineMemory, "");
   if (!wa)
      return 0;

   s = max(s, strrchr(wa->rd->path, '/'));
   s = max(s, strrchr(wa->rd->path, '\\'));
   if (s > wa->rd->path)
      s++;
   _clip_retc(ClipMachineMemory, s);
   return 0;
}
