typedef struct Coll
{
	int count_of_Coll;
	int size_of_Coll;
	int duplicates_of_Coll;
	void **items_of_Coll;
   void (*_free) (void *);
   int (*compare) (void *, void *);
}
Coll;
