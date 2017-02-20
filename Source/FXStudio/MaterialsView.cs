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
        private string m_BmpTempDir = Path.GetTempPath() + @"\FX-Studio\";
        public MaterialsView()
        {
            InitializeComponent();
            if (!Directory.Exists(m_BmpTempDir))
                Directory.CreateDirectory(m_BmpTempDir);
        }

        public void AddMaterial(string materialName, string materialFile)
        {
            string bmpPath = m_BmpTempDir + Path.GetFileNameWithoutExtension(materialFile) + ".bmp";
            RenderMethods.AddMaterial(@"Materials\" + Path.GetFileName(materialFile), bmpPath);

            ListViewItem item = new ListViewItem();
            item.Text = materialName;
            item.Name = materialFile;
            listViewMaterials.Items.Add(item);

            try
            {
                Image original = Image.FromFile(bmpPath);
                if (original != null)
                {
                    imageListLarge.Images.Add(original);
                    item.ImageIndex = imageListLarge.Images.Count - 1;
                }
            }
            catch (OutOfMemoryException)
            {

            }
        }

        private void panelMaterial_Resize(object sender, EventArgs e)
        {

        }

        private void listViewMaterials_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void listViewMaterials_ItemDrag(object sender, ItemDragEventArgs e)
        {
            ListViewItem item = (ListViewItem)e.Item;
            if (item != null)
                DoDragDrop(e.Item, DragDropEffects.Copy);
        }
    }
}
