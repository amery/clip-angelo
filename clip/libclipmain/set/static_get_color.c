static void
get_color(ClipMachine * ClipMachineMemory, char *buf, int buflen, int get_num)
{
   int no;

   char *s, *e;

   int l;

   for (no = 0, s = buf, e = buf + buflen - 1; s < e && no < 10; ++no)
   {
      int attr = 0;

      if (no)
	 *s++ = ',';

      switch (no)
      {
      case 0:
	 attr = ClipMachineMemory->attr.standard_of_ClipAttr;
	 break;
      case 1:
	 attr = ClipMachineMemory->attr.enhanced_of_ClipAttr;
	 break;
      case 2:
	 attr = ClipMachineMemory->attr.border_of_ClipAttr;
	 break;
      case 3:
	 attr = ClipMachineMemory->attr.background_of_ClipAttr;
	 break;
      case 4:
	 attr = ClipMachineMemory->attr.unselected_of_ClipAttr;
	 break;
      case 5:
	 attr = ClipMachineMemory->attr.u1_of_ClipAttr;
	 break;
      case 6:
	 attr = ClipMachineMemory->attr.u2_of_ClipAttr;
	 break;
      case 7:
	 attr = ClipMachineMemory->attr.u3_of_ClipAttr;
	 break;
      case 8:
	 attr = ClipMachineMemory->attr.u4_of_ClipAttr;
	 break;
      case 9:
	 attr = ClipMachineMemory->attr.u5_of_ClipAttr;
	 break;
      }

      l = _clip_attr2str(attr, s, e - s, get_num);

      s += l;
   }
   *s = 0;
}
