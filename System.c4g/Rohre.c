/* Zufluss-/Abflussrohr */

#strict 2

#appendto SPIP
#appendto DPIP

public func LineBreak() {
	_inherited(true);
	var pPump = GetActionTarget(0), pLinekit = GetActionTarget(1);
	if(pPump) {
		pPump -> CreateLinekit(GetID());
		var iX1 = pPump -> GetX(), iY1 = pPump -> GetY(), iX2, iY2;
		if(pLinekit) {
			iX2 = pLinekit -> GetX();
			iY2 = pLinekit -> GetY();
		}
		CustomMessage("$TxtLinebroke$", pPump, NO_OWNER, iX2 - iX1, iY2 - iY1);
	}
	if(pLinekit)
		pLinekit -> RemoveObject();
	return 1;
}
