/* Obsolete - for backwards compatibility only */
static void
do_dead(unsigned char value, char up_flag)
{
   value = ret_diacr[value];
   do_dead2(value, up_flag);
}
