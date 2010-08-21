/*
 * Handle dead key. Note that we now may have several
 * dead keys modifying the same character. Very useful
 * for Vietnamese.
 */
static void
do_dead2(unsigned char value, char up_flag)
{
   if (up_flag)
      return;

   diacr = (diacr ? handle_diacr(value) : value);
}
