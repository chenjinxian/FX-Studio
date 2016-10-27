#include "Logger.h"
#include "../Utilities/ConcurrentQueue.h"

using std::string;


#pragma region Constants, Statics, and Globals
//------------------------------------------------------------------------------------------------------------------------------------
// Some constants
//------------------------------------------------------------------------------------------------------------------------------------

// the log filename
static const char* ERRORLOG_FILENAME = "error.log";

// default display flags
#ifdef _DEBUG
	const unsigned char ERRORFLAG_DEFAULT =		LOGFLAG_WRITE_TO_DEBUGGER;
	const unsigned char WARNINGFLAG_DEFAULT =	LOGFLAG_WRITE_TO_DEBUGGER;
	const unsigned char LOGFLAG_DEFAULT =		LOGFLAG_WRITE_TO_DEBUGGER;
#elif NDEBUG
	const unsigned char ERRORFLAG_DEFAULT =		(LOGFLAG_WRITE_TO_DEBUGGER | LOGFLAG_WRITE_TO_LOG_FILE);
	const unsigned char WARNINGFLAG_DEFAULT =	(LOGFLAG_WRITE_TO_DEBUGGER | LOGFLAG_WRITE_TO_LOG_FILE);
	const unsigned char LOGFLAG_DEFAULT =		(LOGFLAG_WRITE_TO_DEBUGGER | LOGFLAG_WRITE_TO_LOG_FILE);
#else
	const unsigned char ERRORFLAG_DEFAULT =		0;
	const unsigned char WARNINGFLAG_DEFAULT =	0;
	const unsigned char LOGFLAG_DEFAULT =		0;
#endif

// singleton
class LogMgr;
static LogMgr* s_pLogMgr = NULL;
#pragma endregion

#pragma region LogMgr class
//------------------------------------------------------------------------------------------------------------------------------------
// LogMgr class
//------------------------------------------------------------------------------------------------------------------------------------
class LogMgr
{
public:
	enum ErrorDialogResult
	{
		LOGMGR_ERROR_ABORT,
		LOGMGR_ERROR_RETRY,
		LOGMGR_ERROR_IGNORE
	};

	typedef std::map<string, unsigned char> Tags;
	typedef std::list<Logger::ErrorMessenger*> ErrorMessengerList;
	
	Tags m_tags;
	ErrorMessengerList m_errorMessengers;
	
	// thread safety
	boost::mutex m_tagCriticalSection;
	boost::mutex m_messengerCriticalSection;

public:
	// construction
	LogMgr(void);
	~LogMgr(void);
	void Init(const char* loggingConfigFilename);

	// logs
	void Log(const string& tag, const string& message, const char* funcName, const char* sourceFile, uint32_t lineNum);
	void SetDisplayFlags(const std::string& tag, unsigned char flags);

	// error messengers
	void AddErrorMessenger(Logger::ErrorMessenger* pMessenger);
	LogMgr::ErrorDialogResult Error(const std::string& errorMessage, bool isFatal, const char* funcName, const char* sourceFile, uint32_t lineNum);

private:
	// log helpers
	void OutputFinalBufferToLogs(const string& finalBuffer, unsigned char flags);
	void WriteToLogFile(const string& data);
	void GetOutputBuffer(std::string& outOutputBuffer, const string& tag, const string& message, const char* funcName, const char* sourceFile, uint32_t lineNum);
};
#pragma endregion

#pragma region LogMgr class definition
//------------------------------------------------------------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------------------------------------------------------------
LogMgr::LogMgr(void)
{
	// set up the default log tags
	SetDisplayFlags("ERROR", ERRORFLAG_DEFAULT);
	SetDisplayFlags("WARNING", WARNINGFLAG_DEFAULT);
	SetDisplayFlags("INFO", LOGFLAG_DEFAULT);
}


//------------------------------------------------------------------------------------------------------------------------------------
// Destructor
//------------------------------------------------------------------------------------------------------------------------------------
LogMgr::~LogMgr(void)
{
	boost::mutex::scoped_lock lock(m_messengerCriticalSection);
	for (auto it = m_errorMessengers.begin(); it != m_errorMessengers.end(); ++it)
	{
		Logger::ErrorMessenger* pMessenger = (*it);
		delete pMessenger;
	}
	m_errorMessengers.clear();
}


