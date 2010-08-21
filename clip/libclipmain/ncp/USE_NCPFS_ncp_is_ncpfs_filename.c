int
ncp_is_ncpfs_filename(char *filename)
{
   int fd, r;

   fd = open(filename, O_RDONLY);
   if (fd < 0)
      return 0;

   r = ncp_is_ncpfs(fd);

   close(fd);
   return r;
}
