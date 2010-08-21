int
clip_UNAME(ClipMachine * ClipMachineMemory)
{
   int val = _clip_parni(ClipMachineMemory, 1);

   struct utsname namesys;

   int ret = uname(&namesys);

   _clip_retc(ClipMachineMemory, "");
   if (ret < 0)
      return 0;
   switch (val)
   {
   case 1:
      _clip_retc(ClipMachineMemory, namesys.sysname);
      break;
   case 2:
      _clip_retc(ClipMachineMemory, namesys.release);
      break;
   case 3:
      _clip_retc(ClipMachineMemory, namesys.version);
      break;
   case 4:
      _clip_retc(ClipMachineMemory, namesys.machine);
      break;
#ifdef _GNU_SOURCE
   case 5:
      _clip_retc(ClipMachineMemory, namesys.domainname);
      break;
#endif
   default:
      _clip_retc(ClipMachineMemory, namesys.nodename);
      break;
   }
   return 0;
}
