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

    #region Integer (int32) property item class

    /// <summary>
    /// Integer (int32) property item class.
    /// </summary>
    public class Int32Item : BaseItem
    {

        #region Private internal var./properties

        private int mValue = 0;
        private int mDefaultValue = 0;
        private int mMinValue = 0;
        private int mMaxValue = 100;
        private int mIncrement = 1;
        private bool mValidationRangeCheck = false;
        private string mEngineeringUnit = "";
        private bool mHexadecimal = false;              // true=show value as hexadecimal

        #endregion

        #region Constructors

        /// <summary>
        /// Constructor.
        /// </summary>
        public Int32Item()
        {
            this.Text = "New int32 item";
        }

        /// <summary>
        /// Constructor. The DefaultValue is the same as Value.
        /// </summary>
        public Int32Item(string text, int value)
        {
            this.Text = text;
            this.DefaultValue = value;
            this.Value = value;
        }

        /// <summary>
        /// Constructor.
        /// </summary>
        public Int32Item(string text, int value, int defaultValue)
        {
            this.Text = text;
            this.DefaultValue = defaultValue;
            this.Value = value;
        }

        #endregion

        #region Public Properties

        /// <summary>
        /// Get/set the int32 value
        /// </summary>
        public int Value
        {
            get
            {
                return mValue;
            }
            set
            {
                int oldValue = mValue;

                if (mValidationRangeCheck)
                {
                    // Set only a valid value!
                    if (value < mMinValue)
                        mValue = mMinValue;
                    else
                        if (value > mMaxValue)
                            mValue = mMaxValue;
                        else
                            mValue = value;
                }
                else
                    mValue = value;
                this.Changed = (oldValue != mValue);
                if (this.Changed)
                    RaiseValueChanged(mValue);
            }
        }

        /// <summary>
        /// Get/set the int32 default value.  To sets the current Value to the DefaultValue use the SetDefaultValue() method.
        /// </summary>
        public int DefaultValue
        {
            get
            {
                return mDefaultValue;
            }
            set
            {
                if (mValidationRangeCheck)
                {
                    // Set only a valid value!
                    if (value < mMinValue)
                        mDefaultValue = mMinValue;
                    else
                        if (value > mMaxValue)
                            mDefaultValue = mMaxValue;
                        else
                            mDefaultValue = value;
                }
                else
                    mDefaultValue = value;
            }
        }

        /// <summary>
        /// Get the value as a formatted string plus enginering unit. While setting the value do dot pass the enginering unit string.
        /// </summary>
        public override string ValueString
        {
            get
            {
                if (mEngineeringUnit.Length > 0)
                    return (mValue.ToString(this.Format) + " " + EngineeringUnit);
                else
                    return mValue.ToString(this.Format);
            }
            set
            {
                int intValue;
                string strValue;

                try
                {
                    // Try to remove any enginerring unit (char ' ','(','['). Example :
                    // 12 (pz)  pz=pezzi
                    // 12 pz
                    // 12 [pz]
                    // Will set a value of 12
                    strValue = value;
                    intValue = strValue.IndexOf(' ');
                    if (intValue > 0)
                        strValue = strValue.Remove(intValue);
                    intValue = strValue.IndexOf('(');
                    if (intValue > 0)
                        strValue = strValue.Remove(intValue);
                    intValue = strValue.IndexOf('[');
                    if (intValue > 0)
                        strValue = strValue.Remove(intValue);
                    intValue = System.Convert.ToInt32(strValue);
                    this.Value = intValue;
                }
                catch
                {
                    // Convert to int32 failed!
                }
            }
        }

        /// <summary>
        /// Get the minimum valid value if ValidationRangeCheck is true
        /// </summary>
        public int Minimum
        {
            get
            {
                return mMinValue;
            }
        }

        /// <summary>
        /// Get the maximum valid value if ValidationRangeCheck is true
        /// </summary>
        public int Maximum
        {
            get
            {
                return mMaxValue;
            }
        }

        /// <summary>
        /// Get the increment value (while using a spin up/down button to change the value)
        /// </summary>
        public int Increment
        {
            get
            {
                return mIncrement;
            }
        }

        /// <summary>
        /// Get/set the engineering unit. An empty string means that no engineering unit will be displayed.
        /// </summary>
        public string EngineeringUnit
        {
            get
            {
                return mEngineeringUnit;
            }
            set
            {
                mEngineeringUnit = value.Trim();
            }
        }

        /// <summary>
        /// Get/set if a validation range check is enabled or not.
        /// If validation range check is enabled Value must be between Minimum and Maximum.
        /// </summary>
        public bool ValidationRangeCheck
        {
            get
            {
                return mValidationRangeCheck;
            }
            set
            {
                mValidationRangeCheck = value;
            }
        }

        /// <summary>
        /// Get/set if the Value must be show as a hexadecimal number
        /// </summary>
        public bool Hexadecimal
        {
            get
            {
                return mHexadecimal;
            }
            set
            {
                mHexadecimal = value;
            }
        }

        #endregion

        #region Public Methods

        /// <summary>
        /// Sets the validation range parameters. An exception accour if minValue is the same as maxValue.
        /// </summary>
        public void SetValidationRange(int minValue, int maxValue, int incrementStep)
        {
            if (minValue == maxValue)
                // Error
                throw new ArgumentException("Parameters minValue and MaxValue can't be equal!", "minValue, maxValue");
            else
            {
                if (minValue > maxValue)
                {
                    // Swap values!
                    mMinValue = maxValue;
                    mMaxValue = minValue;
                }
                else
                {
                    mMinValue = minValue;
                    mMaxValue = maxValue;
                }
                mIncrement = incrementStep;
                ValidationRangeCheck = true;
                // Correct invalid value!
                if (mValue < mMinValue) mValue = mMinValue;
                if (mValue > mMaxValue) mValue = mMaxValue;
            }
        }

        /// <summary>
        /// Sets the Value property to the current default value.
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
        /// <param name="value">Last int (int32) value.</param>
        public delegate void ValueChangedHandle(object sender, int value);

        /// <summary>
        /// This event accour when the int (int32) value property change.
        /// </summary>
        public event ValueChangedHandle ValueChanged;

        private void RaiseValueChanged(int value)
        {
            if (ValueChanged != null)
                // Raise event
                ValueChanged(this, value);
        }

        #endregion

    }

    #endregion

}
