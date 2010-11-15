#define __NOEXTRA__
//#define __CLIP__
#define LI_LEN   44

#define ITEM_NSTR      1
#define ITEM_CLR       2
#define ITEM_CLRV      3
#define ITEM_BSKIP     4
#define ITEM_BGTOP     5
#define ITEM_BGBOT     6
#define ITEM_BEOF      7
#define ITEM_BBOF      8
#define ITEM_B1        9
#define ITEM_MSF       10
#define ITEM_NAMES     11
#define ITEM_NMCLR     12
#define ITEM_FREEZE    13
#define ITEM_RCOU      14
#define ITEM_MSREC     15
#define ITEM_PRFLT     16
#define ITEM_TEKZP     17
#define ITEM_KOLZ      18
#define ITEM_VALID     19
#define ITEM_WHEN      20
#define ITEM_MSNAME    21
#define ITEM_MSTYP     22
#define ITEM_MSLEN     23
#define ITEM_MSDEC     24
#define ITEM_EXPFI     25
#define ITEM_BDESHIN   26
#define ITEM_BDESHOUT  27
#define ITEM_RECNO     28
#define ITEM_BGOTO     29
#define ITEM_Y1        30
#define ITEM_X1        31
#define ITEM_Y2        32
#define ITEM_X2        33
#define ITEM_LSOHR     34
#define ITEM_LVIEW     35
#define ITEM_NCOLUMNS  36
#define ITEM_NLEFT     37
#define ITEM_COLPOS    38
#define ITEM_XPOS      39
#define ITEM_MSED      40
#define ITEM_COLCOUNT  41
#define ITEM_LADD      42
#define ITEM_SEPARATOR 43
#define ITEM_UPDATED   44

#define LI_NSTR      mslist[ITEM_NSTR]
#define LI_CLR       mslist[ITEM_CLR]
#define LI_CLRV      mslist[ITEM_CLRV]
#define LI_BSKIP     mslist[ITEM_BSKIP]
#define LI_BGTOP     mslist[ITEM_BGTOP]
#define LI_BGBOT     mslist[ITEM_BGBOT]
#define LI_BEOF      mslist[ITEM_BEOF]
#define LI_BBOF      mslist[ITEM_BBOF]
#define LI_B1        mslist[ITEM_B1]
#define LI_MSF       mslist[ITEM_MSF]
#define LI_NAMES     mslist[ITEM_NAMES]
#define LI_NMCLR     mslist[ITEM_NMCLR]
#define LI_FREEZE    mslist[ITEM_FREEZE]
#define LI_RCOU      mslist[ITEM_RCOU]
#define LI_MSREC     mslist[ITEM_MSREC]
#define LI_PRFLT     mslist[ITEM_PRFLT]
#define LI_TEKZP     mslist[ITEM_TEKZP]
#define LI_KOLZ      mslist[ITEM_KOLZ]
#define LI_VALID     mslist[ITEM_VALID]
#define LI_WHEN      mslist[ITEM_WHEN]
#define LI_MSNAME    mslist[ITEM_MSNAME]
#define LI_MSTYP     mslist[ITEM_MSTYP]
#define LI_MSLEN     mslist[ITEM_MSLEN]
#define LI_MSDEC     mslist[ITEM_MSDEC]
#define LI_EXPFI     mslist[ITEM_EXPFI]
#define LI_BDESHIN   mslist[ITEM_BDESHIN]
#define LI_BDESHOUT  mslist[ITEM_BDESHOUT]
#define LI_RECNO     mslist[ITEM_RECNO]
#define LI_BGOTO     mslist[ITEM_BGOTO]
#define LI_Y1        mslist[ITEM_Y1]
#define LI_X1        mslist[ITEM_X1]
#define LI_Y2        mslist[ITEM_Y2]
#define LI_X2        mslist[ITEM_X2]
#define LI_LSOHR     mslist[ITEM_LSOHR]
#define LI_LVIEW     mslist[ITEM_LVIEW]
#define LI_NCOLUMNS  mslist[ITEM_NCOLUMNS]
#define LI_NLEFT     mslist[ITEM_NLEFT]
#define LI_COLPOS    mslist[ITEM_COLPOS]
#define LI_XPOS      mslist[ITEM_XPOS]
#define LI_MSED      mslist[ITEM_MSED]
#define LI_COLCOUNT  mslist[ITEM_COLCOUNT]
#define LI_LADD      mslist[ITEM_LADD]
#define LI_SEPARATOR mslist[ITEM_SEPARATOR]
#define LI_UPDATED   mslist[ITEM_UPDATED]

MemVar strfor, prall, prnext, nrest, prrest, expstr, nrez, mypath, stroka, poz1, msexp
MemVar finame, mslist, msmode, improc, lenmsf, msfile, msknop, msdriv, stra1, stra2, stra3, stra4
MemVar prdbf, prsdf, prdelim, cdelim, msknop1, prrdonly, numdriv, msmsli, ScrClr, que_simpl
MemVar ORAMKA, DRAMKA, chng_kol, lWinChar, modesplit, dformat, memownd, slkeys, aMMenu, txtinf, mode43, nServerType
MemVar x1, x2, y2, _y2, prkorf, prkorst, dbfi_f, dbnumf, prfi, prmemo, impr1, impr2, wndfirst, cdata, prmsf
MemVar quebuf, sword, flen
MemVar Ctrlist
MemVar f_impr, f_usl, max_len, max_dec, msrelat, msaccum
MemVar han, strbuf, poz
