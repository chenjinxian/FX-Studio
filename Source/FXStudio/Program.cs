using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace FXStudio
{
    static class Program
    {
        [DllImport("kernel32.dll")]
        static extern IntPtr LoadLibrary(string dllToLoad);

        [DllImport("kernel32.dll")]
        static extern bool FreeLibrary(IntPtr hModule);
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);


            try
            {
                FXStudioForm form = new FXStudioForm();
                MessageHandler messageHandler = form.GetMessageHandler();
                Application.AddMessageFilter(messageHandler);
                Application.Idle += new EventHandler(messageHandler.Application_Idle);
                Application.Run(form);
            }
            catch (DllNotFoundException ex)
            {
                MessageBox.Show(ex.Message, ex.Source);
                IntPtr hModule = LoadLibrary(RenderMethods.editorDllName);
                FreeLibrary(hModule);
                return;
            }
        }
    }
}
