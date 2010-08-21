/*
	TASKID() -> nID (-1 if no active tasks)
*/
int
clip_TASKID(ClipMachine * ClipMachineMemory)
{
#ifdef USE_TASKS
   _clip_retnl(ClipMachineMemory, Task_ID());
#endif
   return 0;
}
