/*
  winbuf_copy(scr_buf,top1,left1,height1,width1,dest_buf,top2,left2) -
  ����� ������ scr_buf, ������ ������
  top1,left1,height1,width1 �dest_buf � ������top2,left2
*/
int
clip_WINBUF_COPY(ClipMachine * ClipMachineMemory)
{
   int no;

   WinBuf *wp, *d_wp;

   int top, left, hi, wi, d_top, d_left;

   int x, y, d_y, d_x;

   if (ClipMachineMemory->argc < 6)
      return EG_ARG;

   no = _clip_parni(ClipMachineMemory, 1);
   wp = find_WinBuf(no);
   if (!wp)
      return EG_ARG;

   no = _clip_parni(ClipMachineMemory, 6);
   d_wp = find_WinBuf(no);
   if (!d_wp)
      return EG_ARG;

   top = _clip_parni(ClipMachineMemory, 2);
   left = _clip_parni(ClipMachineMemory, 3);

   if (_clip_parinfo(ClipMachineMemory, 4) == NUMERIC_type_of_ClipVarType)
      hi = _clip_parni(ClipMachineMemory, 4);
   else
      hi = wp->hi;
   if (hi > wp->hi)
      hi = wp->hi;

   if (_clip_parinfo(ClipMachineMemory, 5) == NUMERIC_type_of_ClipVarType)
      wi = _clip_parni(ClipMachineMemory, 5);
   else
      wi = wp->wi;
   if (wi > wp->wi)
      wi = wp->wi;

   d_top = _clip_parni(ClipMachineMemory, 7);
   d_left = _clip_parni(ClipMachineMemory, 8);

   for (y = top, d_y = d_top; y < hi && d_y < d_wp->hi; y++, d_y++)
   {
      if (y < 0 || d_y < 0)
	 continue;
      for (x = left, d_x = d_left; x < wi && d_x < d_wp->wi; x++, d_x++)
      {
	 if (x < 0 || d_x < 0)
	    continue;
	 d_wp->chars[d_y][d_x] = wp->chars[y][x];
	 d_wp->colors[d_y][d_x] = wp->colors[y][x];
	 d_wp->attrs[d_y][d_x] = wp->attrs[y][x];
      }
   }

   return 0;
}
