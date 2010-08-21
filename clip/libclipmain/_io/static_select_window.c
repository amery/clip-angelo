static int
select_window(ClipMachine * ClipMachineMemory, int n)
{
   int no = -1;

   ClipWindow *wp = 0;

   int ono = -1, nno, i;

   for (i = 0; i <= ClipMachineMemory->wnum; i++)
   {
      wp = ClipMachineMemory->windows + i;
      if (wp->no_of_ClipWindow == n)
      {
	 ono = i;
	 break;
      }
   }
   if (ono != -1)
   {
      ClipWindow w;

      nno = ClipMachineMemory->wnum;

      if ((ClipMachineMemory->windows + nno)->no_of_ClipWindow != 0)
	 save_window(ClipMachineMemory, ClipMachineMemory->windows + nno);
      if (nno != ono)
      {
	 w = ClipMachineMemory->windows[ono];
	 for (i = ono; i < ClipMachineMemory->wnum; i++)
	    ClipMachineMemory->windows[i] = ClipMachineMemory->windows[i + 1];
	 ClipMachineMemory->windows[ClipMachineMemory->wnum] = w;
      }
      if (n != 0)
	 rest_window(ClipMachineMemory, ClipMachineMemory->windows + ClipMachineMemory->wnum);

      sync_mp(ClipMachineMemory);
   }

   return no;
}
