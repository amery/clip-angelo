CLIP_DLLEXPORT int
_clip_main_func(ClipMachine * ClipMachineMemory, ClipFunction func, int argc, char **argv, char **envp)
{
   int ret;

   ClipMachineMemory->main_func_level++;
   ret = do_main(ClipMachineMemory, 0, func, NULL, argc, argv, envp);
   ClipMachineMemory->main_func_level--;
   if (ClipMachineMemory->main_func_level == 0)
      delete_ClipMachine(ClipMachineMemory);
   return ret;
}
