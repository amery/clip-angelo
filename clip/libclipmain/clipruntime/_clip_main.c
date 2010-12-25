CLIP_DLLEXPORT int
_clip_main(ClipMachine * ClipMachineMemory, long hash, int argc, char **argv, char **envp)
{
   int ret;

#ifdef USE_TASK
   if (ClipMachineMemory->main_func_level == 0)
      Task_INIT();
#endif
   ClipMachineMemory->main_func_level++;
   ret = do_main(ClipMachineMemory, hash, NULL, NULL, argc, argv, envp);
   ClipMachineMemory->main_func_level--;
   if (ClipMachineMemory->main_func_level == 0)
      delete_ClipMachine(ClipMachineMemory);
   return ret;
}
