/*-- Lorenwarp zur Goldstatue --*/

#strict 2

#appendto GIDL

protected func ControlDigDouble(object pClonk) {
	[Lore rufen|Image=LORY]
	var pLorry = aLorrys[GetPlayerTeam(pClonk -> GetOwner()) - 1];
	pLorry -> Fly2(this);
	return 1;
}

global func Fly2(object pObj) {
	return AddEffect("IntFly2", this, 300, 1, 0, GIDL, pObj -> GetX(), pObj -> GetY());
}

public func FxIntFly2Start(object pTarget, int iEffectNumber, int iTemp, int iX, int iY) {
	if(iTemp)
		return;
	EffectVar(0, pTarget, iEffectNumber) = [iX, iY];
}

public func FxIntFly2Timer(object pTarget, int iEffectNumber) {
	var iObjX = pTarget -> GetX(), iObjY = pTarget -> GetY();
	var iX2 = EffectVar(0, pTarget, iEffectNumber)[0] - iObjX, iY2 = EffectVar(0, pTarget, iEffectNumber)[1] - iObjY;
	var iSteigung = iY2 / iX2;
	var iX = pTarget -> GetX(), iChange;
	if(EffectVar(0, pTarget, iEffectNumber)[0] < iX)
		iChange--;
	else
		iChange++;
	iX += iChange;
	var iY = pTarget -> GetY() + iSteigung * iChange;
	pTarget -> SetPosition(iX, iY);
	pTarget -> SetXDir();
	pTarget -> SetYDir();
	if(Distance(EffectVar(0, pTarget, iEffectNumber)[0], EffectVar(0, pTarget, iEffectNumber)[1], iX, iY) < 10)
		return -1;
}
