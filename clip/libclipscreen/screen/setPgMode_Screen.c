int
setPgMode_Screen(Screen * scr, int newmode)
{
   int r;

   ScreenData *dp;

   dp = ((ScreenData *) scr->base->data);
   r = dp->pg_mode;
   dp->pg_mode = newmode;

   return r;
}
