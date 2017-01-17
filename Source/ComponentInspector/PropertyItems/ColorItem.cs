using System;
using System.Collections.Generic;
using System.Text;

namespace Inspector
{
    public class ColorItem : BaseItem
    {

        #region Private internal var./properties

        private System.Drawing.Color mValue = System.Drawing.Color.Black;
        private System.Drawing.Color mDefaultValue = System.Drawing.Color.Black;

        #endregion

        #region Constructors

        public ColorItem()
        {
            this.Name = "New color item";
        }

        public ColorItem(string text, System.Drawing.Color value)
        {
            this.Name = text;
            this.Value = value;
        }

        #endregion

        #region Public Properties

        public System.Drawing.Color Value
        {
            get
            {
                return mValue;
            }
            set
            {
                System.Drawing.Color oldValue = mValue;

                mValue = value;
                this.Changed = (oldValue.ToArgb() != mValue.ToArgb());
                if (this.Changed)
                    RaiseValueChanged(mValue);
            }
        }

        public override string ValueString
        {
            get
            {
                string colStr;

                colStr = mValue.ToString();
                colStr = colStr.Replace("Color [", "");
                colStr = colStr.Replace("]", "");
                return colStr;
            }
        }

        #endregion

        #region Public events

        public delegate void ValueChangedHandle(object sender, System.Drawing.Color value);

        public event ValueChangedHandle ValueChanged;

        private void RaiseValueChanged(System.Drawing.Color value)
        {
            if (ValueChanged != null)
                ValueChanged(this, value);
        }

        #endregion

    }
}
