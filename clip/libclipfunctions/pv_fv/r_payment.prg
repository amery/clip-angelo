FUNC R_Payment(nTotal,nPercent,nNumber,nPay,nInv)
//обратное от FV()
LOCAL res
DEFAULT nPay TO 1,nInv TO 1
nPercent:=nPercent*0.01
res:=1+nPercent/nPay
res:=nTotal/((res^(nNumber*nPay) - 1)/((res^(nPay/nInv)-1)*nInv))
RETURN res
