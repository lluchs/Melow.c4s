/*--- In der Tiefe ---*/

#strict 2

/* Initialisierung */

protected func Initialize()
  {
  // Leuchten von all der Lava
  SetGamma(0, RGB(160,110,90), RGB(255,255,255));
  // Fahrzeuge in die Höhle
  var cnt=Random(5); while (++cnt<8) PlaceVehicle();
  var iX, iY, ID;
  for(var i = 0; i < RandomX(5, 20); i++) {
  	ID = IDOL;
  	if(!Random(3))
  		ID = GIDL;
  	if(!Random(2) || !FindVehPos(iX, iY))
  		PlaceVegetation(ID, 25, 300, LandscapeWidth()-50,LandscapeHeight()-350, 100000);
  	else
  		CreateObject(ID, iX, iY, NO_OWNER);
  }
  // Statuenteile erzeugen
  for(var i=0; i<2; i++) {
  	CreateStatuePart(_PA1); CreateStatuePart(_PA2); CreateStatuePart(_PA3);
  	CreateStatuePart(_PA4); CreateStatuePart(_PA5); CreateStatuePart(_PA6);
  }
  
  aPlayers = [[], []];
  aLorrys = CreateArray(2);
  aRelaunches = CreateArray(2);
  aMarkable = CreateArray(2);
  // Fertig
  return 1;
  }

global func CreateStatuePart(id idPart)
  {
  // Manchmal im Container erzeugen
  var cont;
  if (!Random(5))
    if (cont = FindObject(RndStatContID(), Random(LandscapeWidth()), Random(LandscapeHeight()), -1,-1))
      return(CreateContents(idPart, cont));
  // Sonst im freien erzeugen
  var x,y;
  if (!FindVehPos(x,y))
    {
    // Keine Position gefunden: Dann halt irgendwo...
    x=Random(LandscapeWidth()-100)+50;
    y=Random(LandscapeHeight()*2/3-150)+LandscapeHeight()/3+50;
    }
  var pPart = CreateObject(idPart, x, y, -1);
  pPart->SetR(Random(360));
  return(pPart);
  }

// Ein zufälliges Fahrzeug platzieren
private func PlaceVehicle(id idVeh, int x, int y)
  {
  // Nicht gegebene Parameter mit Zufall füllen
  var obj;
  if (!x && !y) if (!FindVehPos(x, y)) return;
  if (!idVeh) idVeh = RndVehID();
  // Fahrzeug erzeugen
  obj=CreateObject(idVeh,x,y,-1);
  // Haubitze: Schießpulver
  if (GetID(obj)==CANN) while (Var()++<5) CreateContents(GUNP,obj);
  // Armbrust: Pfeile und sonst nix
  if (GetID(obj)==XBOW) { while (Var()++<10) CreateContents(XARW,obj); return(obj); }
  // Normale Objekte mit zufälligem Material
  var cnt; while(++cnt<Random(50)) CreateContents(RndCntID(obj),obj);
  // Loren kriegen immer Standardmaterial
  if (GetID(obj)==LORY) AddEffect("IntLorryFill", obj, 1, 10000);
  return obj;
  }

// Position für ein Fahrzeug suchen
// PlaceAnimal ginge; aber dann stecken sie ja nicht so schön an der Decke =)
global func FindVehPos(&x, &y)
  {
  var i;
  while (i++<25)
    {
    x=Random(LandscapeWidth()-50)+25;
    y=Random(LandscapeHeight()-350)+300;
    if (GBackSolid(x, y-25)) y+=25;
    if (!GBackSolid(x, y) && !GBackLiquid(x, y)) return(true);
    }
  return(false);
  }

// Zufällige Fahrzeug-ID
private func RndVehID()
  {
  var num;
  if (!(num=Random(6))) return(LORY);
  if (!--num)           return(CATA);
  if (!--num)           return(CANN);
  if (!--num)           return(WAGN);
  if (!--num)           return(XBOW);
                        return(HUT1);
  }

// Zufälliges Materialteil
global func RndCntID()
  {
  var num;
  if (!(num=Random(25))) return(SFLN);
  if (!--num)            return(STFN);
  if (!--num)            return(GUNP);
  if (!--num)            return(EFLN);
  if (!--num)            return(FBMP);
  if (!--num)            return(WBRL);
  if (!--num)            return(CNKT);
  if (!(num=Random(4)))  return(FLNT);
  if (!--num)            return(LOAM);
  if (!--num)            return(TFLN);
                         return(METL);
  }

// Zufälliger Container für ein Statuenteil
global func RndStatContID()
  {
  if (!Random(2)) return(HUT1); return(WAGN);
  }



