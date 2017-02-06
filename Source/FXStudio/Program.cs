using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace FXStudio
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            FXStudioForm form = new FXStudioForm();
            MessageHandler messageHandler = form.GetMessageHandler();
            Application.AddMessageFilter(messageHandler);
            Application.Idle += new EventHandler(messageHandler.Application_Idle);
            Application.Run(form);
        }
    }
}
