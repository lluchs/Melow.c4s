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
	if(Check(pClonk))
		return 1;
  // Herausnehmen per Graben: Holen-Menü öffnen
  pClonk -> SetCommand(0, "Get", this, 0,0, 0, 1);
}

protected func ControlThrow(object pClonk) {
	if(Check(pClonk))
		return 1;
}

private func Check(object pClonk) {
	if(iTeam && GetPlayerTeam(pClonk -> GetOwner()) != iTeam) {
		CreateObject(TIM1) -> Explode(RandomX(20, 30));
		return 1;
	}
}

private func MaxContents() { return 100; }
