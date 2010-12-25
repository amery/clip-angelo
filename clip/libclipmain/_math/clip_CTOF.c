int
clip_CTOF(ClipMachine * ClipMachineMemory)
{
   int SLen;

   char *S = _clip_parcl(ClipMachineMemory, 1, &SLen);

   _clip_retnd(ClipMachineMemory, *((double *) S));
   return 0;
}
