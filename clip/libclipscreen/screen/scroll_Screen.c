void
scroll_Screen(Screen * scr, int beg, int end, int num, unsigned char attr)
{
   int columns = scr->base->Columns;

   scrollw_Screen(scr, beg, 0, end, columns - 1, num, attr);
}
