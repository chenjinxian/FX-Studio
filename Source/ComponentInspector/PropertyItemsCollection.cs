using System;
using System.Collections;
using System.Text;

namespace Inspector
{

    #region Private [1] generic sorted list object collection

    /// <summary>
    /// If you add PropertyItemGeneric objects to this collection the items will be sorted
    /// automaticaly by the Text property.
    /// </summary>
    public class SortedObjectCollection
    {
        #region Private internal var./properties

        private System.Collections.SortedList listSorted;       // fullKey = PropertyItemGeneric.Text + "." + Key
        private System.Collections.SortedList listByKey;        // Collection of index to access the listSorted collection (by key)

        #endregion 

        #region Constructors

        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="initialCapacity">Set the initial capacity of the internal sorted list</param>
        public SortedObjectCollection(int initialCapacity)
        {
            listSorted = new SortedList(initialCapacity);
            listByKey = new SortedList(initialCapacity);
        }

        /// <summary>
        /// Constructor.
        /// </summary>
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
            string[] splittedStr = null;

            splittedStr = fullKey.Split('.');
            if (splittedStr.Length >= 1)
                key = splittedStr[splittedStr.Length - 1];
            else
                key = fullKey;
            return key;
        }

        /// <summary>
        /// Richiamare questa funzione se la collection listSorted viene modificata
        /// (ad esempio com metodi Add,Delete,DeleteAt) in modo da aggiornare anche
        /// la collection listByKey ad essa collegata
        /// </summary>
        private void RefreshIndex()
        {
            int t;
            string fullKey = "";
            string shortKey = "";
            int index = -1;

            for (t = 0; t < listSorted.Count; t++)
                if (listSorted.GetByIndex(t) is PropertyItemGeneric)
                {
                    fullKey = (string)listSorted.GetKey(t);
                    shortKey = ExtractKey(fullKey);
                    index = (int)listByKey[shortKey];
                    if (index != t)
                        // Aggiusta indice al valore corretto in quanto
                        // la collection listSorted viene ordinata automaticamente
                        // ad ogni inserimento in base alla chiave fullKey
                        listByKey[shortKey] = t;
                }
        }

        #endregion

        #region Public methods

        /// <summary>
        /// Return the key
        /// </summary>
        public string GetKey(int index)
        {
            return (string)listByKey.GetKey(index);
        }

        /// <summary>
        /// Add any object to the sorted list. A unique key must be specified or an exception accour.
        /// </summary>
        /// <param name="key">Unique string value used to identify an PropertyItemGeneric into a sorted list</param>
        /// <param name="Value">Object (eg. PropertyItemGeneric object) to add to a sorted list</param>
        public void Add(String key, object Value)
        {
            PropertyItemGeneric item = null;
            string fullKey = key;
            int index = -1;
            
            if (Value is PropertyItemGeneric)
            {
                item = (PropertyItemGeneric)Value;
                fullKey = item.Text + "." + key;
            }
            listSorted.Add(fullKey, Value);
            index = listSorted.IndexOfKey(fullKey);
            listByKey.Add(key, index);
            // Now check existing index stored into listByKey
            // (some index could be changed if if the object with key fullKey
            //  is not been append at the end of the listSorted collection)
            if (index != listSorted.Count - 1)
                RefreshIndex(); 
        }

        /// <summary>
        /// Return true if the key is found into the collection
        /// </summary>
        /// <param name="key">Unique string value used to identify an PropertyItemGeneric into a sorted list</param>
        /// <returns>true:the key is been found  false:can't find the key in the sorted list</returns>
        public bool Contains(string key)
        {
            return listByKey.Contains(key);
        }

        /// <summary>
        /// Delete an PropertyItemGeneric from the sorted list referring it by key
        /// </summary>
        /// <param name="key">Unique string value used to identify an PropertyItemGeneric into a sorted list</param>
        public void Remove(string key)
        {
            int index = -1;

            if (!listByKey.Contains(key)) return;
            index = (int)listByKey[key];
            listSorted.RemoveAt(index);
            listByKey.Remove(key);
            RefreshIndex();
        }

        /// <summary>
        /// Delete an PropertyItemGeneric from the sorted list referring it by index
        /// </summary>
        /// <param name="index">Unique index value (0 based) used to identify an PropertyItemGeneric into a sorted list</param>
        public void RemoveAt(int index)
        {
            string key = "";
            string fullKey = "";
            
            fullKey = (string)listSorted.GetKey(index);
            key = ExtractKey(fullKey);
            listSorted.Remove(fullKey);
            listByKey.Remove(key);
            RefreshIndex();
        }

        /// <summary>
        /// Remove all objects (property items) from the sorted list
        /// </summary>
        public void Clear()
        {
            listSorted.Clear();
            listByKey.Clear();
        }
  

        #endregion

        #region Public properties

