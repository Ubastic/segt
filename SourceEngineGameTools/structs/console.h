#pragma once

class ConVar;
class IConVar;
class CCommand;
class ConCommand;
class ConCommandBase;

// Called when a ConVar changes value
// NOTE: For FCVAR_NEVER_AS_STRING ConVars, pOldValue == NULL
typedef void(*FnChangeCallback_t)(IConVar *var, const char *pOldValue, float flOldValue);

class CCvar
{
public:
	///--------------------------------------------------------------------
	/// Methods from IAppSystem
	///--------------------------------------------------------------------
	virtual bool                       Connect(CreateInterfaceFn factory) = 0;                                              // 0
	virtual void                       Disconnect() = 0;                                                                      // 1
	virtual void*                      QueryInterface(const char *pInterfaceName) = 0;                                      // 2
	virtual int                        Init() = 0;                                                                            // 3
	virtual void                       Shutdown() = 0;                                                                        // 4
	virtual const void*                GetDependencies() = 0;                                                                 // 5
	virtual int                        GetTier() = 0;                                                                         // 6
	virtual void                       Reconnect(CreateInterfaceFn factory, const char *pInterfaceName) = 0;                // 7
	virtual void                       UnkFunc() = 0;                                                                         // 8
	
	///--------------------------------------------------------------------
	virtual int                        AllocateDLLIdentifier() = 0;                                                           // 9
	virtual void                       RegisterConCommand(ConCommandBase *pCommandBase) = 0;                                  // 10
	virtual void                       UnregisterConCommand(ConCommandBase *pCommandBase) = 0;                                // 11
	virtual void                       UnregisterConCommands(int id) = 0;                                     // 12
	virtual const char*                GetCommandLineValue(const char *pVariableName) = 0;                                    // 13
	virtual ConCommandBase*            FindCommandBase(const char *name) = 0;                                                 // 14
	virtual const ConCommandBase*      FindCommandBase(const char *name) const = 0;                                           // 15
	virtual ConVar*                    FindVar(const char *var_name) = 0;                                                     // 16
	virtual const ConVar*              FindVar(const char *var_name) const = 0;                                               // 17
	virtual ConCommand*                FindCommand(const char *name) = 0;                                                     // 18
	virtual const ConCommand*          FindCommand(const char *name) const = 0;                                               // 19
	virtual void                       InstallGlobalChangeCallback(FnChangeCallback_t callback) = 0;                          // 20
	virtual void                       RemoveGlobalChangeCallback(FnChangeCallback_t callback) = 0;                           // 21
	virtual void                       CallGlobalChangeCallbacks(ConVar *var, const char *pOldString, float flOldValue) = 0;  // 22
	virtual void                       InstallConsoleDisplayFunc(void* pDisplayFunc) = 0;                      // 23
	virtual void                       RemoveConsoleDisplayFunc(void* pDisplayFunc) = 0;                       // 24
	virtual void                       ConsoleColorPrintf(const Color& clr, const char *pFormat, ...) const = 0;              // 25
	virtual void                       ConsolePrintf(const char *pFormat, ...) const = 0;                                     // 26
	virtual void                       ConsoleDPrintf(const char *pFormat, ...) const = 0;                                    // 27
	virtual void                       RevertFlaggedConVars(int nFlag) = 0;                                                   // 28
};

class IConVar
{
public:
	// Value set
	virtual void SetValue(const char *pValue) = 0;
	virtual void SetValue(float flValue) = 0;
	virtual void SetValue(int nValue) = 0;

	// Return name of command
	virtual const char *GetName(void)const = 0;

	// Accessors.. not as efficient as using GetState()/GetInfo()
	// if you call these methods multiple times on the same IConVar
	virtual bool IsFlagSet(int nFlag) const = 0;
};

class IConCommandBaseAccessor
{
public:
	virtual bool RegisterConCommandBase(ConCommandBase *pVar) = 0;
};

void ConVar_Register(int nCVarFlag, IConCommandBaseAccessor *pAccessor = NULL);

class ConCommandBase
{
public:
	ConCommandBase(void);
	ConCommandBase(const char *pName, const char *pHelpString = 0, int flags = 0);

