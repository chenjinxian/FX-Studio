/**
 luacom.cpp
 Lua binding for COM/OLE.

  original authors: Renato Cerqueira, Vinicius Almendra
*/

// RCS Info
static char const * const rcsid = "$Id: luacom$";

#include <string.h>
#include <stdlib.h>

#include <ole2.h>
#include <ocidl.h>
#ifndef NO_HTMLHELP
#include <htmlhelp.h> // HtmlHelp
#endif

#include <assert.h>
#include <stdio.h>
#include <math.h>

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
}
#include "LuaCompat.h"

#include "luabeans.h"
#include "luacom.h"
#include "tLuaDispatch.h"
#include "tLuaControl.h"
#include "tLuaCOM.h"
#include "tLuaCOMException.h"
#include "tCOMUtil.h"
#include "tLuaCOMClassFactory.h"
#include "tLuaCOMTypeHandler.h"
#include "tLuaCOMConnPoints.h"

#include "luacom_internal.h"

#include "LuaAux.h"
#include "tUtil.h"

#include "tLuaCOMEnumerator.h"
#include "tLuaTLB.h"

#include <wchar.h> // for MINGW/Wine

#if LUA_VERSION_NUM >= 502
#define luaL_register(a, b, c) luaL_setfuncs(a, c, 0)
#endif

// some string constants

#define GET_PREFIX "get"
#define PUT_PREFIX "set"

static bool luacom_runningInprocess(lua_State* L);

HINSTANCE g_hInstance;
CRITICAL_SECTION g_CriticalSection;
HWND g_hwndParking;



///
/// Internal functions.
///



static tLuaCOM* ImplInterface(lua_State* L,
                              ITypeLib* typelib,
                              CLSID clsid,
                              const char* interface_name,
                              int unlocked_ref
                              )
{
  CHECKPARAM(typelib && interface_name && L);

  tCOMPtr<ITypeInfo> interface_typeinfo;
  interface_typeinfo.Attach(tCOMUtil::GetInterfaceTypeInfo(typelib, interface_name));
  if(interface_typeinfo == NULL)
    return NULL;

  tCOMPtr<tLuaDispatch> iluacom;
  iluacom.Attach(tLuaDispatch::CreateLuaDispatch(L, interface_typeinfo, unlocked_ref));
  if(iluacom == NULL)
    return NULL;

  tLuaCOM *lcom;
  try
  {
    lcom = tLuaCOM::CreateLuaCOM(L, iluacom, clsid, interface_typeinfo);
  }
  catch(class tLuaCOMException&)
  {
    lcom = NULL;
  }

  return lcom;
}


/**
  Deals with runtime errors, calling lua_error or
  logging them, depending on the client's choice
  The message string is left on the top of the
  stack, to allow passing it to the caller
*/
static void luacom_err(lua_State* L, const char* message, bool is_API_function)
{
  LUASTACK_SET(L);

  lua_pushstring(L, message);
  luaCompat_moduleSet(L, MODULENAME, LUACOM_LAST_ERROR);

  if(is_API_function)
    luaCompat_moduleGet(L, MODULENAME, LUACOM_SHOULD_ABORT_API);
  else
    luaCompat_moduleGet(L, MODULENAME, LUACOM_SHOULD_ABORT);

  if(lua_toboolean(L, -1))
  {
    luaL_error(L, "%s", message);
  }

  lua_pop(L, 1);

  LUASTACK_CLEAN(L, 0);
}

void luacom_error(lua_State* L, const char* message)
{
  luacom_err(L, message, false);
}

static void luacom_APIerror(lua_State* L, const char* message)
{
  luacom_err(L, message, true);
}



///
/// Functions exported by Lua.
///



/**
  Shows help from the type library associated with the
  LuaCOM object.

  Parameters: 
   (1) LuaCOM object
*/
static int luacom_ShowHelp(lua_State *L)
{
  char *pHelpFile = NULL; 
  DWORD context = 0;

  tLuaCOM* luacom = (tLuaCOM *) LuaBeans::check_tag(L, 1);

  luacom->getHelpInfo(&pHelpFile, &context);

  if(pHelpFile != NULL)
  {
    size_t len = strlen(pHelpFile);
    if (len >= 4 && _stricmp(pHelpFile + len - 4, ".chm") == 0)
    {
#ifdef NO_HTMLHELP
      ::MessageBoxA(NULL, "Error: HtmlHelp support not included",
        "LuaCOM", MB_ICONEXCLAMATION);
#else
      if(context != 0)
        HtmlHelpA(NULL, pHelpFile, HH_HELP_CONTEXT, context);
      else
        HtmlHelpA(NULL, pHelpFile, HH_DISPLAY_TOC, 0);
#endif
    }
    else
    {
      if(context != 0)
        WinHelpA(NULL, pHelpFile, HELP_CONTEXT, context);
      else
        WinHelpA(NULL, pHelpFile, HELP_FINDER, 0);
    }
  }

  return 0;
}


/**
  Parameters (Lua):
    objeto luacom
    tabela que implementara a interface de conexao

  Returns
    objeto luacom que encapsula objeto luacom implementado pela
    tabela fornecida ou nil se nao for poss�vel estabelecer a
    conexao
    cookir do connection point

  Cria um connection point utilizando a tabela
  passada como parametro e o conecta com o objeto
  passado tambem como parametro
 */
static int luacom_Connect(lua_State *L)
{
  // check parameters
  tLuaCOM* client = (tLuaCOM *) LuaBeans::check_tag(L, 1);

  if(lua_type(L, 2) != LUA_TTABLE && lua_type(L, 2) != LUA_TUSERDATA)
  {
    luaL_argerror(L, 2, "Implementation must be a table or a userdata");
  }

  tLuaCOM* server = NULL;
  DWORD cookie;
  try
  {
    tCOMPtr<ITypeInfo> pTypeinfo;
    pTypeinfo.Attach(client->GetDefaultEventsInterface());
    CHK_LCOM_ERR(pTypeinfo, "Could not get default events interface.");

    /* gets a reference to the implementation */
    lua_pushvalue(L, 2);
    int ref = luaL_ref(L, LUA_REGISTRYINDEX);

    tCOMPtr<tLuaDispatch> server_disp;
    server_disp.Attach(tLuaDispatch::CreateLuaDispatch(L, pTypeinfo, ref));
    luaL_unref(L, LUA_REGISTRYINDEX, ref);
    CHECKPOSCOND(server_disp);

    server = tLuaCOM::CreateLuaCOM(L, server_disp);
    CHECKPOSCOND(server);

    cookie = client->addConnection(server);
    CHECKPOSCOND(cookie);
  }
  catch(class tLuaCOMException& e)
  {
    luacom_APIerror(L, e.getMessage());
    return 0;
  }

  LuaBeans::push(L, server); 
  lua_pushnumber(L, cookie);
  return 2;
}


/**
  Parameters:
    1. Tabela de implementacao
    2. Nome da type library
    3. Nome da interface a ser implementada
    4. (Opcional) Nome da CoClass da qual a interface faz parte
       necessario no caso de se querer expor objetos implementados
       com essa interface ou no caso de se querer localizar uma
       interface source (eventos)

 Returns:
    1. Objeto LuaCOM que encapsula a implementacao via Lua
        ou nil em caso de erro

  Implementa uma interface descrida em uma type library
  dada
*/
static tLuaCOM *luacom_ImplInterfaceFromTypelibHelper(lua_State *L)
{
  if(lua_type(L, 1) != LUA_TTABLE && lua_type(L,1) != LUA_TUSERDATA)
  {
    luaL_argerror(L, 1, "Implementation must be a table or a userdata");
  }

  lua_pushvalue(L, 1);
  const int ref = luaL_ref(L, LUA_REGISTRYINDEX);

  tStringBuffer typelib_name(luaL_checklstring(L, 2, NULL));
  tStringBuffer pcInterface(luaL_checklstring(L, 3, NULL));
  tStringBuffer coclassname(luaL_optlstring(L, 4, NULL, NULL));

  tLuaCOM* lcom = NULL;
  try
  {
    // gets typelib
    tCOMPtr<ITypeLib> typelib;
    typelib.Attach(tCOMUtil::LoadTypeLibByName(typelib_name));
    CHK_LCOM_ERR(typelib, "Could not load type library.");

    // gets coclass typeinfo
    CLSID clsid = IID_NULL;
    if(coclassname)
    {
      tCOMPtr<ITypeInfo> coclassinfo;
      coclassinfo.Attach(tCOMUtil::GetCoClassTypeInfo(typelib, coclassname));
      CHK_LCOM_ERR(coclassinfo, "Could not get ccoclass typeinfo.");
      clsid = tCOMUtil::GetCLSID(coclassinfo);
    }

    lcom = ImplInterface(L, typelib, clsid, pcInterface, ref);
    luaL_unref(L, LUA_REGISTRYINDEX, ref);
    CHECKPOSCOND(lcom);
  }
  catch(class tLuaCOMException& e)
  {
    luacom_APIerror(L, e.getMessage());
    return 0;
  }

  return lcom;
}

