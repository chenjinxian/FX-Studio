using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing.Imaging;
/*
 * 09/08/2008
 * 
 * Part of the open source project Inspector
 * Developer : mbr ® (Massimiliano Brugnerotto)
 *  
 */
namespace Inspector
{

    #region Public enum.

    /// <summary>
    /// Drop down item style.
    /// </summary>
    public enum DropDownStyleType : int
    {
        /// <summary>
        /// Simple style. Value can be edit.
        /// </summary>
        Simple = 0,
        /// <summary>
        /// Drop down style.
        /// </summary>
        DropDown = 1,
        /// <summary>
        /// Drop down list style. Value can't be edit.
        /// </summary>
        DropDownList = 2
    }

    /// <summary>
    /// Date and/or time type.
    /// </summary>
    public enum DateTimeType : int
    {
        /// <summary>
        /// Only date item.
        /// </summary>
        OnlyDate = 0,
        /// <summary>
        /// Only time item.
        /// </summary>
        OnlyTime = 1,
        /// <summary>
        /// Date and time item.
        /// </summary>
        DateAndTime = 2
    }

    /// <summary>
    /// Validation range check type.
    /// </summary>
    public enum ValidationRangeCheckType : int
    {
        /// <summary>
        /// Disabled (no validation range check).
        /// </summary>
        Disabled = 0,
        /// <summary>
        /// Automatic (auto data validation range. Set value to Minimum or to Maximum if Value is out of range).
        /// </summary>
        Automatic = 1,
        /// <summary>
        /// Manual (manual data validation range. If Value is out of range the program show a warning to the user).
        /// </summary>
        Manual = 2
    }

    #endregion

    // All classes like "PropertyItem..." inherit directly from BaseItem or another
    // class (eg. StringItem) but the base class must be BaseItem

    #region Generic Property Item (all items objects inherit from this object)

    /// <summary>
    /// Generic property item class (common items property and methods). All property grid items inherit from this class. 
    /// </summary>
    public class BaseItem
    {

        #region Private internal var./properties

        private bool mEnabled = true;
        private string mText = "New generic item";
        private string mFormat = "";
        private bool mVisible = true;
        private string mHelpCaption = "";
        private string mHelpText = "";
        private bool mHelpVisible = true;
        private bool mChanged = false;              // True when the Value property (in derived classes) is been changed by user
        private bool mShowExpandButton = false;
        // Video coordinate (mouse control)
        // Empty rectangle if the line is not been painted on the screen
        System.Drawing.Rectangle mRect = new System.Drawing.Rectangle();

        #endregion

        /// <summary>
        /// Constructor
        /// </summary>
        public BaseItem()
        {
        }

        #region Public properties

        /// <summary>
        /// Gets/sets the rectangle area on the screen where will be print the property item value.
        /// </summary>
        public System.Drawing.Rectangle rectangle
        {
            get
            {
                return mRect;
            }
            set
            {
                mRect = value;
            }
        }

        /// <summary>
        /// Gets/sets the property item status (true=enabled for user interaction).
        /// </summary>
        public bool Enabled
        {
            get
            {
                return mEnabled;
            }
            set
            {
                mEnabled = value;
            }
        }

        /// <summary>
        /// Gets/sets the property item visible status.
        /// </summary>
        public bool Visible
        {
            get
            {
                return mVisible;
            }
            set
            {
                mVisible = value;
            }
        }

        /// <summary>
        /// Gets/sets the text that describe the property item.
        /// </summary>
        public string Text
        {
            get
            {
                return mText;
            }
            set
            {
                mText = value;
                if (mHelpCaption == "")
                    mHelpCaption = mText;
            }
        }

        /// <summary>
        /// Gets/sets the property item format used by the ValueString property. 
        /// </summary>
        public string Format
        {
            get
            {
                return mFormat;
            }
            set
            {
                mFormat = value;
            }
        }

        /// <summary>
        /// Get(/set) the value as a formatted string.
        /// </summary>
        public virtual string ValueString
        {
            get
            {
                return "";
            }
            set
            {
            }
        }

        /// <summary>
        /// Gets/sets the property help caption (title).
        /// </summary>
        public string HelpCaption
        {
            get
            {
                return mHelpCaption;
            }
            set
            {
                mHelpCaption = value;
            }
        }

        /// <summary>
        /// Gets/sets the property help text (message).
        /// </summary>
        public string HelpText
        {
            get
            {
                return mHelpText;
            }
            set
            {
                mHelpText = value;
            }
        }

        /// <summary>
        /// Gets/sets the property help visible status.
        /// </summary>
        public bool HelpVisible
        {
            get
            {
                return mHelpVisible;
            }
            set
            {
                mHelpVisible = value;
            }
        }

        /// <summary>
        /// Gets/sets if the Value property is been changed by user (true) or not (false)
        /// </summary>
        public bool Changed
        {
            get
            {
                return mChanged;
            }
            set
            {
                mChanged = value;
            }
        }

        /// <summary>
        /// Gets/sets the expand button visible status ("...", more button). Some property items do not use this feature.
        /// </summary>
        public bool ShowExpandButton
        {
            get
            {
                return mShowExpandButton;
            }
            set
            {
                mShowExpandButton = value;
            }
        }

        #endregion

        #region Public methods

        /// <summary>
        /// Sets the help caption (title) and the help text (message)
        /// </summary>
        /// <param name="helpCaption">Help title (string).</param>
        /// <param name="helpText">Help message (string).</param>
        public void SetHelpCaptionText(string helpCaption, string helpText)
        {
            HelpCaption = helpCaption;
            HelpText = helpText;
            HelpVisible = true;
        }

        /// <summary>
        /// Sets the Value property to the default value.
        /// </summary>
        public virtual void SetDefaultValue()
        {
            // In a derived class this method will run the following code :
            // this.Value = this.DefaultValue;
        }

        #endregion

    }

    #endregion

}