	virtual						~ConCommandBase(void);
	virtual	bool				IsCommand(void) const;
	virtual bool					IsFlagSet(int flag) const;
	virtual void					AddFlags(int flags);
	virtual void					RemoveFlags(int flags);
	virtual int					GetFlags() const;
	virtual const char*			GetName(void) const;
	virtual const char*			GetHelpText(void) const;
	virtual bool					IsRegistered(void) const;
	virtual int					GetDLLIdentifier() const;
	virtual void					Create(const char *pName, const char *pHelpString = 0, int flags = 0);
	virtual void					Init();

public:
	ConCommandBase*					m_pNext;
	bool							m_bRegistered;
	const char*						m_pszName;
	const char*						m_pszHelpString;
	int								m_nFlags;

public:
	static ConCommandBase*			s_pConCommandBases;
	static IConCommandBaseAccessor*	s_pAccessor;
};

class ConVar : public ConCommandBase, public IConVar
{
public:
	typedef ConCommandBase BaseClass;

	ConVar(const char *pName, const char *pDefaultValue, int flags = 0);

	ConVar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString);
	ConVar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString, bool bMin, float fMin, bool bMax, float fMax);
	ConVar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString, FnChangeCallback_t callback);
	ConVar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString, bool bMin, float fMin, bool bMax, float fMax, FnChangeCallback_t callback);

	virtual						~ConVar(void);

	virtual bool					IsFlagSet(int flag) const;
	virtual const char*				GetHelpText(void) const;
	virtual bool					IsRegistered(void) const;
	virtual const char*				GetName(void) const;
	virtual const char*				GetBaseName(void) const;
	virtual int						GetSplitScreenPlayerSlot() const;
	virtual void					AddFlags(int flags);
	virtual int						GetFlags() const;
	virtual	bool					IsCommand(void) const;
	virtual void					SetValue(const char *value);
	virtual void					SetValue(float value);
	virtual void					SetValue(int value);
	virtual void					SetValue(DWORD value);
	virtual void					InternalSetValue(const char *value);
	virtual void					InternalSetFloatValue(float fNewValue);
	virtual void					InternalSetIntValue(int nValue);
	virtual void					InternalSetColorValue(DWORD value);
	virtual bool					ClampValue(float& value);
	virtual void					ChangeStringValue(const char *tempVal, float flOldValue);
	virtual void					Create(const char *pName, const char *pDefaultValue, int flags = 0,
		const char *pHelpString = 0, bool bMin = false, float fMin = 0.0,
		bool bMax = false, float fMax = false, FnChangeCallback_t callback = 0);


	//----------------------------
	// Non-virtual helper methods
	//----------------------------
	float					GetFloat(void) const;
	int						GetInt(void) const;
	DWORD					GetColor(void) const;
	const char*				GetString(void) const;
	const char*				GetDefault(void) const;

	// Value
	struct CVValue_t
	{
		char*	m_pszString;
		int		m_StringLength;
		float	m_fValue;
		int		m_nValue;
	};

	ConVar*					m_pParent;
	const char	*			m_pszDefaultValue;
	CVValue_t				m_Value;
	bool						m_bHasMin;
	float					m_fMinVal;
	bool						m_bHasMax;
	float					m_fMaxVal;
	FnChangeCallback_t		m_fnChangeCallback;
};

// moved from macros to class to not pollute the global namespace
enum class ConvarFlags
{
	// The default, no flags at all
	FCVAR_NONE = 0,

	// Command to ConVars and ConCommands
	// ConVar Systems
	FCVAR_UNREGISTERED = (1 << 0), // If this is set, don't add to linked list, etc.
	FCVAR_DEVELOPMENTONLY = (1 << 1), // Hidden in released products. Flag is removed automatically if ALLOW_DEVELOPMENT_CVARS is defined.
	FCVAR_GAMEDLL = (1 << 2), // defined by the game DLL
	FCVAR_CLIENTDLL = (1 << 3), // defined by the client DLL
	FCVAR_HIDDEN = (1 << 4), // Hidden. Doesn't appear in find or autocomplete. Like DEVELOPMENTONLY, but can't be compiled out.

	// ConVar only
	FCVAR_PROTECTED = (1 << 5),  // It's a server cvar, but we don't send the data since it's a password, etc.  Sends 1 if it's not bland/zero, 0 otherwise as value
	FCVAR_SPONLY = (1 << 6),  // This cvar cannot be changed by clients connected to a multiplayer server.
	FCVAR_ARCHIVE = (1 << 7),  // set to cause it to be saved to vars.rc
	FCVAR_NOTIFY = (1 << 8),  // notifies players when changed
	FCVAR_USERINFO = (1 << 9),  // changes the client's info string
	FCVAR_CHEAT = (1 << 14), // Only useable in singleplayer / debug / multiplayer & sv_cheats

