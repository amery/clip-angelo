static WinBuf *
find_WinBuf(int no)
{
   if (!winbufs)
      return 0;

	if (no >= 0 && no < winbufs->count_of_Coll)
   {
		return (WinBuf *) winbufs->items_of_Coll[no];
   }

   return 0;
}
