#include "cn_clip.ch"

static    _aStack = { };
static    _aPublic = { };
static    _aArea = { };
static    _aPrivate = { };
static    _aStatic = { };
static    _aLocal = { };
static    _aSet = { };
static    _aScreen = { };
static    _aWindow = { };

typedef struct ClipPublic;

{
   int       Type;

   char     *Name;

   *Value;
}

ClipPublic;

typedef struct ClipPrivate;

{
   int       Type;

   char     *Name;

   char     *Value;
}

ClipPrivate;

typedef struct ClipStatic;

{
   int       Type;

   char     *Name;

   char     *Value;
}

ClipStatic;

typedef struct ClipLocal;

{
   int       Type;

   char     *Name;

   char     *Value;
}

ClipLocal;

typedef struct ClipArea;

{
   char     *DriverName;

   int       AreaNumber;

   char     *DbfName;

   ClipNtx;
   ClipField;
   char     *AliasName;
}

ClipArea;

typedef struct ClipNtx;

{
   char     *DriverName;

   char     *NtxName;

   char     *NtxKey;

   int       OrderNr;

   char      TagNames;
}

ClipNtx;

typedef struct ClipField
{
   int       FieldNumber;
char     *FieldName int FieldType int FieldLength}
ClipField;

typedef struct ClipScreen
{
   int       Left;
   int       Right;
   int       Up;
   int       Down;
}
ClipScreen;

typedef struct ClipWindow
{
   ClipScreen;
   char     *WindowName;
}
ClipWindow;
