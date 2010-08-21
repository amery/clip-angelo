typedef struct SeqNode
{
   Node node;
   Node *list;
   Node *using;
   Node *recover;
   int seqNo;
   int curSeqNo;
   int toffs;
   int seqLevel;
}
SeqNode;