static int luacom_ImplInterfaceFromTypelib(lua_State *L)
{
  tLuaCOM *lcom = luacom_ImplInterfaceFromTypelibHelper(L);
  if(lcom)
  {
    LuaBeans::push(L, lcom);
    return 1;
  }
  else
    return 0;
}


/**
  Cria uma implementacao IDispatch para um
  objeto Lua dado um ProgID e o nome da
  interface
 
  In: Implementation: table,
      ProgID: string,
      Interface: string,
  Out: LuaCOM_obj (table with tag LuaCOM)
*/
static tLuaCOM *luacom_ImplInterfaceHelper(lua_State *L)
{
  // gets parameters
  if(lua_type(L, 1) != LUA_TTABLE && lua_type(L,1) != LUA_TUSERDATA)
  {
    luaL_argerror(L, 1, "Implementation must be a table or a userdata");
  }

  // pushes lua table on top of stack
  lua_pushvalue(L, 1);
  const int ref = luaL_ref(L, LUA_REGISTRYINDEX);

  tStringBuffer pcProgID(luaL_checklstring(L, 2, NULL));
  tStringBuffer pcInterface(luaL_checklstring(L, 3, NULL));

  tLuaCOM* lcom = NULL;
  try
  {
    HRESULT hr  = S_OK;
    
    // gets typelib

    CLSID clsid = IID_NULL;
    CHK_COM_CODE(tCOMUtil::ProgID2CLSID(&clsid, pcProgID));

    tCOMPtr<ITypeLib> typelib;
    typelib.Attach(tCOMUtil::LoadTypeLibFromCLSID(clsid));
    CHK_LCOM_ERR(typelib, "Could not load type library.");

    lcom = ImplInterface(L, typelib, clsid, pcInterface, ref);

    luaL_unref(L, LUA_REGISTRYINDEX, ref);

    CHECKPOSCOND(lcom);
  }
  catch(class tLuaCOMException& e)
  {
    luacom_APIerror(L, e.getMessage());
    return 0;
  }
  return lcom;
}

static int luacom_ImplInterface(lua_State *L)
{
  tLuaCOM* lcom = luacom_ImplInterfaceHelper(L);
  if(lcom)
  {
    LuaBeans::push(L, lcom);
    return 1;
  }
  else return 0;
}


/**
  Returns a string containing the CLSID associated with a ProgID.
*/
static int luacom_CLSIDfromProgID(lua_State *L)
{
  tStringBuffer str(luaL_checklstring(L, 1, NULL));
  
  HRESULT hr = S_OK;

  wchar_t* progId = NULL;
  char* id_str = NULL;
  try
  {
    progId = (wchar_t*) malloc( (strlen(str) + 1) * sizeof(wchar_t));
    mbstowcs(progId,str,strlen(str)+1);

    CLSID clsid = IID_NULL;
    CHK_COM_CODE(CLSIDFromProgID(progId, &clsid));

    wchar_t* clsid_str = NULL;
    CHK_COM_CODE(StringFromCLSID(clsid, &clsid_str));
    
    id_str = (char*) malloc( (wcslen(clsid_str) + 1) * sizeof(char));
    wcstombs(id_str,clsid_str,wcslen(clsid_str)+1);
  }
  catch(class tLuaCOMException& e)
  {
    SAFEFREE(progId);
    SAFEFREE(id_str);

    luacom_APIerror(L, e.getMessage());
    return 0;
  }

  lua_pushstring(L, id_str);

  SAFEFREE(progId);
  SAFEFREE(id_str);

  return 1;
}


/**
  Returns a string containing the ProgID associaded with a CLSID.
*/
static int luacom_ProgIDfromCLSID(lua_State *L)
{
  tStringBuffer str(luaL_checklstring(L, 1, NULL));
  wchar_t* clsid_str = NULL;
  LPOLESTR progId = NULL;
  CLSID clsid = IID_NULL;
  HRESULT hr = S_OK;
  char* id_str = NULL;

  try
  {
    clsid_str = (wchar_t*) malloc( (strlen(str) + 1) * sizeof(wchar_t));
    mbstowcs(clsid_str,str,strlen(str)+1);

    CHK_COM_CODE(CLSIDFromString(clsid_str, &clsid));

    CHK_COM_CODE(ProgIDFromCLSID(clsid, &progId));

    id_str = (char*) malloc( (wcslen(progId) + 1) * sizeof(char));
    wcstombs(id_str,progId,wcslen(progId)+1);
  }
  catch(class tLuaCOMException& e)
  {
    SAFEFREE(progId);
    SAFEFREE(clsid_str);

    luacom_APIerror(L, e.getMessage());
    return 0;
  }

  lua_pushstring(L, id_str);

  SAFEFREE(progId);
  SAFEFREE(clsid_str);

  return 1;
}


/**
  Retorna um objeto LuaCOM que instancia o objeto
  COM identificado pelo ProgID dado
*/
static int luacom_CreateObject(lua_State *L)
{
  HRESULT hr = S_OK;
  
  tStringBuffer progId(luaL_checklstring(L, 1, NULL));
  tStringBuffer creation_mode(lua_tostring(L, 2));
  const bool untyped = lua_toboolean(L, 3) != 0;

  DWORD context = CLSCTX_SERVER;
  if(creation_mode != NULL)
  {
    if(strcmp(creation_mode, "local_server") == 0)
      context = CLSCTX_LOCAL_SERVER;
    else if(strcmp(creation_mode, "inproc_server") == 0)
      context = CLSCTX_INPROC_SERVER;
  }

  tLuaCOM* lcom = NULL;
  try
  {
    CLSID clsid = IID_NULL;
    CHK_COM_CODE(tCOMUtil::ProgID2CLSID(&clsid, progId));

    tCOMPtr<IDispatch> pdisp;
    CHK_COM_CODE(CoCreateInstance(clsid,
                          NULL,
                          context,
                          IID_IDispatch,
                          (void**)&pdisp));

    // Initializes object (some require this step to work)
    tCOMPtr<IPersistStreamInit> psi;
    hr = pdisp->QueryInterface(IID_IPersistStreamInit, (void**) &psi);
    if(SUCCEEDED(hr))
      psi->InitNew();

    lcom = tLuaCOM::CreateLuaCOM(L, pdisp, clsid, NULL, untyped);
  }
  catch(class tLuaCOMException& e)
  {
    luacom_APIerror(L, e.getMessage());
    return 0;
  }

  LuaBeans::push(L, lcom);
  return 1;
}


/**
  Cria um objeto LuaCOM associado a uma instancia
  ja' existente do objeto COM identificado pelo 
  ProgID dado
*/
static int luacom_GetObject(lua_State *L)
{
  HRESULT hr;

  tStringBuffer progId(luaL_checklstring(L, 1, NULL));

  tLuaCOM* lcom = NULL;
  try
  {
    CLSID clsid = IID_NULL;
    hr = tCOMUtil::ProgID2CLSID(&clsid, progId);
    
    tCOMPtr<IUnknown> punk;
    if(SUCCEEDED(hr))
    {
      CHK_COM_CODE(GetActiveObject(clsid,NULL,&punk));
    }
    else  // tests whether the user passed in a DisplayName of a moniker
    {
      tCOMPtr<IBindCtx> pbc;
      CHK_COM_CODE(CreateBindCtx(0, &pbc));

      tCOMPtr<IMoniker> pmk;
      BSTR bstr = tUtil::string2bstr(progId);
      ULONG chEaten = 0;
      hr = MkParseDisplayName(pbc, bstr, &chEaten, &pmk);
      SysFreeString(bstr);
      CHK_COM_CODE(hr);

      CHK_COM_CODE(pmk->BindToObject(pbc, NULL, IID_IUnknown, (void **) &punk));
    }

    tCOMPtr<IDispatch> pdisp;
    CHK_COM_CODE(punk->QueryInterface(IID_IDispatch, (void **) &pdisp));

    lcom = tLuaCOM::CreateLuaCOM(L, pdisp, clsid);
    CHECKPOSCOND(lcom);
  }
  catch(class tLuaCOMException& e)
  {
    luacom_APIerror(L, e.getMessage());
    return 0;
  }

  LuaBeans::push(L, lcom);
  return 1;
}


