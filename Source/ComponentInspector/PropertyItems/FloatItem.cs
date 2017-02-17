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

        public FloatItem(string categoryKey, string itemKey, string variable, float value, float min, float max, float step)
        {
            this.CategoryName = categoryKey;
            this.UIName = itemKey;
            this.VariableName = variable;
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
                return m_Value.ToString();
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

        public delegate void ValueChangedHandle(object sender, float value);

        public event ValueChangedHandle ValueChanged;

        private void RaiseValueChanged(float value)
        {
            if (ValueChanged != null)
                ValueChanged(this, value);
        }
    }
}
