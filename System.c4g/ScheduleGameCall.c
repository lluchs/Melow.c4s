#strict 2

global func ScheduleGameCall(string szFunction, int iInterval, int iRepeat) {
	szFunction = Format("GameCall(\"%s\"", szFunction);
	var par;
	for(var i = 3; par = Par(i); i++) {
		if(GetType(par) == C4V_C4Object) {
			par = Format("Object(%d)", ObjectNumber(par));
			szFunction = Format("%s, %s", szFunction, par);
			continue;
		}
		//par = CastAny(par);
		szFunction = Format("%s, %v", szFunction, par);
	}
	szFunction = Format("%s)", szFunction);
	return Schedule(szFunction, iInterval, iRepeat);
}
