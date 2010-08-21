int
clip_FTOC(ClipMachine * ClipMachineMemory)
{
   double res = _clip_parnd(ClipMachineMemory, 1);

   _clip_retcn(ClipMachineMemory, (char *) (&res), 8);
   return 0;
}