	FCVAR_PRINTABLEONLY = (1 << 10), // This cvar's string cannot contain unprintable characters ( e.g., used for player name etc ).
	FCVAR_UNLOGGED = (1 << 11), // If this is a FCVAR_SERVER, don't log changes to the log file / console if we are creating a log
	FCVAR_NEVER_AS_STRING = (1 << 12), // never try to print that cvar

	// It's a ConVar that's shared between the client and the server.
	// At signon, the values of all such ConVars are sent from the server to the client (skipped for local
	//  client, of course )
	// If a change is requested it must come from the console (i.e., no remote client changes)
	// If a value is changed while a server is active, it's replicated to all connected clients
	FCVAR_REPLICATED = (1 << 13), // server setting enforced on clients, TODO rename to FCAR_SERVER at some time
	FCVAR_DEMO = (1 << 16), // record this cvar when starting a demo file
	FCVAR_DONTRECORD = (1 << 17), // don't record these command in demofiles
	FCVAR_RELOAD_MATERIALS = (1 << 20), // If this cvar changes, it forces a material reload
	FCVAR_RELOAD_TEXTURES = (1 << 21), // If this cvar changes, if forces a texture reload

	FCVAR_NOT_CONNECTED = (1 << 22), // cvar cannot be changed by a client that is connected to a server
	FCVAR_MATERIAL_SYSTEM_THREAD = (1 << 23), // Indicates this cvar is read from the material system thread
	FCVAR_ARCHIVE_XBOX = (1 << 24), // cvar written to config.cfg on the Xbox

	FCVAR_ACCESSIBLE_FROM_THREADS = (1 << 25), // used as a debugging tool necessary to check material system thread convars

	FCVAR_SERVER_CAN_EXECUTE = (1 << 28), // the server is allowed to execute this command on clients via ClientCommand/NET_StringCmd/CBaseClientState::ProcessStringCmd.
	FCVAR_SERVER_CANNOT_QUERY = (1 << 29), // If this is set, then the server is not allowed to query this cvar's value (via IServerPluginHelpers::StartQueryCvarValue).
	FCVAR_CLIENTCMD_CAN_EXECUTE = (1 << 30), // IVEngineClient::ClientCmd is allowed to execute this command.

	FCVAR_MATERIAL_THREAD_MASK = (FCVAR_RELOAD_MATERIALS | FCVAR_RELOAD_TEXTURES | FCVAR_MATERIAL_SYSTEM_THREAD)
};

class CDefaultAccessor : public IConCommandBaseAccessor
{
public:
	virtual bool RegisterConCommandBase(ConCommandBase *pVar)
	{
		printf_s("Registering ConVar %s\n", pVar->GetName());
		Interfaces.Cvar->RegisterConCommand(pVar);
		return true;
	}
};

ConCommandBase*		ConCommandBase::s_pConCommandBases = NULL;
static int			s_nCVarFlag = 0;
static int			s_nDLLIdentifier = -1;
static bool			s_bRegistered = false;
static CDefaultAccessor s_DefaultAccessor;
IConCommandBaseAccessor	*ConCommandBase::s_pAccessor = &s_DefaultAccessor;

void ConVar_Register(int nCVarFlag, IConCommandBaseAccessor *pAccessor)
{
	if (!Interfaces.Cvar || s_bRegistered)
		return;

	s_bRegistered = true;
	s_nCVarFlag = nCVarFlag;
	s_nDLLIdentifier = Interfaces.Cvar->AllocateDLLIdentifier();

	ConCommandBase *pCur, *pNext;

	ConCommandBase::s_pAccessor = pAccessor ? pAccessor : &s_DefaultAccessor;
	pCur = ConCommandBase::s_pConCommandBases;
	while (pCur)
	{
		pNext = pCur->m_pNext;
		pCur->AddFlags(s_nCVarFlag);
		pCur->Init();
		pCur = pNext;
	}

	ConCommandBase::s_pConCommandBases = NULL;
}

//-------------------------------------------------------------
// ConVar methods
//-------------------------------------------------------------
ConVar::ConVar(const char *pName, const char *pDefaultValue, int flags)
{
	Create(pName, pDefaultValue, flags);
}

