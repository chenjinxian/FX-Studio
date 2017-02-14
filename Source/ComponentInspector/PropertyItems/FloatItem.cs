using System;
using System.Collections.Generic;
using System.Text;

namespace Inspector
{
    public class FloatItem : BaseItem
    {
        private float m_Value = 0.0f;
        private float m_MinValue = 0.0f;
        private float m_MaxValue = 100.0f;
        private float m_Step = 1.0f;
        private ValidationRangeCheckType mValidationRangeCheck = ValidationRangeCheckType.Disabled;
        private string mEngineeringUnit = "";
        private int mDecimalPlaces = 1;

        public FloatItem(string categoryKey, string itemKey, float value, float min, float max, float step)
        {
            this.CategoryName = categoryKey;
            this.ItemName = itemKey;
            this.Value = value;
            this.m_MinValue = min;
            this.m_MaxValue = max;
            this.m_Step = step;
        }

        public float Value
        {
            get
            {
                return m_Value;
            }
            set
            {
                float oldValue = m_Value;

                if (mValidationRangeCheck == ValidationRangeCheckType.Automatic)
                {
                    if (value < m_MinValue)
                        m_Value = m_MinValue;
                    else
                        if (value > m_MaxValue)
                        m_Value = m_MaxValue;
                    else
                        m_Value = value;
                }
                else
                    m_Value = value;
                this.Changed = (oldValue != m_Value);
                if (this.Changed)
                    RaiseValueChanged(m_Value);
            }
        }

        public override string ValueString
        {
            get
            {
                if (mEngineeringUnit.Length > 0)
                    return (m_Value + " " + EngineeringUnit);
                else
                    return m_Value.ToString();
            }
            set
            {
                float dblValue;
                string strValue;
                int index;

                try
                {
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
                    dblValue = System.Convert.ToSingle(strValue);
                    this.Value = dblValue;
                }
                catch
                {
                }
            }
        }

        public float Minimum
        {
            get
            {
                return m_MinValue;
            }
        }

        public float Maximum
        {
            get
            {
                return m_MaxValue;
            }
        }

        public float Increment
        {
            get
            {
                return m_Step;
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

        public void SetValidationRange(float minValue, float maxValue, float incrementStep, ValidationRangeCheckType validationRangeCheck)
        {
            if (minValue == maxValue)
                throw new ArgumentException("Parameters minValue and MaxValue can't be equal!", "minValue, maxValue");
            else
            {
                if (minValue > maxValue)
                {
                    m_MinValue = maxValue;
                    m_MaxValue = minValue;
                }
                else
                {
                    m_MinValue = minValue;
                    m_MaxValue = maxValue;
                }
                m_Step = incrementStep;
                ValidationRangeCheck = validationRangeCheck;
                if (validationRangeCheck == ValidationRangeCheckType.Automatic)
                {
                    if (m_Value < m_MinValue) m_Value = m_MinValue;
                    if (m_Value > m_MaxValue) m_Value = m_MaxValue;
                }
            }
        }

        public delegate void ValueChangedHandle(object sender, float value);

        public event ValueChangedHandle ValueChanged;

        private void RaiseValueChanged(float value)
        {
            if (ValueChanged != null)
                ValueChanged(this, value);
        }
    }
}
