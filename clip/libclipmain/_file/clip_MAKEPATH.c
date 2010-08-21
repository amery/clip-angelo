int
clip_MAKEPATH(ClipMachine * ClipMachineMemory)
{
   char *path = _clip_parc(ClipMachineMemory, 1);

   if (path)
   {
      char buf[PATH_MAX];

      _clip_translate_path(ClipMachineMemory, path, buf, sizeof(buf));
      _clip_retc(ClipMachineMemory, buf);
   }
   return 0;
}
