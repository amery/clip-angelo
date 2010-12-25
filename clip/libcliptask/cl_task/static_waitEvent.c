static void
waitEvent(int block)
{
   struct timeval tv;

   fd_set rfds, wfds, efds;

   int r, r1, n;

   long tim, dt;

   Task *tp;

   if (empty_List(&waitTasks))
      return;

   /*  check timeouts */
   tim = times(&tms_buf);

   n = 0;
   for (first_List(&waitTasks); (tp = (Task *) waitTasks.current);)
   {
      if (tp->wakeUp <= tim)
      {
	 remove_List(&waitTasks);
	 prepend_List(&readyTasks, tp);
	 tp->state = Task_ready;
	 tp->isTimed = 1;
	 if (tp->isRead)
	 {
	    FD_CLR_BY(&readFiles, &tp->rfileset);
	    tp->isRead = 0;
	 }
	 if (tp->isWrite)
	 {
	    FD_CLR_BY(&writeFiles, &tp->wfileset);
	    tp->isWrite = 0;
	 }
	 if (tp->isExcept)
	 {
	    FD_CLR_BY(&exceptFiles, &tp->efileset);
	    tp->isExcept = 0;
	 }
	 n = 1;
      }
      else
	 break;
   }

   if (empty_List(&waitTasks))
      return;

   if (n)
      block = 0;

   /*  check files */

   if (block)
   {
      long clk_tck = sysconf(_SC_CLK_TCK);	//CLK_TCK;

      dt = ((Task *) waitTasks.current)->wakeUp - tim + 1;
      tv.tv_sec = dt / clk_tck;
      tv.tv_usec = (dt % clk_tck) * (1000000 / clk_tck);
   }
   else
   {
      tv.tv_sec = 0;
      tv.tv_usec = 0;
   }

   rfds = readFiles;
   wfds = writeFiles;
   efds = exceptFiles;

   /*  main program waitpoint */
   r = t_select(FD_SETSIZE, &rfds, &wfds, &efds, &tv);
   if (r > 0)
   {
      for (n = first_List(&waitTasks); n && (tp = (Task *) waitTasks.current);)
      {
	 r1 = tp->isRead;
	 if (r1)
	    r1 = FD_ISSET_BY(&rfds, &(tp->rfileset));
	 if (!r1 && tp->isWrite)
	    r1 = FD_ISSET_BY(&wfds, &(tp->wfileset));
	 if (!r1 && tp->isExcept)
	    r1 = FD_ISSET_BY(&efds, &(tp->efileset));
	 if (r1)
	 {
	    remove_List(&waitTasks);
	    prepend_List(&readyTasks, tp);
	    tp->state = Task_ready;
	    n = !empty_List(&waitTasks);
	    FD_AND_BY(&(tp->rfileset), &rfds);
	    FD_AND_BY(&(tp->wfileset), &wfds);
	    FD_AND_BY(&(tp->efileset), &efds);
	    tp->isRead = 0;
	    tp->isWrite = 0;
	    tp->isExcept = 0;
	    continue;
	 }
	 n = next_List(&waitTasks);
      }
      FD_CLR_BY(&readFiles, &rfds);
      FD_CLR_BY(&writeFiles, &wfds);
      FD_CLR_BY(&exceptFiles, &efds);
   }

   if (block)
   {
      /*  again check timeouts */
      tim = times(&tms_buf);

      for (first_List(&waitTasks); (tp = (Task *) waitTasks.current);)
      {
	 if (tp->wakeUp <= tim)
	 {
	    remove_List(&waitTasks);
	    prepend_List(&readyTasks, tp);
	    tp->state = Task_ready;
	    tp->isTimed = 1;
	    if (tp->isRead)
	    {
	       FD_CLR_BY(&readFiles, &tp->rfileset);
	       tp->isRead = 0;
	    }
	    if (tp->isWrite)
	    {
	       FD_CLR_BY(&writeFiles, &tp->wfileset);
	       tp->isWrite = 0;
	    }
	    if (tp->isExcept)
	    {
	       FD_CLR_BY(&exceptFiles, &tp->efileset);
	       tp->isExcept = 0;
	    }
	 }
	 else
	    break;
      }

   }

}
