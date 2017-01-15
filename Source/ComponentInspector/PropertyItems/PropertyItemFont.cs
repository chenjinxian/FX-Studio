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

    #region Font property item class

    /// <summary>
    /// Font property item class.
    /// </summary>
    public class PropertyItemFont : PropertyItemGeneric
    {

        #region Private internal var./properties

        private System.Drawing.Font mValue = null;
        private System.Drawing.Font mDefaultValue = null;

        #endregion

        #region Constructors

        /// <summary>
        /// Constructor.
        /// </summary>
        public PropertyItemFont()
        {
            this.Text = "New font item";
        }

        /// <summary>
        /// Constructor. The DefaultValue is the same as Value.
        /// </summary>
        public PropertyItemFont(string text, System.Drawing.Font value)
        {
            this.Text = text;
            this.DefaultValue = value;
            this.Value = value;
        }

        /// <summary>
        /// Constructor.
        /// </summary>
        public PropertyItemFont(string text, System.Drawing.Font value, System.Drawing.Font defaultValue)
        {
            this.Text = text;
            this.DefaultValue = defaultValue;
            this.Value = value;
        }

        #endregion

        #region Public Properties

        /// <summary>
        /// Gets/sets the font value.
        /// </summary>
        public System.Drawing.Font Value
        {
            get
            {
                return mValue;
            }
            set
            {
                System.Drawing.Font oldValue;

                if (mValue == null)
                    oldValue = null;
                else
                    oldValue = (System.Drawing.Font)mValue.Clone();
                mValue = value;
                if (oldValue != null && mValue != null)
                {
                    this.Changed = (oldValue.ToString() != mValue.ToString());
                    oldValue.Dispose();
                }
                else
                    // Maybe the value is changed
                    this.Changed = true;
                if (this.Changed)
                    RaiseValueChanged(mValue);
            }
        }

        /// <summary>
        /// Gets/sets the default value. To sets the current value to the default value use the method SetSefaultValue().
        /// </summary>
        public System.Drawing.Font DefaultValue
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
        /// Gets a human readable text rappresentation of the font object.
        /// </summary>
        public override string ValueString
        {
            get
            {
                string fontStr = "";

                if (mValue == null)
                    fontStr = "(none)";
                else
                    fontStr = mValue.ToString();
                return fontStr;
            }
        }

        #endregion

        #region Public Methods

        /// <summary>
        /// Sets the Value property as the current DefaultValue property.
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
        /// <param name="value">Last font value.</param>
        public delegate void ValueChangedHandle(object sender, System.Drawing.Font value);

        /// <summary>
        /// This event accour when the font value property change.
        /// </summary>
        public event ValueChangedHandle ValueChanged;

        private void RaiseValueChanged(System.Drawing.Font value)
        {
            if (ValueChanged != null)
                // Raise event
                ValueChanged(this, value);
        }

        #endregion

    }

    #endregion

}
