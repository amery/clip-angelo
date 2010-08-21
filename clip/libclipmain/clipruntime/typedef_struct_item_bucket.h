typedef struct
{
   void *item;
   void (*destroy) (void *);
}
item_bucket;
