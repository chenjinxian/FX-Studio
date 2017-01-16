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

    public class ItemCollection
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

        public void Add(String key, FloatItem  Value)
        {
            ItemList.Add(key, Value);
            Value.Changed = false;
        }

        public void Add(String key, BooleanItem Value)
        {
            ItemList.Add(key, Value);
            Value.Changed = false;
        }

        /// <summary>
        /// Add a ColorItem to the sorted list. A unique key must be specified or an exception accour.
        /// </summary>
        /// <param name="key">Unique string value used to identify an BaseItem into a sorted list.</param>
        /// <param name="Value">Color item.</param>
        public void Add(String key, ColorItem Value)
        {
            ItemList.Add(key, Value);
            Value.Changed = false;
        }

        /// <summary>
        /// Add an ImageItem to the sorted list. A unique key must be specified or an exception accour.
        /// </summary>
        /// <param name="key">Unique string value used to identify an BaseItem into a sorted list.</param>
        /// <param name="Value">Picture (image BaseItem).</param>
        public void Add(String key, ImageItem Value)
        {
            ItemList.Add(key, Value);
            Value.Changed = false;
        }

        /// <summary>
        /// Add a FileItem to the sorted list. A unique key must be specified or an exception accour.
        /// </summary>
        /// <param name="key">Unique string value used to identify an BaseItem into a sorted list.</param>
        /// <param name="Value">File BaseItem (a file name string reference).</param>
        public void Add(String key, FileItem Value)
        {
            ItemList.Add(key, Value);
            Value.Changed = false;
        }

        
        //TODO:Add any new BaseItem management here... (Add() function)
        

        /// <summary>
        /// Return true if the sorted list contain the key
        /// </summary>
        /// <param name="key">Unique string value used to identify an BaseItem into a sorted list.</param>
        /// <returns>true:key found  false:key not found</returns>
        public bool Contains(String key)
        {
            return ItemList.Contains(key);
        }

        /// <summary>
        /// Remove a specific BaseItem from the sorted list referring it by key.
        /// </summary>
        /// <param name="key">Unique string value used to identify an BaseItem into a sorted list.</param>
        public void Remove(String key)
        {
            ItemList.Remove(key);
        }

        /// <summary>
        /// Remove a specific BaseItem from the sorted list referring it by index.
        /// </summary>
        /// <param name="index">Unique string value used to identify an BaseItem into a sorted list.</param>
        public void RemoveAt(int index)
        {
            ItemList.RemoveAt(index);
        }

        /// <summary>
        /// Remove all property items form the sorted list.
        /// </summary>
        public void Clear()
        {
            ItemList.Clear();
        }
  
        #endregion

        #region Public properties

        /// <summary>
        /// Get the number of property items stored into the sorted list.
        /// </summary>
        public int Count
        {
            get
            {
                return ItemList.Count;
            }
        }

        #endregion
  
    }

    #endregion

    #region Public Class CategoryCollection (use the collection [1])

    /// <summary>
    /// This class allow to handle a list of Category sorted by the Text property. 
    /// Each category contains a list of property items sorted by the Text property.
    /// </summary>
    public class CategoryCollection     // : IEnumerable
    {
        // Declare this class as   public class CategoryCollection : IEnumerable
        // to enable category items enumeration

        private SortedObjectCollection CategoryList;
        
        /// <summary>
        /// Constructr
        /// </summary>
        /// <param name="initialCapacity">Initial sorted list elements capacity</param>
        public CategoryCollection(int initialCapacity)
        {
            CategoryList = new SortedObjectCollection(initialCapacity);
        }

        /// <summary>
        /// Constructor
        /// </summary>
        public CategoryCollection()
        {
            CategoryList = new SortedObjectCollection();
        }

        /// <summary>
        /// Destructor (clear memory)
        /// </summary>
        ~CategoryCollection()
        {
            CategoryList.Clear();
        }

        #region Public methods

        /// <summary>
        /// Gets/sets a Category into the collection referring it by key.
        /// </summary>
        /// <param name="key">Unique string value used to identify an BaseItem into a sorted list.</param>
        /// <returns></returns>
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

        /// <summary>
        /// Gets/sets a Category into the collection referring it by index.
        /// </summary>
        /// <param name="index">Unique int value (0 based) used to identify an object into a sorted list.</param>
        /// <returns></returns>
        public CategoryItem this[int index]
        {
            get
            {
                if (index < 0 || index >= CategoryList.Count)			// Es. -1
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

        /// <summary>
        /// Return the key using the index.
        /// </summary>
        /// <param name="index">Unique int value (0 based) used to identify an object into a sorted list.</param>
        /// <returns></returns>
        public string GetKey(int index)
        {
            return (string)CategoryList.GetKey(index);
        }

        /// <summary>
        /// Add a category BaseItem to the sorted list. A unique key must be specified or an exception accour.
        /// </summary>
        /// <param name="key">Unique string value used to identify an BaseItem into a sorted list.</param>
        /// <param name="Value">Category BaseItem used to group a set of items.</param>
        public void Add(String key, CategoryItem Value)
        {
            CategoryList.Add(key, Value);
        }

        /// <summary>
        /// Return true if a specific category BaseItem exit into the collection.
        /// </summary>
        /// <param name="key">Unique string value used to identify an BaseItem into a collection.</param>
        /// <returns></returns>
        public bool Contains(String key)
        {
            return CategoryList.Contains(key);
        }

        /// <summary>
        /// Remove a specific category BaseItem from the collection.
        /// </summary>
        /// <param name="key">Unique string value used to identify an BaseItem into a collection.</param>
        public void Remove(String key)
        {
            CategoryList.Remove(key);
        }

        /// <summary>
        /// Remove a specific category BaseItem from the collection.
        /// </summary>
        /// <param name="index">Index (0 based) of a CategoryItem in the collection.</param>
        public void RemoveAt(int index)
        {
            CategoryList.RemoveAt(index);
        }

        /// <summary>
        /// Remove all category items from the collection.
        /// </summary>
        public void Clear()
        {
            CategoryList.Clear();
        }

        #endregion

        #region Public properties

        /// <summary>
        /// Return the number of total category items stored into the collection.
        /// </summary>
        public int Count
        {
            get
            {
                return CategoryList.Count;
            }
        }

        #endregion

    }

    #endregion

}
