void
newMatch_Key(ScreenBase * base)
{
   ScreenData *dp = (ScreenData *) base->data;

   dp->matchpos = 0;
   dp->matchno = 0;
   has_esc = 0;
}
