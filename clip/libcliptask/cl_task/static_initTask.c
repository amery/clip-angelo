static void
initTask(Task * task)
{
   static jmp_buf stkswitch;

   if (task->isInited)
      return;

   task->isInited = 1;		/*   we are initialized */

   /*   switch to private stack now */

   if (!setjmp(stkswitch))
   {
      unsigned char *sp = ((unsigned char *) task->stack) + task->stacklen;

      sp -= WINDOWSIZE + SA(MINFRAME);
      sp = (unsigned char *) ((unsigned long) (sp) & 0xfffffff8);
#undef UNKNOWN_SYSTEM
#define UNKNOWN_SYSTEM
#ifdef __TURBOC__
      uchar far *farstk = (uchar far *) sp;

      stkswitch[0].j_ss = FP_SEG(farstk);
      stkswitch[0].j_sp = FP_OFF(farstk);
#undef UNKNOWN_SYSTEM
#endif
#ifdef OS_LINUX
#ifdef ARCH_S390
      stkswitch[0].__jmpbuf[0].gregs[9] = (unsigned int) (stack + stklen - sizeof(jmp_buf));
#else
      stkswitch[0].__jmpbuf[4] = (int) sp;
#endif
#undef UNKNOWN_SYSTEM
#endif
#ifdef OS_BSDI
      stkswitch[2] = (unsigned) (sp);
#undef UNKNOWN_SYSTEM
#endif
#ifdef OS_FREEBSD
      stkswitch[0]._jb[2] = (unsigned) (sp);
#undef UNKNOWN_SYSTEM
#endif
#ifdef OS_NETBSD
      ((unsigned *) &stkswitch)[2] = (unsigned) (sp);
#undef UNKNOWN_SYSTEM
#endif
#ifdef OS_OPENBSD
      ((unsigned *) &stkswitch)[2] = (unsigned) (sp);
#undef UNKNOWN_SYSTEM
#endif
#ifdef OS_SUNOS
      stkswitch[0].__fp = task->stack + task->stacklen - sizeof(jmp_buf);
#undef UNKNOWN_SYSTEM
#endif
#ifdef OS_SOLARIS_8
#if defined(sparc) || defined(__sparc)
      stkswitch[1] = (long) sp;
      stkswitch[2] = (long) &call_curr_task;
#undef UNKNOWN_SYSTEM
#elif defined(i386) || defined(__i386)
      stkswitch[4] = (long) sp;
      stkswitch[5] = (long) &call_curr_task;
#undef UNKNOWN_SYSTEM
#endif
#endif
#ifdef _WIN32
      stkswitch[7] = (unsigned) (sp);
#undef UNKNOWN_SYSTEM
#endif
#ifdef DJGPP
      stkswitch[0].__esp = (unsigned) (sp);
#undef UNKNOWN_SYSTEM
#endif

      /*  ((unsigned*)&stkswitch)[ SP_IN_JMPBUF_NO ] = (unsigned int)stack + stklen - sizeof(jmp_buf); */
#ifdef UNKNOWN_SYSTEM
#error Unknown System!
#endif

      longjmp(stkswitch, 1);
   }

   /*  now is in private stack, start task running ... */
   callTaskRun(currTask);
   /*   ... and never return */
}
