using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Collections;

namespace FXStudio.ComponentItems
{
    public class SortedObjectCollection
    {
        private SortedList listSorted;
        private SortedList listByKey;

        public SortedObjectCollection(int capacity)
        {
            listSorted = new SortedList(capacity);
            listByKey = new SortedList(capacity);
        }

        public SortedObjectCollection()
        {
            listSorted = new SortedList();
            listByKey = new SortedList();
        }

        private string ExtractKey(string fullKey)
        {
            string key = "";
            string[] split = fullKey.Split('.');

            if (split.Length > 0)
                key = split[split.Length - 1];
            else
                key = fullKey;

            return key;
        }

        private void RefreshIndex()
        {
            string fullKey = "";
            string shortKey = "";
            int index = -1;

            for (int i = 0; i < listSorted.Count; i++)
            {
                if (listSorted.GetByIndex(i) is BaseItem)
                {
                    fullKey = (string)listSorted.GetKey(i);
                    shortKey = ExtractKey(fullKey);
                    index = (int)listByKey[shortKey];

                    if (index != i)
                        listByKey[shortKey] = i;
                }
            }
        }

        public string GetKey(int index)
        {
            return (string)listByKey.GetKey(index);
        }

        public void Add(String key, object value)
        {

        }

        public bool Contains(string key)
        {
            return listByKey.Contains(key);
        }

        public void Remove(string key)
        {

        }

        public void RemoveAt(int index)
        {

        }

        public void Clear()
        {

        }
    }

    public class ItemCollection
    {
    }
}
