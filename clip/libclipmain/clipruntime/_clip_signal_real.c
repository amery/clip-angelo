void
_clip_signal_real(int sig)
{
   const char *msg;

   ClipFrame *fp;

   ClipMachine *ClipMachineMemory;

   switch (sig)
   {
   case 0:
      return;
#ifndef OS_MINGW
   case SIGHUP:
      msg = "SIGHUP";
      break;
   case SIGQUIT:
      msg = "SIGQUIT";
      break;
   case SIGKILL:
      msg = "SIGKILL";
      break;
   case SIGPIPE:
      msg = "SIGPIPE";
      break;
   case SIGALRM:
      msg = "SIGALRM";
      break;
#endif
   case SIGINT:
      msg = "SIGINT";
      break;
   case SIGILL:
      msg = "SIGILL";
      break;
   case SIGABRT:
      msg = "SIGABRT";
      break;
   case SIGFPE:
      msg = "SIGFPE";
      break;
   case SIGSEGV:
      msg = "SIGSEGV";
      break;
   case SIGTERM:
      msg = "SIGTERM";
      break;
   default:
      msg = "UNKNOWN";
      break;
   }
   ClipMachineMemory = first_mp;	/*cur_ClipMachine(); */
   _clip_sig_flag = 0;
   _clip_trap_err(ClipMachineMemory, EG_SIGNAL, 0, 0, "SIGNAL", sig, msg);
   _clip_call_errblock(ClipMachineMemory, EG_SIGNAL);

   _clip_logg(0, "got signal %s, exiting...", msg);
   fprintf(stderr, "\n%s: got signal %s, exiting...\n", _clip_progname, msg);
   fflush(stderr);

   for (fp = cur_mp->fp; fp; fp = fp->ClipFrame_up_of_ClipFrame)
   {
      _clip_logg(0, "trace: file '%s' line %d", fp->filename_of_ClipFrame, fp->line_of_ClipFrame);
      fprintf(stderr, "trace: file '%s' line %d\n", fp->filename_of_ClipFrame, fp->line_of_ClipFrame);
      fflush(stderr);
   }

/*_clip_exit();*/
   exit(100 + sig);
}
