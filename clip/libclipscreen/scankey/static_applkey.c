static void
applkey(int key, char mode)
{
   static char buf[] = { 0x1b, 'O', 0x00, 0x00 };

   buf[1] = (mode ? 'O' : '[');
   buf[2] = key;
   puts_queue(buf);
}
