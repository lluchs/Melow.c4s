/* Menusystem für Luchs, in toll! */

#strict 2

static const MS4C_Typ_Bool  = 0;
static const MS4C_Typ_Enum  = 1;
static const MS4C_Typ_Range = 2;

local pMenuObject;     //Objekt, in dem das Menu gezeigt wird
local pCommandObject;  //Objekt für den Callback (wenn Null, GameCall)
local szCallback;      //Wird in pCommandObject, wenn mit Auswahl fertig
local idSymbol;        //Symbol für CreateMenu
local szCaption;       //Caption für CreateMenu
local hMenu;           //Hash in dem die Werte gespeichert werden
local aSequence;       //Array, um Reihenfolge zu erhalten, macht den Hash irgendwie etwas sinnlos.
local ExtraData;       //ExtraData, wird auch dem Callback übergeben, sinnvoll, um viele Optionsmenüs auf ein mal zu erstellen

//Array indices
static const MS4C_MenuObject_Index    = 0;
static const MS4C_CommandObject_Index = 1;
static const MS4C_Callback_Index      = 2;
static const MS4C_Symbol_Index        = 3;
static const MS4C_Caption_Index       = 4;
static const MS4C_Menu_Index          = 5;
static const MS4C_Sequence_Index      = 6;
static const MS4C_ExtraData_Index     = 7;

static const MS4C_Type_Index          = 0;
static const MS4C_Name_Index          = 1;
static const MS4C_Id_Index            = 2;
static const MS4C_Data_Index          = 3;

//Erstellt Optionen für das Optionsmenusystem :/
global func CreateMenuOptions()
{
  var aOptions=CreateArray(8);
  aOptions[MS4C_Menu_Index]=CreateHash();
  aOptions[MS4C_Sequence_Index]=CreateArray();
  return aOptions;
}

//Setzt allgemeine Optionen für ein Optionsarray, Parameter s. oben
global func SetGeneralMenuOptions(&options, object MenuObject, object CommandObject, string Callback, id Symbol, string Caption)
{
  options[MS4C_MenuObject_Index]=MenuObject;
  options[MS4C_CommandObject_Index]=CommandObject;
  options[MS4C_Callback_Index]=Callback;
  options[MS4C_Symbol_Index]=Symbol;
  options[MS4C_Caption_Index]=Caption;  
}

//Setzt die ExtraData
global func SetExtraData(&options, data)
{
	options[MS4C_ExtraData_Index]=data;
}

//Fügt dem Menü eine Boolauswahlmöglichkeit hinzu, Key wird zur Identifizierung benutzt, sollte eindeutig sein
global func AddBoolChoice(&options, Key, string szName, id idItem, bool fDefault)
{
  if (HashContains(options[MS4C_Menu_Index], Key)) return false;
  HashPut(options[MS4C_Menu_Index],Key,[MS4C_Typ_Bool,szName,idItem,fDefault]);
  PushBack(Key,options[MS4C_Sequence_Index]);
  return true;
}

//Fügt dem Menü eine Enumauswahl hinzu
global func AddEnumChoice(&options, Key, string szName, id idItem)
{
  if (HashContains(options[MS4C_Menu_Index], Key)) return false;
  HashPut(options[MS4C_Menu_Index],Key,[MS4C_Typ_Enum,szName,idItem,[CreateHash(),0,0,[]]]);
  PushBack(Key,options[MS4C_Sequence_Index]);
  return true;
}

