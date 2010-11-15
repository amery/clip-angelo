/*   Business graphics - linear diagram with cumulation      */
/*   Copyright (C) 2001  ITK                                 */
/*   Author  : Elena Kornilova (alena@itk.ru)                */
/*   Licence : (GPL) http://www.itk.ru/clipper/licence.html  */

#include "ci_gdinfo.ch"
#include "ci_bggraph.ch"

function BgLineCumulNew(width, high)
local obj
	obj := BgBaseGraphic(width, high)
	obj:classnmae	:= "BG_LINE_CUMUL"
	obj:outFile	:= ""
	obj:typeFile	:= GDFILE_PNG
	obj:arr		:= {}
	obj:legend	:= {}
	obj:alignLeg	:= 0  // �� ���� �������
	obj:signat	:= {}
	obj:title	:= ""
	obj:subTitle	:= ""
	obj:vrtTitle	:= ""
	obj:volume	:= .f.
	obj:valVolume	:= 0
	obj:average	:= 0   //1-�������� ������� ��������, 2-�������� ����������� ��������,
			       //3-� ������� � ������, 0-������ �� ����������
	obj:valAverage	:= 0
	obj:valCritic	:= 0

	obj:totalSumma	:= 0
	obj:summa	:= {}
	obj:lowsumma	:= {}
	obj:newarr	:= {}

	obj:onlyLegend	:= .f.
	obj:__isData	:= .f.
	obj:__datarow	:= 0
	obj:__category	:= 0
	obj:__isLegend	:= .f.
	obj:__isSignat	:= .f.
	obj:__isTitle	:= .f.
	obj:__isSubTitle:= .f.
	obj:__isVrtTitle:= .f.
	_recover_bg_line_cumul(obj)

return obj
************
function _recover_bg_line_cumul(obj)
	obj:setData	:= @bg_setData()
	obj:setInfo	:= @bg_setInfo()
	obj:drawImage	:= @bg_drawImage()
	obj:setVolume	:= @bg_setVolume()
	obj:setAverage	:= @bg_setAverage()
return obj
************
static function bg_setData(arr, legend, alignLeg, signat)
local i, j, l, ts
	if empty(arr) .or. !(valtype(arr)$"AM") .or. len(arr)==0
		return .f.
	endif
	::__isData := .t.
	if valtype(arr[1])!="A"
		aadd(::arr, arr)
	else
		::arr := aclone(arr)
	endif

	::__datarow := len(::arr)

	::totalSumma := 0
	for i=1 to ::__datarow
		l:= len(::arr[i])
		if l>=::__category
			::__category := l
		endif
		ts := 0
		aeval(::arr[i], {|x| ts += x})
		::totalSumma += ts
	next

	::summa := {}
	::lowsumma:= {}
	asize(::summa, ::__category)
	afill(::summa, 0)
	asize(::lowsumma, ::__category)
	afill(::lowsumma, 0)
	for i=1 to ::__datarow
		l:= len(::arr[i])
		if l<::__category
			asize(::arr[i], ::__category)
			afill(::arr[i], 0, l+1)
		endif
		::findval(::arr[i])
		for j=1 to ::__category
			if ::arr[i][j] >= 0
				::summa[j] += ::arr[i][j]
			else
				::lowsumma[j] += ::arr[i][j]
			endif
		next
	next
	::findval(::summa)
	::findval(::lowsumma)
	::max := iif(::plsmax>::decmax, ::plsmax, ::decmax)
	::min := iif(::plsmin<::decmin, ::plsmin, ::decmin)
	if (::max-::min<1000)
		::max := iif(::max>0, ::max, 0)
		::min := iif(::min<0, ::min, 0)
	endif

	::valAverage := (::totalSumma)/(::__datarow*::__category)

	if valtype(legend)=="A" .and. !empty(legend)
		::legend := {}
		::alignLeg := iif(between(alignleg, 0, 4), alignleg, ::alignLeg)
		l := len(legend)
		for i=1 to ::__datarow
			aadd(::legend, iif(i<=l, legend[i], "Row"+ZSTR(i)))
		next
		::__isLegend := iif(::alignLeg>0, .t., .f.)
	endif

	if valtype(signat)=="A" .and. !empty(signat)
		::signat := {}
		l := len(signat)
		for i=1 to ::__category
			aadd(::signat, iif(i<=l, signat[i], "Col"+ZSTR(i)))
		next
		::__isSignat := .t.
	endif

