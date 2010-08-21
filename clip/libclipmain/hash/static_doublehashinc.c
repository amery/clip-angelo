static unsigned long
doublehashinc(unsigned long h, unsigned long s)
{
   unsigned long dh = (h / s) % s;

   return dh > 1 ? dh : 1;
}
