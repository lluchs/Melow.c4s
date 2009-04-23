/* Abflussrohr */

#strict

#appendto DPIP

private func Transfer() // Abflussrohr führt zur Zeit keine Prüfung von AcceptTransfer beim Zielobjekt durch...
{
  var i;
  // Angeschlossene Objekte ermitteln
  var pPumpSource = GetActionTarget(0);
  var pPumpTarget = GetActionTarget(1);
  // Kein Quellobjekt
  if (!pPumpSource) return(0);
  // Quellobjekt hat nicht genug Energie
  if (!EnergyCheck(1000, pPumpSource)) return(0);
  // Zielobjekt vorhanden
  if (pPumpTarget)
  {
    // Materialtransfer
    for (i = 0; i < iPumpSpeed; i++)  
      if(!ObjectInsertMaterial(ObjectExtractLiquid(pPumpSource), pPumpTarget))
      	break;
  }
  // Kein Zielobjekt vorhanden
  else
  {
    // Materialtransfer
    for (i = 0; i < iPumpSpeed; i++)    
      if(!InsertMaterial(ObjectExtractLiquid(pPumpSource), GetVertex(50,0), GetVertex(50,1)))
      	break;
  }
}
