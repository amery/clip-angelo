int
read_tcapbuf(char *buf, Terminfo * info, char *Errbuf, int Errbuflen)
{
   errbuf = Errbuf;
   errbuflen = Errbuflen;
   errcode = 0;

   split_entry(info, buf);

   return 0;
}
