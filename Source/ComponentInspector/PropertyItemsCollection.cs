using System;
using System.Collections;
using System.Text;

namespace Inspector
{

    #region Private [1] generic sorted list object collection

    public class SortedObjectCollection
    {
        #region Private internal var./properties

        private System.Collections.SortedList listSorted;
        private System.Collections.SortedList listByKey;

        #endregion 

        #region Constructors

        public SortedObjectCollection(int initialCapacity)
        {
            listSorted = new SortedList(initialCapacity);
            listByKey = new SortedList(initialCapacity);
        }

        public SortedObjectCollection()
        {
            listSorted = new SortedList();
            listByKey = new SortedList();
        }

        #endregion

        #region Private internal functions

        private string ExtractKey(string fullKey)
        {
            string key = "";

            string[] splittedStr = fullKey.Split('.');
            if (splittedStr.Length >= 1)
                key = splittedStr[splittedStr.Length - 1];
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

        #endregion

        #region Public methods

        public string GetKey(int index)
        {
            return (string)listByKey.GetKey(index);
        }

        public void Add(String key, object Value)
        {
            string fullKey = key;
            if (Value is BaseItem)
            {
                BaseItem item = (BaseItem)Value;
                fullKey = item.Name + "." + key;
            }

            listSorted.Add(fullKey, Value);
            int index = listSorted.IndexOfKey(fullKey);
            listByKey.Add(key, index);
            if (index != listSorted.Count - 1)
                RefreshIndex();
        }

        public bool Contains(string key)
        {
            return listByKey.Contains(key);
        }

        public void Remove(string key)
        {
            if (!listByKey.Contains(key)) return;

            int index = (int)listByKey[key];
            listSorted.RemoveAt(index);
            listByKey.Remove(key);
            RefreshIndex();
        }

        public void RemoveAt(int index)
        {
            string fullKey = (string)listSorted.GetKey(index);
            string key = ExtractKey(fullKey);
            listSorted.Remove(fullKey);
            listByKey.Remove(key);
            RefreshIndex();
        }

        public void Clear()
        {
            listSorted.Clear();
            listByKey.Clear();
        }


        #endregion

        #region Public properties

        public object this[String key]
        {
            get
            {
                int index = -1;

                try
                {
                    index = (int)listByKey[key];
                    return listSorted.GetByIndex(index);
                }
                catch
                {
                    return null;
                }
            }
            set
            {
                listSorted[key] = value;
            }
        }

        public object this[int index]
        {
            get
            {
                if (index < 0 || index >= listSorted.Count)
                    return null;
                else
                    return listSorted.GetByIndex(index);
            }
            set
            {
                string fullKey;

                fullKey = (string)listSorted.GetKey(index);
                listSorted[fullKey] = value;
            }
        }

        public int Count
        {
            get
            {
                return listSorted.Count;
            }
        }

        #endregion

    }

    #endregion 

    #region Public Class ItemCollection (use the collection [1])

    public class ItemCollection : IEnumerable
    {
        private SortedObjectCollection ItemList;

        public ItemCollection(int initialCapacity)
        {
            ItemList = new SortedObjectCollection(initialCapacity);
        }

        public ItemCollection()
        {
            ItemList = new SortedObjectCollection();
        }

        ~ItemCollection()
        {
            ItemList.Clear();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return (IEnumerator)GetEnumerator();
        }

        public BaseEnumerator GetEnumerator()
        {
            return new BaseEnumerator(ItemList);
        }

        #region Public methods

        public BaseItem this[String key]
        {
            get
            {
                return (BaseItem)ItemList[key];
            }
            set
            {
                ItemList[key] = value;
            }
        }

        public BaseItem this[int index]
        {
            get
            {
                if (index < 0 || index >= ItemList.Count)
                    return null;
                else
                    return (BaseItem)ItemList[index];
            }
            set
            {
                string key;

                key = this.GetKey(index);
                ItemList[key] = value;
            }
        }

        public string GetKey(int index)
        {
            return (string)ItemList.GetKey(index);
        }

        public void Add(String key, StringItem Value)
        {
            ItemList.Add(key, Value);
            Value.Changed = false;
        }

        public void Add(String key, Int32Item Value)
        {
            ItemList.Add(key, Value);
            Value.Changed = false;
        }

        public void Add(String key, FloatItem Value)
        {
            ItemList.Add(key, Value);
            Value.Changed = false;
        }

