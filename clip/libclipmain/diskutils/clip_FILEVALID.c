int
clip_FILEVALID(ClipMachine * ClipMachineMemory)	/* Tests whether a string has a valid file name */
{
   char *dname = _clip_parc(ClipMachineMemory, 1);

   if (dname == NULL && strlen(dname) > 64)
      _clip_retl(ClipMachineMemory, 0);
   else
      _clip_retl(ClipMachineMemory, 1);
   return 0;
}
