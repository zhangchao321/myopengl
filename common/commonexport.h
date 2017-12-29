#pragma once
#ifdef _COMMON_EXPORT
#define COMMON_EXPORT __declspec(dllexport)
#else
#define COMMON_EXPORT __declspec(dllimport)
#endif

#define COMMON_EXPORT 
