using System;
using System.Collections.Generic;
using System.Text;

namespace Inspector
{
    public class CategoryItem : BaseItem
    {
        private bool m_Expanded = true;
        private Dictionary<string, BaseItem> m_PropertyByName = null;

        public CategoryItem()
        {
            this.Name = "Misc";
        }

        public CategoryItem(string text)
        {
            this.Name = text;
            m_PropertyByName = new Dictionary<string, BaseItem>();
        }

        public bool Expanded { get { return m_Expanded; } set { m_Expanded = value; } }
        public Dictionary<string, BaseItem> ItemList { get { return m_PropertyByName; } set { m_PropertyByName = value; } }
    }
}
