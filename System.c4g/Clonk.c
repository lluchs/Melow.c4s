/* Clonkappend zum Leichenrelaunch */

#strict 2

#appendto CLNK

local iOwnerTeam;

protected func Initialize() {
	iOwnerTeam = GetPlayerTeam(GetOwner());
	AddEffect("IntDamageCount", this, 100, 0, this, GetID());
	return _inherited(...);
}

protected func FxIntDamageCountDamage(object pTarget, int iEffectNumber, int iDmgEngy, int iCause, int iBy) {
	if(Contained() && Contained() -> GetID() == TIM1)
		return iDmgEngy;
	var iPlr = GetOwner();
	if(iDmgEngy > 0) { // Heilung?
		HealVal(iPlr) += iDmgEngy;
		return iDmgEngy;
	}
	
	if(iBy != -1 && GetPlayerName(iBy)) {
		var iTeam = GetPlayerTeam(iPlr), iOtherTeam = GetPlayerTeam(iBy);
		if(iTeam == iOtherTeam) {
			TeamDamageVal(iBy) += iDmgEngy;
		}
	
		else if(iTeam != iOtherTeam) {
			TeamDamageVal(iBy) += iDmgEngy;
		}
		return iDmgEngy;
	}
	
	return iDmgEngy;
}

protected func Death() {
	if(fCorpseRelaunch)
		AddEffect("IntRelauncher", this, 100, 50, this, GetID(this));
	return _inherited(...);
}

protected func FxIntRelauncherTimer(object pTarget, int iEffectNumber) {
	var pClonk = FindObject2(Find_OCF(OCF_CrewMember), Find_Not(Find_Team(iOwnerTeam)), Find_Func("Waits4Relaunch"), Sort_Func("ObjectNumber"));
	if(!pClonk)
		return;
	var pRelauncher = FindObject2(Find_Distance(GetDefWidth(GetID())), Find_OCF(OCF_CrewMember), Find_Not(Find_Team(iOwnerTeam)), Find_Not(Find_Func("Waits4Relaunch")));
	if(!pRelauncher)
		return;
	var pObj = pClonk -> Contained();
	if(pObj) {
		pClonk -> CloseMenu();
		pClonk -> Exit();
		pObj -> RemoveObject();
	}
	pClonk -> SetPosition(GetX(), GetY());
	SetViewCursor(pClonk -> GetOwner(), pClonk);
	SetCursor(pClonk -> GetOwner(), pClonk, true);
	pClonk -> DoEnergy(100);
	pClonk -> Extinguish();
	// Effekte und so
	Sound("Magic1");
	CastParticles("MSpark", 5, 50, 0,0, 100, 200, RGBa(255,100,100,128), RGBa(255,0,0,0));
	IncRelaunches(pRelauncher -> GetOwner());
	var szName = GetPlayerName(pRelauncher -> GetOwner()), iTeam = GetPlayerTeam(pClonk -> GetOwner());
	Log("<c %x>Spieler %s relauncht %s!</c>", GetTeamColor(iTeam), szName, GetPlayerName(pClonk -> GetOwner()));
	RemoveObject();
	UpdatePlayerScoreboard(pClonk -> GetOwner());
	UpdatePlayerScoreboard(pRelauncher -> GetOwner());
	UpdateTeamScoreboard(iOwnerTeam);
	return -1;
}
