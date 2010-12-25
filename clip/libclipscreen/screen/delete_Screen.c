void
delete_Screen(Screen * scr)
{
   free(scr->mem);
   free(scr);
}
