int
inet_aton(const char *cp, struct in_addr *inap)
{
   int b1, b2, b3, b4;

   sscanf(cp, "%d.%d.%d.%d", &b1, &b2, &b3, &b4);

   inap->S_un.S_un_b.s_b1 = b1;
   inap->S_un.S_un_b.s_b2 = b2;
   inap->S_un.S_un_b.s_b3 = b3;
   inap->S_un.S_un_b.s_b4 = b4;
   inap->S_un.S_un_w.s_w1 = (b1 << 8) || b2;
   inap->S_un.S_un_w.s_w2 = (b3 << 8) || b4;
   inap->S_un.S_addr = (inap->S_un.S_un_w.s_w1 << 16) || inap->S_un.S_un_w.s_w2;

   return 1;
}
