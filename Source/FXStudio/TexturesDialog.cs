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
            try
            {
                DevILMethods.ilInit();
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message);
            }
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
                    imageListLarge.Images.Add(FixedSize(original, imageListLarge.ImageSize.Width, imageListLarge.ImageSize.Height));
                    item.ImageIndex = imageListLarge.Images.Count - 1;
                    success = true;
                }
            }
            catch (OutOfMemoryException)
            {
                
            }

            if (!success)
            {
                uint imageID = 0;
                DevILMethods.ilGenImages(1, ref imageID);
                DevILMethods.ilBindImage(imageID);

                if (DevILMethods.ilLoadImage(filePath) != 0)
                {
                    uint width = DevILMethods.ilGetInteger(DevILMethods.IL_IMAGE_WIDTH);
                    uint height = DevILMethods.ilGetInteger(DevILMethods.IL_IMAGE_HEIGHT);
                    uint depth = DevILMethods.ilGetInteger(DevILMethods.IL_IMAGE_DEPTH);
                    Bitmap original = new Bitmap((int)width, (int)height, PixelFormat.Format32bppArgb);

                    Rectangle rect = new Rectangle(0, 0, (int)width, (int)height);
                    BitmapData bmpData = original.LockBits(rect, ImageLockMode.WriteOnly, PixelFormat.Format32bppArgb);

                    DevILMethods.ilConvertImage(DevILMethods.IL_BGRA, DevILMethods.IL_UNSIGNED_BYTE);
                    DevILMethods.ilCopyPixels(0, 0, 0, width, height, depth, DevILMethods.IL_BGRA, DevILMethods.IL_UNSIGNED_BYTE, bmpData.Scan0);

                    original.UnlockBits(bmpData);

                    if (original != null)
                    {
                        imageListLarge.Images.Add(FixedSize(original, imageListLarge.ImageSize.Width, imageListLarge.ImageSize.Height));
                        item.ImageIndex = imageListLarge.Images.Count - 1;
                    }
                    else
                        item.ImageIndex = 0;
                }

                DevILMethods.ilDeleteImages(1, ref imageID);
            }
        }

        private Image FixedSize(Image original, int width, int height)
        {
            int originalWidth = original.Width;
            int originalHeight = original.Height;
            double ratioX = (double)width / (double)originalWidth;
            double ratioY = (double)height / (double)originalHeight;
            double ratio = ratioX < ratioY ? ratioX : ratioY;

            int destX = Convert.ToInt32((width - (originalWidth * ratio)) / 2);
            int destY = Convert.ToInt32((height - (originalHeight * ratio)) / 2);
            int destWidth = Convert.ToInt32(originalWidth * ratio);
            int destHeight = Convert.ToInt32(originalHeight * ratio);

            Bitmap bmPhoto = new Bitmap(width, height, PixelFormat.Format32bppArgb);
            bmPhoto.SetResolution(original.HorizontalResolution, original.VerticalResolution);

            Graphics grPhoto = Graphics.FromImage(bmPhoto);
            grPhoto.Clear(Color.White);
            grPhoto.InterpolationMode = InterpolationMode.HighQualityBicubic;

            grPhoto.DrawImage(original,
                new Rectangle(destX, destY, destWidth, destHeight),
                new Rectangle(0, 0, originalWidth, originalHeight),
                GraphicsUnit.Pixel);

            grPhoto.Dispose();
            return bmPhoto;
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
    }
}
