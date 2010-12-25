static void
out_std(ClipMachine * ClipMachineMemory, char *buf, int n, int attr)
{
   int i;

   if (ClipMachineMemory->obuf)
   {
      int i;

      for (i = 0; i < n; i++)
	 putByte_Buf(ClipMachineMemory->obuf, _clip_outtbl[(unsigned char) buf[i]]);
      return;
   }

   for (i = 0; i < n; i++)
      fputc(_clip_outtbl[(unsigned char) buf[i]], stdout);
}
