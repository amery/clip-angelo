#ifdef _WIN32
static int
to_clipboard(const unsigned char *data, int nType, int len)
{
   HANDLE    hmem;

   unsigned char *mem;

   if (!OpenClipboard(0))
      return 0;

   if (nType <= 0)
      nType = CF_TEXT;

   hmem = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, len + 1);
   mem = GlobalLock(hmem);

   memcpy(mem, data, len + 1);

   GlobalUnlock(hmem);

   EmptyClipboard();
   SetClipboardData(nType, hmem);

   CloseClipboard();
   return 1;
}
#else
static int
to_clipboard(const unsigned char *data, int nType, int len)
{
   int       xterm_clipboard_present = 0;

   char      path[256], *s;

   if (nType <= 0)
      nType = CF_TEXT;

   if (nType >= CF_MAX)
      return 0;

  /* xterm detecting */
   s = getenv("DISPLAY");
   if (s && *s)
    {
       snprintf(path, sizeof(path), "%s/bin/xclip", CLIPROOT);
       if (!access(path, X_OK))
	{
	   snprintf(path, sizeof(path), "%s/bin/xclip -i", CLIPROOT);
	   xterm_clipboard_present = 1;
	}
       if (!xterm_clipboard_present)
	{
	   snprintf(path, sizeof(path), "/usr/bin/xclip");
	   if (!access(path, X_OK))
	    {
	       snprintf(path, sizeof(path), "/usr/bin/xclip -i");
	       xterm_clipboard_present = 1;
	    }
	}
    }

   if (xterm_clipboard_present)
    {
       FILE     *f = popen(path, "w");

       if (f)
	{
	   int       r, l, d;

	   l = 0;
	   d = len;

	   while (l < d)
	    {
	       r = fwrite(data + l, 1, d - l, f);
	       if (r <= 0)
		  break;
	       l += r;
	    }
	   pclose(f);
	}

       return 1;
    }

   if (_clip_clipboard_data[nType] != NULL)
      free(_clip_clipboard_data[nType]);
   _clip_clipboard_data[nType] = malloc(len + 1);
   _clip_clipboard_len[nType] = len;
   memcpy(_clip_clipboard_data[nType], data, len + 1);
   return 1;
}
#endif
