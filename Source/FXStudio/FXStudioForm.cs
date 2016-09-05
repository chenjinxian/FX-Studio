using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace FXStudio
{
    public partial class FXStudioForm : Form
    {
        private MessageHandler m_messageHandler;

        public FXStudioForm()
        {
            InitializeComponent();

            try
            {
                IntPtr hInstance = Marshal.GetHINSTANCE(this.GetType().Module);
                IntPtr hwnd = this.displayPanel.Handle;
                RenderMethods.CreateInstance(hInstance, IntPtr.Zero, hwnd, 1, this.displayPanel.Width, this.displayPanel.Height);
                m_messageHandler = new MessageHandler(this.displayPanel, this);
            }
            catch (Exception e)
            {
                MessageBox.Show("Error: " + e.ToString());
            }
        }

        public MessageHandler GetMessageHandler()
        {
            return m_messageHandler;
        }

        private void ShutDown()
        {
            RenderMethods.DestroyInstance();
            Application.Exit();
        }
    }
}