        public void Add(String key, Vector3Item Value)
        {
            ItemList.Add(key, Value);
            Value.Changed = false;
        }

        public void Add(String key, BooleanItem Value)
        {
            ItemList.Add(key, Value);
            Value.Changed = false;
        }

        public void Add(String key, ColorItem Value)
        {
            ItemList.Add(key, Value);
            Value.Changed = false;
        }

        public void Add(String key, ImageItem Value)
        {
            ItemList.Add(key, Value);
            Value.Changed = false;
        }

        public void Add(String key, FileItem Value)
        {
            ItemList.Add(key, Value);
            Value.Changed = false;
        }


        //TODO:Add any new BaseItem management here... (Add() function)


        public bool Contains(String key)
        {
            return ItemList.Contains(key);
        }

        public void Remove(String key)
        {
            ItemList.Remove(key);
        }

        public void RemoveAt(int index)
        {
            ItemList.RemoveAt(index);
        }

        public void Clear()
        {
            ItemList.Clear();
        }

        #endregion

        #region Public properties

        public int Count
        {
            get
            {
                return ItemList.Count;
            }
        }

        #endregion

    }

    public class BaseEnumerator : IEnumerator
    {
        private int index = -1;
        public SortedObjectCollection m_ItemList;

        public BaseEnumerator(SortedObjectCollection itemList)
        {
            m_ItemList = itemList;
        }

        public bool MoveNext()
        {
            index++;
            return (index < m_ItemList.Count);
        }

        public void Reset()
        {
            index = -1;
        }

        object IEnumerator.Current { get { return Current; } }

        public BaseItem Current
        {
            get
            {
                try
                {
                    return (BaseItem)m_ItemList[index];
                }
                catch (IndexOutOfRangeException)
                {
                    throw new InvalidOperationException();
                }
            }
        }
    }

    #endregion

    #region Public Class CategoryCollection (use the collection [1])

    public class CategoryCollection : IEnumerable
    {
        private SortedObjectCollection CategoryList;

        public CategoryCollection(int initialCapacity)
        {
            CategoryList = new SortedObjectCollection(initialCapacity);
        }

        public CategoryCollection()
        {
            CategoryList = new SortedObjectCollection();
        }

        ~CategoryCollection()
        {
            CategoryList.Clear();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return (IEnumerator)GetEnumerator();
        }

        public CategoryEnumerator GetEnumerator()
        {
            return new CategoryEnumerator(CategoryList);
        }

        #region Public methods 

        public CategoryItem this[String key]
        {
            get
            {
                return (CategoryItem)CategoryList[key];
            }
            set
            {
                CategoryList[key] = value;
            }
        }

        public CategoryItem this[int index]
        {
            get
            {
                if (index < 0 || index >= CategoryList.Count)
                    return null;
                else
                    return (CategoryItem)CategoryList[index];
            }
            set
            {
                string key;

                key = this.GetKey(index);
                CategoryList[key] = value;
            }
        }

        public string GetKey(int index)
        {
            return (string)CategoryList.GetKey(index);
        }

        public void Add(String key, CategoryItem Value)
        {
            CategoryList.Add(key, Value);
        }

        public bool Contains(String key)
        {
            return CategoryList.Contains(key);
        }

        public void Remove(String key)
        {
            CategoryList.Remove(key);
        }

        public void RemoveAt(int index)
        {
            CategoryList.RemoveAt(index);
        }

        public void Clear()
        {
            CategoryList.Clear();
        }

        #endregion

        #region Public properties

        public int Count
        {
            get
            {
                return CategoryList.Count;
            }
        }

        #endregion

    }

    public class CategoryEnumerator : IEnumerator
    {
        private int index = -1;
        public SortedObjectCollection m_CategoryList;

        public CategoryEnumerator(SortedObjectCollection categoryList)
        {
            m_CategoryList = categoryList;
        }

        public bool MoveNext()
        {
            index++;
            return (index < m_CategoryList.Count);
        }

        public void Reset()
        {
            index = -1;
        }

        object IEnumerator.Current { get { return Current; } }

        public CategoryItem Current
        {
            get
            {
                try
                {
                    return (CategoryItem)m_CategoryList[index];
                }
                catch (IndexOutOfRangeException)
                {
                    throw new InvalidOperationException();
                }
            }
        }
    }

    #endregion

}
