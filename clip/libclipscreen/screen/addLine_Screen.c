void
addLine_Screen(Screen * scr, int line, unsigned char attr)
{
   int i;

   int Lines = scr->base->Lines;

   int Columns = scr->base->Columns;

   if (line < 0)
      line = 0;
   if (line >= Lines)
      line = Lines - 1;

   for (i = Lines - 1; i > line; i--)
   {
      memcpy(scr->chars[i], scr->chars[i - 1], Columns);
      memcpy(scr->colors[i], scr->colors[i - 1], Columns);
      memcpy(scr->attrs[i], scr->attrs[i - 1], Columns);
      scr->lnums[i] = scr->lnums[i - 1];
      scr->touched[i] = 1;
   }

   scr->lnums[line] = -1;
   scr->touched[line] = 1;
   memset(scr->chars[line], ' ', Columns);
   memset(scr->colors[line], attr, Columns);
   memset(scr->attrs[line], 0, Columns);
}
