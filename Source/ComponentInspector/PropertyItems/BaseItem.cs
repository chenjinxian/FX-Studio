using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing.Imaging;

namespace Inspector
{
    public class BaseItem
    {
        private bool m_Enabled = true;
        private bool m_Visible = true;
        private bool m_Changed = false;
        private bool m_ShowExpandButton = false;
        private string m_CategoryName = "";
        private string m_UIName = "";
        private string m_VariableName = "";

        public BaseItem()
        {
        }

        public bool Enabled { get { return m_Enabled; } set { m_Enabled = value; } }

        public bool Visible { get { return m_Visible; } set { m_Visible = value; } }

        public bool Changed { get { return m_Changed; } set { m_Changed = value; } }

        public bool ShowExpandButton { get { return m_ShowExpandButton; } set { m_ShowExpandButton = value; } }

        public string CategoryName { get { return m_CategoryName; } set { m_CategoryName = value; } }

        public string UIName { get { return m_UIName; } set { m_UIName = value; } }

        public string VariableName { get { return m_VariableName; } set { m_VariableName = value; } }

        public virtual string ValueString { get { return ""; } set { } }
    }
}
