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

    #region Double property item class

    /// <summary>
    /// Double property item class.
    /// </summary>
    public class FloatItem : BaseItem
    {

        #region Private internal var./properties

        private double mValue = 0.0;
        private double mDefaultValue = 0.0;
        private double mMinValue = 0.0;
        private double mMaxValue = 100.0;
        private double mIncrement = 1.0;
        private ValidationRangeCheckType mValidationRangeCheck = ValidationRangeCheckType.Disabled;
        private string mEngineeringUnit = "";
        private int mDecimalPlaces = 1;                     // (0..10) Number of decimal digits
        // The Value number will be format using the property .Format while printing it on the screen

        #endregion

        #region Constructors

        /// <summary> 
        /// Constructor.
        /// </summary>
        public FloatItem()
        {
            this.Name = "New double item";
        }

        /// <summary>
        /// Constructor. The DefaultValue is the same as Value.
        /// </summary>
        public FloatItem(string text, double value)
        {
            this.Name = text;
            this.DefaultValue = value;
            this.Value = value;
        }

        /// <summary>
        /// Constructor.
        /// </summary>
        public FloatItem(string text, double value, double defaultValue)
        {
            this.Name = text;
            this.DefaultValue = defaultValue;
            this.Value = value;
        }

        #endregion

        #region Public Properties

        /// <summary>
        /// Gets/sets a double value.
        /// </summary>
        public double Value
        {
            get
            {
                return mValue;
            }
            set
            {
                double oldValue = mValue;

                if (mValidationRangeCheck == ValidationRangeCheckType.Automatic)
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
        /// Gets/sets the default double value. To sets the current Value to the DefaultValue use the SetDefaultValue() method.
        /// </summary>
        public double DefaultValue
        {
            get
            {
                return mDefaultValue;
            }
            set
            {
                if (mValidationRangeCheck == ValidationRangeCheckType.Automatic)
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
                double dblValue;
                string strValue;
                int index;

                try
                {
                    // Try to remove any enginerring unit (char ' ','(','['). Example :
                    // 12.5 (m)  m=metri
                    // 12.5 m
                    // 12.5 [m]
                    // Will set a value of 12.5
                    strValue = value;
                    index = strValue.IndexOf(' ');
                    if (index > 0)
                        strValue = strValue.Remove(index);
                    index = strValue.IndexOf('(');
                    if (index > 0)
                        strValue = strValue.Remove(index);
                    index = strValue.IndexOf('[');
                    if (index > 0)
                        strValue = strValue.Remove(index);
                    dblValue = System.Convert.ToDouble(strValue);
                    this.Value = dblValue;
                }
                catch
                {
                    // Convert to double failed!
                }
            }
        }

        /// <summary>
        /// Gets the current minimum value. Use the methos SetValidationRange() to set this parameter.
        /// </summary>
        public double Minimum
        {
            get
            {
                return mMinValue;
            }
        }

        /// <summary>
        /// Gets the current maximum value. Use the methos SetValidationRange() to set this parameter.
        /// </summary>
        public double Maximum
        {
            get
            {
                return mMaxValue;
            }
        }

        /// <summary>
        /// Gets the current increment value (while using the spin up/down buttons). Use the methos SetValidationRange() to set this parameter.
        /// </summary>
        public double Increment
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
        /// Gets/sets if the validation range check is enabled (true) or not (false).
        /// </summary>
        public ValidationRangeCheckType ValidationRangeCheck
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
        /// Gets/sets the decimal places in input data mode. Use the Format() method to show
        /// decimals in output mode.
        /// </summary>
        public int DecimalPlaces
        {
            get
            {
                return mDecimalPlaces;
            }
            set
            {
                if (value < 0)
                    mDecimalPlaces = 0;
                else
                    if (value > 10)
                        mDecimalPlaces = 10;
                    else
                        mDecimalPlaces = value;
            }
        }

        #endregion

        #region Public Methods

        /// <summary>
        /// Sets the validation range parameters. An exception accour if the minValue is the same as the maxValue. 
        /// </summary>
        public void SetValidationRange(double minValue, double maxValue, double incrementStep, ValidationRangeCheckType validationRangeCheck)
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
                ValidationRangeCheck = validationRangeCheck;
                if (validationRangeCheck == ValidationRangeCheckType.Automatic)
                {
                    // Correct invalid value!
                    if (mValue < mMinValue) mValue = mMinValue;
                    if (mValue > mMaxValue) mValue = mMaxValue;
                }
            }
        }

        /// <summary>
        /// Sets the default value.
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
        /// <param name="value">Last double value.</param>
        public delegate void ValueChangedHandle(object sender, double value);

        /// <summary>
        /// This event accour when the double value property is changed.
        /// </summary>
        public event ValueChangedHandle ValueChanged;

        private void RaiseValueChanged(double value)
        {
            if (ValueChanged != null)
                // Raise event
                ValueChanged(this, value);
        }

        #endregion

    }

    #endregion

}