/**
  Associa um connection point de um objeto COM com
  outro objeto COM
*/
static int luacom_addConnection(lua_State *L)
{
  tLuaCOM* client = (tLuaCOM*) LuaBeans::check_tag(L, 1);
  tLuaCOM* server = (tLuaCOM*) LuaBeans::check_tag(L, 2);

  DWORD cookie = client->addConnection(server);
  if(cookie == 0)
  {
    luacom_APIerror(L, "Could not establish connection");
    return 0;
  }

  lua_pushnumber(L, cookie);
  return 1;
}


/**
  Desfaz connection point associado a um objeto LuaCOM
*/
static int luacom_releaseConnection(lua_State *L)
{
  tLuaCOM* lcom = (tLuaCOM*) LuaBeans::check_tag(L, 1);
  if(lua_gettop(L) > 1)
  {
    tLuaCOM* server = (tLuaCOM*) LuaBeans::check_tag(L, 2);
    DWORD cookie = (DWORD)lua_tonumber(L, 3);

    try { lcom->releaseConnection(server, cookie); }
    catch(class tLuaCOMException& e)
    {
      luacom_APIerror(L, e.getMessage());
      return 0;
    }
  }
  else
  {
    try { lcom->releaseConnection(); }
    catch(class tLuaCOMException& e)
    {
      luacom_APIerror(L, e.getMessage());
      return 0;
    }
  }

  return 0;
}

/**
  Informa se existe algum metodo ou propriedade com
  o nome passado como parametro em lua
*/
static int luacom_isMember(lua_State *L)
{
  // objeto luacom
  tLuaCOM* lcom = (tLuaCOM*) LuaBeans::check_tag(L, 1);
  tStringBuffer member_name(luaL_checklstring(L, 2, NULL));

  lua_pushboolean(L, lcom->isMember(member_name));
  return 1;
}


/**
  Creates a Component Object implemented in luacom
*/
static int luacom_NewObjectOrControl(lua_State *L, int type)
{
  HRESULT hr = S_OK;

  // gets parameters
  if(lua_type(L, 1) != LUA_TTABLE && lua_type(L,1) != LUA_TUSERDATA)
  {
    luaL_argerror(L, 1, "Implementation must be a table or a userdata");
  }

  // pushes lua table on top of stack
  lua_pushvalue(L, 1);
  const int ref = luaL_ref(L, LUA_REGISTRYINDEX);

  tStringBuffer pcProgID(luaL_checklstring(L, 2, NULL));

  tLuaCOM *lcom = NULL;
  tLuaCOMConnPoint *cp = NULL;
  try
  {
    // gets typelib
    CLSID clsid = IID_NULL;
    CHK_COM_CODE(tCOMUtil::ProgID2CLSID(&clsid, pcProgID));
    //
    tCOMPtr<ITypeLib> typelib;
    typelib.Attach(tCOMUtil::LoadTypeLibFromCLSID(clsid));
    CHK_LCOM_ERR(typelib, "Could not load type library.");

    // gets coclass typeinfo
    tCOMPtr<ITypeInfo> coclassinfo;
    coclassinfo.Attach(tCOMUtil::GetCoClassTypeInfo(typelib, clsid));
    CHK_LCOM_ERR(coclassinfo, "CoClass not found in type library.");

    // gets the default interface typeinfo
    tCOMPtr<ITypeInfo> interface_typeinfo;
    interface_typeinfo.Attach(tCOMUtil::GetDefaultInterfaceTypeInfo(coclassinfo, false));
    CHK_LCOM_ERR(interface_typeinfo, 
      "Could not find a suitable default interface.");

    // Creates IDispatch implementation
    tCOMPtr<tLuaDispatch> iluacom;
    if(type) // Control
      iluacom.Attach((tLuaDispatch*)tLuaControl::CreateLuaControl(L, interface_typeinfo, ref));
    else // Object
      iluacom.Attach(tLuaDispatch::CreateLuaDispatch(L, interface_typeinfo, ref));

    luaL_unref(L, LUA_REGISTRYINDEX, ref);

    // Creates associated luacom object
    lcom = tLuaCOM::CreateLuaCOM(L, iluacom, clsid);

    // Informs tLuaDispatch of coclassinfo (this allows implementation of
    // IProvideClassInfo[x]
    iluacom->SetCoClassinfo(coclassinfo);

    // Create connection points container and tells it to initialize
    // itself from coclassinfo and exports connection point for 
    // default source
    iluacom->BeConnectable();
    if(iluacom->GetConnPointContainer())
      cp = iluacom->GetConnPointContainer()->GetDefault();
    else
      cp = NULL;
  }
  catch(class tLuaCOMException& e)
  {
    luacom_APIerror(L, e.getMessage());
    return 0;
  }

  // returns LuaCOM object and connection point
  // for default source interface
  int retvals = 1;
  LuaBeans::push(L, lcom);
  if(cp)
  {
    cp->push();
    retvals++;
  }
  return retvals;
}

static int luacom_NewObject(lua_State *L) {
  return luacom_NewObjectOrControl(L, 0);
}

static int luacom_NewControl(lua_State *L) {
  return luacom_NewObjectOrControl(L, 1);
}


/**
  Creates a class factory that exposes a COM object

  Parameters:
    1. LuaCOM object
  Return values
    1. Cookie to unexpose object
*/
static int luacom_ExposeObject(lua_State *L)
{
  // check parameters
  tLuaCOM* luacom = (tLuaCOM *) LuaBeans::check_tag(L, 1);

  DWORD cookie = (DWORD)-1;
  try
  {
    if(luacom_runningInprocess(L)) {
      // Inprocess "registration": stores object in the Lua registry
      lua_pushvalue(L, LUA_REGISTRYINDEX);
      lua_pushstring(L,"object");
      lua_pushlightuserdata(L,(void*)luacom->GetIDispatch());
      lua_settable(L,-3);
      lua_pop(L,1);
    } else {
      tCOMPtr<tLuaCOMClassFactory> luacom_cf;
      luacom_cf.Attach(new tLuaCOMClassFactory(luacom->GetIDispatch()));
      luacom_cf->AddRef();

      CLSID clsid = luacom->GetCLSID();
      CHK_LCOM_ERR(clsid != IID_NULL,
        "Object does not have coclass type information");
  
      CHK_COM_CODE(CoRegisterClassObject(
        clsid,
        luacom_cf,
        CLSCTX_LOCAL_SERVER,
        REGCLS_SINGLEUSE,
        &cookie));
    }
  }
  catch(class tLuaCOMException& e)
  {
    luacom_APIerror(L, e.getMessage());
    return 0;
  }

  lua_pushnumber(L, cookie);
  return 1;
}


/**
  Revokes a previously registered class factory

  Parameters:
    1. Cookie
  Return values
    1. non-nil if succeeded
*/
static int luacom_RevokeObject(lua_State *L)
{
  // check parameters
  const int cookie = (int) luaL_checknumber(L, 1);

  // revokes class object
  try
  {
    CHK_COM_CODE(CoRevokeClassObject(cookie));
  }
  catch(class tLuaCOMException& e)
  {
    luacom_APIerror(L, e.getMessage());
    return 0;
  }

  lua_pushboolean(L, true);
  return 1;
}


