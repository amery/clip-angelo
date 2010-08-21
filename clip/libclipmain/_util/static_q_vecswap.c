static void
q_vecswap(ClipVar * pi, ClipVar * pj, int i)
{
   while (i > 0)
   {
      ClipVar t = *pi;

      *pi++ = *pj;
      *pj++ = t;
      i--;
   }
}
