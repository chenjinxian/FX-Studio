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
using System.Diagnostics;

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
            int i = 0;
            foreach (ToolStripButton button in m_TransformButtons)
            {
                if (button == select)
                {
                    button.Checked = true;
                    RenderMethods.SetTransformType(i);
                }
                else
                    button.Checked = false;

                i++;
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

        private void panelRender_DragEnter(object sender, DragEventArgs e)
        {

        }

        private void panelRender_DragDrop(object sender, DragEventArgs e)
        {

        }

        private void panelRender_DragOver(object sender, DragEventArgs e)
        {
            Point targetPoint = panelRender.PointToClient(new Point(e.X, e.Y));
            uint actorId = RenderMethods.GetPickedActor(targetPoint.X, targetPoint.Y);
            if (actorId != 0)
                e.Effect = DragDropEffects.Copy;
            else
                e.Effect = DragDropEffects.None;
        }
    }
}
