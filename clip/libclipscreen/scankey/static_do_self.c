static void
do_self(unsigned char value, char up_flag)
{
#ifdef DBG2
   printf("do_self: %d\r\n", value);
#endif

   //printf("\nr=%d\n",r);

   if (up_flag)
      return;			/* no action, if this is a key release */

   if (shift_state & ((1 << KG_ALT) | (1 << KG_ALTGR)))
   {
      switch (value)
      {
      case '/':
	 break;
      case '[':
      case ']':
      case ';':
      case '\'':
      case ',':
      case '.':
	 break;
      case '-':
	 value = '_';
	 break;
      case '=':
      case '\\':
	 break;
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
	 break;
      default:
	 goto norm;
      }
      do_meta(value, up_flag);
      return;
   }
 norm:
   if (diacr)
      value = handle_diacr(value);

   if (dead_key_next)
   {
      dead_key_next = 0;
      diacr = value;
      return;
   }

   switch (value)
   {
   case 0x7f:
   case '2':
      put_acqueue(value);
      break;
   case 0x9:
      put_fqueue(value);
      break;
   default:
      put_queue(value);
      break;
   }
}
