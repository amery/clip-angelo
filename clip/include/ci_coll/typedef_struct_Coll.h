typedef struct Coll
{
   int       count;
   int       size;
   int       duplicates;
   void    **items;
   void      (*_free) (void *);
   int       (*compare) (void *, void *);
}
Coll;