        /// <summary>
        /// Gets/sets and PropertyItemGeneric from a sorted list using the Key.
        /// </summary>
        /// <param name="key">Unique string value used to identify an PropertyItemGeneric into a sorted list</param>
        /// <returns></returns>
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
                    // Error! (Invalid key or index)
                    return null;
                }
            }
            set
            {
                listSorted[key] = value;
            }
        }

        /// <summary>
        /// Gets/sets and PropertyItemGeneric referring it by index.
        /// </summary>
        /// <param name="index">A 0 based integer value</param>
        /// <returns></returns>
        public object this[int index]
        {
            get
            {
                if (index < 0 || index >= listSorted.Count)			// Example. -1
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

        /// <summary>
        /// Gets the number of elements (property items) in the sorted list.
        /// </summary>
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

    /// <summary>
    /// This class allow to handle a list of property items sorted by the Text property
    /// </summary>
    public class ItemCollection //: IEnumerable
    {
        // Declare this class as   public class ItemCollection : IEnumerable
        // to enable property items enumeration

        private SortedObjectCollection ItemList;

        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="initialCapacity">Initial sorted list capacity</param>
        public ItemCollection(int initialCapacity)
        {
            ItemList = new SortedObjectCollection(initialCapacity);
        }

        /// <summary>
        /// Constructor
        /// </summary>
        public ItemCollection()
        {
            ItemList = new SortedObjectCollection();
        }

        /// <summary>
        /// Destructor (clear memory)
        /// </summary>
        ~ItemCollection()
        {
            // Distruttore
            ItemList.Clear();
        }


        #region Public methods

        /// <summary>
        /// Gets/sets a PropertyItemGeneric into the collection referring it by key.
        /// </summary>
        /// <param name="key">Unique string value used to identify an PropertyItemGeneric into a sorted list.</param>
        /// <returns></returns>
        public PropertyItemGeneric this[String key]
        {
            get
            {
                return (PropertyItemGeneric)ItemList[key];
            }
            set
            {
                ItemList[key] = value;
            }
        }

        /// <summary>
        /// Gets/sets an PropertyItemGeneric into the collection referring it by index. 
        /// </summary>
        /// <param name="index">Unique index value (0 based) used to identify an PropertyItemGeneric into a sorted list.</param>
        /// <returns></returns>
        public PropertyItemGeneric this[int index]
        {
            get
            {
                if (index < 0 || index >= ItemList.Count)			// Ex. -1
                    return null;
                else
                    return (PropertyItemGeneric)ItemList[index];
            }
            set
            {
                string key;

                key = this.GetKey(index);
                ItemList[key] = value;
            }
        }

        /// <summary>
        /// Return an PropertyItemGeneric key
        /// </summary>
        /// <param name="index">Unique index value (0 based) used to identify an PropertyItemGeneric into a sorted list.</param>
        /// <returns>Unique string value used to identify an PropertyItemGeneric into a sorted list.</returns>
        public string GetKey(int index)
        {
            return (string)ItemList.GetKey(index);
        }

        /// <summary>
        /// Add a PropertyItemString to the sorted list. A unique key must be specified or an exception accour.
        /// </summary>
        /// <param name="key">Unique string value used to identify an PropertyItemGeneric into a sorted list.</param>
        /// <param name="Value">Simple text string item.</param>
        public void Add(String key, PropertyItemString Value)
        {
            ItemList.Add(key, Value);
            Value.Changed = false;
        }

        /// <summary>
        /// Add an PropertyItemInt32 to the sorted list. A unique key must be specified or an exception accour.
        /// </summary>
        /// <param name="key">Unique string value used to identify an PropertyItemGeneric into a sorted list.</param>
        /// <param name="Value">Integer (32 bits) number item.</param>
        public void Add(String key, PropertyItemInt32 Value)
        {
            ItemList.Add(key, Value);
            Value.Changed = false;
        }

        /// <summary>
        /// Add a  PropertyItemDouble  to the sorted list. A unique key must be specified or an exception accour.
        /// </summary>
        /// <param name="key">Unique string value used to identify an PropertyItemGeneric into a sorted list.</param>
        /// <param name="Value">Double (64 bits) number item.</param>
        public void Add(String key, PropertyItemDouble  Value)
        {
            ItemList.Add(key, Value);
            Value.Changed = false;
        }

        /// <summary>
        /// Add a PropertyItemBoolean to the sorted list. A unique key must be specified or an exception accour.
        /// </summary>
        /// <param name="key">Unique string value used to identify an PropertyItemGeneric into a sorted list.</param>
        /// <param name="Value">Boolean PropertyItemGeneric (true/false, yes/no, small/large, female/male, ...).</param>
        public void Add(String key, PropertyItemBoolean Value)
        {
            ItemList.Add(key, Value);
            Value.Changed = false;
        }

        /// <summary>
        /// Add a PropertyItemColor to the sorted list. A unique key must be specified or an exception accour.
        /// </summary>
        /// <param name="key">Unique string value used to identify an PropertyItemGeneric into a sorted list.</param>
        /// <param name="Value">Color item.</param>
        public void Add(String key, PropertyItemColor Value)
        {
            ItemList.Add(key, Value);
            Value.Changed = false;
        }

        /// <summary>
        /// Add a PropertyItemDateTime to the sorted list. A unique key must be specified or an exception accour.
        /// </summary>
        /// <param name="key">Unique string value used to identify an PropertyItemGeneric into a sorted list.</param>
        /// <param name="Value">Date, time or date+time item.</param>
        public void Add(String key, PropertyItemDateTime Value)
        {
            ItemList.Add(key, Value);
            Value.Changed = false;
        }

        /// <summary>
        /// Add a PropertyItemFont to the sorted list. A unique key must be specified or an exception accour.
        /// </summary>
        /// <param name="key">Unique string value used to identify an PropertyItemGeneric into a sorted list.</param>
        /// <param name="Value">Font PropertyItemGeneric (for text formatting).</param>
        public void Add(String key, PropertyItemFont Value)
        {
            ItemList.Add(key, Value);
            Value.Changed = false;
        }

        /// <summary>
        /// Add an PropertyItemImage to the sorted list. A unique key must be specified or an exception accour.
        /// </summary>
        /// <param name="key">Unique string value used to identify an PropertyItemGeneric into a sorted list.</param>
        /// <param name="Value">Picture (image PropertyItemGeneric).</param>
        public void Add(String key, PropertyItemImage Value)
        {
            ItemList.Add(key, Value);
            Value.Changed = false;
        }

        /// <summary>
        /// Add a PropertyItemFile to the sorted list. A unique key must be specified or an exception accour.
        /// </summary>
        /// <param name="key">Unique string value used to identify an PropertyItemGeneric into a sorted list.</param>
        /// <param name="Value">File PropertyItemGeneric (a file name string reference).</param>
        public void Add(String key, PropertyItemFile Value)
        {
            ItemList.Add(key, Value);
            Value.Changed = false;
        }

        /// <summary>
        /// Add a PropertyItemDirectory to the sorted list. A unique key must be specified or an exception accour.
        /// </summary>
        /// <param name="key">Unique string value used to identify an PropertyItemGeneric into a sorted list.</param>
        /// <param name="Value">Directory PropertyItemGeneric (a path string reference).</param>
        public void Add(String key, PropertyItemDirectory Value)
        {
            ItemList.Add(key, Value);
            Value.Changed = false;
        }

        /// <summary>
        /// Add a PropertyItemProgressBar to the sorted list. A unique key must be specified or an exception accour.
        /// </summary>
        /// <param name="key">Unique string value used to identify an PropertyItemGeneric into a sorted list.</param>
        /// <param name="Value">A progress bar item.</param>
        public void Add(String key, PropertyItemProgressBar Value)
        {
            ItemList.Add(key, Value);
            Value.Changed = false;
        }

        
        //TODO:Add any new PropertyItemGeneric management here... (Add() function)
        

        /// <summary>
        /// Return true if the sorted list contain the key
        /// </summary>
        /// <param name="key">Unique string value used to identify an PropertyItemGeneric into a sorted list.</param>
        /// <returns>true:key found  false:key not found</returns>
        public bool Contains(String key)
        {
            return ItemList.Contains(key);
        }

        /// <summary>
        /// Remove a specific PropertyItemGeneric from the sorted list referring it by key.
        /// </summary>
        /// <param name="key">Unique string value used to identify an PropertyItemGeneric into a sorted list.</param>
        public void Remove(String key)
        {
            ItemList.Remove(key);
        }

        /// <summary>
        /// Remove a specific PropertyItemGeneric from the sorted list referring it by index.
        /// </summary>
        /// <param name="index">Unique string value used to identify an PropertyItemGeneric into a sorted list.</param>
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
        /// <param name="key">Unique string value used to identify an PropertyItemGeneric into a sorted list.</param>
        /// <returns></returns>
        public PropertyItemCategory this[String key]
        {
            get
            {
                return (PropertyItemCategory)CategoryList[key];
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
        public PropertyItemCategory this[int index]
        {
            get
            {
                if (index < 0 || index >= CategoryList.Count)			// Es. -1
                    return null;
                else
                    return (PropertyItemCategory)CategoryList[index];
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
        /// Add a category PropertyItemGeneric to the sorted list. A unique key must be specified or an exception accour.
        /// </summary>
        /// <param name="key">Unique string value used to identify an PropertyItemGeneric into a sorted list.</param>
        /// <param name="Value">Category PropertyItemGeneric used to group a set of items.</param>
        public void Add(String key, PropertyItemCategory Value)
        {
            CategoryList.Add(key, Value);
        }

        /// <summary>
        /// Return true if a specific category PropertyItemGeneric exit into the collection.
        /// </summary>
        /// <param name="key">Unique string value used to identify an PropertyItemGeneric into a collection.</param>
        /// <returns></returns>
        public bool Contains(String key)
        {
            return CategoryList.Contains(key);
        }

        /// <summary>
        /// Remove a specific category PropertyItemGeneric from the collection.
        /// </summary>
        /// <param name="key">Unique string value used to identify an PropertyItemGeneric into a collection.</param>
        public void Remove(String key)
        {
            CategoryList.Remove(key);
        }

        /// <summary>
        /// Remove a specific category PropertyItemGeneric from the collection.
        /// </summary>
        /// <param name="index">Index (0 based) of a PropertyItemCategory in the collection.</param>
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
