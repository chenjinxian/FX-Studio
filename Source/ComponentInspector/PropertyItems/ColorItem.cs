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
    // class (eg. StringItem) but the base class must be BaseItem

    #region Color property item class

    /// <summary>
    /// Color property item class.
    /// </summary>
    public class ColorItem : BaseItem
    {

        #region Private internal var./properties

        private System.Drawing.Color mValue = System.Drawing.Color.Black;
        private System.Drawing.Color mDefaultValue = System.Drawing.Color.Black;

        #endregion

        #region Constructors

        /// <summary>
        /// Constructor.
        /// </summary>
        public ColorItem()
        {
            this.Text = "New color item";
        }

        /// <summary>
        /// Constructor. The DefaultValue is the same as Value.
        /// </summary>
        public ColorItem(string text, System.Drawing.Color value)
        {
            this.Text = text;
            this.DefaultValue = value;
            this.Value = value;
        }

        /// <summary>
        /// Constructor
        /// </summary>
        public ColorItem(string text, System.Drawing.Color value, System.Drawing.Color defaultValue)
        {
            this.Text = text;
            this.DefaultValue = defaultValue;
            this.Value = value;
        }

        #endregion

        #region Public Properties

        /// <summary>
        /// Gets/sets a color value.
        /// </summary>
        public System.Drawing.Color Value
        {
            get
            {
                return mValue;
            }
            set
            {
                System.Drawing.Color oldValue = mValue;

                mValue = value;
                this.Changed = (oldValue.ToArgb() != mValue.ToArgb());
                if (this.Changed)
                    RaiseValueChanged(mValue);
            }
        }

        /// <summary>
        /// Gets/sets the current default color value. To set the current value to the default value use the method SetDefaultValue().
        /// </summary>
        public System.Drawing.Color DefaultValue
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
        /// Return a human readable text rappresentation of the color value.
        /// </summary>
        public override string ValueString
        {
            get
            {
                string colStr;

                colStr = mValue.ToString();
                colStr = colStr.Replace("Color [", "");
                colStr = colStr.Replace("]", "");
                return colStr;
            }
        }

        #endregion

        #region Public Methods

        /// <summary>
        /// Sets the current default color value.
        /// </summary>
        public override void SetDefaultValue()
        {
            this.Value = this.DefaultValue;
        }

        #endregion

        #region Public events

        /// <summary>
        /// Delegate for ValueChanged event.
        /// </summary>
        /// <param name="sender">Object sender.</param>
        /// <param name="value">Last color value.</param>
        public delegate void ValueChangedHandle(object sender, System.Drawing.Color value);

        /// <summary>
        /// This event accour when the color value property change.
        /// </summary>
        public event ValueChangedHandle ValueChanged;

        private void RaiseValueChanged(System.Drawing.Color value)
        {
            if (ValueChanged != null)
                // Raise event
                ValueChanged(this, value);
        }

        #endregion

    }

    #endregion

}
