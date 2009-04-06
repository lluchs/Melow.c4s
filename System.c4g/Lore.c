/*-- Lore --*/

#strict 2

#appendto LORY

protected func Initialize() {
	SetMass(GetMass() / 3);
}

local iTeam;

public func SetTeam(int iNew) {
	SetClrModulation(GetTeamColor(iNew));
	return iTeam = iNew;
}

// Steuerung

protected func ControlDig(object pClonk) {
  // Herausnehmen per Graben: Holen-Menü öffnen
  pClonk -> SetCommand(0, "Get", this, 0,0, 0, 1);
}

private func MaxContents() { return 100; }

/* Anti-Rausnehmen */

protected func Grabbed(object pClonk, bool fGrab) {
	if(iTeam && GetPlayerTeam(pClonk -> GetOwner()) != iTeam) {
		if(fGrab)
			AddEffect("IntGetCheck", pClonk, 100, 1, this, GetID());
		else
			RemoveEffect("IntGetCheck", pClonk);
	}
}

protected func FxIntGetCheckTimer(object pTarget, int iEffectNumber) {
	if(pTarget -> GetAction() != "Push" || pTarget -> GetActionTarget() != this)
		return -1;
	var idMenu = pTarget -> GetMenu();
	if(idMenu == 0013 || idMenu == 0018) {
		CreateObject(TIM1) -> Explode(RandomX(20, 30));
		pTarget -> CloseMenu();
		return -1;
	}
}
