int
match_Key(ScreenBase * base, unsigned char b, unsigned long *keyp)
{
   if (scr_scan_mode)
      return scan_match_Key(base, b, keyp);
   else
      return term_match_Key(base, b, keyp);
}
