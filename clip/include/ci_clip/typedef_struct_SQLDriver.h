typedef struct SQLDriver
{
   char      id_of_SQLDriver[11];
   char      name_of_SQLDriver[21];
   char      desc_of_SQLDriver[101];
   int       (*connect) (struct ClipMachine *);
} SQLDriver;
