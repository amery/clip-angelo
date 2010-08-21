/*
  winbuf_display(buf,top,left[,r_top,r_left[,r_height,r_width]]) -
	������ ���buf ������ ������top,left. �� ����	r_top,r_left,r_height,r_width - ������ � buf, ���� ����
	������ ���������. ���� r_top,r_left ����� �top,left
*/

int
clip_WINBUF_DISPLAY(ClipMachine * ClipMachineMemory)
{
   int no;

   WinBuf *wp;

   int top, left;

   int r_top, r_left, r_bottom, r_right;

   int d_top, d_left, d_hi, d_wi;

   int x, y, xi, yi;

   Screen *sp;

   if (ClipMachineMemory->argc < 3)
      return EG_ARG;

   no = _clip_parni(ClipMachineMemory, 1);
   wp = find_WinBuf(no);
   if (!wp)
      return EG_ARG;

   top = _clip_parni(ClipMachineMemory, 2);
   left = _clip_parni(ClipMachineMemory, 3);

   if (_clip_parinfo(ClipMachineMemory, 4) == NUMERIC_type_of_ClipVarType)
      d_top = _clip_parni(ClipMachineMemory, 4);
   else
      d_top = 0;
   if (d_top < 0)
      d_top = 0;

   if (_clip_parinfo(ClipMachineMemory, 5) == NUMERIC_type_of_ClipVarType)
      d_left = _clip_parni(ClipMachineMemory, 5);
   else
      d_left = 0;
   if (d_left < 0)
      d_left = 0;

   if (_clip_parinfo(ClipMachineMemory, 6) == NUMERIC_type_of_ClipVarType)
      d_hi = _clip_parni(ClipMachineMemory, 6);
   else
      d_hi = wp->hi;
   if (d_hi > wp->hi)
      d_hi = wp->hi;

   if (_clip_parinfo(ClipMachineMemory, 7) == NUMERIC_type_of_ClipVarType)
      d_wi = _clip_parni(ClipMachineMemory, 7);
   else
      d_wi = wp->wi;
   if (d_wi > wp->wi)
      d_wi = wp->wi;

   _clip_fullscreen(ClipMachineMemory);
   sp = ClipMachineMemory->screen;

   r_top = top;
   r_left = left;
   r_bottom = r_top + d_hi;
   r_right = r_left + d_wi;

   clip_region(ClipMachineMemory, &r_top, &r_left, &r_bottom, &r_right, 0, -1);

   for (y = top, yi = d_top; yi < d_hi; ++y, ++yi)
   {
      if (y < r_top || y > r_bottom)
	 continue;
      sp->touched[y] = 1;
      for (x = left, xi = d_left; xi < d_wi; ++x, ++xi)
      {
	 if (x < r_left || x > r_right)
	    continue;
	 sp->chars[y][x] = wp->chars[yi][xi];
	 sp->colors[y][x] = wp->colors[yi][xi];
	 sp->attrs[y][x] = wp->attrs[yi][xi];
      }
   }

   sync_mp(ClipMachineMemory);

   return 0;
}
