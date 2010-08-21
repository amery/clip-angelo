int
clip_TRANSFORM(ClipMachine * ClipMachineMemory)
{
   ClipVar *vp = _clip_par(ClipMachineMemory, 1);

   char *pict = _clip_parc(ClipMachineMemory, 2);

   transform(ClipMachineMemory, vp, pict);
   _clip_retcn(ClipMachineMemory, ClipMachineMemory->buf, strlen(ClipMachineMemory->buf));

   return 0;
}
