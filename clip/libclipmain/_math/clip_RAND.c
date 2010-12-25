int
clip_RAND(ClipMachine * ClipMachineMemory)
{
   double ret = ((double) random()) / 0x7FFFFFFF;

   if (ret < 0)
      ret = 0 - ret;
   _clip_retnd(ClipMachineMemory, ret);
   return 0;
}
