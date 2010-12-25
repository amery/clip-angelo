typedef struct ClipWindow
{
   ClipRect ClipRect_rect_of_ClipWindow;
   ClipRect ClipRect_format_of_ClipWindow;
   int no_of_ClipWindow;
   void *save_of_ClipWindow;
   int shadow_of_ClipWindow;	/* -1 if none */
}
ClipWindow;
