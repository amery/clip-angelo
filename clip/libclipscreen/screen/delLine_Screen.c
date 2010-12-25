void
delLine_Screen(Screen * scr, int line, unsigned char attr)
{
   int i;

   int Lines = scr->base->Lines;

   int Columns = scr->base->Columns;

   if (line < 0)
      line = 0;
   if (line >= Lines)
      line = Lines - 1;

   for (i = line; i < Lines - 1; i++)
   {
      memcpy(scr->chars[i], scr->chars[i + 1], Columns);
      memcpy(scr->colors[i], scr->colors[i + 1], Columns);
      memcpy(scr->attrs[i], scr->attrs[i + 1], Columns);
      scr->lnums[i] = scr->lnums[i + 1];
      scr->touched[i] = 1;
   }

   scr->lnums[Lines - 1] = -1;
   scr->touched[Lines - 1] = 1;
   memset(scr->chars[Lines - 1], ' ', Columns);
   memset(scr->colors[Lines - 1], attr, Columns);
   memset(scr->attrs[Lines - 1], 0, Columns);
}
