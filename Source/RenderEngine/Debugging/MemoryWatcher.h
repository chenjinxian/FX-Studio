#pragma once

#ifdef _DEBUG
#include "../Utilities/UtilityString.h"

#define GCC_MEMORY_WATCHER_DECLARATION() \
    public: \
        static long s_numAllocs; \
        static void* operator new(size_t size, int memBlock = _NORMAL_BLOCK, const char* filename = "<unknown>", int lineNum = 0); \
        static void operator delete(void* pPtr); \
        static void* operator new[](size_t size, int memBlock = _NORMAL_BLOCK, const char* filename = "<unknown>", int lineNum = 0); \
        static void operator delete[](void* pPtr); \
    private:

#define GCC_MEMORY_WATCHER_DEFINITION(_className_) \
    long _className_::s_numAllocs = 0; \
    void* _className_::operator new(size_t size, int memBlock, const char* filename, int lineNum) \
    { \
        void* pMem = malloc(size); \
        ++s_numAllocs; \
        GCC_LOG("MemoryWatcher", "NEW: 0x" + ToStr(reinterpret_cast<unsigned long>(pMem), 16) + " " + ToStr(size) + " (x" + ToStr(s_numAllocs) + ") -> " + std::string(filename) + ":" + ToStr(lineNum)); \
        return pMem; \
    } \
    void _className_::operator delete(void* pPtr) \
    { \
        GCC_LOG("MemoryWatcher", "DELETE: 0x" + ToStr(reinterpret_cast<unsigned long>(pPtr), 16) + " (x" + ToStr(s_numAllocs) + ")"); \
        free(pPtr); \
        --s_numAllocs; \
    } \
    void* _className_::operator new[](size_t size, int memBlock, const char* filename, int lineNum) \
    { \
        void* pMem = malloc(size); \
        GCC_LOG("MemoryWatcher", "NEW: 0x" + ToStr(reinterpret_cast<unsigned long>(pMem), 16) + " " + ToStr(size) + " (x" + ToStr(s_numAllocs) + ") -> " + std::string(filename) + ":" + ToStr(lineNum)); \
        return pMem; \
    } \
    void _className_::operator delete[](void* pPtr) \
    { \
        GCC_LOG("MemoryWatcher", "DELETE: 0x" + ToStr(reinterpret_cast<unsigned long>(pPtr), 16) + " (x" + ToStr(s_numAllocs) + ")"); \
        free(pPtr); \
        --s_numAllocs; \
    }

#else  // !defined(_DEBUG)
    #define GCC_MEMORY_WATCHER_DECLARATION() (sizeof(void*))
    #define GCC_MEMORY_WATCHER_DEFINITION(__className__) (sizeof(__className__))
#endif  // _DEBUG



