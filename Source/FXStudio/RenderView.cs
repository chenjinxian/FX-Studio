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
    public partial class RenderView : ViewWindow
    {
        public RenderView()
        {
            InitializeComponent();

            IntPtr hInstance = Marshal.GetHINSTANCE(this.GetType().Module);
            RenderMethods.CreateInstance(hInstance, IntPtr.Zero, panelRender.Handle, 1, panelRender.Width, panelRender.Height);
        }

        public Panel GetRenderPanel()
        {
            return this.panelRender;
        }

        private void RenderView_Resize(object sender, EventArgs e)
        {
            Control control = (Control)sender;
            if (control.Width != 0 && control.Height != 0)
                RenderMethods.ResizeWnd(control.Width, control.Height);
        }
    }
}
