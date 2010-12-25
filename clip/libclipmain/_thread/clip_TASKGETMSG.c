/*
	TaskGetMsg() -> vMsg
	get message from task input queue
	If no messages available, wait forever
*/
int
clip_TASKGETMSG(ClipMachine * ClipMachineMemory)
{
#ifdef USE_TASKS
   TaskMessage *tp;

   tp = Task_getMessage();
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
