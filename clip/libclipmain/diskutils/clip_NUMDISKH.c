
/*
 * NUMDISKH() --> nNumber
 */
int
clip_NUMDISKH(ClipMachine * ClipMachineMemory)	/* Determines the number of hard disks */
{
   char hstr[3];

   int i = 0, n = 0;

   hstr[1] = ':';
   hstr[2] = 0;

   for (i = 'A'; i <= 'Z'; i++)
   {
      hstr[0] = i;
      if (_clip_fetch_item(ClipMachineMemory, _clip_hashstr(hstr)) == NULL)
	 continue;
      n++;
   }
   _clip_retni(ClipMachineMemory, n);
   return 0;
}
