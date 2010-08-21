static int
hs_close(ClipMachine * ClipMachineMemory, HIPER * hs, const char *__PROC__)
{
   if (hs->file.md != (caddr_t) - 1)
#ifdef OS_MINGW
      free(hs->file.md);
#else
      if (munmap(hs->file.md, hs->file.mapsize) == -1)
	 return rdd_err(ClipMachineMemory, EG_CLOSE, errno, __FILE__, __LINE__, __PROC__, er_ioerror);
#endif

   if (close(hs->file.fd))
      return rdd_err(ClipMachineMemory, EG_CLOSE, errno, __FILE__, __LINE__, __PROC__, er_ioerror);
   destroy_hiper(hs);

   return 0;
}
