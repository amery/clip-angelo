int
clip_FILEMOVE(ClipMachine * ClipMachineMemory)	/* Moves files to another directory */
{
/*
	0      NO_DISK_ERR            No errors
	   -2      ER_FILE_NOT_FOUND      File not found
	   -3      ER_PATH_NOT_FOUND      Access path not found
	   -5      ER_ACCESS_DENIED       Access denied (e.g., network)
	   -17     ER_DIFFERENT_DEVICE    Target file not on same drive
*/
   char *src = _get_unix_name(ClipMachineMemory, _clip_parc(ClipMachineMemory, 1));

   char *dst = _get_unix_name(ClipMachineMemory, _clip_parc(ClipMachineMemory, 2));

   int ret = 0;

   if (src == NULL || dst == NULL)
   {
      ret = _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, inv_arg);
		/*_clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "invalid argument");*/
      goto end;
   }
   if (access(dst, F_OK) == 0)
   {				/* file exist */
      _clip_retni(ClipMachineMemory, ER_ACCESS_DENIED);
      goto end;
   }

   if (rename(src, dst) == 0)
   {
      _clip_retni(ClipMachineMemory, NO_DISK_ERR);
   }
   else if (errno == EXDEV)
   {
      _clip_retni(ClipMachineMemory, -17);
   }
   else
   {
      _check_error(ClipMachineMemory, src, 0);
   }

 end:
   if (src)
      free(src);
   if (dst)
      free(dst);
   return ret;
}
