/* Farbspeicher als Relaunchwarteobjekt! */

#strict 2

#appendto TIM1

protected func Collection2(object pClonk) {
	ShowMenu(pClonk);
	SetName(Format("%s hat versagt!", GetPlayerName(pClonk -> GetOwner())));
}

protected func ShowMenu(object pClonk) {
	if(!pClonk)
		return RemoveObject();
	var iSelection = GetMenuSelection(pClonk);
	pClonk -> CloseMenu();
	var iPlr = pClonk -> GetOwner(), iTeam = GetPlayerTeam(iPlr);
	CreateMenu(CLNK, pClonk, this, C4MN_Extra_None, "Sicht", 0, C4MN_Style_Context, true);
	for(var pLivingClonk in FindObjects(Find_OCF(OCF_CrewMember), Find_Team(iTeam), Find_Func("StillAlive"))) {
		var iLivingPlr = pLivingClonk -> GetOwner(), szLivingPlr = GetPlayerName(iLivingPlr);
		AddMenuItem(szLivingPlr, Format("SetPlrView(%d, Object(%d))", iPlr, ObjectNumber(pLivingClonk)), 0, pClonk, 0, 0, Format("Sicht auf Spieler %s stellen", szLivingPlr), 4, pLivingClonk); 
	}
	SelectMenuItem(iSelection, pClonk);
	ScheduleCall(this, "ShowMenu", 10, 0, pClonk);
}

protected func ContainedDown() {
	[Nicht verlassen|Method=None]
	return 1;
}

