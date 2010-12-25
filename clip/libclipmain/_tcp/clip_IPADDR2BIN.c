/*****************************/
int
clip_IPADDR2BIN(ClipMachine * ClipMachineMemory)
{
   char *addr = _clip_parc(ClipMachineMemory, 1);

   struct in_addr ina;

   if (tcp_host_addr(addr, &ina) == 0)
      _clip_retcn(ClipMachineMemory, (char *) &ina, sizeof(struct in_addr));
   else
      _clip_retc(ClipMachineMemory, "");

   return 0;
}
