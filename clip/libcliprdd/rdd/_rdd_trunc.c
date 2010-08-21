int
_rdd_trunc(ClipMachine * ClipMachineMemory, RDD_FILE * file, unsigned int len, const char *__PROC__)
{
#ifdef HAVE_MMAN_H
   if (file->md != (caddr_t) - 1)
   {
      if (munmap(file->md, file->mapsize) == -1)
	 goto err;
      if (ftruncate(file->fd, len) == -1)
	 goto err;
      file->mapsize = len;
      file->md = (caddr_t) mmap(0, file->mapsize, PROT_READ | PROT_WRITE, MAP_SHARED, file->fd, 0);
   }
   else
   {
#endif
      if (ftruncate(file->fd, len) == -1)
	 goto err;
#ifdef HAVE_MMAN_H
   }
#endif
   return 0;
 err:
   return rdd_err(ClipMachineMemory, EG_WRITE, errno, __FILE__, __LINE__, __PROC__, er_ioerror);
}
