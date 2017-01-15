using System;
using System.Collections.Generic;
using System.Text;
/*
 * 11/08/2008
 * 
 * Part of the open source project Inspector
 * Developer : mbr ® (Massimiliano Brugnerotto)
 *  
 */
namespace Inspector
{
    // All classes like "PropertyItem..." inherit directly from PropertyItemGeneric or another
    // class (eg. PropertyItemString) but the base class must be PropertyItemGeneric

    #region Category property item class

    /// <summary>
    /// Category property item class.
    /// </summary>
    public class PropertyItemCategory : PropertyItemGeneric
    {

        #region Private internal var./properties

        private bool mExpanded = true;
        /// <summary>
        /// Items grouped into this category.
        /// </summary>
        public ItemCollection ItemList = null;

        #endregion

        #region Constructors

        /// <summary>
        /// Constructor.
        /// </summary>
        public PropertyItemCategory()
        {
            this.Text = "New string item";
        }

        /// <summary>
        /// Constructor.
        /// </summary>
        public PropertyItemCategory(string text)
        {
            this.Text = text;
            ItemList = new ItemCollection();
        }

        #endregion

        #region Public Properties

        /// <summary>
        /// Get/set the category expanded/collapsed status.
        /// </summary>
        public bool Expanded
        {
            get
            {
                return mExpanded;
            }
            set
            {
                mExpanded = value;
            }
        }

        #endregion

    }

    #endregion

}
