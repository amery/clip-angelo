int
clip_ISDIGIT(ClipMachine * ClipMachineMemory)
{
   char ch;

   int ret = 0;

   char *vp = _clip_parc(ClipMachineMemory, 1);

   _clip_retl(ClipMachineMemory, 0);
   if (vp == NULL)
      return 0;
   ch = vp[0];
   if (ch >= '0' && ch <= '9')
      ret = 1;
   _clip_retl(ClipMachineMemory, ret > 0);
   return 0;
}
