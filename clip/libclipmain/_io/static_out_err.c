static void
out_err(ClipMachine * ClipMachineMemory, char *buf, int n, int attr)
{
   int i;

   if (ClipMachineMemory->ebuf)
   {
      int i;

      for (i = 0; i < n; i++)
	 putByte_Buf(ClipMachineMemory->ebuf, _clip_outtbl[(unsigned char) buf[i]]);
      return;
   }

   for (i = 0; i < n; i++)
      fputc(_clip_outtbl[(unsigned char) buf[i]], stderr);
}
