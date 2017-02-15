using System;
using System.Collections.Generic;
using System.Text;

namespace Inspector
{
    public class StringItem : BaseItem
    {
        private string m_Value = "";
        private int m_MaxLength = 0;
        public StringItem(string categoryKey, string itemKey, string variable, string value)
        {
            this.CategoryName = categoryKey;
            this.UIName = itemKey;
            this.VariableName = variable;
            this.Value = value;
        }
        public string Value
        {
            get
            {
                if (m_MaxLength > 0 && m_Value.Length > m_MaxLength)
                    m_Value = m_Value.Substring(0, m_MaxLength);
                return m_Value;
            }
            set
            {
                string oldValue = m_Value;

                if (m_MaxLength > 0 && value.Length > m_MaxLength)
                    m_Value = value.Substring(0, m_MaxLength);
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
                return Value;
            }
            set
            {
                this.Value = value;
            }
        }

        public virtual int MaxLength
        {
            get
            {
                return m_MaxLength;
            }
            set
            {
                if (value < 0)
                    m_MaxLength = 0;
                else
                    m_MaxLength = value;
            }
        }

        public delegate void ValueChangedHandle(object sender, string value);

        public event ValueChangedHandle ValueChanged;

        private void RaiseValueChanged(string value)
        {
            if (ValueChanged != null)
                // Raise event
                ValueChanged(this, value);
        }
    }
}