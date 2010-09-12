#ifdef _WIN32
static char *
from_clipboard(int nType, int *len)
{
   char *ret;

   *len = 0;

   if (nType <= 0)
      nType = CF_TEXT;

   if (OpenClipboard(0))
   {
      HANDLE hmem;

      LPVOID mem;

      hmem = GetClipboardData(nType);

      if (hmem)
      {
	 mem = GlobalLock(hmem);

	 *len = strlen(mem);
	 ret = malloc((*len) + 1);
	 memcpy(ret, mem, (*len));
	 ret[(*len)] = 0;

	 GlobalUnlock(hmem);
      }
      else
      {
	 ret = malloc(1);
	 ret[0] = 0;
      }
      CloseClipboard();
   }
   else
   {
      ret = malloc(1);
      ret[0] = 0;
   }
   return ret;
}
#else
static unsigned char *
from_clipboard(int nType, int *len)
{
   int xterm_clipboard_present = 0;

   unsigned char *ret = 0;

   /* xterm detecting */
   char path[256], *s;

   *len = 0;
   if (nType <= 0)
      nType = CF_TEXT;

   if (nType >= CF_MAX)
      return NULL;

   s = getenv("DISPLAY");
   if (s && *s)
   {
      snprintf(path, sizeof(path), "%s/bin/xclip", CLIPROOT);
      if (!access(path, X_OK))
      {
	 snprintf(path, sizeof(path), "%s/bin/xclip -o", CLIPROOT);
	 xterm_clipboard_present = 1;
      }
      if (!xterm_clipboard_present)
      {
	 snprintf(path, sizeof(path), "/usr/bin/xclip");
	 if (!access(path, X_OK))
	 {
	    snprintf(path, sizeof(path), "/usr/bin/xclip -o");
	    xterm_clipboard_present = 1;
	 }
      }
   }

   if (xterm_clipboard_present)
   {
      FILE *f = popen(path, "r");

      if (f)
      {
	 int r, l, d;

	 l = 0;
	 d = 128;

	 for (;;)
	 {
	    ret = (unsigned char *) realloc(ret, l + d);
	    r = fread(ret + l, 1, d, f);
	    if (r <= 0)
	       break;
	    l += r;
	 }
	 pclose(f);
	 ret = (unsigned char *) realloc(ret, l + 1);
	 ret[l] = 0;
	 *len = l;
      }
      else
      {
	 ret = malloc(1);
	 ret[0] = 0;
      }
   }
   else
   {
      if (_clip_clipboard_data[nType] == NULL)
      {
	 ret = malloc(1);
	 ret[0] = 0;
      }
      else
      {
	 *len = _clip_clipboard_len[nType];
	 ret = malloc((*len) + 1);
	 memcpy(ret, _clip_clipboard_data[nType], (*len) + 1);
      }
   }
   return ret;
}
#endif
