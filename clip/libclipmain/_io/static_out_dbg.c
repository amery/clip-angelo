static void
out_dbg(ClipMachine * ClipMachineMemory, char *buf, int n)
{
   if (_clip_dbg_out)
   {
      int i;

      for (i = 0; i < n; i++)
	 fputc(_clip_outtbl[(unsigned char) buf[i]], _clip_dbg_out);
   }
}
