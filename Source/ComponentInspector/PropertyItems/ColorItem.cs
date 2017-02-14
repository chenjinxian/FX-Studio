using System;
using System.Collections.Generic;
using System.Text;

namespace Inspector
{
    public class ColorItem : BaseItem
    {
        private System.Drawing.Color m_Value = System.Drawing.Color.Black;

        public ColorItem(string categoryKey, string itemKey, System.Drawing.Color value)
        {
            this.CategoryName = categoryKey;
            this.ItemName = itemKey;
            this.Value = value;
        }

        public System.Drawing.Color Value
        {
            get
            {
                return m_Value;
            }
            set
            {
                System.Drawing.Color oldValue = m_Value;

                m_Value = value;
                this.Changed = (oldValue.ToArgb() != m_Value.ToArgb());
                if (this.Changed)
                    RaiseValueChanged(m_Value);
            }
        }

        public override string ValueString
        {
            get
            {
                return (m_Value.R / 255.0F).ToString("F1") + " " +
                    (m_Value.G / 255.0F).ToString("F1") + " " +
                    (m_Value.B / 255.0F).ToString("F1") + " " +
                    (m_Value.A / 255.0F).ToString("F1");
            }
        }

        public delegate void ValueChangedHandle(object sender, System.Drawing.Color value);

        public event ValueChangedHandle ValueChanged;

        private void RaiseValueChanged(System.Drawing.Color value)
        {
            if (ValueChanged != null)
                ValueChanged(this, value);
        }
    }
}