/* Spielerinitialisierung */

static aPlayers, aLorrys, aRelaunches, aMarkable;
protected func InitializePlayer(int iPlr) {
	var iTeam = GetPlayerTeam(iPlr);
	iTeam--;
	PushBack(iPlr, aPlayers[iTeam]);
	if(GetLength(aPlayers[iTeam]) == 1) {
  	var pLorry = PlaceLorry();
  	if(OtherLorry(iTeam + 1)) {
  		for(var i = 0; (pLorry -> ObjectDistance(OtherLorry(iTeam + 1)) < 200); i++) {
  			if(pLorry)
  				pLorry -> RemoveObject();
  			pLorry = PlaceLorry();
  			if(i > 50)
  				break;
  		}
  	}
  	pLorry -> SetPosition(pLorry -> GetX(), pLorry -> GetY() - 6);
  	var iX = pLorry -> GetX(), iY = pLorry -> GetY();
  	CreateObject(WIPF, iX, iY, NO_OWNER) -> SetClrModulation(GetTeamColor(iTeam + 1));
  	pLorry -> SetTeam(iTeam + 1);
  	aLorrys[iTeam] = pLorry;
  	// Objektversorgung
  	AddEffect("IntTeamLorryFill", pLorry, 100, 5000, 0, 0, iTeam);
  	if(pLorry -> Stuck())
  		ShakeFree(iX, iY, GetDefWidth(pLorry -> GetID()));
  	ScheduleGameCall("StuckCheck", 30, 0, GetCrew(iPlr));
	}
	else
		var pLorry = aLorrys[iTeam];
	GetCrew(iPlr) -> SetPosition(pLorry -> GetX(), pLorry -> GetY());
	AddEffect("BanBurnPotion", GetCrew(iPlr), 210, 1, 0, PFIR, PFIR -> EffectDuration());
}

public func StuckCheck(object pClonk) {
	  if(pClonk -> Stuck() && !(pClonk -> Contents()))
  		pClonk -> CreateContents(TFLN);
}

static iPlaceLorryY;
private func PlaceLorry() {
	if(!iPlaceLorryY)
		iPlaceLorryY = RandomX(LandscapeHeight() / 3, LandscapeHeight() - (LandscapeHeight() / 3));
	return PlaceVegetation(LORY, LandscapeWidth() / 4, iPlaceLorryY, LandscapeWidth() / 2, LandscapeHeight() / 4, 100000);
}

protected func RemovePlayer(int iPlr, int iTeam) {
	// Spiel vorbei? -> egal
	if(fGameOver)
		return;
	var pClonk = GetCrew(iPlr);
	// Clonk wartete auf Relaunch? -> nichts tun
	if(!pClonk -> StillAlive(iTeam))
		return;
	// wartet ein Teammitglied auf Relaunch?
	var pWaitingClonk = FindObject2(Find_OCF(OCF_CrewMember), Find_Team(iTeam), Find_Func("Waits4Relaunch"), Sort_Func("ObjectNumber"));
	if(pWaitingClonk) {
		// Balancerelaunch abziehen, da direkt gerelauncht wird
		aMarkable[iTeam - 1]--;
		// an Stelle des alten Clonks einsetzen!
		var pObj = pWaitingClonk -> Contained();
		pWaitingClonk -> Exit();
		pWaitingClonk -> SetPosition(pClonk -> GetX(), pClonk -> GetY());
		SetViewCursor(pWaitingClonk -> GetOwner(), pWaitingClonk);
		SetCursor(pWaitingClonk -> GetOwner(), pWaitingClonk, true);
		pWaitingClonk -> DoEnergy(100);
		pWaitingClonk -> Extinguish();
		pObj -> RemoveObject();
		// Effekte und so
		pWaitingClonk -> Sound("Magic1");
		pWaitingClonk -> CastParticles("MSpark", 5, 50, 0,0, 100, 200, RGBa(255,100,100,128), RGBa(255,0,0,0));
		Log("<c %x>Spieler %s verlässt das Spiel und wird daher durch %s ersetzt!</c>", GetTeamColor(iTeam), GetPlayerName(iPlr), GetPlayerName(pWaitingClonk -> GetOwner()));
		return 1;
	}
	else {
		// Relaunch geben
		aMarkable[iTeam - 1]++;
		Log("<c %x>Balance (wegen Leaven von %s): Team %d kann ein Statuenteil mehr markieren!</c>", GetTeamColor(iTeam), GetPlayerName(iPlr), iTeam);
		return 1;
	}
}

