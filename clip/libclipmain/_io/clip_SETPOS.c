int
clip_SETPOS(ClipMachine * ClipMachineMemory)
{
   int y = _clip_parnl(ClipMachineMemory, 1);

   int x = _clip_parnl(ClipMachineMemory, 2);

   int scr = _clip_parl(ClipMachineMemory, 3);

   if (ClipMachineMemory->flags & DEVICE_FLAG)
   {
      int i;

      FILE *printer = (FILE *) ClipMachineMemory->printer;

      if (!printer)
	 goto norm;

      if (ClipMachineMemory->prow > y)
	 eject(ClipMachineMemory);

      if (ClipMachineMemory->prow < y)
      {
	 char *pEol = (char *) _clip_fetch_item(ClipMachineMemory,
						_clip_hashstr("PRINTER_EOL"));

	 while (ClipMachineMemory->prow < y)
	 {
	    ClipMachineMemory->pcol = 0;
	    ClipMachineMemory->prow++;
	    if (pEol == NULL || *pEol == 0)
	       fputc('\n', printer);
	    else
	       fputs(pEol, printer);

	 }
	 for (i = 0; i < ClipMachineMemory->margin; ++i)
	    fputc(' ', printer);
      }

      if (ClipMachineMemory->pcol > x)
      {
	 fputc('\r', printer);
	 ClipMachineMemory->pcol = 0;
	 for (i = 0; i < ClipMachineMemory->margin; ++i)
	    fputc(' ', printer);
      }

      while (ClipMachineMemory->pcol < x)
      {
	 fputc(' ', printer);
	 ClipMachineMemory->pcol++;
      }
   }

 norm:
   _clip_fullscreen(ClipMachineMemory);

   if (scr)
   {
      if (x < 0)
	 x = 0;
      if (x >= ClipMachineMemory->screen->base->Columns)
	 x = ClipMachineMemory->screen->base->Columns - 1;
      if (y < 0)
	 y = 0;
      if (y >= ClipMachineMemory->screen->base->Lines)
	 y = ClipMachineMemory->screen->base->Lines - 1;
   }
   else
      clip_region(ClipMachineMemory, &y, &x, 0, 0, 1, -1);

   ClipMachineMemory->screen->y = y;
   ClipMachineMemory->screen->x = x;
   sync_mp(ClipMachineMemory);

   return 0;
}
