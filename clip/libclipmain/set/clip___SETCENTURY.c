int
clip___SETCENTURY(ClipMachine * ClipMachineMemory)
{
   int c_flag = (ClipMachineMemory->flags & CENTURY_FLAG);

   char *s, *e, *new_format, ch = 'y';

   int i, j, pos[10];

   set_flag0(ClipMachineMemory, CENTURY_FLAG, 0);

   /* remake data of SET DATE FORMAT */
   new_format = calloc(1, 11);
   e = ClipMachineMemory->date_format + strlen(ClipMachineMemory->date_format);
   for (i = 0, s = ClipMachineMemory->date_format; s < e; s++)
   {
      if (*s != 'y' && *s != 'Y')
	 continue;
      if (*s == 'Y')
	 ch = 'Y';
      pos[i] = s - ClipMachineMemory->date_format;
      i++;
   }
   /* new CENTURY setting -> ON */
   if (ClipMachineMemory->flags & CENTURY_FLAG)
   {
      if (i != 2)
      {
	 _clip_retl(ClipMachineMemory, c_flag);
	 return 0;
      }
      strcpy(new_format, ClipMachineMemory->date_format);
      new_format[pos[1] + 1] = ch;
      new_format[pos[1] + 2] = ch;
      for (i = pos[1] + 3, s = ClipMachineMemory->date_format + pos[1] + 1; s < e; s++, i++)
	 new_format[i] = *s;
      new_format[i] = 0;
   }
   /* new CENTURY setting -> OFF */
   else
   {
      if (i < 4)
      {
	 _clip_retl(ClipMachineMemory, c_flag);
	 return 0;
      }
      for (i = 0, j = 0, s = ClipMachineMemory->date_format; s < e; s++, i++)
      {
	 if (i == pos[2] || i == pos[3])
	    continue;

	 new_format[j] = *s;
	 j++;
      }
      new_format[j] = 0;
   }

   if (ClipMachineMemory->date_format != NULL)
      free(ClipMachineMemory->date_format);

   ClipMachineMemory->date_format = new_format;
   _clip_retl(ClipMachineMemory, c_flag);
   return 0;
}
