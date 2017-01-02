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
        private ToolStripButton[] m_TransformButtons;

        public RenderView()
        {
            InitializeComponent();
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

        private void toolStripButtonTransform_Click(object sender, EventArgs e)
        {
            ToolStripButton select = sender as ToolStripButton;
            foreach (ToolStripButton button in m_TransformButtons)
            {
                button.Checked = (button == select);
            }
        }

        private void RenderView_Load(object sender, EventArgs e)
        {
            m_TransformButtons = new ToolStripButton[]
            {
                toolStripButtonSelect,
                toolStripButtonTranslate,
                toolStripButtonRotate,
                toolStripButtonScale
            };
        }
    }
}