static fGameOver;
protected func OnClonkDeath(object pClonk) {
	if(!GetPlayerName(pClonk -> GetOwner()))
		return;
	var pStatuePart = FindObject2(Find_Func("IsHolyStatuePart"), Find_Func("IsMarkedFor", GetPlayerTeam(pClonk -> GetOwner())), Sort_Random());
	if(!pStatuePart && EliminationCheck(GetPlayerTeam(pClonk -> GetOwner())))
		return;
	var pNewClonk = CreateObject(CLNK, 0, 0, pClonk -> GetOwner());
	pNewClonk -> GrabObjectInfo(pClonk);
	pNewClonk -> DoEnergy(100);
	if(pStatuePart) {
		pStatuePart -> RejoinClonk(0, -1, pNewClonk);
		return 1;
	}
	AddEffect("Anti", pNewClonk, 300, 10);
	pNewClonk -> Enter(CreateObject(TIM1, pClonk -> GetX(), pClonk -> GetY(), NO_OWNER));
	Log("<c %x>Spieler %s wartet auf einen Relaunch (Statuenteil anfassen + Doppelgraben)!</c>", GetTeamColor(GetPlayerTeam(pClonk -> GetOwner())), GetPlayerName(pClonk -> GetOwner()));
	SetCursor(pClonk -> GetOwner(), pClonk, 1, 1);
}

private func EliminationCheck(int iTeam) {
	if(!ObjectCount2(Find_OCF(OCF_CrewMember), Find_Team(iTeam), Find_Func("StillAlive"))) {
		EliminateTeam(iTeam);
		fGameOver = 1;
		return 1;
	}
}

global func FxAntiDamage() {
	return;
}

global func FxAntiEffect(string szNewEffect) {
	// Feuer abblocken
	if(WildcardMatch(szNewEffect, "*Fire*"))
		return -1;
	// Alles andere ist OK
	return;
}

global func FxAntiTimer(object pTarget) {
	if(!(pTarget -> Contained()))
		return -1;
}

global func StillAlive() {
	if(!Contained() || Contained() -> GetID() != TIM1)
		return 1;
}

global func EliminateTeam(int iTeam) {
	for(var iPlr in GetPlayerByTeam(iTeam))
		EliminatePlayer(iPlr);
}



/* Automatisches Füllen der Anfangslore */

global func FxIntTeamLorryFillStart(object pTarget, int iEffectNumber, int iTemp, int iTeam) {
	if(!iTemp) {
		EffectVar(0, pTarget, iEffectNumber) = iTeam;
		Schedule(Format("FxIntTeamLorryFillTimer(Object(%d), %d)", ObjectNumber(pTarget), iEffectNumber), 5);
	}
}

global func OtherTeam(int iTeam) {
	// \o/
	return (!(iTeam - 1)) + 1;
}

global func OtherLorry(int iTeam) {
	return aLorrys[OtherTeam(iTeam) - 1];
}

global func FxIntTeamLorryFillTimer(object pTarget, int iEffectNumber, int iEffectTime) {
	var iTeam = EffectVar(0, pTarget, iEffectNumber), aFillIDs = [[WBRL, 1, 1], [LOAM, 10, 3], [METL, 7], [FLNT, 10, 3], [CBRD, GetLength(aPlayers[iTeam])], [SFLN, 5]], aSpecialFill = [[CNKT, 2], [EFLN, 2], [BRED, GetLength(aPlayers[iTeam])]], iMin;
	if(OtherLorry(iTeam) && ((OtherLorry(iTeam) -> GetY()) < (pTarget -> GetY()))) {
		iMin = RandomX(1, 7);
		for(var aID in aSpecialFill) {
			if(ObjectCount2(Find_Container(pTarget), Find_ID(aID[0])) < aID[1])
				pTarget -> CreateContents(aID[0], 0, Random(aID[1]));
		}
		iMin = Random(5);
	}
	var iMin2;
	for(var aID in aFillIDs) {
		iMin2 = iMin;
		if(!iMin2)
			iMin2 = aID[2];
		if(ObjectCount2(Find_Container(pTarget), Find_ID(aID[0])) < aID[1])
			pTarget -> CreateContents(aID[0], 0, RandomX(iMin2, aID[1]));
	}
	
  Sparkle(5, pTarget -> GetX(), pTarget -> GetY());
}

global func FxIntLorryFillTimer(object pTarget, int iEffectNumber) {
	var cnt; while(++cnt<Random(50)) pTarget -> CreateContents(RndCntID());
	Sparkle(5, pTarget -> GetX(), pTarget -> GetY());
}
