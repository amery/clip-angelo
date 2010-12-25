
/*
 *    ������-> ����� */
CLIP_DLLEXPORT long
_clip_jdate(int dd, int mm, int yy)
{
   int a, b;

   b = 0;
   if (mm <= 2)
   {
      yy--;
      mm += 12;
   }
   if ((yy * 10000.0 + mm * 100.0 + dd) >= 15821015.0)
   {
      a = yy / 100;
      b = 2 - a + a / 4;
   }
   return ((long) (365.25 * yy - (yy ? 0.0 : 0.75)) + (long) (30.6001 * (mm + 1) + dd + 1720995 + b));
}
