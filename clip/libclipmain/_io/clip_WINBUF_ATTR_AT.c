/*
winbuf_attr_at(buf,top,left,bottom,right,cAttr)
���� �������top,left,bottom,right ����� cAttr ���� buf.
*/

int
clip_WINBUF_ATTR_AT(ClipMachine * ClipMachineMemory)
{
   int no;

   WinBuf *wp;

   int top, left, bottom, right;

   ClipVar *vp;

   int attr;

   int y, x;

   if (ClipMachineMemory->argc < 5)
      return EG_ARG;

   no = _clip_parni(ClipMachineMemory, 1);
   wp = find_WinBuf(no);

   if (!wp)
      return EG_ARG;

   top = _clip_parni(ClipMachineMemory, 2);
   left = _clip_parni(ClipMachineMemory, 3);
   bottom = _clip_parni(ClipMachineMemory, 4);
   right = _clip_parni(ClipMachineMemory, 5);

   vp = _clip_par(ClipMachineMemory, 6);
   if (vp)
   {
      if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
	 attr = _clip_parni(ClipMachineMemory, 6);
      else
	 attr = get_color(ClipMachineMemory, _clip_parc(ClipMachineMemory, 6));
   }
   else
      attr = _clip_colorSelect(ClipMachineMemory);

   if (top < 0)
      top = 0;
   if (left < 0)
      left = 0;

   for (y = top; y <= bottom && y < wp->hi; y++)
   {
      for (x = left; x <= right && x < wp->wi; x++)
      {
	 wp->colors[y][x] = attr;
      }
   }

   return 0;
}
