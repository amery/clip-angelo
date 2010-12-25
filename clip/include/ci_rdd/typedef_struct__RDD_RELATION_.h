typedef struct _RDD_RELATION_
{
   char *expr;
   char simpexpr;		/* Simple expression (just a fieldname) */
   int simpfno;			/* No of key field (for simple expressions) */
   ClipVar block;		/* Compiled key expression */
   int scoped;
   RDD_DATA *child;
   RDD_DATA *parent;
} RDD_RELATION;
