using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing.Imaging;
using System.IO;

namespace Inspector
{
    public class ImageItem : BaseItem
    {
        private System.Drawing.Image m_Value = null;
        private string m_FileName = "";
        private string tempFileName = "";

        public ImageItem(string categoryKey, string itemKey, string fileName)
        {
            this.CategoryName = categoryKey;
            this.ItemName = itemKey;
            this.FileName = fileName;
        }

        public System.Drawing.Image Value
        {
            get
            {
                return m_Value;
            }
            set
            {
                System.Drawing.Image oldValue = m_Value;

                m_Value = value;
                //this.Changed = true;
                this.Changed = (m_Value != oldValue);
            }
        }

        public override string ValueString
        {
            get
            {
                return Path.GetFileName(m_FileName);
            }
            set
            {
                string oldValue = m_FileName;

                m_FileName = value;
                this.Changed = (oldValue.ToLower() != m_FileName.ToLower());
            }
        }

        public string FileName
        {
            get
            {
                return m_FileName;
            }
            set
            {
                m_FileName = value;
                ImportImage();
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

                    if (m_Value != null) m_Value.Dispose();
                    m_Value = System.Drawing.Image.FromFile(tempFileName);
                    return true;
                }
                else
                    return false;
            }
            catch
            {
                m_Value = null;
                return false;
            }
        }

        public void Refresh()
        {
            if (m_Value != null) m_Value.Dispose();
            if (tempFileName.Length > 0 && System.IO.File.Exists(tempFileName))
                System.IO.File.Delete(tempFileName);
            ImportImage();
        }
    }
}
