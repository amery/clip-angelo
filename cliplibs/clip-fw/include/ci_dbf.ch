// Warning: Under development. Not available yet!

#ifndef CI__DBF_CH
#define CI__DBF_CH

#xcommand DBF [<oDbf>] ;
             [ <file: FILE, FILENAME, DISK> <cFileName> ] ;
             [ ALIAS <cAlias> ] ;
             [ <user: FROM USER> ] ;
       => ;
          [ <oDbf> := ] DbfBegin( <cFileName>, <cAlias>, <.user.> )

#xcommand FIELD <cName> ;
             [ AS <type: STRING, NUMBER, DATE, LOGIC, MEMO> ] ;
             [ LEN <nLen> ] ;
             [ DEC <nDec> ] ;
       => ;
          FldDef( <(cName)>, <(type)>, <nLen>, <nDec> )

#xcommand ENDDBF => DbfEnd()

#endif
