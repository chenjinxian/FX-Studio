#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

// TODO: reference additional headers your program requires here
#include <windows.h>
#include <tchar.h>
#include <mmsystem.h>

#include <memory>
#include <algorithm>
#include <string>
#include <list>
#include <vector>
#include <queue>
#include <map>

using std::tr1::shared_ptr;
using std::tr1::weak_ptr;
using std::tr1::static_pointer_cast;
using std::tr1::dynamic_pointer_cast;

#if defined(_DEBUG)
#	define GCC_NEW new(_NORMAL_BLOCK,__FILE__, __LINE__)
#else
#	define GCC_NEW new
#endif

#include "DXUT.h"
#include "SDKmisc.h"
#include "SimpleMath.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;

#include "boost/noncopyable.hpp"
#include "tinyxml.h"

#include "Debugging\Logger.h"
#include "Utilities\templates.h"

#pragma warning( disable : 4996 ) // 'function' declared deprecated - gets rid of all those 2005 warnings....

#if !defined(SAFE_DELETE)
#define SAFE_DELETE(x) if(x) delete x; x=NULL;
#endif

#if !defined(SAFE_DELETE_ARRAY)
#define SAFE_DELETE_ARRAY(x) if (x) delete [] x; x=NULL; 
#endif

#if !defined(SAFE_RELEASE)
#define SAFE_RELEASE(x) if(x) x->Release(); x=NULL;
#endif

#ifdef UNICODE
#define _tcssprintf wsprintf
#define tcsplitpath _wsplitpath
#else
#define _tcssprintf sprintf
#define tcsplitpath _splitpath
#endif

#define __STR2__(x) #x
#define __STR1__(x) __STR2__(x)
#define __LOC__ __FILE__ "("__STR1__(__LINE__)") : Warning Msg: "
