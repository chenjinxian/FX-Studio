using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing.Imaging;

namespace Inspector
{
    public class ImageItem : BaseItem
    {

        #region Private internal var./properties

        private System.Drawing.Image mValue = null;
        private string mFileName = "";
        private string tempFileName = "";

        #endregion

        #region Constructors

        public ImageItem()
        {
            this.CategoryName = "Misc";
            this.ItemName = "";
        }

        public ImageItem(string categoryKey, string itemKey, string fileName)
        {
            this.CategoryName = categoryKey;
            this.ItemName = itemKey;
            this.FileName = fileName;
        }

        #endregion

        #region Public Properties

        public System.Drawing.Image Value
        {
            get
            {
                return mValue;
            }
            set
            {
                System.Drawing.Image oldValue = mValue;

                mValue = value;
                //this.Changed = true;
                this.Changed = (mValue != oldValue);
            }
        }

        public override string ValueString
        {
            get
            {
                return mFileName;
            }
            set
            {
                string oldValue = mFileName;

                mFileName = value;
                this.Changed = (oldValue.ToLower() != mFileName.ToLower());
            }
        }

        public string FileName
        {
            get
            {
                return mFileName;
            }
            set
            {
                mFileName = value;
                ImportImage();
            }
        }

        #endregion

        #region Public Methods

        public bool ExportImage()
        {
            return ExportImage(this.FileName, ImageFormat.Png);
        }

        public bool ExportImage(string fileName, ImageFormat imgFormat)
        {
            string path;
            string fileExt = "";

            try
            {
                if (imgFormat == ImageFormat.Bmp) fileExt = ".bmp";
                if (imgFormat == ImageFormat.Emf) fileExt = ".emf";
                if (imgFormat == ImageFormat.Gif) fileExt = ".gif";
                if (imgFormat == ImageFormat.Icon) fileExt = ".ico";
                if (imgFormat == ImageFormat.Jpeg) fileExt = ".jpg";
                if (imgFormat == ImageFormat.Png) fileExt = ".png";
                if (imgFormat == ImageFormat.Tiff) fileExt = ".tif";
                if (imgFormat == ImageFormat.Wmf) fileExt = ".wmf";
                path = System.IO.Path.GetDirectoryName(fileName);
                if (fileName.Length > 0 && System.IO.Directory.Exists(path))
                {
                    if (fileExt.Length > 0)
                    {
                        fileName = System.IO.Path.GetFileNameWithoutExtension(fileName);
                        if (!path.EndsWith(@"\"))
                            path += @"\";
                        fileName = path + fileName + fileExt;
                    }
                    if (mValue != null)
                    {
                        mValue.Save(fileName, imgFormat);
                        return true;
                    }
                    else return false;
                }
                else
                    return false;
            }
            catch
            {
                return false;
            }
        }

        public bool ImportImage()
        {
            return ImportImage(this.FileName);
        }

        public bool ImportImage(string fileName)
        {
            string tempName = "";
            string tempPath = "";
            string tempExt = "";

            try
            {
                if (fileName.Length > 0 && System.IO.File.Exists(fileName))
                {
                    tempExt = System.IO.Path.GetExtension(fileName);
                    if (!tempExt.StartsWith("."))
                        tempExt = "." + tempExt;
                    if (tempExt.Length <= 1)
                        tempExt = ".png";
                    tempPath = System.IO.Path.GetTempPath();
                    if (!tempPath.EndsWith(@"\"))
                        tempPath += @"\";
                    tempName = System.IO.Path.GetTempFileName();
                    tempName = System.IO.Path.GetFileNameWithoutExtension(tempName);
                    tempFileName = tempPath + tempName + tempExt;
                    System.IO.File.Copy(fileName, tempFileName, true);

                    if (mValue != null) mValue.Dispose();
                    mValue = System.Drawing.Image.FromFile(tempFileName);
                    return true;
                }
                else
                    return false;
            }
            catch
            {
                mValue = null;
                return false;
            }
        }

        public void Refresh()
        {
            if (mValue != null) mValue.Dispose();
            if (tempFileName.Length > 0 && System.IO.File.Exists(tempFileName))
                System.IO.File.Delete(tempFileName);
            ImportImage();
        }


        #endregion

    }
}
