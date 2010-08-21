int
_clip_ftype(int fd)
{
   int retval = FT_ERROR;

   struct stat st;

   if (fstat(fd, &st) != -1)
   {
      if (S_ISREG(st.st_mode))
	 retval = FT_FILE;
      else if (S_ISFIFO(st.st_mode))
	 retval = FT_PIPE;
#ifndef OS_MINGW
      else if (S_ISSOCK(st.st_mode))
	 retval = FT_SOCKET;
      else if (S_ISLNK(st.st_mode))
	 retval = FT_LINK;
#endif
      else if (S_ISCHR(st.st_mode))
	 retval = FT_CHARDEV;
      else if (S_ISBLK(st.st_mode))
	 retval = FT_BLOCKDEV;
      else if (S_ISDIR(st.st_mode))
	 retval = FT_DIRECTORY;
      else
	 retval = FT_UNKNOWN;
      /*
         TODO: For native windows code we should check for other
         type of      handle
         Unimplemented so far:
         For COM_* function use FT_TERM
         For GZIP* -> FT_GZIP
         For BZIP2 -> FT_BZIP2
         It needs changes in clip-* libs.
       */
   }
   return retval;
}
