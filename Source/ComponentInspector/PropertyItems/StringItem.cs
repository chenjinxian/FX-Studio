using System;
using System.Collections.Generic;
using System.Text;

namespace Inspector
{
    #region String property item class

    public class StringItem : BaseItem
    {

        #region Private internal var./properties

        private string m_Value = "";
        private int m_MaxLength = 0;

        #endregion

        #region Contructors

        public StringItem()
        {
            this.CategoryName = "Misc";
            this.ItemName = "";
        }

        public StringItem(string categoryKey, string itemKey, string value)
        {
            this.CategoryName = categoryKey;
            this.ItemName = itemKey;
            this.Value = value;
        }

        #endregion

        #region Public Properties

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

        #endregion

        #region Public events

        public delegate void ValueChangedHandle(object sender, string value);

        public event ValueChangedHandle ValueChanged;

        private void RaiseValueChanged(string value)
        {
            if (ValueChanged != null)
                // Raise event
                ValueChanged(this, value);
        }

        #endregion

    }

    #endregion

}