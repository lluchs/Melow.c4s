/* Statuenteilgrundscript */

#strict 2

local iOwnerTeam, szRelauncher;

protected func Hit()
{
  Sound("RockHit*");
  return 1;
}

public func RejoinClonk(int iTeam, int iPlr, object pClonk) {
	//iTeam--;
	//var pClonk = aDeathClonks[iTeam][0];
	if(!pClonk) {
		pClonk = FindObject2(Find_OCF(OCF_CrewMember), Find_Team(iTeam), Find_Func("Waits4Relaunch"), Sort_Func("ObjectNumber"));
		if(!pClonk)
			return;
	}
	//DeleteArrayItem(0, aDeathClonks[iTeam]);
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
	if(iPlr != -1)
		var szName = GetPlayerName(iPlr);
	else
		var szName = szRelauncher;
	if(!iTeam)
		iTeam = iOwnerTeam;
	Log("<c %x>Spieler %s relauncht %s!</c>", GetTeamColor(iTeam), szName, GetPlayerName(pClonk -> GetOwner()));
	CreateStatuePart(GetID());
	RemoveObject();
	return 1;
}

global func Waits4Relaunch() {
	if(Contained() && (Contained() -> GetID() == TIM1))
		return 1;
}

protected func ControlDigDouble(object pClonk) {
	[Relaunch|Image=CLNK]
	var iTeam = GetPlayerTeam(pClonk -> GetOwner());
	if(!RejoinClonk(iTeam, pClonk -> GetOwner())) {
		var iCount = ObjectCount2(Find_Func("IsHolyStatuePart"), Find_Func("IsMarkedFor", iTeam));
		if((iCount - iMarkable) < (GetLength(GetPlayerByTeam(OtherTeam(iTeam))) - GetLength(GetPlayerByTeam(iTeam)) + aMarkable[iTeam - 1])) {
			iOwnerTeam = iTeam;
			szRelauncher = GetPlayerName(pClonk -> GetOwner());
			SetClrModulation(GetTeamColor(iTeam));
			return 1;
		}
	}
	else
		return 1;
	Sound("Error");
	return 1;
}

//GetLength(GetPlayerByTeam(OtherTeam(1))) - GetLength(GetPlayerByTeam(1)) + aMarkable[1 - 1]))

public func IsHolyStatuePart() { return 1; }

public func IsMarkedFor(int iTeam) { return iTeam == iOwnerTeam; }
