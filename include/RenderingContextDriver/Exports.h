#pragma once 

#ifndef WIN32
	#define RENDERINGCONTEXTDRIVER_API
#else
	#ifdef RENDERINGCONTEXTDRIVER_EXPORT
		#define RENDERINGCONTEXTDRIVER_API  __declspec( dllexport )
	#else
		#define RENDERINGCONTEXTDRIVER_API  __declspec( dllimport )
	#endif // 0
#endif // !WIN32