/**
  Registers a COM Object in the system registry

  Parameters:
    1. registration table or userdata
  Return values
    1. non-nil if successful
*/
static int luacom_RegisterObject(lua_State *L)
{
  if(lua_type(L, 1) != LUA_TTABLE && lua_type(L,1) != LUA_TUSERDATA)
  {
    luaL_argerror(L, 1, "Registration info must be a table or userdata");
  }

  HRESULT hr = S_OK;
  const int bufsize = 1000;

  try
  {

    lua_pushstring(L, "Control");
    lua_gettable(L, 1);
    bool control = lua_toboolean(L, -1) != 0;

    // gets the registration information from the registration table
    lua_pushstring(L, "VersionIndependentProgID");
    lua_gettable(L, 1);
	  tStringBuffer VersionIndependentProgID(lua_tostring(L, -1));

    // gets the registration information from the registration table
    lua_pushstring(L, "ProgID");
    lua_gettable(L, 1);
    tStringBuffer ProgID(lua_tostring(L, -1));

    lua_pushstring(L, "TypeLib");
    lua_gettable(L, 1);
    tStringBuffer typelib_path(lua_tostring(L, -1));

    lua_pushstring(L, "CoClass");
    lua_gettable(L, 1);
    tStringBuffer CoClass(lua_tostring(L, -1));

    lua_pushstring(L, "ComponentName");
    lua_gettable(L, 1);
    tStringBuffer ComponentName(lua_tostring(L, -1));

    lua_pushstring(L, "Arguments");
    lua_gettable(L, 1);
    tStringBuffer arguments(lua_tostring(L, -1));

    lua_pushstring(L, "ScriptFile");
    lua_gettable(L, 1);
    tStringBuffer scriptFile(lua_tostring(L, -1));

    CHK_LCOM_ERR(ProgID && typelib_path &&  CoClass, 
      "Incomplete registration table.");

    // Loads and registers the typelib
    tCOMPtr<ITypeLib> typelib;
    wchar_t wcTypelib_path[bufsize];
    mbstowcs(wcTypelib_path, typelib_path, strlen(typelib_path)+1);
    CHK_COM_CODE(LoadTypeLibEx(wcTypelib_path, REGKIND_REGISTER, &typelib));

    // Gets the type library version and LIBID
    char version[30];
    char libId[bufsize];
    {
      TLIBATTR *plibattr = NULL;
      typelib->GetLibAttr(&plibattr);

      // gets version
      sprintf(version, "%d.%d", plibattr->wMajorVerNum, plibattr->wMinorVerNum);

      // gets libid
      wchar_t *wcLibId = NULL;
      CHK_COM_CODE(StringFromCLSID(plibattr->guid, &wcLibId));

      wcstombs(libId, wcLibId, wcslen(wcLibId)+1);
      CoTaskMemFree(wcLibId);

      typelib->ReleaseTLibAttr(plibattr);
    }

    // gets the CoClass TypeInfo to get the CLSID
    tCOMPtr<ITypeInfo> coclassinfo;
    coclassinfo.Attach(tCOMUtil::GetCoClassTypeInfo(typelib, CoClass));
    CHK_LCOM_ERR(coclassinfo, "Could not get coclass typeinfo.");

    // gets the CLSID
    char clsid[bufsize];
    {
      TYPEATTR* ptypeattr = NULL;
      wchar_t* wcClsid=  NULL;

      coclassinfo->GetTypeAttr(&ptypeattr);

      CHK_COM_CODE(StringFromCLSID(ptypeattr->guid, &wcClsid));

      wcstombs(clsid, wcClsid,wcslen(wcClsid)+1);

      coclassinfo->ReleaseTypeAttr(ptypeattr);
      CoTaskMemFree(wcClsid);
    }


    //// Now we have all the information needed to perform the registration

    // registers ProgID
    
    char ModulePath[bufsize];
    ModulePath[0] = 0;
    GetModuleFileNameA(
      NULL,
      ModulePath,
      sizeof(ModulePath));

    if(scriptFile)
    {
      strcat(ModulePath, " ");
      strcat(ModulePath, scriptFile);
    }

    if(arguments)
    {
      strcat(ModulePath, " ");
      strcat(ModulePath, arguments);
    }

    // Create some base key strings.
    char CLSID[bufsize];
    strcpy(CLSID, "CLSID\\");
    strcat(CLSID, clsid);

    // Create ProgID keys.
    tCOMUtil::SetRegKeyValue(
      ProgID,
      NULL,
      ComponentName);

    tCOMUtil::SetRegKeyValue(
      ProgID,
      "CLSID",
      clsid);

    // Create VersionIndependentProgID keys.
    tCOMUtil::SetRegKeyValue(
      VersionIndependentProgID,
      NULL,
      ComponentName);

    tCOMUtil::SetRegKeyValue(
      VersionIndependentProgID,
      "CurVer",
      ProgID);

    tCOMUtil::SetRegKeyValue(
      VersionIndependentProgID,
      "CLSID",
      clsid);

    // Create entries under CLSID.
    tCOMUtil::SetRegKeyValue(
      CLSID,
      NULL,
      ComponentName);

    tCOMUtil::SetRegKeyValue(
      CLSID,
      "ProgID",
      ProgID);

    tCOMUtil::SetRegKeyValue(
      CLSID,
      "VersionIndependentProgID",
      VersionIndependentProgID);

    tCOMUtil::SetRegKeyValue(
      CLSID,
      "LocalServer32",
      ModulePath);

    if(scriptFile) {
      tCOMUtil::SetRegKeyValue(
      CLSID,
      "InprocServer32",
      LUACOM_DLL);
    tCOMUtil::SetRegKeyValue(
      CLSID,
      "ScriptFile",
      scriptFile);
    }

    if(control) {
      tCOMUtil::SetRegKeyValue(
        CLSID,
        "Implemented Categories\\{0DE86A53-2BAA-11CF-A229-00AA003D7352}",
        NULL);
      tCOMUtil::SetRegKeyValue(
        CLSID,
        "Implemented Categories\\{0DE86A57-2BAA-11CF-A229-00AA003D7352}",
        NULL);
      tCOMUtil::SetRegKeyValue(
        CLSID,
        "Implemented Categories\\{40FC6ED4-2438-11CF-A3DB-080036F12502}",
        NULL);
      tCOMUtil::SetRegKeyValue(
        CLSID,
        "Implemented Categories\\{40FC6ED5-2438-11CF-A3DB-080036F12502}",
        NULL);
      tCOMUtil::SetRegKeyValue(
        CLSID,
        "Implemented Categories\\{7DD95801-9882-11CF-9FA9-00AA006C42C4}",
        NULL);
      tCOMUtil::SetRegKeyValue(
        CLSID,
        "Implemented Categories\\{7DD95802-9882-11CF-9FA9-00AA006C42C4}",
        NULL);
    }

    tCOMUtil::SetRegKeyValue(
      CLSID,
      "Version",
      version);

    tCOMUtil::SetRegKeyValue(
      CLSID,
      "TypeLib",
      libId);

    tCOMUtil::SetRegKeyValue(
      CLSID,
      "Programmable",
      NULL);
  }
  catch(class tLuaCOMException& e)
  {
    luacom_APIerror(L, e.getMessage());
    return 0;
  }

  // signals success
  lua_pushboolean(L, true);
  return 1;
}


