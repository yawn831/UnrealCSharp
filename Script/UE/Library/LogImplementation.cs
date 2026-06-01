#if WITH_MONO
using System.Runtime.CompilerServices;
#else
using Interop;
#endif

namespace Script.Library
{
    public static class LogImplementation
    {
#if WITH_MONO
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void Log_LogImplementation(char[] InBuffer, uint InReadOffset);
#else
        private static unsafe delegate* unmanaged[Cdecl]<char*, uint, void> __Log_LogImplementation;

        public static unsafe void Log_LogImplementation(char[] InBuffer, uint InReadOffset)
        {
            if (__Log_LogImplementation == null)
            {
                __Log_LogImplementation = (delegate* unmanaged[Cdecl]<char*, uint, void>)
                    MethodBridge.GetMethod("Script.Log::Log_Implementation");
            }

            fixed (char* Ptr = InBuffer)
            {
                __Log_LogImplementation(Ptr, InReadOffset);
            }
        }
#endif
    }
}