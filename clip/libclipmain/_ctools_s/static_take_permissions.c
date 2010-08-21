static int
take_permissions(ClipMachine * ClipMachineMemory, int port)
{
#ifdef OS_LINUX
   if (plevel > 2)
      return 0;
   if (port >= 0x400)
   {
      if (plevel < 3)
      {
       try_iopl:
	 if (iopl(3))
	 {
	    _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "Cannot take io permissons: %s", strerror(errno));
	    return _clip_call_errblock(ClipMachineMemory, -1);
	 }
	 plevel = 3;
	 return 0;
      }
   }
   else
   {
      if (ioperm(port, 1, 1))
	 goto try_iopl;
   }
#endif

#ifdef OS_FREEBSD
#ifdef ARCH_i386
   if (i386_set_ioperm(port, 1, 1))
   {
      _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "Cannot take io permissons: %s", strerror(errno));
      return _clip_call_errblock(ClipMachineMemory, -1);
   }
#endif
#endif

#ifdef OS_GYGWIN
#endif

   return 0;
}
