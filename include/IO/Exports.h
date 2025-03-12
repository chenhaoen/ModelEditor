#pragma once 

#ifndef WIN32
	#define IO_API
#else
	#ifdef IO_EXPORT
		#define IO_API  __declspec( dllexport )
	#else
		#define IO_API  __declspec( dllimport )
	#endif // 0
#endif // !WIN32