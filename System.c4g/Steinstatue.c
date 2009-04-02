/*-- Steinstatuen löschen --*/

#strict 2

#appendto IDOL

protected func Initialize() {
	if(GetID() == IDOL) {
		if(!FrameCounter())
			AddEffect("Extinguish", this, 100, 20, this, GetID());
		else
			AddEffect("Reload", this, 100, 2, this, GetID());
	}
	return _inherited(...);
}

public func FxExtinguishStart(object pTarget, int iEffectNumber, int iTemp) {
	if(!iTemp) {
		pTarget -> SetClrModulation(RGB(0, 0, 255));
	}
}
public func FxExtinguishTimer(object pTarget, int iEffectNumber) {
	var pObj = FindObject2(Find_Distance(GetObjHeight() * 3), Find_OCF(OCF_OnFire), Find_OCF(OCF_CrewMember), Find_Not(Find_Func("InLava")));
	if(pObj) {
		pTarget -> Sound("Splash1");
		// Lösche es
  	pObj -> Extinguish();
		// Eine paar Effekte...
		CastObjects(MSTB,10,25, pObj -> GetX() - pTarget -> GetX(), pObj -> GetY() - pTarget -> GetY());
		CastParticles("MSpark", 200, 50, pObj -> GetX() - pTarget -> GetX(), pObj -> GetY() - pTarget -> GetY(), 10, 50, RGBa(100,100,255,128), RGBa(0,0,255,0));
		pTarget -> SetClrModulation();
		AddEffect("Reload", pTarget, 100, 2, pTarget, pTarget -> GetID());
		return -1;
	}
}

global func InLava() {
	return WildcardMatch(MaterialName(GetMaterial()), "*Lava*");
}

public func FxReloadTimer(object pTarget, int iEffectNumber) {
	var iRed = GetRGBaValue(pTarget -> GetClrModulation(), 1), iGreen = GetRGBaValue(pTarget -> GetClrModulation(), 2);
	iRed--;
	iGreen--;
	pTarget -> SetClrModulation(RGB(iRed, iGreen, 255));
	if(!iRed) {
		AddEffect("Extinguish", pTarget, 100, 10, pTarget, pTarget -> GetID());
		return -1;
	}
}
