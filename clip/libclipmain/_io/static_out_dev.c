static void
out_dev(ClipMachine * ClipMachineMemory, char *buf, int n, int attr, int wrap)
{
   if (ClipMachineMemory->flags & DEVICE_FLAG)
      goto prn;

   if (ClipMachineMemory->fullscreen)
   {
      if (wrap == 0 || (ClipMachineMemory->flags & CONSOLE_FLAG))
	 out_scr(ClipMachineMemory, buf, n, attr, wrap);
   }
   else if ((ClipMachineMemory->flags & CONSOLE_FLAG) || ClipMachineMemory->out != stdout)
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
	 fputc(_clip_outtbl[(unsigned char) buf[i]], (FILE *) ClipMachineMemory->out);
   }

   if (wrap == 1 && (ClipMachineMemory->flags & ALTERNATE_FLAG))
   {
      FILE *alt = (FILE *) ClipMachineMemory->alt;

      if (alt)
      {
	 int i;

	 for (i = 0; i < n; i++)
	    fputc(_clip_outtbl[(unsigned char) buf[i]], alt);
      }
   }

   if (wrap == 1 && (ClipMachineMemory->flags & EXTRA_FLAG))
   {
      FILE *extra = (FILE *) ClipMachineMemory->extra;

      if (extra)
      {
	 int i;

	 for (i = 0; i < n; i++)
	    fputc(_clip_outtbl[(unsigned char) buf[i]], extra);
      }
   }

   if ((ClipMachineMemory->flags & PRINTER_FLAG) && wrap)
   {
      FILE *printer;

      int i;

      char *s;

    prn:
      if (ClipMachineMemory->pbuf)
      {
	 putBuf_Buf(ClipMachineMemory->pbuf, buf, n);
      }
      else
      {
	 printer = (FILE *) ClipMachineMemory->printer;
	 if (printer)
	 {
	    for (i = 0, s = buf; i < n; ++i, ++s)
	    {
	       if (*s == '\n')
	       {
		  ClipMachineMemory->pcol = 0;
		  ClipMachineMemory->prow++;
	       }
	       else
	       {
		  if (*s != '\r')
		     ++ClipMachineMemory->pcol;
	       }
	    }
	    for (i = 0; i < ClipMachineMemory->margin; ++i)
	       fputc(' ', printer);
	    for (i = 0; i < n; i++)
	       fputc(ClipMachineMemory->prntbl[(ClipMachineMemory->prn_xlat[(unsigned char) (buf[i])])], printer);
	 }
      }
   }
}
