/*****************************/
int
clip_GETDOMAINNAME(ClipMachine * ClipMachineMemory)
{
   /* return host domain name of process */
   char buf[256];

#ifdef OS_MINGW
   DWORD s;

   GetComputerName(buf, &s);
#else
   if (getdomainname(buf, 255) != 0)
      _clip_retc(ClipMachineMemory, "");
   else
#endif
      _clip_retc(ClipMachineMemory, buf);
   return 0;
}
