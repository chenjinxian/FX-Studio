using System;
using System.Collections.Generic;
using System.Text;
/*
 * 09/08/2008
 * 
 * Part of the open source project Inspector
 * Developer : mbr ® (Massimiliano Brugnerotto)
 *  
 */
namespace Inspector
{

    // All classes like "PropertyItem..." inherit directly from PropertyItemGeneric or another
    // class (eg. PropertyItemString) but the base class must be PropertyItemGeneric

    #region String property item class

    /// <summary>
    /// String property item class.
    /// </summary>
    public class PropertyItemString : PropertyItemGeneric
    {

        #region Private internal var./properties

        private string mValue = "";
        private string mDefaultValue = "";
        private int mMaxLength = 0;         // 0 = No max string length limit

        #endregion

        #region Contructors

        /// <summary>
        /// Property item string constructor.
        /// </summary>
        public PropertyItemString()
        {
            this.Text = "New string item";
        }

        /// <summary>
        /// Property item string constructor. The DefaultValue is the same as Value.
        /// </summary>
        public PropertyItemString(string text, string value)
        {
            this.Text = text;
            this.DefaultValue = value;
            this.Value = value;
        }

        /// <summary>
        /// Property item string constructor.
        /// </summary>
        public PropertyItemString(string text, string value, string defaultValue)
        {
            this.Text = text;
            this.DefaultValue = defaultValue;
            this.Value = value;
        }

        #endregion

        #region Public Properties

        /// <summary>
        /// Gets/sets the property string value. If the MaxLength property is > 0 the maximum value string length will be set to MaxLength.
        /// </summary>
        public string Value
        {
            get
            {
                if (mMaxLength > 0 && mValue.Length > mMaxLength)
                    mValue = mValue.Substring(0, mMaxLength);
                return mValue;
            }
            set
            {
                string oldValue = mValue;

                if (mMaxLength > 0 && value.Length > mMaxLength)
                    mValue = value.Substring(0, mMaxLength);
                else
                    mValue = value;
                this.Changed = (oldValue != mValue);
                if (this.Changed)
                    RaiseValueChanged(mValue);
            }
        }

        /// <summary>
        /// Return the string value (as the value property).
        /// </summary>
        public override string ValueString
        {
            get
            {
                return Value;
            }
            set
            {
                // Sets the string value using the .Value property 
                this.Value = value;
            }
        }

        /// <summary>
        /// Gets/sets the default string value. While setting a value a string length check is performed if MaxLength > 0.
        /// To sets the current Value to the DefaultValue use the SetDefaultValue() method.
        /// </summary>
        public string DefaultValue
        {
            get
            {
                if (mMaxLength > 0 && mDefaultValue.Length > mMaxLength)
                    mDefaultValue = mDefaultValue.Substring(0, mMaxLength);
                return mDefaultValue;
            }
            set
            {
                string oldValue = mDefaultValue;

                if (mMaxLength > 0 && value.Length > mMaxLength)
                    mDefaultValue = value.Substring(0, mMaxLength);
                else
                    mDefaultValue = value;
            }
        }

        /// <summary>
        /// Gets/sets the maximum number of charactes for the string value. MaxLength=0 : no charactes limit.
        /// </summary>
        public virtual int MaxLength
        {
            get
            {
                return mMaxLength;
            }
            set
            {
                if (value < 0)
                    mMaxLength = 0;
                else
                    mMaxLength = value;
            }
        }

        #endregion

        #region Public Methods

        /// <summary>
        /// Sets the string default value.
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
        /// <param name="value">Last string value.</param>
        public delegate void ValueChangedHandle(object sender, string value);

        /// <summary>
        /// This event accour when the string value property change.
        /// </summary>
        public event ValueChangedHandle ValueChanged;

        private void RaiseValueChanged(string value)
        {
            if (ValueChanged != null)
                // Raise event
                ValueChanged(this, value);
        }

        #endregion

    }

    #endregion

}