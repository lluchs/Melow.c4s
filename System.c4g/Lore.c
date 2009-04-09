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

protected func RejectContents() {
	if(!iTeam)
		return;
	var aClonks = FindObjects(Find_Action("Push"), Find_ActionTarget(this));
	for(var pClonk in aClonks) {
		if(GetPlayerTeam(pClonk -> GetOwner()) != iTeam && (GetLength(aClonks) == 1 || (pClonk -> GetMenu() == 0013 || pClonk -> GetMenu() == 0018))) {
			CreateObject(TIM1) -> Explode(RandomX(20, 30));
			return 1;
		}
	}
}
