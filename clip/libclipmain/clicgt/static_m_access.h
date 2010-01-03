static int
m_access(const char *path, int mode)
{
   int       r;

   struct stat st;

   if ((r = stat(path, &st)))
      return r;

   if (!S_ISREG(st.st_mode))
    {
       v_printf(2, "no access to file '%s': not a regular file\n", path);
       return 1;
    }

   r = access(path, mode);
   if (r)
      v_printf(2, "no access to file '%s': %s\n", path, strerror(errno));

   return r;
}
