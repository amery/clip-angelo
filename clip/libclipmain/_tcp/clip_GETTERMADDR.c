/*****************************/
int
clip_GETTERMADDR(ClipMachine * ClipMachineMemory)
{
   struct in_addr ina;

   if (tcp_term_addr(&ina) == 0)
      _clip_retc(ClipMachineMemory, inet_ntoa(ina));
   else
      _clip_retc(ClipMachineMemory, "");

   return 0;
}
