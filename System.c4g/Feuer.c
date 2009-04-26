/* Feuereffekt */

#strict 2

global func FxFireStart(object pTarget) {
	if(pTarget -> ~IsClonk())
		UpdatePlayerScoreboard(pTarget -> GetOwner());
	return _inherited(pTarget, ...);
}

global func FxFireStop(object pTarget) {
	if(pTarget -> ~IsClonk() && pTarget -> GetAlive())
		Schedule(Format("UpdatePlayerScoreboard(%d)", pTarget -> GetOwner()), 1);
	return _inherited(pTarget, ...);
}
