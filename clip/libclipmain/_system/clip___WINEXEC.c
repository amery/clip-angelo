int
clip___WINEXEC(ClipMachine * ClipMachineMemory)
{
   char *com = _clip_parc(ClipMachineMemory, 1);

   if (com == NULL)
   {
      _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "invalid argument");
      _clip_retni(ClipMachineMemory, -1);
      return 1;
   }

   _clip_retni(ClipMachineMemory, WinExec(com, 1));

   return 0;
}
