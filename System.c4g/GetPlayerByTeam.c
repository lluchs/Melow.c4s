#strict 2

global func GetPlayerByTeam(int iTeam) {
	if(!GetTeamName(iTeam))
		return [];
	var aPlayers = CreateArray();
	for(var i = 0; i < GetPlayerCount(); i++)
		if(GetPlayerTeam(GetPlayerByIndex(i)) == iTeam)
			aPlayers[GetLength(aPlayers)] = GetPlayerByIndex(i);
	return aPlayers;
}
/*
for(var i = 0, aPlayers; i < GetTeamCount(); i++)
	if(aPlayers = GetPlayerByTeam(GetTeamByIndex(i)))
		DoWealth(aPlayers[0], 60);
*/
