static void
set_color(ClipMachine * ClipMachineMemory, char *str)
{
   char *s;

   int no = 0, attr, ls, l;

   if (strchr(str, 'X') || strchr(str, 'x'))
   {
      ClipMachineMemory->attr.standard_of_ClipAttr = 0;
      ClipMachineMemory->attr.enhanced_of_ClipAttr = 0;
      ClipMachineMemory->attr.border_of_ClipAttr = 0;
      ClipMachineMemory->attr.background_of_ClipAttr = 0;
      ClipMachineMemory->attr.unselected_of_ClipAttr = 0;
      ClipMachineMemory->attr.u1_of_ClipAttr = 0;
      ClipMachineMemory->attr.u2_of_ClipAttr = 0;
      ClipMachineMemory->attr.u3_of_ClipAttr = 0;
      ClipMachineMemory->attr.u4_of_ClipAttr = 0;
      ClipMachineMemory->attr.u5_of_ClipAttr = 0;
   }
   for (s = str; s && *s && no < 10; ++no)
   {
      ls = strspn(s, " \t\n\r");
      l = strcspn(s, ",");

      if (l != ls)
      {
	 attr = _clip_str2attr(s, l);
	 s += l;
	 if (*s)
	    ++s;
      }
      else
      {
	 s += l;
	 if (*s)
	    ++s;
	 continue;
      }

      switch (no)
      {
      case 0:
	 ClipMachineMemory->attr.standard_of_ClipAttr = attr;
	 break;
      case 1:
	 ClipMachineMemory->attr.enhanced_of_ClipAttr = attr;
	 ClipMachineMemory->attr.unselected_of_ClipAttr = attr;
	 break;
      case 2:
	 ClipMachineMemory->attr.border_of_ClipAttr = attr;
	 break;
      case 3:
	 ClipMachineMemory->attr.background_of_ClipAttr = attr;
	 break;
      case 4:
	 ClipMachineMemory->attr.unselected_of_ClipAttr = attr;
	 break;
      case 5:
	 ClipMachineMemory->attr.u1_of_ClipAttr = attr;
	 break;
      case 6:
	 ClipMachineMemory->attr.u2_of_ClipAttr = attr;
	 break;
      case 7:
	 ClipMachineMemory->attr.u3_of_ClipAttr = attr;
	 break;
      case 8:
	 ClipMachineMemory->attr.u4_of_ClipAttr = attr;
	 break;
      case 9:
	 ClipMachineMemory->attr.u5_of_ClipAttr = attr;
	 break;
      }
   }
}