/**
  Unregisters a COM Object in the system registry
 
  Parameters:
    1. registration table or userdata
  Return values
    1. non-nil if successful
*/
static int luacom_UnRegisterObject(lua_State *L)
{
  if(lua_type(L, 1) != LUA_TTABLE && lua_type(L,1) != LUA_TUSERDATA)
  {
    luaL_argerror(L, 1, "Registration info must be a table or userdata");
  }

  HRESULT hr = S_OK;
  const int bufsize = 1000;

  try
  {

    // gets the registration information from the registration table
    lua_pushstring(L, "VersionIndependentProgID");
    lua_gettable(L, 1);
    tStringBuffer VersionIndependentProgID(lua_tostring(L, -1));

    // gets the registration information from the registration table
    lua_pushstring(L, "ProgID");
    lua_gettable(L, 1);
    tStringBuffer ProgID(lua_tostring(L, -1));

    lua_pushstring(L, "TypeLib");
    lua_gettable(L, 1);
    tStringBuffer typelib_path(lua_tostring(L, -1));

    lua_pushstring(L, "CoClass");
    lua_gettable(L, 1);
    tStringBuffer CoClass(lua_tostring(L, -1));

    CHK_LCOM_ERR(ProgID && typelib_path &&  CoClass, 
      "Incomplete registration table.");

    // Loads the typelib
    tCOMPtr<ITypeLib> typelib;
    wchar_t wcTypelib_path[bufsize];
    mbstowcs(wcTypelib_path, typelib_path, strlen(typelib_path)+1);
    CHK_COM_CODE(LoadTypeLibEx(wcTypelib_path, REGKIND_NONE, &typelib));

    // Unregisters the typelib
    {
      TLIBATTR *plibattr = NULL;
      typelib->GetLibAttr(&plibattr);

      UnRegisterTypeLib(plibattr->guid, plibattr->wMajorVerNum, plibattr->wMinorVerNum,
      plibattr->lcid, plibattr->syskind);

      typelib->ReleaseTLibAttr(plibattr);
    }

    // gets the CoClass TypeInfo to get the CLSID
    tCOMPtr<ITypeInfo> coclassinfo;
    coclassinfo.Attach(tCOMUtil::GetCoClassTypeInfo(typelib, CoClass));
    CHK_LCOM_ERR(coclassinfo, "Could not get coclass typeinfo.");

    // gets the CLSID
    char clsid[bufsize];
    {
      TYPEATTR* ptypeattr = NULL;
      wchar_t* wcClsid = NULL;

      coclassinfo->GetTypeAttr(&ptypeattr);

      CHK_COM_CODE(StringFromCLSID(ptypeattr->guid, &wcClsid));

      wcstombs(clsid, wcClsid,wcslen(wcClsid)+1);

      coclassinfo->ReleaseTypeAttr(ptypeattr);
      CoTaskMemFree(wcClsid);
    }


    //// Now we have all the information needed to perform the unregistration

    // unregisters ProgID
    
    // Create some base key strings.
    char CLSID[bufsize];
    strcpy(CLSID, "CLSID\\");
    strcat(CLSID, clsid);

    // Delete ProgID keys.
    tCOMUtil::DelRegKey(
      ProgID,
      NULL);

    // Delete VersionIndependentProgID keys.
    tCOMUtil::DelRegKey(
      VersionIndependentProgID,
      NULL);

    // Delete entries under CLSID.
    tCOMUtil::DelRegKey(
      CLSID,
      NULL);

  }
  catch(class tLuaCOMException& e)
  {
    luacom_APIerror(L, e.getMessage());
    return 0;
  }

  // signals success
  lua_pushboolean(L, true);
  return 1;
}


/**
  Returns the a IUnknown interface for a LuaCOM object.

  Parameters:
    1. LuaCOM object
  Return values
    1. IUnknown pointer (a userdata)
*/
static int luacom_GetIUnknown(lua_State *L)
{
  // check parameters
  tLuaCOM* luacom = (tLuaCOM *) LuaBeans::check_tag(L, 1);

  IDispatch* pdisp = luacom->GetIDispatch();
  IUnknown* punk = NULL;
  
  pdisp->QueryInterface(IID_IUnknown, (void **) &punk);

  // checks whether there is a usertag for this IUnknown
  // If exists, simply uses it

  // gets IUnknown tag
  luaCompat_pushTypeByName(L, MODULENAME, LCOM_IUNKNOWN_TYPENAME);

  // sets object type
  int newref = luaCompat_newTypedObject(L, punk);

  if(!newref)
  {
    // there was already a userdata for that pointer, 
    // so we have to decrement the reference count,
    // as the garbage collector will be called just
    // once for any userdata
    punk->Release();
  }

  return 1;
}


static int luacom_GetTypeInfo(lua_State *L)
{
  // check parameters
  tLuaCOM* obj = (tLuaCOM *) LuaBeans::check_tag(L, 1);

  if(obj->hasTypeInfo())
  {
    tLuaTypeInfo::pushNew(L, obj->GetTypeInfo());
    return 1;
  }
  else
    return 0;
}


static int luacom_DumpTypeInfo(lua_State *L)
{
  // check parameters
  tLuaCOM* obj = (tLuaCOM *) LuaBeans::check_tag(L, 1);

  if(obj->hasTypeInfo())
  {
    try
    {
      tCOMUtil::DumpTypeInfo(obj->GetTypeInfo());
    }
    catch(class tLuaCOMException& e)
    {
      luacom_APIerror(L, e.getMessage());

      return 0;
    }
  }

  return 0;
}


/**
  Starts logging
*/
static int luacom_StartLog(lua_State* L)
{
  tStringBuffer filename(luaL_checklstring(L, 1, NULL));

  bool result = tUtil::OpenLogFile(filename);

  lua_pushboolean(L, result);

  return 1;
}


/**
  Ends logging
*/
static int luacom_EndLog(lua_State* L)
{
  UNUSED(L);

  tUtil::CloseLogFile();

  return 0;
}


/**
  Gets the IEnumVariant interface for an LuaCOM object.
*/
static int luacom_GetEnumerator(lua_State *L)
{
  // check parameters
  tLuaCOM* luacom = (tLuaCOM *) LuaBeans::check_tag(L, 1);
  
  int retvals = 0;
  try
  {
    retvals = 
      luacom->call(L, DISPID_NEWENUM, INVOKE_PROPERTYGET, NULL, tLuaObjList());
    CHECKPOSCOND(retvals != 0);
  }
  catch(class tLuaCOMException& e)
  {
    luacom_APIerror(L, e.getMessage());
    return 0;
  }

  return retvals;
}



static int luacom_LoadTypeLibrary(lua_State *L)
{
  tStringBuffer typelib_name(luaL_checklstring(L, -1, NULL));

  try
  {
    // tries to get the typelib by filename
    tCOMPtr<ITypeLib> typelib;
    typelib.Attach(tCOMUtil::LoadTypeLibByName(typelib_name));

    if(typelib == NULL)
    {
      HRESULT hr  = S_OK;
      
      // tries to get by ProgID
      CLSID clsid = IID_NULL;
      hr = tCOMUtil::ProgID2CLSID(&clsid, typelib_name);
      if(SUCCEEDED(hr))
        typelib.Attach(tCOMUtil::LoadTypeLibFromCLSID(clsid));
    }

    CHK_LCOM_ERR(typelib, "Could not load type library.");
    
    tLuaTLB::pushNew(L, typelib);
    return 1;
  }
  catch(class tLuaCOMException& e)
  {
    luacom_APIerror(L, e.getMessage());
    return 0;
  }
}


/**
  Creates a luacom object using an IUnknown pointer.
*/
int luacom_CreateLuaCOM(lua_State* L)
{
  // checks whether to object on the stack is of this type
  if(!luaCompat_isOfType(L, MODULENAME, LCOM_IUNKNOWN_TYPENAME))
  {
    luacom_APIerror(L, "IUnknown typed object expected.");
    return 0;
  }

  try
  {
    IUnknown* punk = (IUnknown *)*(void **)lua_touserdata(L, -1);
    CHECKPOSCOND(punk);

    tCOMPtr<IDispatch> pdisp;
    CHK_COM_CODE(punk->QueryInterface(IID_IDispatch, (void **) &pdisp));

    tLuaCOM* lcom = tLuaCOM::CreateLuaCOM(L, pdisp);
    
    LuaBeans::push(L, lcom);
    return 1;
  }
  catch(class tLuaCOMException& e)
  {
    luacom_APIerror(L, e.getMessage());
    return 0;
  }
}

int luacom_StartMessageLoop(lua_State *L)
{
  MSG msg;
  if(lua_gettop(L) > 0) {
    tStringBuffer err;
    if(luaCompat_call(L, lua_gettop(L)-1, 0, err)) {
          luacom_APIerror(L, err);
      return 0;
    }
  }
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    if(lua_gettop(L) > 0) {
      tStringBuffer err;
      if(luaCompat_call(L, lua_gettop(L)-1, 0, err)) {
              luacom_APIerror(L, err);
        return 0;
      }
    }
  }
  return 0;
}


