using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace FXStudio
{
    class DevILMethods
    {
        public const uint IL_VERSION_NUM = 0x0DE2;
        public const uint IL_IMAGE_WIDTH = 0x0DE4;
        public const uint IL_IMAGE_HEIGHT = 0x0DE5;
        public const uint IL_IMAGE_DEPTH = 0x0DE6;
        public const uint IL_IMAGE_SIZE_OF_DATA = 0x0DE7;
        public const uint IL_IMAGE_BPP = 0x0DE8;
        public const uint IL_IMAGE_BYTES_PER_PIXEL = 0x0DE8;
        public const uint IL_IMAGE_BITS_PER_PIXEL = 0x0DE9;
        public const uint IL_IMAGE_FORMAT = 0x0DEA;
        public const uint IL_IMAGE_TYPE = 0x0DEB;
        public const uint IL_PALETTE_TYPE = 0x0DEC;
        public const uint IL_PALETTE_SIZE = 0x0DED;
        public const uint IL_PALETTE_BPP = 0x0DEE;
        public const uint IL_PALETTE_NUM_COLS = 0x0DEF;
        public const uint IL_PALETTE_BASE_TYPE = 0x0DF0;
        public const uint IL_NUM_FACES = 0x0DE1;
        public const uint IL_NUM_IMAGES = 0x0DF1;
        public const uint IL_NUM_MIPMAPS = 0x0DF2;
        public const uint IL_NUM_LAYERS = 0x0DF3;
        public const uint IL_ACTIVE_IMAGE = 0x0DF4;
        public const uint IL_ACTIVE_MIPMAP = 0x0DF5;
        public const uint IL_ACTIVE_LAYER = 0x0DF6;
        public const uint IL_ACTIVE_FACE = 0x0E00;
        public const uint IL_CUR_IMAGE = 0x0DF7;
        public const uint IL_IMAGE_DURATION = 0x0DF8;
        public const uint IL_IMAGE_PLANESIZE = 0x0DF9;
        public const uint IL_IMAGE_BPC = 0x0DFA;
        public const uint IL_IMAGE_OFFX = 0x0DFB;
        public const uint IL_IMAGE_OFFY = 0x0DFC;
        public const uint IL_IMAGE_CUBEFLAGS = 0x0DFD;
        public const uint IL_IMAGE_ORIGIN = 0x0DFE;
        public const uint IL_IMAGE_CHANNELS = 0x0DFF;

        public const uint IL_BGRA = 0x80E1;
        public const uint IL_UNSIGNED_BYTE = 0x1401;

        public const string devILDllName = "DevIL_x64.dll";

        [DllImport(devILDllName, CallingConvention = CallingConvention.StdCall)]
        public static extern void ilInit();

        [DllImport(devILDllName, CallingConvention = CallingConvention.StdCall)]
        public static extern void ilGenImages(uint num, ref uint image);

        [DllImport(devILDllName, CallingConvention = CallingConvention.StdCall)]
        public static extern void ilBindImage(uint image);

        [DllImport(devILDllName, CallingConvention = CallingConvention.StdCall)]
        public static extern byte ilLoadImage([MarshalAs(UnmanagedType.LPStr)] string fileName);

        [DllImport(devILDllName, CallingConvention = CallingConvention.StdCall)]
        public static extern void ilDeleteImages(uint num, ref uint image);

        [DllImport(devILDllName, CallingConvention = CallingConvention.StdCall)]
        public static extern uint ilGetInteger(uint mode);

        [DllImport(devILDllName, CallingConvention = CallingConvention.StdCall)]
        public static extern byte ilConvertImage(uint destFormat, uint destTpye);

        [DllImport(devILDllName, CallingConvention = CallingConvention.StdCall)]
        public static extern byte ilCopyPixels(
            uint xOff, uint yOff, uint zOff, uint width, uint height, uint depth, uint format, uint type, IntPtr data);
    }
}