ConVar::ConVar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString)
{
	Create(pName, pDefaultValue, flags, pHelpString);
}

ConVar::ConVar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString, bool bMin, float fMin, bool bMax, float fMax)
{
	Create(pName, pDefaultValue, flags, pHelpString, bMin, fMin, bMax, fMax);
}

ConVar::ConVar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString, FnChangeCallback_t callback)
{
	Create(pName, pDefaultValue, flags, pHelpString, false, 0.0, false, 0.0, callback);
}

ConVar::ConVar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString, bool bMin, float fMin, bool bMax, float fMax, FnChangeCallback_t callback)
{
	Create(pName, pDefaultValue, flags, pHelpString, bMin, fMin, bMax, fMax, callback);
}

ConVar::~ConVar(void)
{
	if (m_Value.m_pszString)
	{
		delete[] m_Value.m_pszString;
		m_Value.m_pszString = NULL;
	}
}
bool ConVar::IsFlagSet(int flag) const
{
	return (flag & m_pParent->m_nFlags) ? true : false;
}

const char *ConVar::GetHelpText(void) const
{
	return m_pParent->m_pszHelpString;
}

void ConVar::AddFlags(int flags)
{
	m_pParent->m_nFlags |= flags;
}

int ConVar::GetFlags(void) const
{
	return m_pParent->m_nFlags;
}

bool ConVar::IsRegistered(void) const
{
	return m_pParent->m_bRegistered;
}
bool ConVar::IsCommand(void) const
{
	return false;
}

const char *ConVar::GetName(void) const
{
	return m_pParent->m_pszName;
}
const char* ConVar::GetBaseName(void) const
{
	return m_pParent->m_pszName;
}
float ConVar::GetFloat(void) const
{
	return m_pParent->m_Value.m_fValue;
}
int ConVar::GetInt(void) const
{
	return m_pParent->m_Value.m_nValue;
}
const char* ConVar::GetString(void) const
{
	return m_pParent->m_Value.m_pszString;
}
int ConVar::GetSplitScreenPlayerSlot(void) const
{
	return 0;
}
DWORD ConVar::GetColor(void) const
{
	unsigned char *pColorElement = ((unsigned char *)&m_pParent->m_Value.m_nValue);
	return COLORCODE(pColorElement[0], pColorElement[1], pColorElement[2], pColorElement[3]);
}
void ConVar::InternalSetValue(const char *value)
{
	float fNewValue;
	char  tempVal[32];
	char  *val;

	float flOldValue = m_Value.m_fValue;

	val = (char *)value;
	fNewValue = (float)atof(value);

	if (ClampValue(fNewValue))
	{
		val = tempVal;
	}

	// Redetermine value
	m_Value.m_fValue = fNewValue;
	m_Value.m_nValue = (int)(fNewValue);

	if (!(m_nFlags & (int)ConvarFlags::FCVAR_NEVER_AS_STRING))
	{
		ChangeStringValue(val, flOldValue);
	}
}
void ConVar::ChangeStringValue(const char *tempVal, float flOldValue)
{
	UNREFERENCED_PARAMETER(flOldValue);
	int len = strlen(tempVal) + 1;

	if (len > m_Value.m_StringLength)
	{
		if (m_Value.m_pszString)
		{
			delete[] m_Value.m_pszString;
		}

		m_Value.m_pszString = new char[len];
		m_Value.m_StringLength = len;
	}

	memcpy(m_Value.m_pszString, tempVal, len);

}
bool ConVar::ClampValue(float& value)
{
	if (m_bHasMin && (value < m_fMinVal))
	{
		value = m_fMinVal;
		return true;
	}

	if (m_bHasMax && (value > m_fMaxVal))
	{
		value = m_fMaxVal;
		return true;
	}

	return false;
}
void ConVar::InternalSetFloatValue(float fNewValue)
{
	if (fNewValue == m_Value.m_fValue)
		return;
	// Check bounds
	ClampValue(fNewValue);

	// Redetermine value
	float flOldValue = m_Value.m_fValue;
	m_Value.m_fValue = fNewValue;
	m_Value.m_nValue = (int)fNewValue;

	if (!(m_nFlags & (int)ConvarFlags::FCVAR_NEVER_AS_STRING))
	{
		char tempVal[32];
		ChangeStringValue(tempVal, flOldValue);
	}
}
void ConVar::InternalSetIntValue(int nValue)
{
	if (nValue == m_Value.m_nValue)
		return;

	float fValue = (float)nValue;
	if (ClampValue(fValue))
	{
		nValue = (int)(fValue);
	}

	float flOldValue = m_Value.m_fValue;
	m_Value.m_fValue = fValue;
	m_Value.m_nValue = nValue;

	if (!(m_nFlags & (int)ConvarFlags::FCVAR_NEVER_AS_STRING))
	{
		char tempVal[32];
		ChangeStringValue(tempVal, flOldValue);
	}
}
void ConVar::InternalSetColorValue(DWORD cValue)
{
	int color = (int)cValue;
	InternalSetIntValue(color);
}
void ConVar::Create(const char *pName, const char *pDefaultValue,
	int flags, const char *pHelpString, bool bMin, float fMin,
	bool bMax, float fMax, FnChangeCallback_t callback)
{
	static char *empty_string = "";

	m_pParent = this;

	// Name should be static data
	m_pszDefaultValue = pDefaultValue ? pDefaultValue : empty_string;

	m_Value.m_StringLength = strlen(m_pszDefaultValue) + 1;
	m_Value.m_pszString = new char[m_Value.m_StringLength];
	memcpy(m_Value.m_pszString, m_pszDefaultValue, m_Value.m_StringLength);

	m_bHasMin = bMin;
	m_fMinVal = fMin;
	m_bHasMax = bMax;
	m_fMaxVal = fMax;

	m_fnChangeCallback = callback;
	m_Value.m_fValue = (float)atof(m_Value.m_pszString);
	m_Value.m_nValue = (int)m_Value.m_fValue;

	BaseClass::Create(pName, pHelpString, flags);
}
void ConVar::SetValue(const char *value)
{
	m_pParent->InternalSetValue(value);
}
void ConVar::SetValue(float value)
{
	m_pParent->InternalSetFloatValue(value);
}
void ConVar::SetValue(int value)
{
	m_pParent->InternalSetIntValue(value);
}
void ConVar::SetValue(DWORD value)
{
	m_pParent->InternalSetColorValue(value);
}
const char* ConVar::GetDefault(void) const
{
	return m_pParent->m_pszDefaultValue;
}

