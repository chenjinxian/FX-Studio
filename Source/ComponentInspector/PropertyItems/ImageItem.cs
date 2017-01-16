using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing.Imaging;
/*
 * 27/08/2008
 * 
 * Part of the open source project Inspector
 * Developer : mbr ® (Massimiliano Brugnerotto)
 *  
 */
namespace Inspector
{
    // All classes like "PropertyItem..." inherit directly from BaseItem or another
    // class (eg. StringItem) but the base class must be always BaseItem

    #region Image (image file name with picture preview) property item class

    /// <summary>
    /// Image (image file name with picture preview) property item class.
    /// </summary>
    public class ImageItem : BaseItem
    {

        #region Private internal var./properties

        private System.Drawing.Image mValue = null;
        private System.Drawing.Image mDefaultValue = null;
        private string mFileName = "";                  // File name to load and save
        private string tempFileName = "";               // Temp.file name (a copy of the
        // original mFileName file is done to avoid the "in use/locked" status of the sourse file)

        #endregion

        #region Constructors

        /// <summary>
        /// Constructor.
        /// </summary>
        public ImageItem()
        {
            this.Name = "New image item";
        }

        /// <summary>
        /// Constructor. The DefaultValue is the same as Value.
        /// </summary>
        public ImageItem(string text, System.Drawing.Image value, string fileName)
        {
            this.Name = text;
            this.FileName = fileName;
            this.DefaultValue = value;
            this.Value = value;
        }


        /// <summary>
        /// Constructor.
        /// </summary>
        public ImageItem(string text, System.Drawing.Image value, System.Drawing.Image defaultValue, string fileName)
        {
            this.Name = text;
            this.FileName = fileName;
            this.DefaultValue = defaultValue;
            this.Value = value;
        }

        #endregion

        #region Public Properties

        /// <summary>
        /// Gets/sets the current image value.
        /// </summary>
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

        /// <summary>
        /// Gets/sets the current default image value. To sets the current Value image to the DefaultValue use the SetDefaultValue() method.
        /// </summary>
        public System.Drawing.Image DefaultValue
        {
            get
            {
                return mDefaultValue;
            }
            set
            {
                mDefaultValue = value;
            }
        }

        /// <summary>
        /// Gets/sets the image file name
        /// </summary>
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

        /// <summary>
        /// Gets/sets the current image file name. 
        /// </summary>
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

        /// <summary>
        /// Save the current image (stored into a ram object) to a PNG file using the current FileName property.
        /// </summary>
        public bool ExportImage()
        {
            return ExportImage(this.FileName, ImageFormat.Png, false);
        }

        /// <summary>
        /// Save the current image (stored into a ram object) to a file using a specific fileName and image format.
        /// If saveDefaultImage is false (default setting) this method save the Value image else save its save the DefaultValue image.
        /// </summary>
        public bool ExportImage(string fileName, ImageFormat imgFormat, bool saveDefaultImage)
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
                        // Adjust file name extension according to the image file format
                        fileName = System.IO.Path.GetFileNameWithoutExtension(fileName);
                        if (!path.EndsWith(@"\"))
                            path += @"\";
                        fileName = path + fileName + fileExt;
                    }
                    if (saveDefaultImage)
                    {
                        if (mDefaultValue != null)
                        {
                            mDefaultValue.Save(fileName, imgFormat);
                            return true;
                        }
                        else return false;
                    }
                    else
                    {
                        if (mValue != null)
                        {
                            mValue.Save(fileName, imgFormat);
                            return true;
                        }
                        else return false;
                    }
                }
                else
                    return false;
            }
            catch
            {
                // Errore!
                return false;
            }
        }

        /// <summary>
        /// Sets the image Value property using a picture file (load file using the property FileName).
        /// </summary>
        public bool ImportImage()
        {
            return ImportImage(this.FileName, false);
        }

        /// <summary>
        /// If the loadDefaultImage is false (default) this method sets the image Value property if using a picture file (fileName).
        /// Its sets the DefaultValue image if loadDefaultImage is true.
        /// </summary>
        public bool ImportImage(string fileName, bool loadDefaultImage)
        {
            string tempName = "";
            string tempPath = "";
            string tempExt = "";

            try
            {
                if (fileName.Length > 0 && System.IO.File.Exists(fileName))
                {
                    // Crea copia temporanea
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
                    if (loadDefaultImage)
                    {
                        if (mDefaultValue != null) mDefaultValue.Dispose();
                        mDefaultValue = System.Drawing.Image.FromFile(tempFileName);
                    }

                    else
                    {
                        if (mValue != null) mValue.Dispose();
                        mValue = System.Drawing.Image.FromFile(tempFileName);
                    }
                    return true;
                }
                else
                    // Nome file non valido o file inesistente
                    return false;
            }
            catch
            {
                // Errore!
                if (loadDefaultImage)
                    mDefaultValue = null;
                else
                    mValue = null;
                return false;
            }
        }

        /// <summary>
        /// Sets the Value property as the current DefaultValue property.
        /// </summary>
        public override void SetDefaultValue()
        {
            this.Value = this.DefaultValue;
        }

        /// <summary>
        /// Call this method before image preview if the file image is been edited
        /// </summary>
        public void Refresh()
        {
            if (mValue != null) mValue.Dispose();
            if (tempFileName.Length > 0 && System.IO.File.Exists(tempFileName))
                System.IO.File.Delete(tempFileName);
            ImportImage();
        }


        #endregion

    }

    #endregion

}
