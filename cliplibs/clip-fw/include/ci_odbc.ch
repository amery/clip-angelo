#ifndef CI__ODBC_CH
#define CI__ODBC_CH

//----------------------------------------------------------------------------//

#xcommand DEFINE ODBC <oDbc> ;
             [ NAME  <cName> ] ;
             [ USER <cPath> ] ;
             [ PASSWORD <cPassword> ] ;
             [ CONNECT <cConnect> ] ;
             [ <dlg: FROM USER> ] ;
       => ;
          <oDbc> := TDbOdbc():New( <cName>, <cPath>, <cPassword>,;
             <cConnect>, <.dlg.> )


#xcommand ODBC <oDbc> <sql: SQL, EXECUTE> <cCommand> ;
       => ;
          <oDbc>:Execute( <cCommand> )

//----------------------------------------------------------------------------//

#endif
