#if WITH_CORECLR
#include "Domain/CoreCLR/FCoreCLRFunctionLibrary.h"
#include "Common/FUnrealCSharpFunctionLibrary.h"

FString FCoreCLRFunctionLibrary::GetCoreCLRDirectory()
{
#if WITH_EDITOR
	return FString::Printf(TEXT(
		"%s/Binaries/%s"),
	                       *FUnrealCSharpFunctionLibrary::GetPluginDirectory(),
	                       FPlatformProcess::GetBinariesSubdirectory()
	);
#else
	return FPaths::ConvertRelativePathToFull(FPaths::GetPath(FPlatformProcess::ExecutablePath()));
#endif
}

FString FCoreCLRFunctionLibrary::GetHostFxrPath()
{
	return FString::Printf(TEXT(
		"%s/%s"),
	                       *GetCoreCLRDirectory(),
#if PLATFORM_WINDOWS
	                       TEXT("hostfxr.dll")
#elif PLATFORM_LINUX
	                       TEXT("libhostfxr.so")
#elif PLATFORM_MAC_X86
	                       TEXT("libhostfxr.dylib")
#elif PLATFORM_MAC_ARM64
	                       TEXT("libhostfxr.dylib")
#endif
	);
}

FString FCoreCLRFunctionLibrary::GetRuntimeConfigPath()
{
	return FString::Printf(TEXT(
		"%s/%s"),
	                       *GetCoreCLRDirectory(),
	                       TEXT("CoreCLR.runtimeconfig.json")
	);
}

FString FCoreCLRFunctionLibrary::GetHostPath()
{
	return FPlatformProcess::ExecutablePath();
}
#endif
