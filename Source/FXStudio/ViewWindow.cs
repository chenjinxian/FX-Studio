using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using WeifenLuo.WinFormsUI.Docking;

namespace FXStudio
{
    public partial class ViewWindow : DockContent
    {
        public ViewWindow()
        {
            InitializeComponent();
        }

        private void toolStripMenuItemFloat_Click(object sender, EventArgs e)
        {
            this.FloatAt(new Rectangle(98, 133, 200, 383));
        }

        private void toolStripMenuItemDock_Click(object sender, EventArgs e)
        {

        }

        private void toolStripMenuItemAutoHide_Click(object sender, EventArgs e)
        {

        }

        private void toolStripMenuItemHide_Click(object sender, EventArgs e)
        {
            this.Hide();
        }
    }
}
