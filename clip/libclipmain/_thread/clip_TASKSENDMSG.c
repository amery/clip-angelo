/*
	TaskSendMsg( nReceiverID, vMsg [, lWaitProcessed] ) --> lResult
	send var vMsg to task with id nReceiverID
*/
int
clip_TASKSENDMSG(ClipMachine * ClipMachineMemory)
{
#ifdef USE_TASKS
   long receiver;

   ClipVar *vp;

   int wait = 0, r;

   TaskMessage *tp;

   if (ClipMachineMemory->argc < 2)
      return EG_ARG;

   receiver = _clip_parnl(ClipMachineMemory, 1);
   vp = _clip_par(ClipMachineMemory, 2);
   wait = _clip_parl(ClipMachineMemory, 3);
   tp = TaskMessage_new(HASH_VarMessage, msg_data_new(ClipMachineMemory, vp), msg_data_destroy);

   if (wait)
      r = Task_sendMessageWait(receiver, tp);
   else
      r = Task_sendMessage(receiver, tp);

   _clip_retl(ClipMachineMemory, r);

#endif
   return 0;
}
