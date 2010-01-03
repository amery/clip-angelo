Node     *
new2_CallNode(const char *name, Node * expr1, Node * expr2)
{
   return new3_CallNode(name, expr1, expr2, 0);
}
