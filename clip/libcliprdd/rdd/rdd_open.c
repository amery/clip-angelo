int
rdd_open(ClipMachine * ClipMachineMemory, char *path, int readonly, int shared, int *fd, const char *__PROC__)
{
   *fd = _clip_open(ClipMachineMemory, path, readonly ? O_RDONLY : O_RDWR, 0, !shared);
   if (*fd == -1)
   {
      return rdd_err(ClipMachineMemory, EG_OPEN, errno, __FILE__, __LINE__, __PROC__, path);
   }
   return 0;
}