//Fügt der Enumauswahl mit dem Key Enumkey eine weitere Möglichkeit hinzu
global func AddEnumChoiceItem(&options, Enumkey, Itemkey, string szName, id idItem, Itemvalue, bool fDefault)
{
  if (!HashContains(options[MS4C_Menu_Index],Enumkey)) return false;
  if (HashContains(HashGet(options[MS4C_Menu_Index],Enumkey)[MS4C_Data_Index][0], Itemkey)) return false;
  HashPut(HashGet(options[MS4C_Menu_Index],Enumkey)[MS4C_Data_Index][0],Itemkey,[szName, idItem, Itemvalue]);
  if (!HashGet(options[MS4C_Menu_Index],Enumkey)[MS4C_Data_Index][1] || fDefault)
  {
    HashGet(options[MS4C_Menu_Index],Enumkey)[MS4C_Data_Index][1] = true;
    HashGet(options[MS4C_Menu_Index],Enumkey)[MS4C_Data_Index][2] = Itemkey;
  }
  PushBack(Itemkey,HashGet(options[MS4C_Menu_Index],Enumkey)[MS4C_Data_Index][3]);
  return true;
}

//Fügt dem Menü eine Rangeauswahl hinzu
global func AddRangeChoice(&options, Key, string szName, id idItem, int iMin, int iMax, int iStep, int iDefault)
{
  if (!szName) return false;
  if (HashContains(options[MS4C_Menu_Index], Key)) return false;
  HashPut(options[MS4C_Menu_Index],Key,[MS4C_Typ_Range,szName,idItem,[iMin,iMax,iStep,iDefault]]);
  PushBack(Key,options[MS4C_Sequence_Index]);
  return true;
}

//Erstellt ein Menü ausgehend von den Optionen
global func CreateMenuByOptions(array options)
{
  var pMenu=CreateObject(MS4C, 0, 0, GetOwner());
  return pMenu->Init(options);
}

public func Init(array options)
{
  pMenuObject     = options[MS4C_MenuObject_Index];
  pCommandObject  = options[MS4C_CommandObject_Index];
  szCallback      = options[MS4C_Callback_Index];
  idSymbol        = options[MS4C_Symbol_Index];
  szCaption       = options[MS4C_Caption_Index];
  hMenu           = options[MS4C_Menu_Index];
  aSequence       = options[MS4C_Sequence_Index];
  ExtraData       = options[MS4C_ExtraData_Index];
  ParseMenu();
}

public func GetValues()
{
  var hValues=CreateHash();
  var iter = HashIter(hMenu); // Iterator erzeugen
  var node;
  while(node = HashIterNext(iter)) { // Solange es weitere Elemente gibt
  if(node[1][MS4C_Type_Index]==MS4C_Typ_Bool)
    HashPut(hValues,node[0],node[1][MS4C_Data_Index]);
  else if (node[1][MS4C_Type_Index]==MS4C_Typ_Enum)
    HashPut(hValues,node[0],HashGet(node[1][MS4C_Data_Index][0],node[1][MS4C_Data_Index][2])[2]);
  else if (node[1][MS4C_Type_Index]==MS4C_Typ_Range)
    HashPut(hValues,node[0],node[1][MS4C_Data_Index][3]);
  else
    DebugLog("Unknown value typ");
  }
  return hValues;
}