/**
  Detects automation in a pure Lua server
*/
int luacom_LuaDetectAutomation(lua_State* L)
{
  lua_getglobal(L,"arg");
  if(lua_istable(L,-1))
  {
    // Running out-of-process
    lua_pushnumber(L,1);
    lua_gettable(L,-2);
    tStringBuffer cmdSwitch(lua_tostring(L,-1));
    lua_pop(L,2);
    if(cmdSwitch != NULL) {
      tStringBuffer errMsg;
      if(_stricmp("/Register",cmdSwitch) == 0) {
        lua_pushstring(L,"Register");
        lua_gettable(L,-2);
        lua_pushvalue(L,-2);
        if(luaCompat_call(L,1,0,errMsg)) {
          lua_pushnil(L);
          lua_pushstring(L,errMsg);
          return 2;
        } else {
          return 1;
        }
      } else if(_stricmp("/UnRegister",cmdSwitch) == 0) {
        lua_pushstring(L,"UnRegister");
        lua_gettable(L,-2);
        lua_pushvalue(L,-2);
        if(luaCompat_call(L,1,0,errMsg)) {
          lua_pushnil(L);
          lua_pushstring(L,errMsg);
          return 2;
        } else {
          return 1;
        }
      } else if(_stricmp("/Automation",cmdSwitch) == 0) {
        lua_pushstring(L,"StartAutomation");
        lua_gettable(L,-2);
        lua_pushvalue(L,-2);
        if(luaCompat_call(L,1,0,errMsg)) {
          lua_pushnil(L);
          lua_pushstring(L,errMsg);
          return 2;
        } else {
          lua_settop(L, 0);
          luacom_StartMessageLoop(L);
          return 0;
        }
      }
      lua_pushnil(L);
      lua_pushstring(L,"no valid command-line switch");
      return 2;
    } else {
      lua_pushnil(L);
      lua_pushstring(L,"no valid command-line switch");
      return 2;
    }
  }
  else
  {
    // Running in-process
    lua_pop(L,1);
    return 1;
  }
}


/**
  Transforms a pointer into an IUnknown typed object.
*/
int luacom_ImportIUnknown(lua_State* L)
{
  IUnknown* punk = (IUnknown*) luaCompat_getPointer(L, -1);

  if(!punk)
  {
    luacom_APIerror(L, "Pointer expected.");
    return 0;
  }

  punk->AddRef();

  luaCompat_pushTypeByName(L, MODULENAME, LCOM_IUNKNOWN_TYPENAME);
  int newref = luaCompat_newTypedObject(L, (void*) punk);

  if(!newref)
  {
    // there was already a userdata for that pointer, 
    // so we have to decrement the reference count,
    // as the garbage collector will be called just
    // once for any userdata
    punk->Release();
  }

  return 1;
}


/**
  Returns current directory
*/
int luacom_GetCurrentDirectory(lua_State* L)
{
  char buffer[1025];

  DWORD size = GetCurrentDirectoryA(1024, buffer);

  if(!size || size > 1023)
    return 0;

  lua_pushstring(L, buffer);
  return 1;
}



///
/// TAG METHODS DO USERTAG IUNKNOWN //
///



/**
  Tag method that manages garbage collection.
*/
static int IUnknown_tag_gc(lua_State *L)
{
  IUnknown* punk = (IUnknown*)*(void **)lua_touserdata(L, -1);

   if(punk != NULL)
     punk->Release();

   return 0;
}

/**
  Checks for IUnknown equality
*/
static int IUnknown_eq(lua_State *L)
{
  LUASTACK_SET(L);

  IUnknown* punk1 = (IUnknown*)*(void **)lua_touserdata(L, -1);
  IUnknown* punk2 = (IUnknown*)*(void **)lua_touserdata(L, -2);

  lua_pushboolean(L, punk1 == punk2);

  LUASTACK_CLEAN(L, 1);

  return 1;
}



///
///  TAG METHODS FOR LUACOM OBJECT
///



/**
 Tag method that manages garbage collection.
*/
static int tagmeth_gc(lua_State *L)
{
  tLuaCOM* lcom = (tLuaCOM*)*(void **)lua_touserdata(L, -1);
  assert(lcom);

  if(lcom != NULL) {
    lcom->Unlock();
  }

  return 0;
}


/**
  tag method que gerencia atribuicoes a campos
  do objeto luacom
*/
static int tagmeth_settable(lua_State *L)
{
  DISPID dispid;
  HRESULT hr = S_OK;

  /* indexes to the parameters coming from lua */
  const int table_param = 1; 
  const int index_param = 2;
  const int value_param = 3;

  try
  {
    tLuaCOM* lcom = (tLuaCOM*) LuaBeans::from_lua(L, table_param);
    CHECK(lcom, INTERNAL_ERROR);

    tStringBuffer field_name(lua_tostring(L, index_param));

    if(!(field_name.getBuffer()))
      return 0;

    if(lua_type(L, value_param) == LUA_TFUNCTION)
    {
      // Here we have a redefinition of the field to a lua
      // function
      lua_rawset(L, table_param);

      return 0;
    }
    
    bool set = false;

    // Here we have two possible situations: the object
    // has type info or the object does not have

    if(lcom->hasTypeInfo())
    {
      FuncInfo funcinfo;
      bool found = lcom->getFUNCDESC(field_name, funcinfo);

      if(found && !funcinfo.propput)
      {
        // Redefinition of a read only field using a value (not a function)
        // is not allowed. We do this to avoid confusion, as the user might
        // inadvertently set the value of a read-only field and LuaCOM would
        // silently store it in the table, "fooling" the client about the
        // real value the field in the COM object. If the user really wishes
        // to redefine the field, he should use a lua function instead

        luacom_error(L, "LuaCOM error: trying to set a read-only field in a COM object.");
        return 0;
      }
      else if(found)
      {
        lcom->call(L, funcinfo.propput->memid,
                   INVOKE_PROPERTYPUT,
                   funcinfo.propput,
                   tLuaObjList(value_param, 1)
                   );

        set = true;
      }
    }

    if(!set && lcom->getDISPID(field_name, &dispid))
    {
      lcom->call(L, dispid,
                 INVOKE_PROPERTYPUT,
                 NULL,
                 tLuaObjList(value_param, 1)
                 );
      set = true;
    }

    if(!set)
    {
      lua_rawset(L, table_param);

      return 0;
    }
  }
  catch(class tLuaCOMException& e)
  {
    luacom_error(L, e.getMessage());
    return 0;
  }

  return 0;
}


/**
  Closure que gerencia chamadas de metodos
*/
static int callhook(lua_State *L)
{
  try
  {
    // upvalues
    const int luacom_obj_param  = lua_upvalueindex(1);
    const int dispid_param      = lua_upvalueindex(2);
    const int invkind_param     = lua_upvalueindex(3);
    const int funcdesc_param    = lua_upvalueindex(4);

    // self param
    const int self_param  = 1;
    const int first_param = 2;

    // number of lua parameters, excluding the self param
    const int num_params = lua_gettop(L) - 1;
    CHECKPARAM_MSG(num_params >= 0, "self parameter not found.");

    // retrieves parameters from lua stack
    tLuaCOM* lcom = (tLuaCOM *) lua_touserdata(L, luacom_obj_param);
    DISPID dispid = (DISPID) lua_tonumber(L, dispid_param);
    long invkind   = (long) lua_tonumber(L, invkind_param);
    FUNCDESC * pfuncdesc = (FUNCDESC*) lua_touserdata(L, funcdesc_param);

    CHECK(lcom, INTERNAL_ERROR);

      // sets the parameter list excluding the 'self' param
    tLuaObjList params = tLuaObjList(first_param, num_params);

    int num_return_values = lcom->call(L, dispid, invkind, pfuncdesc, params);
    return num_return_values;
  }
  catch(class tLuaCOMException& e)
  {
    luacom_error(L, e.getMessage());
    return 0;
  }
}

static bool checkPrefix(const char* pName, const char** ppStripped_name, bool* propget)
{
  if(strncmp(pName, GET_PREFIX, strlen(GET_PREFIX)) == 0)
  {
    *ppStripped_name = pName+strlen(GET_PREFIX);
    *propget = true;
    return true;
  }
  else if(strncmp(pName, PUT_PREFIX, strlen(PUT_PREFIX)) == 0)
  {
    *ppStripped_name = pName+strlen(PUT_PREFIX);
    *propget = false;
    return true;
  }
  else
    return false;
}



