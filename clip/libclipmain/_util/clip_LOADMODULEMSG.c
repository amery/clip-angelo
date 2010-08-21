/*
	loadModuleMsg(cModule, cFilename_mo) -> bResult
*/
int
clip_LOADMODULEMSG(ClipMachine * ClipMachineMemory)
{
   char *module = _clip_parc(ClipMachineMemory, 1);

   char *filename = _clip_parc(ClipMachineMemory, 2);

   int r;

   if (!module || !filename)
      return EG_ARG;

   r = _clip_module_locale(module, filename);

   _clip_retl(ClipMachineMemory, !r);
   return 0;
}