private func ParseMenu(int iSelection)
{
  CreateMenu(idSymbol, pMenuObject, this,0,szCaption,0,1);
  var i;
  for (var Key in aSequence)
  {
    var value=HashGet(hMenu,Key);
    if(value[MS4C_Type_Index]==MS4C_Typ_Bool)
    {
      var def=CreateDummy(value[MS4C_Id_Index], value[MS4C_Data_Index]);
      if(value[MS4C_Data_Index])
        AddMenuItem(Format("$Deactivate$",value[MS4C_Name_Index]),Format("DeactivateBool(%v,%d)",Key,i++),0,pMenuObject,0,0,0,4,def);
      else
        AddMenuItem(Format("$Activate$",value[MS4C_Name_Index]),Format("ActivateBool(%v,%d)",Key,i++),0,pMenuObject,0,0,0,4,def);
      RemoveObject(def);
    }
    else if(value[MS4C_Type_Index]==MS4C_Typ_Enum)
    {
      for (var itemkey in value[MS4C_Data_Index][3])
      {
        var itemvalue=HashGet(value[MS4C_Data_Index][0],itemkey);
        var def=CreateDummy(itemvalue[1], itemkey==value[MS4C_Data_Index][2]);
        AddMenuItem(Format("$Choose$",itemvalue[0],value[MS4C_Name_Index]),Format("Choose(%v,%v,%d)",Key,itemkey,i++),0,pMenuObject,0,0,0,4,def);
        RemoveObject(def);
      }
    }
    else if(value[MS4C_Type_Index]==MS4C_Typ_Range)
    {
      AddMenuItem(value[1],Format("ParseMenu(%d)",i++),value[MS4C_Id_Index],pMenuObject,value[3][3]);
      AddMenuItem(Format("$Increase$",value[MS4C_Name_Index],value[MS4C_Data_Index][2]),Format("Increase(%v,%d)",Key,i++),MS4C,pMenuObject,0,0,0,2,1);
      AddMenuItem(Format("$Decrease$",value[MS4C_Name_Index],value[MS4C_Data_Index][2]),Format("Decrease(%v,%d)",Key,i++),MS4C,pMenuObject,0,0,0,2,2);
    }
  }
  AddMenuItem("$Finished$","Finished",MS4C,pMenuObject,0,0,0,2,3);
  SelectMenuItem(iSelection,pMenuObject);
}

protected func DeactivateBool(Key, int iSelection)
{
  HashGet(hMenu, Key)[MS4C_Data_Index]=false;
  ParseMenu(iSelection);
}

protected func ActivateBool(Key, int iSelection)
{
  HashGet(hMenu, Key)[MS4C_Data_Index]=true;
  ParseMenu(iSelection);
}

protected func Choose(Key,itemkey, int iSelection)
{
  HashGet(hMenu, Key)[MS4C_Data_Index][2]=itemkey;
  ParseMenu(iSelection);
}

protected func Increase(Key, int iSelection)
{
  var values=HashGet(hMenu, Key)[MS4C_Data_Index];
  HashGet(hMenu, Key)[MS4C_Data_Index][3]=BoundBy(values[3]+values[2],values[0],values[1]);
  ParseMenu(iSelection);
}

protected func Decrease(Key, int iSelection)
{
  var values=HashGet(hMenu, Key)[MS4C_Data_Index];
  HashGet(hMenu, Key)[MS4C_Data_Index][3]=BoundBy(values[3]-values[2],values[0],values[1]);
  ParseMenu(iSelection);  
}

protected func Finished()
{
  if (pCommandObject) pCommandObject->Call(szCallback,GetValues(),ExtraData);
  else GameCall(szCallback,GetValues(),ExtraData);
  RemoveObject();
}

public func MenuQueryCancel() { return(1); }

private func CreateDummy(id idItem, bool fChecked)
{
  var pDummy=CreateObject(MS4C,0,0,-1);
  if(idItem && FindDefinition(idItem))
  {
    SetObjectPicture(idItem, pDummy);
    if (fChecked)
    {
      SetGraphics("Chosen", pDummy, MS4C, 1, GFXOV_MODE_Picture);
      SetObjDrawTransform(650,0,5000,0,650,5000, pDummy, 1);      
    }
  }
  else
  {
    if (fChecked)
      SetPicture(64*3, 0, 64, 64, pDummy);
    else
      SetPicture(64*4, 0, 64, 64, pDummy);
    SetGraphics(0, pDummy, MS4C);   
  }
  return pDummy;
}

//Aus Doku geklaut
global func SetObjectPicture(idSrcDef, pObj)
  {
  SetPicture(GetDefCoreVal("Picture", "DefCore", idSrcDef, 0),
             GetDefCoreVal("Picture", "DefCore", idSrcDef, 1),
             GetDefCoreVal("Picture", "DefCore", idSrcDef, 2),
             GetDefCoreVal("Picture", "DefCore", idSrcDef, 3), pObj);
  SetGraphics(0, pObj, idSrcDef);
  return(1);
  }
