static int
m_in_portb(ClipMachine * ClipMachineMemory, int port)
{
   int r;

   r = take_permissions(ClipMachineMemory, port);
   if (r)
      return r;

#if defined(OS_LINUX) || defined(OS_FREEBSD)
   r = inb(port);
#endif

   return r;
}
