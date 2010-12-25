
static int
m_out_portb(ClipMachine * ClipMachineMemory, int port, int val)
{
   int r;

   r = take_permissions(ClipMachineMemory, port);
   if (r)
      return r;

#if defined(OS_LINUX) || defined(OS_FREEBSD)
   outb(val, port);
#endif

   return 0;
}
