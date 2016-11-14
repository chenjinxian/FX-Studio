#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <windowsx.h>

#include <crtdbg.h>

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <mmsystem.h>

#include <string>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <map>
#include <algorithm>
#include <functional>
#include <memory>

using std::shared_ptr;
using std::weak_ptr;
using std::unique_ptr;
using std::static_pointer_cast;
using std::dynamic_pointer_cast;

#if defined(_DEBUG)
#	define DEBUG_NEW new(_NORMAL_BLOCK,__FILE__, __LINE__)
#else
#	define DEBUG_NEW new
#endif

#include <d3d11_1.h>
#include "SimpleMath.h"
#include "d3dx11effect.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;

#include "tinyxml2.h"
#include "boost/noncopyable.hpp"

#include "Debugging/Logger.h"
#include "Utilities/Utility.h"

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

extern INT WINAPI WindowBaseMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow);
