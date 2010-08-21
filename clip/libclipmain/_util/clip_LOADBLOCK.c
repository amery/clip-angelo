/* LOADBLOCK(pobj-name) -> codeblock */
int
clip_LOADBLOCK(ClipMachine * ClipMachineMemory)
{
   char *name = _clip_parc(ClipMachineMemory, 1);

   int r;

   if (!name)
   {
      _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "no name given");
      return 1;
   }

   r = _clip_load(ClipMachineMemory, name, 0, RETPTR(ClipMachineMemory));

   return r;
}
