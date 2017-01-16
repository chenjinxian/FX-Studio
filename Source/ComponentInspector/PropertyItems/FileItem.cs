using System;
using System.Collections.Generic;
using System.Text;
/*
 * 10/08/2008
 * 
 * Part of the open source project Inspector
 * Developer : mbr ® (Massimiliano Brugnerotto)
 *  
 */
namespace Inspector
{
    // All classes like "PropertyItem..." inherit directly from BaseItem or another
    // class (eg. StringItem) but the base class must be always BaseItem

    #region File (file string name) property item class (inherit from StringItem)

    /// <summary>
    /// File (file string name) property item class (inherit from StringItem).
    /// </summary>
    public class FileItem : StringItem
    {

        #region Private internal var./properties

        private string mFilter = "All files (*.*)|*.*";
        private int mFilterIndex = 1;

        #endregion

        #region Constructors

        /// <summary>
        /// Constructor.
        /// </summary>
        public FileItem()
        {
            this.Name = "New file item";
        }

        /// <summary>
        /// Constructor. The DefaultValue is the same as Value.
        /// </summary>
        public FileItem(string text, string value)
        {
            this.Name = text;
            this.Value = value;
        }

        #endregion

        #region Public properties

        /// <summary>
        /// Gets/sets the filter patterns. File types are separeted with semicolons, for example, "Image Files(*.BMP;*.JPG;*.GIF)|*.BMP;*.JPG;*.GIF|All files (*.*)|*.*"
        /// </summary>
        public string Filter
        {
            get
            {
                return mFilter;
            }
            set
            {
                mFilter = value;
            }
        }

        /// <summary>
        /// Gets/sets the index of the filter currently selected in the file dialog box. The default (first) value is 1.
        /// </summary>
        public int FilterIndex
        {
            get
            {
                return mFilterIndex;
            }
            set
            {
                mFilterIndex = value;
            }
        }

        #endregion

    }

    #endregion

}
