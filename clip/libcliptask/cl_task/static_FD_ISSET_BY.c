static int
FD_ISSET_BY(fd_set * fds, fd_set * mask)
{
#if 0
   int i;

   for (i = 0; i < sizeof(fd_set) / sizeof(unsigned); i++, ((unsigned *) fds)++, ((unsigned *) mask)++)
      if (*((unsigned *) fds) & *((unsigned *) mask))
	 return 1;
#else
   int i;

   unsigned *fds_ptr = (unsigned *) fds;

   unsigned *mask_ptr = (unsigned *) mask;

   for (i = 0; i < sizeof(fd_set) / sizeof(unsigned); i++, fds_ptr++, mask_ptr++)
      if (*fds_ptr & *mask_ptr)
	 return 1;
#endif
   return 0;
}
