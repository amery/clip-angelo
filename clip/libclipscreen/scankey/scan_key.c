long
scan_key(unsigned char scancode)
{
   scan_push(scancode);
   return get_queue();
}
