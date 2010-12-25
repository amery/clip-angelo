int
clip_FACCESS(ClipMachine * ClipMachineMemory)
{
   int ret = 0, mode = 0;

   char *uname = _get_unix_name(ClipMachineMemory, _clip_parc(ClipMachineMemory, 1));

   char *m = _clip_parc(ClipMachineMemory, 2);

   if (uname != NULL && m != NULL)
   {
      while (*m)
      {
	 switch (*m++)
	 {
	 case 'r':
	    mode |= R_OK;
	    break;
	 case 'w':
	    mode |= W_OK;
	    break;
	 case 'x':
	    mode |= X_OK;
	    break;
	 case 'f':
	    mode |= F_OK;
	    break;
	 }
      }
      ret = access(uname, mode) ? 0 : 1;
   }
   _clip_retl(ClipMachineMemory, ret);
   return 0;
}
