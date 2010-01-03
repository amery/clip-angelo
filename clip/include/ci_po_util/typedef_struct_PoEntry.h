typedef struct PoEntry
{
   char     *msg;
   char     *translated;
   char     *msg_plural;
   char    **plural;
   int       nplural;
   char     *file;
   int       line;
   struct PoEntry *next;

   int       ncomments;
   char    **comments;
}
PoEntry;
