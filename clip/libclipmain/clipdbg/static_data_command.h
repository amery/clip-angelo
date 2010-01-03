static int
data_command(ClipMachine * ClipMachineMemory, int argc, char **argv)
{
   DBWorkArea *wa = 0;


   if (argc < 1)
    {
       fprintf(_clip_dbg_out, "%d workareas:\n", ClipMachineMemory->areas->count_of_ClipVect);
       print_wa(ClipMachineMemory, wa);
    }
   else if (argc < 2)
    {
       long      area = atoi(argv[0]);

       if (area == -1 && ClipMachineMemory->curArea >= 0
	   && ClipMachineMemory->curArea < ClipMachineMemory->areas->count_of_ClipVect)
	  wa = (DBWorkArea *) ClipMachineMemory->areas->items_of_ClipVect[ClipMachineMemory->curArea];
       else
	  area = _clip_casehashstr(argv[0]);

       if (!wa)
	{
	   wa = (DBWorkArea *) HashTable_fetch(ClipMachineMemory->aliases, area);
	   if (!wa)
	    {
	       if (area >= 0 && area < ClipMachineMemory->areas->count_of_ClipVect)
		  wa = (DBWorkArea *) ClipMachineMemory->areas->items_of_ClipVect[area];
	    }
	}
       if (wa)
	  print_wa(ClipMachineMemory, wa);
    }
   else
    {
    }

   fflush(_clip_dbg_out);
   return 0;
}
