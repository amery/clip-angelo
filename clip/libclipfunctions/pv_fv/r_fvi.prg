FUNC R_FVI(nConst,nPercent,nNumber,nPay,nInv,lPrenumerando,lSimple)
LOCAL res
DEFAULT nPay TO 1,nInv TO 1,lSimple TO 0,lPrenumerando TO 0
nPercent:=nPercent*0.01
IF lSimple==1
	res:=nInv*nNumber
	res:=res*nConst*(1+(res-1)*nPercent/(2*nInv))
ELSE
	res:=1+nPercent/nPay
	res:=nConst*(res^(nNumber*nPay) - 1)/((res^(nPay/nInv)-1)*nInv)
	IF lPrenumerando=1
		res:=res * (1+nPercent/nInv)^(nInv/nPay)
	ENDIF
ENDIF
RETURN res