//-------------------------------------------------------------
// ConCommandBase methods
//-------------------------------------------------------------
ConCommandBase::ConCommandBase(void)
{
	m_bRegistered = false;
	m_pszName = NULL;
	m_pszHelpString = NULL;

	m_nFlags = 0;
	m_pNext = NULL;
}
ConCommandBase::ConCommandBase(const char *pName, const char *pHelpString /*=0*/, int flags /*= 0*/)
{
	Create(pName, pHelpString, flags);
}
ConCommandBase::~ConCommandBase(void)
{}

bool ConCommandBase::IsCommand(void) const
{
	return true;
}
int ConCommandBase::GetDLLIdentifier() const
{
	return s_nDLLIdentifier;
}
void ConCommandBase::Create(const char *pName, const char *pHelpString /*= 0*/, int flags /*= 0*/)
{
	static const char* empty_string = "";

	m_bRegistered = false;

	m_pszName = pName;
	m_pszHelpString = pHelpString ? pHelpString : empty_string;

	m_nFlags = flags;

	if (!(m_nFlags & (int)ConvarFlags::FCVAR_UNREGISTERED))
	{
		m_pNext = s_pConCommandBases;
		s_pConCommandBases = this;
	}
	else
	{
		m_pNext = NULL;
	}

	Init();
}
void ConCommandBase::Init()
{
	if (s_pAccessor)
	{
		s_pAccessor->RegisterConCommandBase(this);
	}
}
const char* ConCommandBase::GetName(void) const
{
	return m_pszName;
}
bool ConCommandBase::IsFlagSet(int flag) const
{
	return (flag & m_nFlags) ? true : false;
}
void ConCommandBase::AddFlags(int flags)
{
	m_nFlags |= flags;
}
void ConCommandBase::RemoveFlags(int flags)
{
	m_nFlags &= ~flags;
}
int ConCommandBase::GetFlags(void) const
{
	return m_nFlags;
}
const char* ConCommandBase::GetHelpText(void) const
{
	return m_pszHelpString;
}
bool ConCommandBase::IsRegistered(void) const
{
	return m_bRegistered;
}