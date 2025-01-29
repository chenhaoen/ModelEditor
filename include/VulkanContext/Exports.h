#pragma once 

#ifndef WIN32
	#define VULKANCONTEXT_API
#else
	#ifdef VULKANCONTEXT_EXPORT
		#define VULKANCONTEXT_API  __declspec( dllexport )
	#else
		#define VULKANCONTEXT_API  __declspec( dllimport )
	#endif // 0
#endif // !WIN32