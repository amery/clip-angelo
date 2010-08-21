typedef struct TraversePar
{
   int (*func) (Node * np, void *par);
   void *par;
}
TraversePar;
