#pragma once 

#ifndef WIN32
	#define NODES_API
#else
	#ifdef NODES_EXPORT
		#define NODES_API  __declspec( dllexport )
	#else
		#define NODES_API  __declspec( dllimport )
	#endif // 0
#endif // !WIN32