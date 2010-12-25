static int
m_access(const char *path, int mode)
{
   int r;

   struct stat st;

   if ((r = stat(path, &st)))
      return r;

   if (!S_ISREG(st.st_mode))
   {
      _clip_logg(2, "no access to file '%s': not a regular file", path);
      return 1;
   }

   r = access(path, mode);
   if (r)
      _clip_logg(2, "no access to file '%s': %s", path, strerror(errno));

   return r;
}
