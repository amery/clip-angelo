static int
m_in_portw(ClipMachine * ClipMachineMemory, int port)
{
   int r;

   r = take_permissions(ClipMachineMemory, port);
   if (r)
      return r;

#if defined(OS_LINUX) || defined(OS_FREEBSD)
   r = inw(port);
#endif

   return r;
}
