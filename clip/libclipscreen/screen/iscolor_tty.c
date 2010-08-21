int
iscolor_tty(struct ScreenBase *base)
{
   ScreenData *dp = (ScreenData *) base->data;

   return (dp->termcap_Visuals & VisualColors) ? 1 : 0;
}
