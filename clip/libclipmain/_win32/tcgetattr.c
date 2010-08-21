int
tcgetattr(int fd, struct termios *termios_p)
{
   termios_p->c_iflag = 0;
   termios_p->c_oflag = 0;
   termios_p->c_cflag = 0;
   termios_p->c_lflag = 0;

   return 0;
}
