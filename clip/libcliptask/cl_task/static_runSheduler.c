static void
runSheduler(Task * task)
{
   int code;

#if 0
   signal(SIGPIPE, SIG_IGN);
#ifdef DJGPP
   signal(SIGINT, SIG_IGN);
#endif
#endif
   /* first time all what we need is set shedEnv for future */
   if (!(code = setjmp(shedEnv)))
      longjmp(mainEnv, 1);

   /*  destroy all zombie tasks */
#if 1
   while (first_List(&zombieTasks))
   {
      Task *tp = (Task *) zombieTasks.current;

      removeFromList(tp);
      Task_delete(tp);
   }
#if 0
   while (!zombieTasks.empty())
   {
      zombieTasks.first();
      ref Task tp = zombieTasks.get_current();

      zombieTasks.remove();
      hashs.remove(tp.hash());
      allTasks.get(tp);
   }
#endif
#endif

   while (code > 0)
   {
      if (Next_List(&readyTasks))
	 waitEvent(0);
      else if (empty_List(&waitTasks))
      {
	 /*  deathmatch? - no active and no wait tasks; */
	 /*  message-queued tasks cannot be self-activated */
	 deathMatch();
	 continue;
      }
      else
      {
	 waitEvent(1);
	 continue;
      }
      /*  take task from ready queue... */
      currTask = (Task *) readyTasks.current;
      /*  ... and switch to one's context */
      resumeTask(currTask, 1);
   }

   /*  never reached point! */
   currTask = 0;
   longjmp(mainEnv, 1);
}
