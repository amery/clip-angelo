int
setUtf8Mode_Screen(Screen * scr, int newmode)
{
   int r;

   ScreenData *dp;

   dp = ((ScreenData *) scr->base->data);
   r = dp->utf8_mode;
   dp->utf8_mode = newmode;

   return r;
}
