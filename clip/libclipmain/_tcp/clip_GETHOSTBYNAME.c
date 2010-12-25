/*****************************/
int
clip_GETHOSTBYNAME(ClipMachine * ClipMachineMemory)
{
   char *addr = _clip_parc(ClipMachineMemory, 1);

   struct in_addr ina;

   if (tcp_host_addr(addr, &ina) == 0)
      _clip_retc(ClipMachineMemory, inet_ntoa(ina));
   else
      _clip_retc(ClipMachineMemory, "");

   return 0;
}
