static void
set_rawmode(int fd, int mode)
{
#ifdef OS_LINUX
   long cmd;

   if (mode)
   {
      /*cmd = K_RAW; */
      cmd = 0x00;
   }
   else
      /*cmd = K_XLATE; */
      cmd = 0x01;

   /*ioctl(fd, KDSKBMODE, cmd); */
   ioctl(fd, 0x4B45, cmd);
   write(1, "\033(K", 3);
#endif

}
