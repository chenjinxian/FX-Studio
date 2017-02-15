using System;
using System.Collections.Generic;
using System.Text;

namespace Inspector
{
    public class BooleanItem : BaseItem
    {
        private bool mValue = false;
        private string mTrueValue = "true";
        private string mFalseValue = "false";

        public BooleanItem(string categoryKey, string itemKey, string variable, bool value)
        {
            this.CategoryName = categoryKey;
            this.UIName = itemKey;
            this.VariableName = variable;
            this.Value = value;
        }

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

        public delegate void ValueChangedHandle(object sender, bool value);

        public event ValueChangedHandle ValueChanged;

        private void RaiseValueChanged(bool value)
        {
            if (ValueChanged != null)
                ValueChanged(this, value);
        }
    }
}
