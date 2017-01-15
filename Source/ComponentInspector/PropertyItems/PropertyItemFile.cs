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
    // All classes like "PropertyItem..." inherit directly from PropertyItemGeneric or another
    // class (eg. PropertyItemString) but the base class must be always PropertyItemGeneric

    #region File (file string name) property item class (inherit from PropertyItemString)

    /// <summary>
    /// File (file string name) property item class (inherit from PropertyItemString).
    /// </summary>
    public class PropertyItemFile : PropertyItemString
    {

        #region Private internal var./properties

        private string mFilter = "All files (*.*)|*.*";
        private int mFilterIndex = 1;

        #endregion

        #region Constructors

        /// <summary>
        /// Constructor.
        /// </summary>
        public PropertyItemFile()
        {
            this.Text = "New file item";
        }

        /// <summary>
        /// Constructor. The DefaultValue is the same as Value.
        /// </summary>
        public PropertyItemFile(string text, string value)
        {
            this.Text = text;
            this.DefaultValue = value;
            this.Value = value;
        }

        /// <summary>
        /// Constructor.
        /// </summary>
        public PropertyItemFile(string text, string value, string defaultValue)
        {
            this.Text = text;
            this.DefaultValue = defaultValue;
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