return .t.
************
static function bg_setInfo(title, subtitle, vrttitle)
	::title := iif(title!=NIL, title, "")
	::subtitle := iif(subtitle!=NIL, subtitle, "")
	::vrttitle := iif(vrttitle!=NIL, vrttitle, "")
return .t.
************
static function bg_setVolume(volume)
	if volume==NIL .or. valtype(volume)!="L"
		::volume := !::volume
	else
		::volume := volume
	endif
return .t.
************
static function bg_setAverage(type, value)
	if !::__isData
		return .f.
	endif
	do case
	case type == 1	//�������� ������ ������� ��������
		::average := 1
	case type == 2
		::average := 2 	//�������� ���� ��������
		if value!=NIL .and. valtype(value)=="N"
			::valCritic := value
		endif
	case type == 3	//�������� � ������� � ������ ����
		::average := 3
		if value!=NIL .and. valtype(value)=="N"
			::valCritic := value
		endif
	otherwise
		::average := 0
	endcase
return .t.
************
static function bg_drawImage()
local i, j, key, colwidth, dy, x, y, arry:={}, arrx:={}, wCol, volume
local xav, rmin, x1, x2, y1, y2, a, cntColor, va, kl
local val,x4,y4

	if !::__isData
		return .f.
	endif
	::createImage()
	::drawScale2(__self__)

	cntColor := len(::legendColor)/3
	if ::log
		rmin=log10(max(1, int(10^(len(ZSTR(int(::min)))-1))))
	endif
	colwidth := BG_DEF_WIDTH
	volume := ::valVolume
	::Y -= volume
	/* ����������� ��������� ������ ����� ������� */
	wCol := ::Y/::__category
	do while (wCol-6<=colwidth)
		colwidth-- // �������� ������ �������
	enddo
	for i=1 to ::__category
		y := ::Y0-((i-1)*wCol)+3
		aadd(arrx, {::X0, ::X0})
		aadd(arry, y)
		::image:line(::X0-2, y, ::X0+2, y, ::scaleColor)
	next
	/* put category head */
	x := ::sX0
	dy := wCol/2
	::image:line(::sX0, ::Y0-::Y-volume, ::sX0, ::Y0-volume, ::scaleColor)
	for i=1 to ::__category
		if i>1
			::image:line(x-2, arry[i], x+2, arry[i], ::scaleColor)
			::image:line(x, arry[i], x+volume, arry[i]-volume, ::scaleColor)
		endif
		if ::__isSignat
			j := ::signat[i]
			::image:stringVector(j, x, arry[i]-dy+::font_sign_high, ::font_sign, 270, ::scaleColor)
		endif
	next

	dy := int((wCol-colwidth)/2)
	afill(::summa, 0)
	afill(::lowsumma, 0)
	if ::log
		//xav := int(::X0+(log10(abs(max(1, ::valAverage)))-rmin)*iif(::valAverage<0, -1, 1)*::dskl)
		xav := int(::X0+(log10(max(1, abs(::valAverage))))*iif(::valAverage<0, -1, 1)*::dskl)
	else
		xav := int(::X0+::valAverage*::dskl)
	endif
	for i=1 to ::__category
		for key=1 to ::__datarow
			kl := iif(key<=10, key, &(right(alltrim(str(key)),1)))
			kl := iif(kl==0, 10, kl)

			val := ::arr[key]
			j := val[i]
			y2 := arry[i]-dy
			y1 := y2-colwidth
			if j>0
				x1 := arrx[i][1]
				if ::log
					::summa[i] += j
					//x2 := int(::X0+(log10(MAX(1, ::summa[i]))-rmin)*::dskl)
					x2 := int(::X0+(log10(MAX(1, ::summa[i])))*::dskl)
				else
					x2 := int(x1+j*::dskl)
				endif
				arrx[i][1] := x2
			else
				x1 := arrx[i][2]
				if ::log
					::lowsumma[i] += j
					//x2 := int(::X0-(log10(MAX(1, abs(::lowsumma[i])))-rmin)*::dskl)
					x2 := int(::X0-(log10(MAX(1, abs(::lowsumma[i]))))*::dskl)
				else
					x2 := int(x1+j*::dskl)
				endif
				arrx[i][2] := x2
			endif
			if ::volume
				if j>=0
					a := {{x1, y1}, {x1+volume, y1-volume}, {x2+volume, y1-volume}, {x2+volume, y2-volume}, {x2, y2}, {x1, y2}}
					::image:filledPolygon(a, ::legendColor[kl])
					::image:polygon(a, ::scaleColor)
					if ::average>0 .and. ::valAverage>0
						::image:line(xav, ::Y0, xav, ::Y0-::Y, ::scaleColor)
						if x2>xav .and. between(xav, x1, x2) .and. between(xav, x1+volume, x2+volume)
							a := {{xav, y1}, {xav, y1-volume}, {xav+volume, y1-volume}}
							::image:filledPolygon(a, ::legendColor[kl+2*cntColor])
							::image:polygon(a, ::scaleColor)
						elseif between(xav, x1, x2)
							va := xav - x1
							a := {{xav, y1}, {xav, y1-va}, {x1+volume, y1-volume}, {xav+volume, y1-volume}}
							::image:filledPolygon(a, ::legendColor[kl+2*cntColor])
							::image:polygon(a, ::scaleColor)
						elseif between(xav, x2, x2+volume)
							va := xav-x2
							a := {{x2+va, y2-va}, {x2+va, y1-volume}, {x2+volume, y1-volume}, {x2+volume, y2-volume}}
							::image:filledPolygon(a, ::legendColor[kl+2*cntColor])
							::image:polygon(a, ::scaleColor)
						endif
					endif
					::image:line(x1, y1, x2, y1, ::scaleColor)
					::image:line(x2, y1, x2+volume, y1-volume, ::scaleColor)
					::image:line(x2, y1, x2, y2, ::scaleColor)
				else
					if (x1<::X0 .or. arrx[i][1]>::X0)
						x4 := x1
						y4 := y1
					else
						x4 := x1+volume
						y4 := y2-volume
					endif
					a := {{x2, y1}, {x2+volume, y1-volume}, {x1+volume, y1-volume}, {x4, y4}, {x1, y2}, {x2, y2}}
					::image:filledPolygon(a, ::legendColor[kl])
					::image:polygon(a, ::scaleColor)
					// �������
					if x1==::X0
						a := {{x1, y2}, {x1, y1-volume}, {x1+volume, y1-volume}, {x1+volume, y2-volume}}
						::image:filledPolygon(a, ::legendColor[kl+cntColor])
						::image:polygon(a, ::scaleColor)
					endif
					// ������� �����
					if ::average>0 .and. ::valAverage<0
						if x2+volume<=xav .and. between(xav, x1, x2) .and. between(xav, x1+volume, x2+volume)
							a := {{xav, y1}, {xav, y1-volume}, {xav+volume, y1-volume}}
							::image:filledPolygon(a, ::legendColor[kl+cntColor])
							::image:polygon(a, ::scaleColor)
						elseif between(xav, x1+volume, x2+volume)
							va := xav-x1
							a := {{xav, y1-va}, {xav, y1-volume}, {x1+volume, y1-volume}}
							::image:filledPolygon(a, ::legendColor[kl+cntColor])
							::image:polygon(a, ::scaleColor)
						elseif between(x2+volume, xav, xav+volume)
							va := xav-x2
							a := {{xav, y1}, {xav, y1-va}, {x2+volume, y1-volume}, {xav+volume, y1-volume}}
							::image:filledPolygon(a, ::legendColor[kl+cntColor])
							::image:polygon(a, ::scaleColor)
						endif
					endif
					::image:line(xav, ::Y0, xav, ::Y0-::Y, ::scaleColor)
					::image:line(x2, y1, x1, y1, ::scaleColor)
					::image:line(x1, y1, x1+volume, y1-volume, ::scaleColor)
					::image:line(x1, y1, x1, y2, ::scaleColor)
				endif
			else
				if (j>=0)
					::image:filledRectangle(x1, y1,  x2, y2, ::legendColor[kl])
				else
					::image:filledRectangle(x2, y1, x1, y2, ::legendColor[kl])
				endif
				if ::average>0
					::image:line(xav, ::Y0, xav, ::Y0-::Y, ::image:exactColor(::AvColor[1], ::AvColor[2], ::AvColor[3]))
				endif
			endif
		next
	next
return .t.


