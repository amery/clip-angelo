int
clip_GETENV(ClipMachine * ClipMachineMemory)
{
   ClipVar *rp;

   char *str = _clip_parc(ClipMachineMemory, 1);

   int i = 0;

   if (str)
   {
      _clip_retc(ClipMachineMemory, _clip_getenv(str));
   }
   else
   {
      rp = RETPTR(ClipMachineMemory);
      _clip_map(ClipMachineMemory, rp);
      while (_clip_envp[i])
      {
	 int l;

	 char *s = _clip_envp[i];

	 l = strcspn(s, "=");
	 _clip_mputc(ClipMachineMemory, rp, _clip_casehashbytes(0, s, l), s + l + 1, strlen(s + l + 1));
	 i++;
      }
   }
   return 0;
}
