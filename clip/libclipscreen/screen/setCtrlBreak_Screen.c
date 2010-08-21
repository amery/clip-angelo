void
setCtrlBreak_Screen(Screen * scr, int val)
{
   ScreenData *dp = (ScreenData *) scr->base->data;

   if (scr_scan_mode)
      return;

#ifdef _WIN32
   if (w32_console)
      return;
#endif

   if (val)
   {
#if 0
      dp->work_mode.c_oflag |= OPOST;
#endif
      dp->work_mode.c_lflag |= ISIG;
      dp->work_mode.c_cc[VINTR] = 'C' - '@';
   }
   else
   {
#if 0
      dp->work_mode.c_oflag &= ~OPOST;
#endif
      dp->work_mode.c_lflag &= ~ISIG;
      dp->work_mode.c_cc[VINTR] = 0;
   }
   tcsetattr(dp->fd, TCSADRAIN, &dp->work_mode);
}
