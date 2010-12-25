static int
zero_fds(fd_set * fds)
{
   int i;

   for (i = 0; i < sizeof(fd_set) / sizeof(unsigned); i++, ((unsigned *) fds)++)
      if (*((unsigned *) fds))
	 return 0;
   return 1;
}
