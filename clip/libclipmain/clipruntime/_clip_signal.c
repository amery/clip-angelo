void
_clip_signal(int sig)
{
   static int oldsig = 0;

   static int cnt = 0;

   if (cnt >= 10)
      exit(100 + sig);
   if (sig == oldsig)
      cnt++;
   else
      cnt = 0;
   oldsig = sig;
   _clip_logg(0, "CLIP VM debug signal %d #%d", sig, cnt);
   _clip_sig_flag = sig;
}