//------------------------------------------------------------------------------------------------------------------------------------
// Initializes the logger.
//------------------------------------------------------------------------------------------------------------------------------------
void LogMgr::Init(const char* loggingConfigFilename)
{
	if (loggingConfigFilename)
	{
		TiXmlDocument loggingConfigFile(loggingConfigFilename);
		if (loggingConfigFile.LoadFile())
		{
			TiXmlElement* pRoot = loggingConfigFile.RootElement();
			if (!pRoot)
				return;

			// Loop through each child element and load the component
			for (TiXmlElement* pNode = pRoot->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
			{
				unsigned char flags = 0;
				std::string tag(pNode->Attribute("tag"));

				int debugger = 0;
				pNode->Attribute("debugger", &debugger);
				if (debugger)
					flags |= LOGFLAG_WRITE_TO_DEBUGGER;

				int logfile = 0;
				pNode->Attribute("file", &logfile);
				if (logfile)
					flags |= LOGFLAG_WRITE_TO_LOG_FILE;

				SetDisplayFlags(tag, flags);
			}
		}
	}
}


//------------------------------------------------------------------------------------------------------------------------------------
// This function builds up the log string and outputs it to various places based on the display flags (m_displayFlags).
//------------------------------------------------------------------------------------------------------------------------------------
void LogMgr::Log(const string& tag, const string& message, const char* funcName, const char* sourceFile, uint32_t lineNum)
{
	boost::mutex::scoped_lock lock(m_tagCriticalSection);
	Tags::iterator findIt = m_tags.find(tag);
	if (findIt != m_tags.end())
	{		
		string buffer;
		GetOutputBuffer(buffer, tag, message, funcName, sourceFile, lineNum);
		OutputFinalBufferToLogs(buffer, findIt->second);
	}
	else
	{
		// Critical section is exited in the if statement above, so we need to exit it here if that didn't 
		// get executed.
	}
}  // end LogMgr::Log()


//------------------------------------------------------------------------------------------------------------------------------------
// Sets one or more display flags
//------------------------------------------------------------------------------------------------------------------------------------
void LogMgr::SetDisplayFlags(const std::string& tag, unsigned char flags)
{
	boost::mutex::scoped_lock lock(m_tagCriticalSection);
	if (flags != 0)
	{
		Tags::iterator findIt = m_tags.find(tag);
		if (findIt == m_tags.end())
			m_tags.insert(std::make_pair(tag, flags));
		else
			findIt->second = flags;
	}
	else
	{
		m_tags.erase(tag);
	}
}


//------------------------------------------------------------------------------------------------------------------------------------
// Adds an error messenger to the list
//------------------------------------------------------------------------------------------------------------------------------------
void LogMgr::AddErrorMessenger(Logger::ErrorMessenger* pMessenger)
{
	boost::mutex::scoped_lock lock(m_messengerCriticalSection);
	m_errorMessengers.push_back(pMessenger);
}


//------------------------------------------------------------------------------------------------------------------------------------
// Helper for ErrorMessenger.
//------------------------------------------------------------------------------------------------------------------------------------
LogMgr::ErrorDialogResult LogMgr::Error(const std::string& errorMessage, bool isFatal, const char* funcName, const char* sourceFile, uint32_t lineNum)
{
	string tag = ((isFatal) ? ("FATAL") : ("ERROR"));

	// buffer for our final output string
	string buffer;
	GetOutputBuffer(buffer, tag, errorMessage, funcName, sourceFile, lineNum);

	{
		// write the final buffer to all the various logs
		boost::mutex::scoped_lock lock(m_tagCriticalSection);
		Tags::iterator findIt = m_tags.find(tag);
		if (findIt != m_tags.end())
			OutputFinalBufferToLogs(buffer, findIt->second);
	}

	// show the dialog box
	int result = ::MessageBoxA(NULL, buffer.c_str(), tag.c_str(), MB_ABORTRETRYIGNORE|MB_ICONERROR|MB_DEFBUTTON3);

	// act upon the choice
	switch (result)
	{
		case IDIGNORE : return LogMgr::LOGMGR_ERROR_IGNORE;
		case IDABORT  : __debugbreak(); return LogMgr::LOGMGR_ERROR_RETRY;  // assembly language instruction to break into the debugger
		case IDRETRY :	return LogMgr::LOGMGR_ERROR_RETRY;
		default :       return LogMgr::LOGMGR_ERROR_RETRY;
	}
}


//------------------------------------------------------------------------------------------------------------------------------------
// This is a helper function that checks all the display flags and outputs the passed in finalBuffer to the appropriate places.
// 
// IMPORTANT: The two places this function is called from wrap the code in the tag critical section (m_pTagCriticalSection), 
// so that makes this call thread safe.  If you call this from anywhere else, make sure you wrap it in that critical section.
//------------------------------------------------------------------------------------------------------------------------------------
void LogMgr::OutputFinalBufferToLogs(const string& finalBuffer, unsigned char flags)
{
	// Write the log to each display based on the display flags
	if ((flags & LOGFLAG_WRITE_TO_LOG_FILE) > 0)  // log file
		WriteToLogFile(finalBuffer);
	if ((flags & LOGFLAG_WRITE_TO_DEBUGGER) > 0)  // debugger output window
		::OutputDebugStringA(finalBuffer.c_str());
}


//------------------------------------------------------------------------------------------------------------------------------------
// This is a helper function that writes the data string to the log file.
//------------------------------------------------------------------------------------------------------------------------------------
void LogMgr::WriteToLogFile(const string& data)
{
	FILE* pLogFile = NULL;
	fopen_s(&pLogFile, ERRORLOG_FILENAME, "a+");
	if (!pLogFile)
		return;  // can't write to the log file for some reason

	fprintf_s(pLogFile, data.c_str());

	fclose(pLogFile);
}

//------------------------------------------------------------------------------------------------------------------------------------
// Fills outOutputBuffer with the find error string.
//------------------------------------------------------------------------------------------------------------------------------------
void LogMgr::GetOutputBuffer(std::string& outOutputBuffer, const string& tag, const string& message, const char* funcName, const char* sourceFile, uint32_t lineNum)
{
	if (!tag.empty())
		outOutputBuffer = "[" + tag + "] " + message;
	else
		outOutputBuffer = message;

	if (funcName != NULL)
	{
		outOutputBuffer += "\nFunction: ";
		outOutputBuffer += funcName;
	}
	if (sourceFile != NULL)
	{
		outOutputBuffer += "\n";
		outOutputBuffer += sourceFile;
	}
	if (lineNum != 0)
	{
		outOutputBuffer += "\nLine: ";
		char lineNumBuffer[11];
		memset(lineNumBuffer, 0, sizeof(char));
		outOutputBuffer += _itoa_s(lineNum, lineNumBuffer, 10);
	}
	
	outOutputBuffer += "\n";
}

#pragma endregion

#pragma region ErrorMessenger definition
//-----------------------------------------------------------------------------------------------------------------------
// ErrorMessenger
//-----------------------------------------------------------------------------------------------------------------------
Logger::ErrorMessenger::ErrorMessenger(void)
{
	s_pLogMgr->AddErrorMessenger(this);
	m_enabled = true;
}

void Logger::ErrorMessenger::Show(const std::string& errorMessage, bool isFatal, const char* funcName, const char* sourceFile, uint32_t lineNum)
{
	if (m_enabled)
	{
		if (s_pLogMgr->Error(errorMessage, isFatal, funcName, sourceFile, lineNum) == LogMgr::LOGMGR_ERROR_IGNORE)
			m_enabled = false;
	}
}
#pragma endregion

#pragma region C Interface
//-----------------------------------------------------------------------------------------------------------------------
// C Interface
//-----------------------------------------------------------------------------------------------------------------------

namespace Logger {

void Init(const char* loggingConfigFilename)
{
	if (!s_pLogMgr)
	{
		s_pLogMgr = GCC_NEW LogMgr;
		s_pLogMgr->Init(loggingConfigFilename);
	}
}

void Destroy(void)
{
	delete s_pLogMgr;
	s_pLogMgr = NULL;
}

void Log(const string& tag, const string& message, const char* funcName, const char* sourceFile, uint32_t lineNum)
{
	GCC_ASSERT(s_pLogMgr);
	s_pLogMgr->Log(tag, message, funcName, sourceFile, lineNum);
}

void SetDisplayFlags(const std::string& tag, unsigned char flags)
{
	GCC_ASSERT(s_pLogMgr);
	s_pLogMgr->SetDisplayFlags(tag, flags);
}

}  // end namespace Logger
#pragma endregion
