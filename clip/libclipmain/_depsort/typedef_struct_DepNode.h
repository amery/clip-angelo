typedef struct DepNode
{
   ClipVar *vp;			/* payload */
   long hash;
   int ndeps;
   long *deps;
   int setno;
   int in_cycle;
}
DepNode;
