/* Zuflussrohr */

#strict

#appendto SPIP

protected func Transfer()
{
  // Zielobjekt suchen
  var pPumpTarget = GetActionTarget(0);
  // Kein Zielobjekt  
  if (!pPumpTarget) return(0);
  // Zielobjekt akzeptiert keinen Transfer
  if (!pPumpTarget->~AcceptTransfer()) return(0);
  // Zielobjekt hat nicht genug Energie (könnte man auch in AcceptTransfer verlegen...)
  if (!EnergyCheck(1000,pPumpTarget)) return(0);
  // Materialtransfer
  for (var i = 0; i < iPumpSpeed; i++)
  {
    var mat = ObjectExtractLiquid(GetActionTarget(1));
    if (mat == -1) mat = ExtractLiquid(GetVertex(50, 0), GetVertex(50, 1));
    if(mat == -1)
    	break;
    ObjectInsertMaterial(mat, pPumpTarget);
  }
  // Blubbern  
  if (!Random(5)) Bubble(GetVertex(50,0), GetVertex(50,1));
}
