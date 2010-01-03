DBWorkArea *
get_area(ClipMachine * ClipMachineMemory, long area, int any, int *no)
{
   DBWorkArea *wa = NULL;

   int       n;


   if (no)
      *no = 0;

   if (!ClipMachineMemory->areas->count_of_ClipVect)
      return 0;

   if (area == 0)
    {
       for (n = 0; n < ClipMachineMemory->areas->count_of_ClipVect; n++)
	{
	   DBWorkArea *wa;

	   wa = (DBWorkArea *) ClipMachineMemory->areas->items_of_ClipVect[n];
	   if (wa == NULL || !wa->used)
	      break;
	}
       if (n == ClipMachineMemory->areas->count_of_ClipVect)
	  add_ClipVect(ClipMachineMemory->areas, NULL);
       if (no)
	  *no = n;
       return ClipMachineMemory->areas->items_of_ClipVect[n];
    }
   else if (area == -1)
      wa = (DBWorkArea *) ClipMachineMemory->areas->items_of_ClipVect[ClipMachineMemory->curArea];
   else
    {
       wa = (DBWorkArea *) HashTable_fetch(ClipMachineMemory->aliases, area);
       if (wa == NULL)
	{
	   char      buf[11];

	   long      tmp;

	   _clip_hash_name(ClipMachineMemory, area, buf, sizeof(buf));
	   buf[10] = 0;
	   tmp = _clip_hashstr(buf);
	   wa = (DBWorkArea *) HashTable_fetch(ClipMachineMemory->aliases, tmp);
	   if (wa)
	      area = tmp;
	}

       if (!wa)
	{
	   area -= 1;
	   if (area >= 0 && area < ClipMachineMemory->areas->count_of_ClipVect)
	      wa = (DBWorkArea *) ClipMachineMemory->areas->items_of_ClipVect[area];
	   else
	    {
	       int       i, l;

	       area += 1;
	       l = min(11, ClipMachineMemory->areas->count_of_ClipVect);
	       for (i = 0; i < l; i++)
		{
		   if (A_K_aliasHashes[i] == area)
		    {
		       wa = (DBWorkArea *) ClipMachineMemory->areas->items_of_ClipVect[i];
		       break;
		    }
		}
	    }
	}
    }

   if (!wa || (!wa->used && !any))
      return 0;

   if (no)
      *no = wa->no;
   return wa;
}
