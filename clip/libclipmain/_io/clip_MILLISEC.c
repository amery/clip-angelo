int
clip_MILLISEC(ClipMachine * ClipMachineMemory)
{
   int per = _clip_parni(ClipMachineMemory, 1);

   struct timeval tv;

   tv.tv_sec = per / 1000;
   tv.tv_usec = (per % 1000) * 1000;
   select(0, 0, 0, 0, &tv);
   return 0;
}
