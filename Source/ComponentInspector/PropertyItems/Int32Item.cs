using System;
using System.Collections.Generic;
using System.Text;

namespace Inspector
{
    public class Int32Item : BaseItem
    {

        #region Private internal var./properties

        private int mValue = 0;
        private int mMinValue = 0;
        private int mMaxValue = 100;
        private int mIncrement = 1;
        private bool mValidationRangeCheck = false;
        private string mEngineeringUnit = "";
        private bool mHexadecimal = false;

        #endregion

        #region Constructors

        public Int32Item()
        {
            this.Name = "Misc";
        }

        public Int32Item(string text, int value)
        {
            this.Name = text;
            this.Value = value;
        }

        public Int32Item(string text, int value, int defaultValue)
        {
            this.Name = text;
            this.Value = value;
        }

        #endregion

        #region Public Properties

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

        public int Minimum
        {
            get
            {
                return mMinValue;
            }
        }

        public int Maximum
        {
            get
            {
                return mMaxValue;
            }
        }

        public int Increment
        {
            get
            {
                return mIncrement;
            }
        }

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

        public void SetValidationRange(int minValue, int maxValue, int incrementStep)
        {
            if (minValue == maxValue)
                throw new ArgumentException("Parameters minValue and MaxValue can't be equal!", "minValue, maxValue");
            else
            {
                if (minValue > maxValue)
                {
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
                if (mValue < mMinValue) mValue = mMinValue;
                if (mValue > mMaxValue) mValue = mMaxValue;
            }
        }

        #endregion

        #region Public events

        public delegate void ValueChangedHandle(object sender, int value);

        public event ValueChangedHandle ValueChanged;

        private void RaiseValueChanged(int value)
        {
            if (ValueChanged != null)
                ValueChanged(this, value);
        }

        #endregion

    }
}
