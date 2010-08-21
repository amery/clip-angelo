/*****************************/
int
clip_GETHOSTBYADDR(ClipMachine * ClipMachineMemory)
{
   char *addr = _clip_parc(ClipMachineMemory, 1);

   if (addr == NULL || *addr == 0)
      _clip_retc(ClipMachineMemory, "");
   else
      _clip_retc(ClipMachineMemory, (char *) tcp_host_name(addr));

   return 0;
}
