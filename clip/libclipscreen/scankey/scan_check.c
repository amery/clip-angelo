long
scan_check(void)
{
   long value = get_queue();

#ifdef DBG2
   printf("scan_check: %d\r\n", value);
#endif
   return value;
}
