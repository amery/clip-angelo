typedef struct ScreenBase
{
   int Lines;
   int Columns;
   int clear_on_exit;
   int fd;
   char *terminalName;
   Screen *realScreen;
   int mouse_present;
   int mouse_y;
   int mouse_x;
   int mouse_visible;
   int mouse_auto_visible;
   int mouse_top;
   int mouse_bottom;
   int mouse_left;
   int mouse_right;
   int mouse_buttons;
   int mouse_dclick_speed;
   int mouse_clicks_left;
   int mouse_clicks_right;
   int mouse_clicks_middle;
   const char *mouse_driver;
   unsigned char pg_chars[PG_SIZE];
   void *data;
}
ScreenBase;
