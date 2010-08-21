typedef struct LoopExitNode
{
   Node node;
   int isLoop;
   int offs;
   int processed;
}
LoopExitNode;
