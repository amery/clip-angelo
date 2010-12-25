static int
key2clip(unsigned long key, int mask)
{
   int ckey = 0, flags;

   if (key & MOUSE_MASK)
   {
      int type, buttons;

      if (!(mask & (_CLIP_INKEY_MOVE | _CLIP_INKEY_LDOWN | _CLIP_INKEY_LUP | _CLIP_INKEY_RDOWN | _CLIP_INKEY_RUP | _CLIP_INKEY_MDOWN | _CLIP_INKEY_MUP)))
	 return 0;

      type = MOUSE_TYPE(key);
      buttons = MOUSE_BUTTONS(key);

      if (type & MOUSE_TYPE_DOUBLE)
      {
	 if (type & MOUSE_TYPE_DOWN)
	    return 0;
	 if ((mask & (_CLIP_INKEY_LUP | _CLIP_INKEY_LDOWN)) && (buttons & MOUSE_BUTTONS_LEFT))
	    return 1006 /* K_LDBLCLK */ ;
	 else if ((mask & (_CLIP_INKEY_RUP | _CLIP_INKEY_RDOWN)) && (buttons & MOUSE_BUTTONS_RIGHT))
	    return 1007 /* K_RDBLCLK */ ;
	 else if ((mask & (_CLIP_INKEY_MUP | _CLIP_INKEY_MDOWN)) && (buttons & MOUSE_BUTTONS_MIDDLE))
	    return 1010 /* K_MDBLCLK */ ;
	 else
	    return 0;
      }

      switch (type & (MOUSE_TYPE_MOVE | MOUSE_TYPE_DRAG | MOUSE_TYPE_DOWN | MOUSE_TYPE_UP))
      {
      case MOUSE_TYPE_MOVE:
      case MOUSE_TYPE_DRAG:
      case MOUSE_TYPE_MOVE | MOUSE_TYPE_DRAG:
	 if ((mask & _CLIP_INKEY_MOVE))
	    return 1001 /* K_MOUSEMOVE */ ;
	 else
	    return 0;
      case MOUSE_TYPE_DOWN:
	 if ((mask & _CLIP_INKEY_LDOWN) && (buttons & MOUSE_BUTTONS_LEFT))
	    return 1002 /* K_LBUTTONDOWN */ ;
	 else if ((mask & _CLIP_INKEY_RDOWN) && (buttons & MOUSE_BUTTONS_RIGHT))
	    return 1004 /* K_RBUTTONDOWN */ ;
	 else if ((mask & _CLIP_INKEY_MDOWN) && (buttons & MOUSE_BUTTONS_MIDDLE))
	    return 1008 /* K_MBUTTONDOWN */ ;
	 else
	    return 0;
      case MOUSE_TYPE_UP:
	 if ((mask & _CLIP_INKEY_LUP) && (buttons & MOUSE_BUTTONS_LEFT))
	    return 1003 /* K_LBUTTONUP */ ;
	 else if ((mask & _CLIP_INKEY_RUP) && (buttons & MOUSE_BUTTONS_RIGHT))
	    return 1005 /* K_RBUTTONUP */ ;
	 else if ((mask & _CLIP_INKEY_MUP) && (buttons & MOUSE_BUTTONS_MIDDLE))
	    return 1009 /* K_MBUTTONUP */ ;
	 else
	    return 0;
      }
      return 0;
   }

   if (!(mask & _CLIP_INKEY_KEYBOARD))
      return 0;

   switch (key)
   {
   case KEY_LEFT:
      ckey = 19;
      break;
   case KEY_RIGHT:
      ckey = 4;
      break;
   case KEY_UP:
      ckey = 5;
      break;
   case KEY_DOWN:
      ckey = 24;
      break;
   case KEY_PGDN:
      ckey = 3;
      break;
   case KEY_PGUP:
      ckey = 18;
      break;
   case KEY_HOME:
      ckey = 1;
      break;
   case KEY_END:
      ckey = 6;
      break;
   case KEY_INS:
      ckey = 22;
      break;
   case KEY_BS:
   case 8:
   case 0x7f:
      ckey = 8;
      break;
   case KEY_DEL:
      ckey = 7;
      break;
   case KEY_ENTER:
      ckey = 13;
      break;
   case KEY_ESC:
      ckey = 27;
      break;

   case KEY_F1:
      ckey = 28;
      break;
   case KEY_F2:
      ckey = -1;
      break;
   case KEY_F3:
      ckey = -2;
      break;
   case KEY_F4:
      ckey = -3;
      break;
   case KEY_F5:
      ckey = -4;
      break;
   case KEY_F6:
      ckey = -5;
      break;
   case KEY_F7:
      ckey = -6;
      break;
   case KEY_F8:
      ckey = -7;
      break;
   case KEY_F9:
      ckey = -8;
      break;
   case KEY_F10:
      ckey = -9;
      break;

   case KEY_F11:
      ckey = -40;
      break;
   case KEY_F12:
      ckey = -41;
      break;

   case KEY_F13:
      ckey = -12;
      break;
   case KEY_F14:
      ckey = -13;
      break;
   case KEY_F15:
      ckey = -14;
      break;
   case KEY_F16:
      ckey = -15;
      break;
   case KEY_F17:
      ckey = -16;
      break;
   case KEY_F18:
      ckey = -17;
      break;
   case KEY_F19:
      ckey = -18;
      break;
   case KEY_F20:
      ckey = -19;
      break;

   case META1_MASK | KEY_F11:
      ckey = -46;
      break;
   case META1_MASK | KEY_F12:
      ckey = -47;
      break;
   case META2_MASK | KEY_F11:
      ckey = -44;
      break;
   case META2_MASK | KEY_F12:
      ckey = -45;
      break;
   case META1_MASK | META2_MASK | KEY_F11:
      ckey = -42;
      break;
   case META1_MASK | META2_MASK | KEY_F12:
      ckey = -43;
      break;

   case META1_MASK | KEY_UP:
      ckey = 408;
      break;
   case META1_MASK | KEY_DOWN:
      ckey = 416;
      break;
   case META1_MASK | KEY_LEFT:
      ckey = 411;
      break;
   case META1_MASK | KEY_RIGHT:
      ckey = 413;
      break;
   case META1_MASK | KEY_HOME:
      ckey = 407;
      break;
   case META1_MASK | KEY_END:
      ckey = 415;
      break;
   case META1_MASK | KEY_PGUP:
      ckey = 409;
      break;
   case META1_MASK | KEY_PGDN:
      ckey = 417;
      break;

   case META2_MASK | KEY_UP:
      ckey = 397;
      break;
   case META2_MASK | KEY_DOWN:
      ckey = 401;
      break;
   case META2_MASK | KEY_LEFT:
      ckey = 26;
      break;
   case META2_MASK | KEY_RIGHT:
      ckey = 2;
      break;
   case META2_MASK | KEY_HOME:
      ckey = 29;
      break;
   case META2_MASK | KEY_END:
      ckey = 23;
      break;
   case META2_MASK | KEY_PGUP:
      ckey = 31;
      break;
   case META2_MASK | KEY_PGDN:
      ckey = 30;
      break;

   case META1_MASK | '=':
      ckey = 387;
      break;

   case META1_MASK | '_':
      ckey = 386;
      break;

   case META1_MASK | '\\':
      ckey = 299;
      break;

   case META1_MASK | '[':
      ckey = 282;
      break;

   case META1_MASK | ']':
      ckey = 283;
      break;

   case META1_MASK | ';':
      ckey = 295;
      break;

   case META1_MASK | '\'':
      ckey = 296;
      break;

   case META1_MASK | ',':
      ckey = 307;
      break;

   case META1_MASK | '.':
      ckey = 308;
      break;

   case META1_MASK | '/':
      /*ckey = 309; */
      ckey = 420;
      break;

      /* shift-Tab */
   case META1_MASK | META2_MASK | 9:
      ckey = 271;
      break;

   case META1_MASK | KEY_INS:
      ckey = 418;
      break;
   case META1_MASK | KEY_DEL:
      ckey = 419;
      break;
   case META1_MASK | KEY_BS:
   case META1_MASK | 0x7f:
      ckey = 270;
      break;
   case META1_MASK | 9:
      ckey = 421;
      break;

   case META2_MASK | KEY_INS:
      ckey = 402;
      break;
   case META2_MASK | KEY_DEL:
      ckey = 403;
      break;
   case META2_MASK | KEY_BS:
   case META2_MASK | 0x7f:
      ckey = 127;
      break;
   case META2_MASK | 9:
      ckey = 404;
      break;

   case META1_MASK | KEY_ENTER:
      ckey = 422;
      break;
   case META1_MASK | KEY_ESC:
      ckey = 257;
      break;

   case META2_MASK | KEY_ENTER:
      ckey = 10;
      break;

      /*case META1_MASK | '/':
         ckey = 420;
         break; */
   case META1_MASK | '*':
      ckey = 311;
      break;
   case META1_MASK | '-':
      ckey = 330;
      break;
   case META1_MASK | '+':
      ckey = 334;
      break;

   case META2_MASK | '/':
      ckey = 405;
      break;
   case META2_MASK | '*':
      ckey = 406;
      break;
   case META2_MASK | '-':
      ckey = 398;
      break;
   case META2_MASK | '+':
      ckey = 400;
      break;

   case META2_MASK | '2':
      ckey = 259;
      break;

   default:
      if (key & (META_MASK | META1_MASK | META2_MASK))
      {
	 switch (key & ~(META1_MASK | META2_MASK))
	 {
	 case KEY_F1:
	 case KEY_F2:
	 case KEY_F3:
	 case KEY_F4:
	 case KEY_F5:
	 case KEY_F6:
	 case KEY_F7:
	 case KEY_F8:
	 case KEY_F9:
	 case KEY_F10:
	    switch (key & ~(META1_MASK | META2_MASK))
	    {
	    case KEY_F1:
	       ckey = -10;
	       break;
	    case KEY_F2:
	       ckey = -11;
	       break;
	    case KEY_F3:
	       ckey = -12;
	       break;
	    case KEY_F4:
	       ckey = -13;
	       break;
	    case KEY_F5:
	       ckey = -14;
	       break;
	    case KEY_F6:
	       ckey = -15;
	       break;
	    case KEY_F7:
	       ckey = -16;
	       break;
	    case KEY_F8:
	       ckey = -17;
	       break;
	    case KEY_F9:
	       ckey = -18;
	       break;
	    case KEY_F10:
	       ckey = -19;
	       break;
	    }
	    switch (key & (META1_MASK | META2_MASK))
	    {
	    case META2_MASK:
	       ckey -= 10;
	       break;
	    case META1_MASK:
	       ckey -= 20;
	       break;
	    }
	    break;
	 default:
	    flags = key & ~(META1_MASK | META2_MASK);
	    if (flags < 27)
	       ckey = altkeys[flags - 1];
	    else if (flags == '`' || flags == '~')
	       ckey = 297;
	    else if (flags >= '0' && flags <= '9')
	       ckey = altnums[flags - '0'];
	    else if (flags >= 'A' && flags <= 'Z')
	       ckey = altkeys[flags - 'A'];
	    else if (flags >= 'a' && flags <= 'z')
	       ckey = altkeys[flags - 'a'];
	    else if (key == (0x10000 | META2_MASK))
	    {
	       ckey = 'J' - '@';
	       break;
	    }
	    else
	    {
	       ckey = key & ~(META_MASK | META1_MASK | META2_MASK);
	    }

#if 0
	    switch (key & (META1_MASK | META2_MASK))
	    {
	    case META1_MASK:
	       ckey += 256;
	       break;
	    case META2_MASK:
	       ckey += 512;
	       break;
	    case META1_MASK | META2_MASK:
	       ckey += 768;
	       break;
	    }
#endif

	    break;
	 }
      }
      else
	 ckey = key;
      break;
   }

   return ckey;
}
