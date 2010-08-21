
***
*  @...GET...SMART
*

#command @ <row>, <col> GET <var>        ;
                        SMART <smart>    ;
                       [ALIAS <alias>]   ;
                       [ORDER <order>]   ;
                       [PICTURE <pic>]   ;
                       [VALID <valid>]   ;
                       [WHEN <when>]     ;
                       [SEND <msg>]      ;
                                         ;
  => SetPos( <row>, <col> )                        ;
  ; AAdd(                                          ;
          GetList,                                 ;
          _GET_( <var>, <(var)>, <pic>,            ;
                 <{valid}>, <{when}> )             ;
        )                                          ;
  ; ATail(GetList):reader:={|get|SmartReader(get)} ;
  ; ATail(GetList):cargo:={<smart>,<alias>,<order>};
 [; ATail(GetList):<msg>]


/*
These two clauses are optional, because the
information they contain can easily be defaulted in
SmartReader. The ALIAS clause indicates the work
area that SmartReader() will seek on, using the
specified ORDER. These default to the current work
area and index order.


 ATail(GetList):reader:={|get|SmartReader(get)} ;



This assigns a code block to the get object's reader
instance variable. The code block will be evaluated
by the standard READMODAL() function in place of the
call to GetReader().


 ATail(GetList):cargo:={<smart>,<alias>,<order>}


This uses the get object's cargo slot to store the
information that SmartReader() will need to
retrieve. It assumes exclusive use of the cargo
slot. If you are using a customized version of
ReadModal() that also uses the cargo slot, you will
need to change this line to use a more generic
method of storing information in the cargo slot.
*/
