int
clip___QUIT(ClipMachine * ClipMachineMemory)
{	
#ifdef USE_TASKS
/*	Task_killAll();*/
#endif
   exit(errorlevel);
}
