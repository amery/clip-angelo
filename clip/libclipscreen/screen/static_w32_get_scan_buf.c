static unsigned char
w32_get_scan_buf(void)
{
   unsigned char r = 0;

   if (w32_scan_buf_len)
   {
      r = w32_scan_buf[0];
      --w32_scan_buf_len;
      if (w32_scan_buf_len)
	 memmove(w32_scan_buf, w32_scan_buf + 1, w32_scan_buf_len);
   }
   return r;
}
