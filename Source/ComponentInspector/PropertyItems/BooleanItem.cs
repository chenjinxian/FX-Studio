using System;
using System.Collections.Generic;
using System.Text;

namespace Inspector
{
    public class BooleanItem : BaseItem
    {

        #region Private internal var./properties

        private bool mValue = false;
        private string mTrueValue = "true";
        private string mFalseValue = "false";

        #endregion

        #region Constructors

        public BooleanItem()
        {
            this.Name = "New boolean item";
        }

        public BooleanItem(string text, bool value)
        {
            this.Name = text;
            this.Value = value;
        }

        #endregion

        #region Public Properties

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
                    throw new ArgumentException("Invalid value! Valid values expected : " + mTrueValue + "," + mFalseValue);
            }
        }

        public string TrueValue
        {
            get
            {
                return mTrueValue;
            }
        }

        public string FalseValue
        {
            get
            {
                return mFalseValue;
            }
        }

        #endregion

        #region Public Methods

        public void SetValidationRange(string falseValue, string trueValue)
        {
            falseValue = falseValue.Trim();
            trueValue = trueValue.Trim();
            if (falseValue.ToLower() == trueValue.ToLower())
                throw new ArgumentException("Parameters can't be equal!", "falseValue, trueValue");
            else
            {
                mFalseValue = falseValue;
                mTrueValue = trueValue;
            }
        }

        #endregion

        #region Public events

        public delegate void ValueChangedHandle(object sender, bool value);

        public event ValueChangedHandle ValueChanged;

        private void RaiseValueChanged(bool value)
        {
            if (ValueChanged != null)
                ValueChanged(this, value);
        }

        #endregion

    }
}
