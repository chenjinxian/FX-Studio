using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing.Imaging;
using System.IO;

namespace Inspector
{
    public class ImageItem : BaseItem
    {
        private string m_FileName = "";
        private string tempFileName = "";

        public ImageItem(string categoryKey, string itemKey, string variable, string fileName)
        {
            this.CategoryName = categoryKey;
            this.UIName = itemKey;
            this.VariableName = variable;
            this.FileName = fileName;
        }

        public override string ValueString { get { return Path.GetFileName(m_FileName); } }

        public string FileName { get { return m_FileName; } set { m_FileName = value; } }

        public string DelegateChanging() { return RaiseValueChanging(m_FileName); }

        public delegate string ValueChangedHandle(object sender, string value);

        public event ValueChangedHandle ValueChanging;

        private string RaiseValueChanging(string value)
        {
            if (ValueChanging != null)
                return ValueChanging(this, value);
            else
                return "";
        }
    }
}
