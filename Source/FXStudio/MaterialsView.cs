using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace FXStudio
{
    public partial class MaterialsView : ViewWindow
    {
        public MaterialsView()
        {
            InitializeComponent();
        }

        public Panel GetRenderPanel()
        {
            return this.panelMaterial;
        }

        public void AddMaterial(string materialFile)
        {
            RenderMethods.AddMaterial(@"Materials\" + Path.GetFileName(materialFile));
        }

        private void panelMaterial_Resize(object sender, EventArgs e)
        {
            Control control = (Control)sender;
            if (control.Width != 0 && control.Height != 0)
                RenderMethods.ResizeWnd(control.Width, control.Height, 1);
        }
    }
}
