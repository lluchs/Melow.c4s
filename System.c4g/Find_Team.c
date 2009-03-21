/*-- Find_Team --*/

#strict 2

global func Find_Team(int iTeam) {
	var p = [C4FO_Or];
	for(var i = -1; i < GetPlayerCount(); i++)
		if(GetPlayerTeam(GetPlayerByIndex(i)) == iTeam)
			p[GetLength(p)] = Find_Owner(GetPlayerByIndex(i));
	return p;
}
