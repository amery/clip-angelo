/*****************************/
int
clip_BIN2IPADDR(ClipMachine * ClipMachineMemory)
{
   int len;

   char *addr = _clip_parcl(ClipMachineMemory, 1, &len);

   if (addr == NULL || len != sizeof(struct in_addr))
      _clip_retc(ClipMachineMemory, "");
   else
      _clip_retc(ClipMachineMemory, inet_ntoa(*(struct in_addr *) addr));

   return 0;
}
