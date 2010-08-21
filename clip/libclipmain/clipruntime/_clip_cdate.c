/*
 *    �����-> ������ *    ww - �������� (0 - �� 1 - �����)
 */
CLIP_DLLEXPORT void
_clip_cdate(long ju, int *dd, int *mm, int *yy, int *ww)
{
   long alf, a, b, c, d, e;

   alf = (long) ((ju - 1867216.25) / 36524.25);
   a = ju < 2299161L ? ju : ju + 1L + alf - alf / 4;
   b = a + 1524;
   c = (long) ((b - 122.1) / 365.25);
   d = (long) (365.25 * c);
   e = (long) ((b - d) / 30.6001);
   *dd = b - d - (long) (30.6001 * e);
   if (e < 14)
      *mm = e - 1;
   else
      *mm = e - 13;
   if (*mm > 2)
      *yy = c - 4716;
   else
      *yy = c - 4715;
   *ww = (ju + 1) % 7L;
   if (*yy <= 0)
      *yy = 1;
}
