using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace FXStudio
{
    public struct RenderParam
    {
        public IntPtr hInstance;
        public IntPtr handle;
        public int width;
        public int height;
    }

    public partial class RenderView : ViewWindow
    {
        public RenderView()
        {
            InitializeComponent();

            RenderParam param = new RenderParam();
            param.hInstance = Marshal.GetHINSTANCE(this.GetType().Module);
            param.handle = this.panelRender.Handle;
            param.width = this.panelRender.Width;
            param.height = this.panelRender.Height;
            Thread thread = new Thread(new ParameterizedThreadStart(InitRender));
            thread.SetApartmentState(ApartmentState.MTA);
            thread.Start(param);
            thread.Join();

//             IntPtr hInstance = Marshal.GetHINSTANCE(this.GetType().Module);
//             RenderMethods.CreateInstance(hInstance, IntPtr.Zero, panelRender.Handle, 1, panelRender.Width, panelRender.Height);
        }

        public Panel GetRenderPanel()
        {
            return this.panelRender;
        }

        private static void InitRender(object data)
        {
            RenderParam param = (RenderParam)data;
            try
            {
                IntPtr hInstance = param.hInstance;
                RenderMethods.CreateInstance(hInstance, IntPtr.Zero, param.handle, 1, param.width, param.height);
            }
            catch (Exception e)
            {
                MessageBox.Show("Error: " + e.ToString());
            }
        }
    }
}
