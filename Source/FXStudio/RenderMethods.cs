using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace FXStudio
{
    class RenderMethods
    {
#if DEBUG
        public const string editorDllName = "FXStudioCored.dll";
#else
        public const string editorDllName = "FXStudioCore.dll";
#endif

        [DllImport(editorDllName, CallingConvention = CallingConvention.Cdecl)]
        public unsafe static extern bool CreateInstance(
            IntPtr instancePtrAddress,
            IntPtr hPrevInstancePtrAddress,
            IntPtr hWndPtrAddress,
            int nCmdShow,
            int screenWidth, int screenHeight);

        [DllImport(editorDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int DestroyInstance();

        [DllImport(editorDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void ResizeWnd(int wParam, int lParam);

        [DllImport(editorDllName, CallingConvention = CallingConvention.Cdecl)]
        public unsafe static extern void WndProc(IntPtr hWndPtrAddress, int msg, IntPtr wParam, IntPtr lParam);

        [DllImport(editorDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void RenderFrame();

        [DllImport(editorDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void OpenProject([MarshalAs(UnmanagedType.BStr)] string lFileName);

        [DllImport(editorDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void CreateNewProject([MarshalAs(UnmanagedType.BStr)] string lFileName);

        [DllImport(editorDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetTransformType(int type);

        [DllImport(editorDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern uint GetPickedActor(int cusorX, int cusorY);

        [DllImport(editorDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetPickedActor(uint actorId);

        [DllImport(editorDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern uint AddActor([MarshalAs(UnmanagedType.BStr)] string actorXml);

        [DllImport(editorDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool ModifyActor([MarshalAs(UnmanagedType.BStr)] string modificationXml);

        [DllImport(editorDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool RemoveActor(uint actorId);

        [DllImport(editorDllName, CallingConvention = CallingConvention.Cdecl)]
        public unsafe static extern void ImportModel(
            [MarshalAs(UnmanagedType.BStr)] string modelImportPath,
            [MarshalAs(UnmanagedType.BStr)] string modelExportPath,
            [MarshalAs(UnmanagedType.FunctionPtr)] DllProgressCallback callback);

        [DllImport(editorDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern uint AddEffect(
            [MarshalAs(UnmanagedType.BStr)] string effectObjectPath,
            [MarshalAs(UnmanagedType.BStr)] string effectSourcePath,
            [MarshalAs(UnmanagedType.BStr)] string effectName);

        [DllImport(editorDllName, CallingConvention = CallingConvention.Cdecl)]
        public unsafe static extern void GetEffectXml(
            [MarshalAs(UnmanagedType.BStr)] string effectObjectPath, StringBuilder effectXmlPtr, uint size);

        [DllImport(editorDllName, CallingConvention = CallingConvention.Cdecl)]
        public unsafe static extern void SetMoveDelegate(
            [MarshalAs(UnmanagedType.FunctionPtr)] DllMoveDelegate moveDelegate);
    }
}
