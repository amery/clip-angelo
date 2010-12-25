int
clip_DSTRTON(ClipMachine * ClipMachineMemory)
{

   char *s = _clip_parc(ClipMachineMemory, 1);

   if (s != NULL)
      _clip_retndp(ClipMachineMemory, *((double *) s), 0, ClipMachineMemory->decimals);
   else
      _clip_retni(ClipMachineMemory, 0);

   return 0;
}
