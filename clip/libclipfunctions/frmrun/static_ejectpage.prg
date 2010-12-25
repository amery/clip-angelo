/***
*
*  EjectPage()
*  Eject a page if the form feed option is set
*
*/
STATIC PROCEDURE EjectPage
   IF lFormFeeds
      EJECT
   ENDIF
   RETURN
