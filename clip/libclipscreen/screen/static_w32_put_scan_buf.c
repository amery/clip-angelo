static void
w32_put_scan_buf(unsigned char b)
{
   if (w32_scan_buf_len == w32_scan_buf_size)
   {
      int size = w32_scan_buf_size * 2;

      if (size < 8)
	 size = 8;
      w32_scan_buf_size = size;
      w32_scan_buf = (unsigned char *) realloc(w32_scan_buf, size);
   }

   w32_scan_buf[w32_scan_buf_len] = b;
   w32_scan_buf_len++;
}
