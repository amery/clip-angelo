/*****************************************************************************
*  MakeColumn( <aTBinfo>, <aColInfo>, <nPosition> ) -> aTBinfo
*  create a new tBrowse column.
*
*****************************************************************************/
static FUNCTION MakeColumn( aTBinfo, aColInfo, nPosition )
   LOCAL oColumn

/*--- determine if the column is being appended or inserted. */
      IF ( ! VALTYPE( nPosition ) == "N" )
         nPosition := LEN( aTBinfo[ TBR_COLINFO ] ) + 1

      ELSEIF ( nPosition < 1 )
         RETURN ( aTBinfo )

      ELSEIF ( nPosition > LEN( aTBinfo[ TBR_COLINFO ] ) )
         RETURN ( aTBinfo )

      ENDIF

      AADD( aTBinfo[ TBR_COLINFO ], aColInfo[ TBC_OWNERCOL ] )

      IF ( ValType( aTBinfo[ TBR_REFERENCE ] ) == "A" )

         oColumn := tbColumnNew( aColInfo[ TBC_HEADING ], ;
                    { | x | IIF( x == NIL,                                              ;
                    aTBinfo[ TBR_REFERENCE, aTBinfo[ TBR_POSITION ] ],       ;
                    aTBinfo[ TBR_REFERENCE, aTBinfo[ TBR_POSITION ] ] := x ) } )
      ELSE
         oColumn := tbColumnNew( aColInfo[ TBC_HEADING ], ;
         FieldWblock( aColInfo[ TBC_OWNERCOL ], Select( aTBinfo[ TBR_REFERENCE ] ) ) )

      ENDIF

      aColInfo := aSize( aColInfo, TBC_ELEMENTS )

      oColumn:ColorBlock   := aColInfo[ TBC_COLORBLOCK ]
      oColumn:ColSep       := aColInfo[ TBC_COLSEP     ]
      oColumn:DefColor     := aColInfo[ TBC_DEFCOLOR   ]
      oColumn:Footing      := aColInfo[ TBC_FOOTING    ]
      oColumn:FootSep      := aColInfo[ TBC_FOOTSEP    ]
      oColumn:Heading      := aColInfo[ TBC_HEADING    ]
      oColumn:HeadSep      := aColInfo[ TBC_HEADSEP    ]
      oColumn:Width        := aColInfo[ TBC_WIDTH      ]
      oColumn:Picture      := aColInfo[ TBC_PICTURE    ]
      oColumn:PreBlock     := aColInfo[ TBC_PREBLOCK   ]
      oColumn:PostBlock    := aColInfo[ TBC_POSTBLOCK  ]

      IF ( ValType( aColInfo[ TBC_CANWRITE ] ) == "L" )
         oColumn:SetStyle( TBC_READWRITE, aColInfo[ TBC_CANWRITE ] )
      ENDIF

      aTBinfo[ TBR_BROWSE ]:AddColumn( oColumn )


   RETURN ( aTBinfo )
