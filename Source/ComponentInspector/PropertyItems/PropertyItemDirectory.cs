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

    #region Directory (path string name) property item class (inherit from PropertyItemString)

    /// <summary>
    /// Directory (path string name) property item class (inherit from PropertyItemString).
    /// </summary>
    public class PropertyItemDirectory : PropertyItemString
    {

        #region Private internal var./properties

        private string mDescription = "";       // Optional instruction to show in a dialog browse box (while selecting a directory)

        #endregion

        #region Constructors
        
        /// <summary>
        /// Constructor.
        /// </summary>
        public PropertyItemDirectory()
        {
            this.Text = "New directory item";
        }

        /// <summary>
        /// Constructor. The DefaultValue is the same as Value.
        /// </summary>
        public PropertyItemDirectory(string text, string value)
        {
            this.Text = text;
            this.DefaultValue = value;
            this.Value = value;
        }

        /// <summary>
        /// Constructor.
        /// </summary>
        public PropertyItemDirectory(string text, string value, string defaultValue)
        {
            this.Text = text;
            this.DefaultValue = defaultValue;
            this.Value = value;
        }

        #endregion

        #region Public properties

        /// <summary>
        /// The Description property can be used to specify additional information to the user, like instruction to show in a dialog browse box. The default is an empty string (""). 
        /// </summary>
        public string Description
        {
            get
            {
                return mDescription;
            }
            set
            {
                mDescription = value;
            }
        }

        #endregion

    }

    #endregion

}
