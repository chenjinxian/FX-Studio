using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing.Imaging;

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

    #region Generic Property Item (all items objects inherit from this object)

    public class BaseItem
    {

        #region Private internal var./properties

        private bool m_Enabled = true;
        private bool m_Visible = true;
        private bool m_Changed = false;
        private bool m_ShowExpandButton = false;
        private string m_Name = "New generic item";

        System.Drawing.Rectangle m_Rect = new System.Drawing.Rectangle();

        #endregion

        public BaseItem()
        {
        }

        #region Public properties

        public System.Drawing.Rectangle Rect { get { return m_Rect; } set { m_Rect = value; } }

        public bool Enabled { get { return m_Enabled; } set { m_Enabled = value; } }

        public bool Visible { get { return m_Visible; } set { m_Visible = value; } }

        public bool Changed { get { return m_Changed; } set { m_Changed = value; } }

        public bool ShowExpandButton { get { return m_ShowExpandButton; } set { m_ShowExpandButton = value; } }

        public string Name { get { return m_Name; } set { m_Name = value; } }

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

        #endregion

    }

    #endregion

}
