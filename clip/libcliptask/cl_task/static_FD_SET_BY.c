static void
FD_SET_BY(fd_set * fds, fd_set * mask)
{
#if 0
   int i;

   for (i = 0; i < sizeof(fd_set) / sizeof(unsigned); i++, ((unsigned *) fds)++, ((unsigned *) mask)++)
      *((unsigned *) fds) |= *((unsigned *) mask);
#else
   int i;

   unsigned *fds_ptr = (unsigned *) fds;

   unsigned *mask_ptr = (unsigned *) mask;

   for (i = 0; i < sizeof(fd_set) / sizeof(unsigned); i++, fds_ptr++, mask_ptr++)
      *fds_ptr |= *mask_ptr;
#endif
}
