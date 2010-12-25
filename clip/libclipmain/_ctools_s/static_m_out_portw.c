static int
m_out_portw(ClipMachine * ClipMachineMemory, int port, int val)
{
   int r;

   r = take_permissions(ClipMachineMemory, port);
   if (r)
      return r;

#if defined(OS_LINUX) || defined(OS_FREEBSD)
   outw(val, port);
#endif

   return 0;
}
