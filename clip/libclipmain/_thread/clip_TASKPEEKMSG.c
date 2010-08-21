/*
	TaskPeekMsg() -> vMsg|NIL
	try peek message from task input queue
	If no messages available, immediate return NIL
*/
int
clip_TASKPEEKMSG(ClipMachine * ClipMachineMemory)
{
#ifdef USE_TASKS
   TaskMessage *tp;

   tp = Task_peekMessage();
   if (tp)
   {
      msg_data *m;

      m = (msg_data *) TaskMessage_get_data(tp);
      _clip_clone(ClipMachineMemory, RETPTR(ClipMachineMemory), &m->var);
      Task_respond(tp);
   }

#endif
   return 0;
}
