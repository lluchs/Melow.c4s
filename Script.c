/*--- In der Tiefe ---*/

#strict 2

/* Initialisierung */

protected func Initialize()
  {
  // Leuchten von all der Lava
  SetGamma(0, RGB(160,110,90), RGB(255,255,255));
  
  aPlayers = [[], []];
  aMarkable = CreateArray(2);
  // Fertig
  return 1;
  }
  
static fGameStarted;
  
public func StartGame() {
	DrawDefMap(0, 0, LandscapeWidth(), LandscapeHeight(), "CaveParcour");
	
	for(var pObj in FindObjects(Find_Or(Find_ID(LORY), Find_ID(WIPF))))
		pObj -> RemoveObject();
	aLorrys = CreateArray(2);
	
	if(fReflection){
		var aPos=SearchLorryPosition();
		if(GetType(aPos)!=C4V_Array){
			StartGame();
			return;
		}
		for(var i=0; i<5000; i++){
			if(Distance(aPos[0], aPos[1], LandscapeWidth() - aPos[0], aPos[1]) > iMinLorryDistance){ //We have a Winner!
				PlaceLorry(aPos, 1);
				PlaceLorry([LandscapeWidth() - aPos[0], aPos[1]], 2);
				break;
			}
			aPos=SearchLorryPosition();
		}
	} else {
		var aPositions=CreateArray(), aCurrPos;
		var fWin;
		for(var i=0; i<1500; i++){
			aCurrPos=SearchLorryPosition();
			if(GetType(aCurrPos)!=C4V_Array){
				StartGame();
				return;
			}
			PushBack(aCurrPos,aPositions);
			for(var j=GetLength(aPositions)-2; j>=0; j--){
				if(Distance(aPositions[j][0],aPositions[j][1], aCurrPos[0], aCurrPos[1]) > iMinLorryDistance){ //We have a Winner!
					PlaceLorry(aPositions[j], 1);
					PlaceLorry(aCurrPos, 2);
					fWin=true;
					break;
				}
			}
			if(fWin) break;
		}
		if(!fWin){
			StartGame();
			return;
		}
	}
	
	aRelaunches = CreateArray(GetPlayerCount() + 2);
	aDeaths = CreateArray(GetPlayerCount() + 2);
	
	InitScoreboard();
	
	for(var i = 0; i < GetPlayerCount(); i++) {
		if(InitializePlayer2(GetPlayerByIndex(i)) == -1)
			return;
	}
	
	UpdateTeamScoreboard(1);
	UpdateTeamScoreboard(2);
	
	// Vegetation
	for(var i = 0; i < 5; i++) {
		if(Random(2))
			PlaceVegetation(BONE, 0, 0, LandscapeWidth(), LandscapeHeight(), 100000);
		if(Random(2))
			PlaceVegetation(SKUL, 0, 0, LandscapeWidth(), LandscapeHeight(), 100000);
		if(Random(2))
			PlaceVegetation(TRE2, 0, 0, LandscapeWidth(), LandscapeHeight(), 100000);
	}
	
	// Objekte in der Erde
	PlaceObjects(FLNT, 5 * RandomX(5, 10), "Earth");
	PlaceObjects(LOAM, 20 * RandomX(2, 4), "Earth");
	PlaceObjects(EFLN, 1 * RandomX(1, 10), "Earth");
	PlaceObjects(METL, 2 * RandomX(6, 10), "Earth");
	PlaceObjects(ZAPN, 3 * RandomX(1, 3), "Earth");
	PlaceObjects(FMEG, 3 * RandomX(1, 3), "Earth");
	PlaceObjects(MEGG, 3 * RandomX(1, 3), "Earth");
	PlaceObjects(CNKT, 5 * RandomX(2, 4), "Earth");
	PlaceObjects(TFLN, 5 * RandomX(4, 8), "Earth");
	PlaceObjects(STFN, 2 * RandomX(4, 8), "Earth");
	PlaceObjects(GUNP, 2 * RandomX(5, 8), "Earth");
	PlaceObjects(CNCR, 5 * RandomX(4, 8), "Earth");
	PlaceObjects(LIQG, 5 * RandomX(4, 8), "Earth");
	
	// Tiere
	PlaceAnimal(BIRD);
	
  // Fahrzeuge in die Höhle
  var cnt=Random(5); while (++cnt<13) PlaceVehicle();
  var iX, iY, ID;
  for(var i = 0; i < RandomX(5, 10); i++) {
  	ID = IDOL;
  	if(!Random(3))
  		ID = GIDL;
  	if(!Random(2) || !FindVehPos(iX, iY))
  		PlaceVegetation(ID, 25, 300, LandscapeWidth()-50,LandscapeHeight()-350, 100000);
  	else
  		CreateObject(ID, iX, iY, NO_OWNER);
  }
  
  if(fReflection) {
		// Landschaft spiegeln
		Log("Landschaft wird gespiegelt, bitte warten...");
		var iX = 0, iY = 0, iInc = LandscapeWidth() / 2 / 10, iLog = 1;
		for(var x=0; x < LandscapeWidth() / 2; x++) {
		 if(x == (iLog * iInc)) {
		 	Log("%d%%", iLog * 10);
		 	iLog++;
		 }
		 for(var y=0; y < LandscapeHeight(); y++)
		 	if(GetMaterial(x+iX, y+iY) && GetTexture(x+iX, y+iY))
		   DrawMaterialQuad (Format("%s-%s", MaterialName(GetMaterial(x+iX, y+iY)), GetTexture(x+iX, y+iY)), LandscapeWidth()-x-iX, y+iY, LandscapeWidth()-x-iX, y+iY+1, LandscapeWidth()-x-iX+1, y+iY+1, LandscapeWidth()-x-iX+1, y+iY, !GBackSky(x+iX, y+iY));
		 }
		// alle Objekte auf der rechten Seite entfernen
		Log("Objekte werden gespiegelt...");
		for(var pObj in FindObjects(Find_InRect(LandscapeWidth() / 2, 0, LandscapeWidth() / 2, LandscapeHeight()), Find_Or(Find_Category(C4D_Structure), Find_Category(C4D_Vehicle), Find_Category(C4D_Living), Find_Category(C4D_Object), Find_Func("IsTree")), Find_Not(Find_OCF(OCF_CrewMember)), Find_Not(Find_Func("IsTeamLorry")), Find_Not(Find_ID(WIPF))))
			pObj -> RemoveObject();
		// und durch die linken Objekte ersetzen
		for(var pObj in FindObjects(Find_NoContainer(), Find_InRect(0, 0, LandscapeWidth() / 2, LandscapeHeight()), Find_Or(Find_Category(C4D_Structure), Find_Category(C4D_Vehicle), Find_Category(C4D_Living), Find_Category(C4D_Object), Find_Func("IsTree")), Find_Not(Find_OCF(OCF_CrewMember)), Find_Not(Find_Func("IsTeamLorry")), Find_Not(Find_ID(WIPF))))
			CreateObject(pObj -> GetID(), LandscapeWidth() - pObj -> GetX(), pObj -> GetY() - GetDefCoreVal("Offset", "DefCore", pObj -> GetID(), 1), pObj -> GetOwner());
		var pNewObj, pContainer;
		for(var pObj in FindObjects(Find_AnyContainer(), Find_InRect(0, 0, LandscapeWidth() / 2, LandscapeHeight()), Find_Or(Find_Category(C4D_Structure), Find_Category(C4D_Vehicle), Find_Category(C4D_Living), Find_Category(C4D_Object), Find_Func("IsTree")))) {
			pNewObj = CreateObject(pObj -> GetID(), LandscapeWidth() - pObj -> GetX(), pObj -> GetY() - GetDefCoreVal("Offset", "DefCore", pObj -> GetID(), 1), pObj -> GetOwner());
			pContainer = pObj -> Contained();
			pNewObj -> Enter(FindObject2(Find_ID(pContainer -> GetID()), Find_AtPoint(LandscapeWidth() - pContainer -> GetX(), pContainer -> GetY())));
		}
  }
  
  // Statuenteile erzeugen
  for(var i=0; i<2; i++) {
  	CreateStatuePart(_PA1); CreateStatuePart(_PA2); CreateStatuePart(_PA3);
  	CreateStatuePart(_PA4); CreateStatuePart(_PA5); CreateStatuePart(_PA6);
  }
  fGameStarted = true;
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
  if (!(num=Random(8))) return(HUT1);
  if (!--num)           return(CATA);
  if (!--num)           return(CANN);
  if (!--num)           return(WAGN);
  if (!--num)           return(XBOW);
                        return(LORY);
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

static fPump, iPumpSpeed, fCorpseRelaunch, fReflection, iMinLorryDistance, iMarkable;

protected func InitializePlayer(int iPlr) {
	if(fGameStarted)
		return InitializePlayer2(iPlr);
	var pDummy = CreateObject(TIM1, 10, 10, iPlr);
	pDummy -> NoMenu();
	GetCrew(iPlr) -> Enter(pDummy);
	AddEffect("Anti", GetCrew(iPlr), 300, 10);
	if(iPlr)
		return;
	var opt = CreateMenuOptions();
	SetGeneralMenuOptions(opt, GetCrew(iPlr), 0, "ApplySettings", LBRL, "Einstellungen", MS4C_Verbose_GlobalMessage | MS4C_Verbose_Log);
	AddBoolChoice(opt, "fPump", "Pumpe baubar", PUMP, false);
	AddRangeChoice(opt, "iPumpSpeed", "Pumpgeschwindigkeit", PUMP, 10, 200, 10, 50); //Min, Max, Step, Default
	AddBoolChoice(opt, "fCorpseRelaunch", "Leichenrelaunch", 0, true);
	AddBoolChoice(opt, "fReflection", "Landschaftsspiegelung", 0, false);
	AddRangeChoice(opt, "iMinLorryDistance", "Mindestabstand der Loren", LORY, 0, 700, 100, 200); //Min, Max, Step, Default
	AddRangeChoice(opt, "iMarkable", "Anzahl markierbarer Teile", C4Id(Format("_PA%d", RandomX(1, 6))), 0, 12, 1, 1); //Min, Max, Step, Default
	CreateMenuByOptions(opt);
}

public func ApplySettings(hash, data) {
	var iter = HashIter(hash); // Iterator erzeugen
	var node;
	while(node = HashIterNext(iter)) { // Solange es weitere Elemente gibt
		GlobalN(node[0]) = node[1];
	}
	StartGame();
}

global func & Relaunches(int iPlr) { return aRelaunches[GetPlayerID(iPlr) + 2]; }
global func & Deaths(int iPlr) { return aDeaths[GetPlayerID(iPlr) + 2]; }
global func & TeamRelaunches(int iTeam) { return aRelaunches[iTeam - 1]; }
global func & TeamDeaths(int iTeam) { return aRelaunches[iTeam -1]; }

global func IncRelaunches(int iPlr) {
	Relaunches(iPlr)++;
	TeamRelaunches(GetPlayerTeam(iPlr))++;
	return 1;
}
global func IncDeaths(int iPlr) {
	Deaths(iPlr)++;
	TeamDeaths(GetPlayerTeam(iPlr))++;
	return 1;
}

static aPlayers, aLorrys, aRelaunches, aDeaths, aMarkable;
protected func InitializePlayer2(int iPlr) {
	if(fPump) // Pumpe-baubar-Option
		SetPlrKnowledge(iPlr, PUMP);
	var pObj = GetCrew(iPlr) -> Contained();
	if(pObj) {
		GetCrew(iPlr) -> Exit();
		pObj -> RemoveObject();
	}
	var iTeam = GetPlayerTeam(iPlr);
	iTeam--;
	PushBack(iPlr, aPlayers[iTeam]);
	var pLorry = aLorrys[iTeam];
	GetCrew(iPlr) -> SetPosition(pLorry -> GetX(), pLorry -> GetY());
	if(!GetEffect("BanBurnPotion", GetCrew(iPlr)))
		AddEffect("BanBurnPotion", GetCrew(iPlr), 210, 1, 0, PFIR, PFIR -> EffectDuration());
	UpdatePlayerScoreboard(iPlr);
}

public func StuckCheck(object pClonk) {
	  if(pClonk -> Stuck() && !(pClonk -> Contents()))
  		pClonk -> CreateContents(TFLN);
}

private func PlaceLorry(array aPosition, int iTeam) {
	var pLorry = CreateObject(LORY, aPosition[0], aPosition[1], NO_OWNER);
	pLorry -> SetTeam(iTeam);
	aLorrys[iTeam - 1] = pLorry;
  	// Objektversorgung
  	AddEffect("IntTeamLorryFill", pLorry, 100, 5000, 0, 0, iTeam);
  	if(pLorry -> Stuck())
  		ShakeFree(pLorry -> GetX(), pLorry -> GetY(), GetDefWidth(pLorry -> GetID()));
  	if(pLorry -> Stuck())
  		CreateObject(ROCK, pLorry -> GetX(), pLorry -> GetY(), NO_OWNER) -> Explode(GetDefWidth(pLorry -> GetID()));
}

static iPlaceLorryY;
private func SearchLorryPosition(){
	if(!iPlaceLorryY)
		iPlaceLorryY = RandomX(LandscapeHeight() / 5, LandscapeHeight() - (LandscapeHeight() / 3));
	var iX = LandscapeWidth() / 4, iY = iPlaceLorryY, iWdt = LandscapeWidth() / 2, iHgt = LandscapeHeight() / 4;
	if(fReflection) {
		iX = LandscapeWidth() / 6;
		iWdt = LandscapeWidth() / 2 - LandscapeWidth() / 4;
	}
	var iRX, iRY;
	for(var i = 0; i < 50000; i++) {
		iRX = RandomX(iX, iX + iWdt);
		iRY = RandomX(iY, iY + iHgt);
		if(GetMaterial(iRX, iRY) != Material("Tunnel"))
			continue;
		while(GetMaterial(iRX, iRY) == Material("Tunnel")) 
			iRY += 2;
		iRY -= 5;
		return([iRX, iRY]);
	}
	return 0;
}

protected func RemovePlayer(int iPlr, int iTeam) {
	// Spiel vorbei? -> egal
	if(fGameOver)
		return;
	// im Scoreboard markieren
	SetScoreboardData(GetPlayerID(iPlr) + 2, 0, "{{DOOR}}", iTeam * 2 + 1);
	
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
	IncDeaths(pClonk -> GetOwner());
	var pStatuePart = FindObject2(Find_Func("IsHolyStatuePart"), Find_Func("IsMarkedFor", GetPlayerTeam(pClonk -> GetOwner())), Sort_Random());
	if(pStatuePart || !EliminationCheck(GetPlayerTeam(pClonk -> GetOwner()), pClonk -> GetOwner())) {
		var pNewClonk = CreateObject(CLNK, 0, 0, pClonk -> GetOwner());
		pNewClonk -> GrabObjectInfo(pClonk);
		pNewClonk -> DoEnergy(100);
		if(pStatuePart) {
			pStatuePart -> RejoinClonk(0, -1, pNewClonk);
		}
		else {
			AddEffect("Anti", pNewClonk, 300, 10);
			pNewClonk -> Enter(CreateObject(TIM1, pClonk -> GetX(), pClonk -> GetY(), NO_OWNER));
			Log("<c %x>Spieler %s wartet auf einen Relaunch (Statuenteil anfassen + Doppelgraben)!</c>", GetTeamColor(GetPlayerTeam(pClonk -> GetOwner())), GetPlayerName(pClonk -> GetOwner()));
			SetCursor(pClonk -> GetOwner(), pClonk, 1, 1);
		}
	}
	UpdatePlayerScoreboard(pClonk -> GetOwner());
	UpdateTeamScoreboard(GetPlayerTeam(pClonk -> GetOwner()));
}

private func EliminationCheck(int iTeam, int iLastPlr) {
	if(!ObjectCount2(Find_OCF(OCF_CrewMember), Find_Team(iTeam), Find_Func("StillAlive"))) {
		UpdatePlayerScoreboard(iLastPlr);
		UpdateTeamScoreboard(GetPlayerTeam(iLastPlr));
		DoEvaluation();
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

public func InitScoreboard() {
	SetScoreboardData(SBRD_Caption, SBRD_Caption, "Scoreboard");
	SetScoreboardData(SBRD_Caption, 0, "{{CLNK}}"); // Spalte 0: Status
	SetScoreboardData(SBRD_Caption, 1, Format("{{_PA%d}}", RandomX(5, 6))); // Spalte 1: Relaunches
	SetScoreboardData(SBRD_Caption, 2, "{{SKUL}}"); // Spalte 2: Tode
	
	SetScoreboardData(0, SBRD_Caption, Format("<c %x>{{LORY}}</c>", GetTeamColor(1))); // Team Rot (1)
	SetScoreboardData(0, 0, " ", 2);
	SetScoreboardData(1, SBRD_Caption, Format("<c %x>{{LORY}}</c>", GetTeamColor(2))); // Team Grün (2)
	SetScoreboardData(1, 0, " ", 4);
}

global func UpdatePlayerScoreboard(int iPlr) {
	var iTeam = GetPlayerTeam(iPlr), pClonk = GetCrew(iPlr);
	
	// Spielername
	SetScoreboardData(GetPlayerID(iPlr) + 2, SBRD_Caption, Format("<c %x>%s</c>", GetTeamColor(iTeam), GetPlayerName(iPlr)));
	
	// Status
	if(!pClonk || pClonk -> Waits4Relaunch()) // wartet auf Relaunch/ist tot?
		SetScoreboardData(GetPlayerID(iPlr) + 2, 0, "{{SKUL}}", iTeam * 2 + 1);
	else if(pClonk -> OnFire()) // brennt?
		SetScoreboardData(GetPlayerID(iPlr) + 2, 0, "{{FLAM}}", iTeam * 2 + 1);
	else // alles OK :(
		SetScoreboardData(GetPlayerID(iPlr) + 2, 0, "{{CLNK}}", iTeam * 2 + 1);
	
	// Relaunches
	SetScoreboardData(GetPlayerID(iPlr) + 2, 1, Format("%d", Relaunches(iPlr)), Relaunches(iPlr));
	
	// Tode
	SetScoreboardData(GetPlayerID(iPlr) + 2, 2, Format("%d", Deaths(iPlr)), Deaths(iPlr));
	
	// Neu Sortieren
	SortScoreboard(2); // Tode (je weniger, desto höher)
	SortScoreboard(1, true); // Relaunches
	SortScoreboard(0); // Team (zuerst Team 1, dann 2)
	
	return 1;
}

global func UpdateTeamScoreboard(int iTeam) {
	var iRelaunches = TeamRelaunches(iTeam), iDeaths = TeamDeaths(iTeam);
	
	var szMarked = Format("<c %x>", GetTeamColor(iTeam));
	for(var pPart in FindObjects(Find_Func("IsHolyStatuePart"), Find_Func("IsMarkedFor", iTeam))) {
		szMarked = Format("%s{{%i}} ", szMarked, pPart -> GetID());
	}
	szMarked = Format("%s</c>", szMarked);
	
	// Markierte Statuenteile
	SetScoreboardData(iTeam - 1, 0, szMarked, iTeam * 2);
	
	// Relaunches
	SetScoreboardData(iTeam - 1, 1, Format("<i>%d</i>", iRelaunches), iRelaunches + 1);
	
	// Tode
	SetScoreboardData(iTeam - 1, 2, Format("<i>%d</i>", iDeaths), -1);
	
	return 1;
}

/* Evaluation */

public func DoEvaluation() {
	var i = 1;
	for(var iRelaunches in aRelaunches) {
		if(iRelaunches)
			AddEvaluationData(Format("{{_PA%d}}Relaunches: %d", iRelaunches), i);
		i++;
	}
	
	i = 1;
	for(var iDeaths in aDeaths) {
		if(iDeaths)
			AddEvaluationData(Format("{{SKUL}}Tode: %d", iDeaths), i);
		i++;
	}
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
	var iTeam = EffectVar(0, pTarget, iEffectNumber), aFillIDs = [[WBRL, 1, 1], [LOAM, 10, 3], [METL, 7], [FLNT, 10, 3], [CBRD, GetLength(aPlayers[iTeam - 1]) + 1, 1], [SFLN, 4], [STFN, 4]], aSpecialFill = [[CNKT, 2], [EFLN, 2], [BRED, GetLength(aPlayers[iTeam - 1])]], iMin;
	if(OtherLorry(iTeam) && ((OtherLorry(iTeam) -> GetY()) < (pTarget -> GetY())) && ((pTarget -> GetY()) - (OtherLorry(iTeam) -> GetY()) > 100)) {
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
