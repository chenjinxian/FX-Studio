using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace Inspector
{
    public class ImageItem : BaseItem
    {
        private System.Drawing.Image m_Value;
        private string m_FileName = "";

        public ImageItem(string categoryKey, string itemKey, string variable, string fileName, System.Drawing.Image value)
        {
            this.CategoryName = categoryKey;
            this.UIName = itemKey;
            this.VariableName = variable;
            this.FileName = fileName;
            this.Value = value;
        }

        public System.Drawing.Image Value { get { return m_Value; } set { m_Value = value; } }

        public override string ValueString { get { return Path.GetFileName(m_FileName); } }

        public string FileName { get { return m_FileName; } set { m_FileName = value; } }

        public void DelegateChanging() { RaiseValueChanging(m_FileName); }

        public delegate void ValueChangedHandle(object sender, string value);

        public event ValueChangedHandle ValueChanging;

        private void RaiseValueChanging(string value)
        {
            if (ValueChanging != null)
                ValueChanging(this, value);
        }
    }
}
