#ifndef __PLATFORMDETECTION_H__
#define __PLATFORMDETECTION_H__

#ifdef _WIN32
	#ifdef _WIN64
        #define PN_PLATFORM_WINDOWS
	#else
		#error "Windows x86 Builds are not supported!"
	#endif
#elif defined(__linux__)
	#define PN_PLATFORM_LINUX
#else
	#error "Unknown platform!"
#endif

#endif