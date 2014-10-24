#ifndef vlinder_rtimdb_details_prologue_hpp
#define vlinder_rtimdb_details_prologue_hpp

#ifndef RTIMDB_API
#ifdef rtimdb_EXPORTS
#define RTIMDB_API __declspec(dllexport)
#else
#define RTIMDB_API __declspec(dllimport)
#endif
#endif

#endif

