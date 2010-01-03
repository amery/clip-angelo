void
_clip_sigint_real(int sig)
{
   ClipMachine *ClipMachineMemory;

   if (!debugging)
    {
       _clip_sig_flag = sig;
       _clip_signal_real(sig);
       return;
    }

   ClipMachineMemory = cur_ClipMachine();

   fprintf(_clip_dbg_out, "\nsigint: file %s line %d proc %s",
	   nullstr(ClipMachineMemory->fp->filename_of_ClipFrame), ClipMachineMemory->fp->line_of_ClipFrame,
	   nullstr(ClipMachineMemory->fp->procname_of_ClipFrame));
   fprintf(_clip_dbg_out, "\n.\n");
   fflush(_clip_dbg_out);
   _clip_logg(1, "sigint: file %s line %d proc %s",
	      nullstr(ClipMachineMemory->fp->filename_of_ClipFrame), ClipMachineMemory->fp->line_of_ClipFrame,
	      nullstr(ClipMachineMemory->fp->procname_of_ClipFrame));
   _clip_in_breakpoint();
}
