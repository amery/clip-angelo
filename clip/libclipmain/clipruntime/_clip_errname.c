CLIP_DLLEXPORT const char *
_clip_errname(int code)
{
   char *msg;

   switch (code)
   {
   default:
      msg = _clip_gettext("unknown error");
      break;
   case EG_ARG:
      msg = _clip_gettext("invalid argument");
      break;
   case EG_BOUND:
      msg = _clip_gettext("bound error");
      break;
   case EG_STROVERFLOW:
      msg = _clip_gettext("string too long");
      break;
   case EG_NUMOVERFLOW:
      msg = _clip_gettext("number too big");
      break;
   case EG_ZERODIV:
      msg = _clip_gettext("divide by zero");
      break;
   case EG_NUMERR:
      msg = _clip_gettext("number error");
      break;
   case EG_SYNTAX:
      msg = _clip_gettext("syntax error");
      break;
   case EG_COMPLEXITY:
      msg = _clip_gettext("syntax too complex");
      break;

   case EG_MEM:
      msg = _clip_gettext("memory error");
      break;
   case EG_NOFUNC:
      msg = _clip_gettext("no function");
      break;
   case EG_NOMETHOD:
      msg = _clip_gettext("no method");
      break;
   case EG_NOVAR:
      msg = _clip_gettext("no variable");
      break;
   case EG_NOALIAS:
      msg = _clip_gettext("no alias");
      break;
   case EG_NOVARMETHOD:
      msg = _clip_gettext("no varmethod");
      break;

   case EG_CREATE:
      msg = _clip_gettext("create error");
      break;
   case EG_OPEN:
      msg = _clip_gettext("open error");
      break;
   case EG_CLOSE:
      msg = _clip_gettext("close error");
      break;
   case EG_READ:
      msg = _clip_gettext("read error");
      break;
   case EG_WRITE:
      msg = _clip_gettext("write error");
      break;
   case EG_PRINT:
      msg = _clip_gettext("print error");
      break;

   case EG_UNSUPPORTED:
      msg = _clip_gettext("unsupported");
      break;
   case EG_LIMIT:
      msg = _clip_gettext("limit exhaust");
      break;
   case EG_CORRUPTION:
      msg = _clip_gettext("corruption detected");
      break;
   case EG_DATATYPE:
      msg = _clip_gettext("wrong datatype");
      break;
   case EG_DATAWIDTH:
      msg = _clip_gettext("wrong datawidth");
      break;
   case EG_NOTABLE:
      msg = _clip_gettext("no table");
      break;
   case EG_NOORDER:
      msg = _clip_gettext("no order");
      break;
   case EG_SHARED:
      msg = _clip_gettext("share violation");
      break;
   case EG_UNLOCKED:
      msg = _clip_gettext("unlocked");
      break;
   case EG_READONLY:
      msg = _clip_gettext("readonly");
      break;
   case EG_APPENDLOCK:
      msg = _clip_gettext("appendlock");
      break;
   case EG_BADALIAS:
      msg = _clip_gettext("bad alias");
      break;
   case EG_LOCK:
      msg = _clip_gettext("locking error");
      break;
   case EG_SIGNAL:
      msg = _clip_gettext("system signal");
      break;
   }

   return msg;
}
