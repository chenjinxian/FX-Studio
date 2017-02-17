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
using System.Drawing.Imaging;
using System.Drawing.Drawing2D;

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
                AddNewImage(texture);
            }

            listViewTextures.LargeImageList = imageListLarge;
        }

        private void AddNewImage(string filePath)
        {
            ListViewItem item = new ListViewItem();
            item.Text = Path.GetFileName(filePath);
            item.ToolTipText = filePath;
            listViewTextures.Items.Add(item);

            bool success = false;
            try
            {
                Image original = Image.FromFile(filePath);
                if (original != null)
                {
                    imageListLarge.Images.Add(DevILMethods.FixedSize(original, imageListLarge.ImageSize.Width, imageListLarge.ImageSize.Height));
                    item.ImageIndex = imageListLarge.Images.Count - 1;
                    success = true;
                }
            }
            catch (OutOfMemoryException)
            {

            }

            if (!success)
            {
                Image original = DevILMethods.ImageFromFile(filePath);
                if (original != null)
                {
                    imageListLarge.Images.Add(DevILMethods.FixedSize(original, imageListLarge.ImageSize.Width, imageListLarge.ImageSize.Height));
                    item.ImageIndex = imageListLarge.Images.Count - 1;
                }
                else
                    item.ImageIndex = 0;
            }
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

        private void toolStripButtonAdd_Click(object sender, EventArgs e)
        {
            if (openFileDialogAdd.ShowDialog() == DialogResult.OK)
            {
                string source = openFileDialogAdd.FileName;
                string dest = m_TextureDir + @"\" + Path.GetFileName(source);
                if (!File.Exists(dest))
                {
                    File.Copy(source, dest);
                    AddNewImage(dest);
                }
            }
        }

        private void listViewTextures_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            ListViewItem item = ((ListView)sender).HitTest(e.Location).Item;
            if (item != null)
            {
                m_SelectFile = item.ToolTipText;
                this.DialogResult = DialogResult.OK;
                this.Close();
            }
        }
    }
}
