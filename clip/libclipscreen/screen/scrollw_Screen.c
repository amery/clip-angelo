void
scrollw_Screen(Screen * scr, int beg, int left, int end, int right, int num, unsigned char attr)
{
   int i, dw;

   int Lines = scr->base->Lines;

   int Columns = scr->base->Columns;

   if (beg < 0)
      beg = 0;
   if (end >= Lines)
      end = Lines - 1;
   if (beg > end)
      return;
   if (left < 0)
      left = 0;
   if (right >= Columns)
      right = Columns - 1;
   dw = right - left + 1;

   if (num == 0)
      return;
   for (i = beg; i <= end; i++)
      scr->touched[i] = 1;
   if (num > 0)
   {
      for (i = beg + num; i <= end; i++)
      {
	 memcpy(scr->chars[i - num] + left, scr->chars[i] + left, dw);
	 memcpy(scr->colors[i - num] + left, scr->colors[i] + left, dw);
	 memcpy(scr->attrs[i - num] + left, scr->attrs[i] + left, dw);
	 scr->lnums[i - num] = scr->lnums[i];
	 /*scr->touched[i - num] = 1; */
      }
      if (num > end)
	 num = end;
      for (i = end - num + 1; i <= end; i++)
      {
	 memset(scr->chars[i] + left, ' ', dw);
	 memset(scr->colors[i] + left, attr, dw);
	 memset(scr->attrs[i] + left, 0, dw);
	 scr->lnums[i] = -1;
	 /*scr->touched[i] = 1; */
      }
   }
   else
   {
      for (i = end + num; i >= beg; i--)
      {
	 memcpy(scr->chars[i - num] + left, scr->chars[i] + left, dw);
	 memcpy(scr->colors[i - num] + left, scr->colors[i] + left, dw);
	 memcpy(scr->attrs[i - num] + left, scr->attrs[i] + left, dw);
	 scr->lnums[i - num] = scr->lnums[i];
	 /*scr->touched[i - num] = 1; */
      }
      if ((0 - num) > beg)
	 num = 0 - beg;
      for (i = beg - num - 1; i >= beg; i--)
      {
	 memset(scr->chars[i] + left, ' ', dw);
	 memset(scr->colors[i] + left, attr, dw);
	 memset(scr->attrs[i] + left, 0, dw);
	 scr->lnums[i] = -1;
	 /*scr->touched[i] = 1; */
      }
   }
}
