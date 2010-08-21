int
clip_ISALPHA(ClipMachine * ClipMachineMemory)
{
   char *vp = _clip_parc(ClipMachineMemory, 1);

   _clip_retl(ClipMachineMemory, 0);
   if (vp != NULL)
      _clip_retl(ClipMachineMemory, _clip_isalpha(vp[0]));
   return 0;
}
