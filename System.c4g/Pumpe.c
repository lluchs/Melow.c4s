/* Pumpe */

#strict 2

#appendto PUMP

protected func Initialize() {
	_inherited(...);
	CreateLinekit(SPIP);
	CreateLinekit(DPIP);
}

public func CreateLinekit(id ID) {
	var iX = -10;
	if(ID == DPIP)
		iX = Abs(iX);
	var pLinekit = CreateObject(LNKT, iX, GetDefHeight(GetID()) / 2, GetOwner());
	CreateLine(ID, pLinekit);
	return 1;
}

private func CreateLine(id ID, object pLinekit) {
  var pLine = CreateObject(ID, 0, 0, GetOwner());
  pLine -> SetAction("Connect", this, pLinekit);
  Sound("Connect");
  return pLine;
}
