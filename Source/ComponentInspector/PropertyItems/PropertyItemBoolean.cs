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
    // class (eg. PropertyItemString) but the base class must be PropertyItemGeneric

    #region Boolean property item class

    /// <summary>
    /// Boolean property item class.
    /// </summary>
    public class PropertyItemBoolean : PropertyItemGeneric
    {

        #region Private internal var./properties

        private bool mValue = false;
        private bool mDefaultValue = false;
        private string mTrueValue = "true";
        private string mFalseValue = "false";

        #endregion

        #region Constructors

        /// <summary>
        /// Constructor
        /// </summary>
        public PropertyItemBoolean()
        {
            this.Text = "New boolean item";
        }

        /// <summary>
        /// Constructor. The DefaultValue is the same as Value.
        /// </summary>
        public PropertyItemBoolean(string text, bool value)
        {
            this.Text = text;
            this.DefaultValue = value;
            this.Value = value;
        }

        /// <summary>
        /// Constructor
        /// </summary>
        public PropertyItemBoolean(string text, bool value, bool defaultValue)
        {
            this.Text = text;
            this.DefaultValue = defaultValue;
            this.Value = value;
        }

        #endregion

        #region Public Properties

        /// <summary>
        /// Gets/sets the boolean value.
        /// </summary>
        public bool Value
        {
            get
            {
                return mValue;
            }
            set
            {
                bool oldValue = mValue;

                mValue = value;
                this.Changed = (oldValue != mValue);
                if (this.Changed)
                    RaiseValueChanged(mValue);
            }
        }

        /// <summary>
        /// Gets/sets the boolean default value. To sets the current Value to the DefaultValue use the SetDefaultValue() method.
        /// </summary>
        public bool DefaultValue
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
        /// Gets/sets the boolean value property as a text string (true/false, yes/no, while/black, ...).
        /// The text true/false values can be sets using the method SetValidationRange().
        /// </summary>
        public override string ValueString
        {
            get
            {
                if (mValue)
                    return mTrueValue;
                else
                    return mFalseValue;
            }
            set
            {
                if (value.ToLower() == mTrueValue.ToLower())
                    this.Value = true;
                else
                    if (value.ToLower() == mFalseValue.ToLower())
                        this.Value = false;
                    else
                        // Error
                        throw new ArgumentException("Invalid value! Valid values expected : " + mTrueValue + "," + mFalseValue);
            }
        }

        /// <summary>
        /// Gets the string that rappresent the true value ("Yes", "OK", ...). Use the SetValidationRange() method to sets this parameter.
        /// </summary>
        public string TrueValue
        {
            get
            {
                return mTrueValue;
            }
        }

        /// <summary>
        /// Gets the string that rappresent the false value ("No", "Fail", ...). Use the SetValidationRange() method to sets this parameter.
        /// </summary>
        public string FalseValue
        {
            get
            {
                return mFalseValue;
            }
        }

        #endregion

        #region Public Methods

        /// <summary>
        /// Sets the string format for the false and the true values. Ex. false="no"  true="yes".
        /// </summary>
        public void SetValidationRange(string falseValue, string trueValue)
        {
            falseValue = falseValue.Trim();
            trueValue = trueValue.Trim();
            if (falseValue.ToLower() == trueValue.ToLower())
                // Error
                throw new ArgumentException("Parameters can't be equal!", "falseValue, trueValue");
            else
            {
                mFalseValue = falseValue;
                mTrueValue = trueValue;
            }
        }

        /// <summary>
        /// Sets the current default value.
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
        /// <param name="value">Last boolean value.</param>
        public delegate void ValueChangedHandle(object sender, bool value);

        /// <summary>
        /// This event accour when the boolean value property change.
        /// </summary>
        public event ValueChangedHandle ValueChanged;

        private void RaiseValueChanged(bool value)
        {
            if (ValueChanged != null)
                // Raise event
                ValueChanged(this, value);
        }

        #endregion

    }

    #endregion

}
