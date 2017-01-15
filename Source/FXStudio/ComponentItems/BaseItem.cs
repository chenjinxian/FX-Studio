using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FXStudio.ComponentItems
{
    public class BaseItem
    {
        private bool m_Enables = true;
        private bool m_Hidden = false;
        private bool m_ShowExpand = false;
        private string m_Name = "";
        private string m_Description = "";

        public BaseItem()
        {

        }

        public bool Enabled { get { return m_Enables; } set { m_Enables = value; } }
        public bool Hidden { get { return m_Hidden; } set { m_Hidden = value; } }

        public string Name { get { return m_Name; } set { m_Name = value; } }
        public string Description { get { return m_Description; } set { m_Description = value; } }

        public virtual void SetDefaultValue()
        {

        }
    }
}
