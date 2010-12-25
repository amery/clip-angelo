static void
callTaskRun(Task * t)
{
   if (t != sheduler)
   {
      t->result = t->run(t->data);	/*   call the main function ... */
      suicide(t);		/*   if it returns, the task wants to die */
   }
   else
      runSheduler(sheduler);	/*  or sheduler */
}
