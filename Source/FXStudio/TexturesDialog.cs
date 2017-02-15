using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.IO;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace FXStudio
{
    public partial class TexturesDialog : Form
    {
        private string m_TextureDir;
        private string m_SelectFile;
        public TexturesDialog()
        {
            InitializeComponent();
        }

        public string TextureDirectory { get { return m_TextureDir; } set { m_TextureDir = value; } }

        public string FileName { get { return m_SelectFile; } set { m_SelectFile = value; } }

        private void ImageCollectionsDialog_Load(object sender, EventArgs e)
        {
            foreach (string texture in Directory.GetFiles(m_TextureDir))
            {
                ListViewItem item = new ListViewItem();
                item.Text = Path.GetFileName(texture);
                item.ToolTipText = texture;
                listViewTextures.Items.Add(item);

                if (string.Equals(texture, m_SelectFile, StringComparison.OrdinalIgnoreCase))
                {
                    item.Selected = true;
                }

                try
                {
                    Image image = Image.FromFile(texture);
                    if (image != null)
                    {
                        imageListLarge.Images.Add(image);
                        item.ImageIndex = imageListLarge.Images.Count - 1;
                    }
                }
                catch (OutOfMemoryException)
                {
                    item.ImageIndex = 0;
                }
            }

            listViewTextures.LargeImageList = imageListLarge;
        }

        private void buttonOK_Click(object sender, EventArgs e)
        {
            if (listViewTextures.SelectedIndices.Count > 0)
                m_SelectFile = listViewTextures.SelectedItems[0].ToolTipText;
            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            this.Close();
        }
    }
}
