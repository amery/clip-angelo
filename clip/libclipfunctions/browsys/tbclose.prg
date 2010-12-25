/*****************************************************************************
*  tbClose( <aTBinfo> ) -> aTBinfo
*  close a browse window.
*
*****************************************************************************/
FUNCTION tbClose( aTBinfo )

   LOCAL nLen, n1, n2, n3, n4

      IF ( ( SET( _SET_VIDEOMODE ) == LLG_VIDEO_TXT ) .OR.;
           ( SET( _SET_VIDEOMODE ) == NIL ) )

          RESTSCREEN( ASC( SUBSTR( aTBinfo[ TBR_SAVESCREEN ], 1, 1 ) ), ;
                      ASC( SUBSTR( aTBinfo[ TBR_SAVESCREEN ], 2, 1 ) ), ;
                      ASC( SUBSTR( aTBinfo[ TBR_SAVESCREEN ], 3, 1 ) ), ;
                      ASC( SUBSTR( aTBinfo[ TBR_SAVESCREEN ], 4, 1 ) ), ;
                           SUBSTR( aTBinfo[ TBR_SAVESCREEN ], 5    ) )

      ELSE      // Graphic mode

          nLen := LEN( aTBinfo[ TBR_SAVESCREEN ] )
          n1 := aTBinfo[ TBR_SAVESCREEN ][ nLen - 3 ]
          n2 := aTBinfo[ TBR_SAVESCREEN ][ nLen - 2 ]
          n3 := aTBinfo[ TBR_SAVESCREEN ][ nLen - 1 ]
          n4 := aTBinfo[ TBR_SAVESCREEN ][ nLen ]

          RESTSCREEN( n1, n2, n3, n4, aTBinfo[ TBR_SAVESCREEN ] )

      ENDIF

   RETURN ( NIL )