static int untyped_tagmeth_index(lua_State *L,
                                 tLuaCOM* lcom,
                                 const char *field_name)
{
  DISPID dispid;
  HRESULT hr = S_OK;
  INVOKEKIND invkind = INVOKE_FUNC;

  // tries to get the DISPID
  {
    // positions for parameters received from Lua
    const int table_param = lua_gettop(L)-1;
    const int index_param = lua_gettop(L);

    bool found = lcom->getDISPID(field_name, &dispid);

    const char *stripped_name = NULL;
    bool is_propget = false;

    if (!found && checkPrefix(field_name, &stripped_name, &is_propget)) 
    {

      // checks for field redefinition
      {
        lua_pushstring(L, stripped_name);
        lua_rawget(L, table_param);

        if(lua_type(L, -1) == LUA_TFUNCTION)
        {
          // field has been redefined. Leaves function on top
          // of stack and returns
          return 1;
        }
        else  // no redefinition. Removes the value and continues
          lua_remove(L, -1);
      }

      // the user specified a property get. Tries to 
      // get the DISPID again, skipping the prefix

      found = lcom->getDISPID(stripped_name, &dispid);

      if(found)
      {
        if(is_propget)
          invkind = INVOKE_PROPERTYGET;
        else
          invkind = INVOKE_PROPERTYPUT;
      }
    }

    if(!found)
      return 0;
  }

  // Decides which INVOKEKIND should be used
  {

    switch(invkind)
    {
    case INVOKE_PROPERTYGET:
      {
        // pushes closure
        lua_pushlightuserdata(L, (void *) lcom);
        lua_pushnumber(L, dispid);
        lua_pushnumber(L, INVOKE_PROPERTYGET);
        lua_pushnumber(L, 0); // no funcdesc

        lua_pushcclosure(L, callhook, 4);
        return 1;
      }
      
    case INVOKE_PROPERTYPUT:
      {
        // pushes closure
        lua_pushlightuserdata(L, (void *) lcom);
        lua_pushnumber(L, dispid);
        lua_pushnumber(L, INVOKE_PROPERTYPUT);
        lua_pushnumber(L, 0); // no funcdesc

        lua_pushcclosure(L, callhook, 4);
        return 1;
      }

    default:
      {
        // pushes closure
        lua_pushlightuserdata(L, (void *) lcom);
        lua_pushnumber(L, dispid);
        lua_pushnumber(L, INVOKE_PROPERTYGET | INVOKE_FUNC);
        lua_pushnumber(L, 0); // no funcdesc

        lua_pushcclosure(L, callhook, 4);
        return 1;
      }
    }
  }
}



static int typed_tagmeth_index(lua_State *L,
                               tLuaCOM* lcom,
                               const char *field_name)
{
  HRESULT hr          = S_OK;
  bool is_propget = false;
  bool is_propput = false;
  FuncInfo funcinfo;

  // positions for parameters received from Lua
  const int table_param = lua_gettop(L)-1;
  const int index_param = lua_gettop(L);


  // Now tries to get the FUNCDESC
  {
    // First, assumes the user supplied the right name
    // of the method

    bool found = lcom->getFUNCDESC(field_name, funcinfo);
    const char *stripped_name = NULL;

    // if name not found, check for prefixes and for field
    // redefinition

    if(!found && checkPrefix(field_name, &stripped_name, &is_propget))
    {
      // it seems the user supplied a prefix. Check that
      is_propput = !is_propget;

      // checks for field redefinition
      {
        lua_pushstring(L, stripped_name);
        lua_rawget(L, table_param);

        if(lua_type(L, -1) == LUA_TFUNCTION)
        {
          // field has been redefined. Leaves function on top
          // of stack and returns
          return 1;
        }
        else  // no redefinition. Removes the value and continues
          lua_remove(L, -1);
      }


      // Now tries to get the right FUNCDESC, using the name
      // without the prefix

      if(is_propget)
      {
        // the user specified a property get. Tries to 
        // get the FUNCDESC again, skipping the prefix
        found = lcom->getFUNCDESC(stripped_name, funcinfo);
      }
      else // there are only two prefixes...
      {
        found = lcom->getFUNCDESC(stripped_name, funcinfo);
      }
    }
    else  // check for a constant
    {
      if(lcom->getConstant(L, field_name))
      {
        // as constants don't change, stores the value
        // in the table
        lua_pushvalue(L, -2); // index (the name of the constant
        lua_pushvalue(L, -2); // value of the constant

        lua_settable(L, -5); // stores in the table the value

        return 1;
      }
    }

    if(!found)
      return 0;
  }


  // Tries to decide which invokekind should be used
  {
    // First, tests whether should be a property get

    if(funcinfo.propget && !is_propput)
    {
      // if it has no params and the user did not specify
      // a propertyput, make the call. If it's not, tests
      // whether might be a propertyget with params

      if(funcinfo.propget->cParams == 0 && !is_propget)
      {
        return lcom->call(L, funcinfo.propget->memid,
                          INVOKE_PROPERTYGET,
                          funcinfo.propget,
                          tLuaObjList()
                          );
      }
      else if(funcinfo.propget->cParams > 0 || is_propget)
      {
        // pushes closure
        lua_pushlightuserdata(L, (void *) lcom);
        lua_pushnumber(L, funcinfo.propget->memid);
        lua_pushnumber(L, INVOKE_PROPERTYGET);
        lua_pushlightuserdata(L, (void *) funcinfo.propget);

        lua_pushcclosure(L, callhook, 4);
        return 1;
      }
    }

    if(funcinfo.propput && !is_propget)
    {
      if(funcinfo.propput->cParams > 0)
      {
        // property put with parameters
        // pushes closure
        lua_pushlightuserdata(L, (void *) lcom);
        lua_pushnumber(L, funcinfo.propput->memid);
        lua_pushnumber(L, funcinfo.propput->invkind);
        lua_pushlightuserdata(L, (void *) funcinfo.propput);

        lua_pushcclosure(L, callhook, 4);
        return 1;
      }
    }

    if(funcinfo.func && !is_propget && !is_propput)
    {
      // pushes closure
      lua_pushlightuserdata(L, (void *) lcom);
      lua_pushnumber(L, funcinfo.func->memid);
      lua_pushnumber(L, INVOKE_FUNC);
      lua_pushlightuserdata(L, (void *) funcinfo.func);

      lua_pushcclosure(L, callhook, 4);
      return 1;
    }

    // no match: nothing found
    return 0;
  }
}


/**
  Tag method that manages reading LuaCOM object fields.
 
  In: table, index
*/
static int tagmeth_index(lua_State *L)
{
  const bool isnumber = false;

  // indexes for the parameters in the Lua stack
  const int table_param = 1;
  const int index_param = 2; 

  int retval = 0;
  try
  {
    // retrieves LuaCOM object
    tLuaCOM* lcom = (tLuaCOM*) LuaBeans::from_lua(L, table_param);
    CHECK(lcom, INTERNAL_ERROR);

    // retrieves the field name
    tStringBuffer field_name(lua_tostring(L, index_param));

    if(!field_name)
      return 0;

    tUtil::log_verbose("tagmeth_index", "looking for name '%s'", field_name.getBuffer());

    // checks for some predefined attributes
    if(strcmp(field_name, LCOM_IUNKNOWN_ATTRIBUTE) == 0)
    {
      lua_pushlightuserdata(L, (IUnknown*) lcom->GetIDispatch());
      return 1;
    }

    // Further processing will be done by
    // different functions, depending on
    // the presence of type information
    retval = 0;

    if(lcom->hasTypeInfo() && !isnumber)
      retval = typed_tagmeth_index(L, lcom, field_name);

    // tries the version without type information
    if(retval == 0 && !isnumber)
      retval = untyped_tagmeth_index(L, lcom, field_name);
  }
  catch(class tLuaCOMException& e)
  {
    switch(e.code)
    {
    case tLuaCOMException::INTERNAL_ERROR:
    case tLuaCOMException::PARAMETER_OUT_OF_RANGE:
    case tLuaCOMException::UNSUPPORTED_FEATURE:
    default:
      luacom_error(L, e.getMessage());
      return 0;
      break;
    }

  }

  if(retval == 0)
  {
    // BUG: numeric indices trigger this event
    // even if they have a value. Just to
    // avoid trouble, we do rawget here

    lua_rawget(L, table_param);

    retval = 1;
  }

  return retval;
}

static bool luacom_runningInprocess(lua_State* L) {
  lua_pushvalue(L, LUA_REGISTRYINDEX);
  lua_pushstring(L,"inproc");
  lua_gettable(L,-2);
  bool inproc = lua_toboolean(L,-1) != 0;
  lua_pop(L,2);
  return inproc;
}


/**
  Triggered when someone tries to use a LuaCOM object
  as a function. Calls the default method of the LuaCOM
  object in question.
*/
static int call_event(lua_State *L)
{
  // indexes for the parameters in the Lua stack
  const int luacom_object_param = 1;
  const int first_user_param = 2;
  const int num_user_params = lua_gettop(L) - 1;

  int retval = 0;
  try
  {
    // retrieves LuaCOM object
    tLuaCOM* lcom = (tLuaCOM*) LuaBeans::from_lua(L, luacom_object_param);
    CHECK(lcom, INTERNAL_ERROR);

    retval = lcom->call(L, DISPID_VALUE,
                        INVOKE_PROPERTYGET | INVOKE_FUNC,
                        NULL,
                        tLuaObjList(first_user_param, num_user_params)
                        );
  }
  catch(class tLuaCOMException& e)
  {
    luacom_error(L, e.getMessage());
    return 0;
  }

  return retval;
}


static int luacom_RoundTrip(lua_State *L) {
  VARIANTARG v;

  tLuaCOMTypeHandler *handler = new tLuaCOMTypeHandler(NULL);
  handler->lua2com(L, 1, v);
  handler->com2lua(L, v);
  delete handler;

  return 1;
}



///
/// Table of functions exported by Lua.
///



static struct luaL_Reg functions_tb []= 
{
  {"CreateObject",luacom_CreateObject},
  {"GetObject",luacom_GetObject},
  {"CLSIDfromProgID",luacom_CLSIDfromProgID},
  {"ImplInterface",luacom_ImplInterface},
  {"ImplInterfaceFromTypelib",luacom_ImplInterfaceFromTypelib},
  {"addConnection", luacom_addConnection},
  {"releaseConnection", luacom_releaseConnection},
  {"ProgIDfromCLSID",luacom_ProgIDfromCLSID},
  {"isMember",luacom_isMember},
  {"Connect",luacom_Connect},
  {"ShowHelp",luacom_ShowHelp},
  {"NewObject", luacom_NewObject},
  {"NewControl", luacom_NewControl},
  {"ExposeObject", luacom_ExposeObject},
  {"RevokeObject", luacom_RevokeObject},
  {"RegisterObject", luacom_RegisterObject},
  {"UnRegisterObject", luacom_UnRegisterObject},
  {"GetIUnknown", luacom_GetIUnknown},
  {"StartLog", luacom_StartLog},
  {"EndLog", luacom_EndLog},
  {"DumpTypeInfo", luacom_DumpTypeInfo},
  {"GetTypeInfo", luacom_GetTypeInfo},
  {"GetEnumerator", luacom_GetEnumerator},
  {"LoadTypeLibrary", luacom_LoadTypeLibrary},
  {"CreateLuaCOM", luacom_CreateLuaCOM},
  {"ImportIUnknown", luacom_ImportIUnknown},
  {"GetCurrentDirectory", luacom_GetCurrentDirectory},
  {"DetectAutomation", luacom_LuaDetectAutomation},
  {"StartMessageLoop", luacom_StartMessageLoop},
  {"RoundTrip", luacom_RoundTrip},
  {NULL, NULL}
};
  


///
/// Functions exported by C/C++.
///



/**
  Recebe um ponteiro void* que deve ser
  um ponteiro para uma implementacao de uma
  interface IDispatch. E' criado um objeto
  LuaCOM cuja implementacao e' dada por essa
  interface. O objeto criado e' colocado na
  pilha de Lua
*/
LUACOM_API int luacom_IDispatch2LuaCOM(lua_State *L, void *pdisp_arg)
{
  if(pdisp_arg == NULL)
  {
    lua_pushnil(L);
    return 1;
  }

  tLuaCOM* lcom = NULL;
  try
  {
    lcom = tLuaCOM::CreateLuaCOM(L, (IDispatch*)pdisp_arg);
  }
  catch(class tLuaCOMException& e)
  {
    UNUSED(e);
    lua_pushnil(L);
    return 1;
  }

  LuaBeans::push(L, lcom);
  return 1;
}


/*
 * Initializes the library.
 */
LUACOM_API void luacom_open(lua_State *L)
{
  if(L == NULL)
    return;

  LUASTACK_SET(L);

  // creates LuaCOM library table
  luaL_register(L, LIBNAME, functions_tb);

  // prepares to store configuration table in
  // library table
  lua_pushstring(L, CONFIGTABLE_NAME);

  // creates and registers configuration table
  luaCompat_moduleCreate(L, MODULENAME);

  // adds a reference of the configuration table
  // to the library one
  lua_settable(L, -3);
  lua_pop(L, 1);

  LuaBeans::createBeans(L, MODULENAME, LCOM_LUACOM_TYPENAME);

  {
    LuaBeans::Events obj_events, pointer_events;

    obj_events.index = tagmeth_index;
    obj_events.settable = tagmeth_settable;
    obj_events.call = call_event;
    pointer_events.gc = tagmeth_gc;

    LuaBeans::registerObjectEvents(L, obj_events);
    LuaBeans::registerPointerEvents(L, pointer_events);
  }

  // Creates a lua tag for IUnknown pointes
  luaCompat_newLuaType(L, MODULENAME, LCOM_IUNKNOWN_TYPENAME);

  // Creates a lua tag for Connection Point tables
  // and sets tag method
  luaCompat_newLuaType(L, MODULENAME, LCOM_CONNPOINT_TYPENAME);

  luaCompat_pushTypeByName(L, MODULENAME, LCOM_CONNPOINT_TYPENAME);
  lua_pushcfunction(L, tLuaCOMConnPoint::l_tagmeth_index);

  lua_setfield(L, -2, "__index");

  lua_pop(L, 1);

  // Registers enumerator type
  tLuaCOMEnumerator::registerLuaType(L, MODULENAME);

  // Registers type library types
  tLuaTLB::Init(L);

  // Sets a tag method for garbage collection
  luaCompat_pushTypeByName(L, MODULENAME, LCOM_IUNKNOWN_TYPENAME);
  lua_pushcfunction(L, IUnknown_tag_gc);
  lua_setfield(L, -2, "__gc");

  lua_pushstring(L, "__eq");
  lua_pushcfunction(L, IUnknown_eq);
  lua_settable(L, -3);

  lua_pop(L, 1);

  // sets default behaviour on errors
  lua_pushboolean(L, 1);
  luaCompat_moduleSet(L, MODULENAME, LUACOM_SHOULD_ABORT);

  lua_pushboolean(L, 0);
  luaCompat_moduleSet(L, MODULENAME, LUACOM_SHOULD_ABORT_API);

  // loads the lua code that implements the remaining
  // features of LuaCOM
  int top1 = lua_gettop(L);
#ifdef LUA_DEBUGGING
  lua_dofile(L, "luacom5.lua");
#else
#include "luacom5.loh"
#endif
  if (lua_gettop(L) > top1) lua_error(L); // failed loading

  idxDispatch = (void*)&luacom_runningInprocess;

  LUASTACK_CLEAN(L, 0);
}


/**
 Closes the library.
*/
LUACOM_API void luacom_close(lua_State *L)
{
}


/**
  Helper for implementing automation servers with LuaCOM
*/
LUACOM_API int luacom_detectAutomation(lua_State *L, int argc, char *argv[])
{
  int automation_result = LUACOM_NOAUTOMATION;
  int lua_result = 0;

  int top = lua_gettop(L);

  // expects a table with the callback functions
  luaL_checktype (L, -1, LUA_TTABLE);

  // processes command line, looking for automation switches and
  // calling the appropriate callback functions

  while(argc--)
  {
    if(!strcmp(argv[argc], "/Automation"))
    {
      lua_pushstring(L, "StartAutomation");
      lua_gettable(L, -2);
      lua_result = luaCompat_call(L, 0, 0);

      if(!lua_result)
        automation_result = LUACOM_AUTOMATION;
      else
        automation_result = LUACOM_AUTOMATION_ERROR;

      break;
    }
    else  if(!strcmp(argv[argc], "/Register"))
    {
      lua_pushstring(L, "Register");
      lua_gettable(L, -2);
      lua_result = luaCompat_call(L, 0, 0);

      if(!lua_result)
        automation_result = LUACOM_REGISTER;
      else
        automation_result = LUACOM_AUTOMATION_ERROR;

      break;
    }
  }

  // cleans stack
  lua_settop(L, top);

  return automation_result;
}


// preprocessor warnings
#ifdef NO_HTMLHTML
#ifdef _MSC_VER
#pragma message("htmlhelp.h disabled")
#else
#warning(htmlhtlp.h disabled)
#endif
#endif
