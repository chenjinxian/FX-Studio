using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml;
using System.Reflection;
using System.Globalization;

namespace FXStudio
{
    public partial class PropertiesView : ViewWindow
    {
        private static string m_NameProperty = "Name";
        private static string m_HiddenProperty = "Hidden";
        private static string m_TranslationProperty = "Translation";
        private static string m_ScaleProperty = "Scale";
        private static string m_RotationProperty = "Rotation";
        private static string m_NameDescription = "The name of this graph item";
        private static string m_HiddenDescription = "The hidden state of this graph item";
        private static string m_TranslationDescription = "The translation of this node";
        private static string m_ScaleDescription = "The uniform scale of this node";
        private static string m_RotationDescription = "The rotation of this node";

        private PropertyInstance m_PropertyInstance = null;

        private Dictionary<string, XmlNode> m_ComponentsByName;
        XmlDocument m_SelectedComponents;
        int m_SelectedActorId;
        XmlNode m_ActorXml;

        public PropertiesView()
        {
            InitializeComponent();
            CreateDefaultCategorys();

            m_PropertyInstance = new PropertyInstance();
            PropGridAddItems();
        }

        private void PropGridAddItems()
        {
            Inspector.PropertyItemCategory catItem;
            Inspector.PropertyItemString strItem;
            Inspector.PropertyItemDropDownList cmbItem;
            Inspector.PropertyItemInt32 intItem;
            Inspector.PropertyItemBoolean boolItem;
            Inspector.PropertyItemColor colItem;
            Inspector.PropertyItemDouble dblItem;
            Inspector.PropertyItemDateTime dtTmItem;
            Inspector.PropertyItemFont fontItem;
            Inspector.PropertyItemImage imgItem;
            Inspector.PropertyItemFile fileItem;
            Inspector.PropertyItemDirectory dirItem;
            Inspector.PropertyItemProgressBar progBarItem;

            catItem = new Inspector.PropertyItemCategory("Main Category");
            inspectorComponent.CategoryAdd("CatMain", catItem);

            strItem = new Inspector.PropertyItemString("Line01 - Name (String)", "Jak", "Jak Smith");
            strItem.HelpCaption = "Name";
            strItem.HelpText = "Tell me your name...";
            inspectorComponent.ItemAdd("CatMain", "YourName", strItem);

            intItem = new Inspector.PropertyItemInt32("Line02 - Age (Int32)", 0);
            intItem.SetValidationRange(0, 120, 1);
            intItem.SetHelpCaptionText("Age", "Tell me your age (valid range : 0..120)");
            inspectorComponent.ItemAdd("CatMain", "MyAge", intItem);

            // Set current selectd item (second row) and first column size (pixel)
            inspectorComponent.SelectedItem = intItem;
            inspectorComponent.ColumnWidth = 180;

            strItem = new Inspector.PropertyItemString("Line03 - Job (String)", "A software developer");
            strItem.HelpText = "Tell me about your job";
            strItem.Enabled = false;
            inspectorComponent.ItemAdd("CatMain", "K03", strItem);

            cmbItem = new Inspector.PropertyItemDropDownList("Line04 - Best friend (Combo list)", "Bob", "Rosy", "Max|Bob|Carl|Antony|Rosy");
            cmbItem.HelpCaption = "Best friend";
            cmbItem.HelpText = "Tell me the name of your best friend!";
            inspectorComponent.ItemAdd("CatMain", "K04", cmbItem);

            dblItem = new Inspector.PropertyItemDouble("Line05 - Distance (Double)", 0.0, -1.0);
            dblItem.DecimalPlaces = 5;
            dblItem.Format = "0.000";
            dblItem.EngineeringUnit = "[m]";
            dblItem.SetHelpCaptionText("Distance", "Set the distance offset (m) from the 0 line. Default value is -1.0 m.");
            inspectorComponent.ItemAdd("CatMain", "K05", dblItem);

            boolItem = new Inspector.PropertyItemBoolean("Line06 - Sex (Bool)", true);
            boolItem.SetValidationRange("Male", "Female");
            boolItem.SetHelpCaptionText("Sex", "Male or Female?");
            inspectorComponent.ItemAdd("CatMain", "K06", boolItem);

            catItem = new Inspector.PropertyItemCategory("Options");
            inspectorComponent.CategoryAdd("C1", catItem);

            strItem = new Inspector.PropertyItemString("Line07 - ID Code (String)", "alpha");
            strItem.ShowExpandButton = true;
            strItem.MaxLength = 10;
            strItem.SetHelpCaptionText("Password", "Tell me your ID code (max.10 characters)");
            inspectorComponent.ItemAdd("C1", "K07", strItem);

            boolItem = new Inspector.PropertyItemBoolean("Line08 - Drink wine (Bool)", true);
            boolItem.SetValidationRange("No", "Yes");
            boolItem.SetHelpCaptionText("Drink wine", "Say 'Yes' if you like to drink a glass of good wine");
            inspectorComponent.ItemAdd("C1", "K08", boolItem);

            colItem = new Inspector.PropertyItemColor("Line09 - Back color (Color)", Color.Red, Color.Yellow);
            colItem.SetHelpCaptionText("Color", "You preferit background color");
            inspectorComponent.ItemAdd("C1", "K09", colItem);

            dblItem = new Inspector.PropertyItemDouble("Line10 - Distance (Double)", 94.0);
            dblItem.SetHelpCaptionText("Distance", "The distance (Km) from your home to your office. Valid range : 1..150");
            dblItem.DecimalPlaces = 1;
            dblItem.Format = "0.000";
            dblItem.EngineeringUnit = "Km";
            dblItem.SetValidationRange(1.0, 150.0, 0.1, Inspector.ValidationRangeCheckType.Manual);
            inspectorComponent.ItemAdd("C1", "K10", dblItem);

            dtTmItem = new Inspector.PropertyItemDateTime("Line11 - A Date", DateTime.Now, Inspector.DateTimeType.OnlyDate);
            dtTmItem.SetHelpCaptionText("Date", "Only date");
            inspectorComponent.ItemAdd("C1", "K11", dtTmItem);

            dtTmItem = new Inspector.PropertyItemDateTime("Line12 - A Time", DateTime.Now, Inspector.DateTimeType.OnlyTime);
            dtTmItem.SetHelpCaptionText("Time", "Only time");
            inspectorComponent.ItemAdd("C1", "K12", dtTmItem);

            dtTmItem = new Inspector.PropertyItemDateTime("Line13 - Date and Time", DateTime.Now, Inspector.DateTimeType.DateAndTime);
            dtTmItem.SetHelpCaptionText("Date and Time", "Date and time in the same property line");
            dtTmItem.Format = "HH:mm:ss - dd/MM/yyyy";
            inspectorComponent.ItemAdd("C1", "K13", dtTmItem);

            fontItem = new Inspector.PropertyItemFont("Line14 - Font", new System.Drawing.Font("Arial", 10.0F));
            inspectorComponent.ItemAdd("C1", "K14", fontItem);

            imgItem = new Inspector.PropertyItemImage("Line15 - Image", null, @"c:\windows\zapotec.bmp");
            inspectorComponent.ItemAdd("C1", "K15", imgItem);

            fileItem = new Inspector.PropertyItemFile("Line16 - File", @"c:\windows\setupapi.log");
            inspectorComponent.ItemAdd("C1", "K16", fileItem);

            dirItem = new Inspector.PropertyItemDirectory("Line17 - Directory", @"c:\windows\system32");
            dirItem.SetHelpCaptionText("Directory", "System directory path name");
            dirItem.Description = "Select the system directory path";
            inspectorComponent.ItemAdd("C1", "K17", dirItem);

            progBarItem = new Inspector.PropertyItemProgressBar("Line18 - Progress bar", 90);
            inspectorComponent.ItemAdd("C1", "K18", progBarItem);
            //progBarItem.Enabled = false;


            // Repaint control
            inspectorComponent.RefreshControl(true);
        }

        public void UpdateProperties(XmlNode selectedNode)
        {
            m_PropertyInstance.Properties.Clear();

            m_ActorXml = selectedNode;
            m_SelectedComponents = new XmlDocument();
            XmlNode editorComponents = m_SelectedComponents.CreateElement("Actor");
            m_SelectedComponents.AppendChild(editorComponents);

            foreach (XmlNode component in m_ActorXml.ChildNodes)
            {
                XmlNode sourceEditorComponent;
                if (m_ComponentsByName.TryGetValue(component.Name, out sourceEditorComponent))
                {
                    XmlDocument ownerDoc = editorComponents.OwnerDocument;
                    XmlNode editorComponent = ownerDoc.ImportNode(sourceEditorComponent, true);
                    editorComponents.AppendChild(editorComponent);
                    AddCompoentProperties(component, editorComponent);
                }
            }

            if (m_PropertyInstance.Properties.Count == 0)
            {
                m_PropertyInstance.Properties.Add(new PropertyElement(m_NameProperty, typeof(string), selectedNode.Name, false,
                    new CategoryAttribute("Properties"), new DescriptionAttribute(m_NameDescription)));
                m_PropertyInstance.Properties.Add(new PropertyElement(m_HiddenProperty, typeof(bool), false, true,
                    new CategoryAttribute("Properties"), new DescriptionAttribute(m_HiddenDescription)));
            }
        }

        private void AddCompoentProperties(XmlNode actorComponentValues, XmlNode editorComponentValues)
        {
            string componentName = actorComponentValues.Name.ToString();
            string componentXpath = XmlUtility.GetXPathToNode(actorComponentValues);
            try
            {
                int elementNum = 0;
                foreach (XmlNode inputField in editorComponentValues)
                {
                    string xpath = XmlUtility.GetXPathToNode(inputField);
                    string elementName = inputField.Attributes["name"].Value;
                    string elementType = inputField.Attributes["type"].Value;

                    XmlNode actorValues = actorComponentValues.ChildNodes[elementNum];

                    if (elementType == typeof(int).ToString())
                    {
                        int x = Convert.ToInt32(actorValues.FirstChild.Value);

                        m_PropertyInstance.Properties.Add(
                            new PropertyElement(elementName, typeof(int),
                            x, false, new CategoryAttribute(componentName)));
                    }
                    else if (elementType == typeof(float).ToString())
                    {
                        float x = Convert.ToSingle(actorValues.FirstChild.Value);

                        m_PropertyInstance.Properties.Add(
                            new PropertyElement(elementName, typeof(float),
                            x, false, new CategoryAttribute(componentName)));
                    }
                    else if (elementType == typeof(Size).ToString())
                    {
                        int x = Convert.ToInt32(actorValues.Attributes["x"].Value);
                        int y = Convert.ToInt32(actorValues.Attributes["y"].Value);

                        m_PropertyInstance.Properties.Add(
                            new PropertyElement(elementName, typeof(Size),
                            new Size(x, y), false, new CategoryAttribute(componentName)));
                    }
                    else if (elementType == typeof(Vector3).ToString())
                    {
                        float x = Convert.ToSingle(actorValues.Attributes["x"].Value);
                        float y = Convert.ToSingle(actorValues.Attributes["y"].Value);
                        float z = Convert.ToSingle(actorValues.Attributes["z"].Value);

                        m_PropertyInstance.Properties.Add(
                            new PropertyElement(elementName, typeof(Vector3),
                            new Vector3(x, y, z), false, new CategoryAttribute(componentName)));
                    }
                    else if (elementType == typeof(Color).ToString())
                    {
                        float r = Convert.ToSingle(actorValues.Attributes["r"].Value);
                        float g = Convert.ToSingle(actorValues.Attributes["g"].Value);
                        float b = Convert.ToSingle(actorValues.Attributes["b"].Value);
                        float a = Convert.ToSingle(actorValues.Attributes["a"].Value);

                        m_PropertyInstance.Properties.Add(
                            new PropertyElement(elementName, typeof(Color),
                            Color.FromArgb((int)(a * 255.0), (int)(r * 255.0), (int)(g * 255.0), (int)(b * 255.0)),
                            false, new CategoryAttribute(componentName)));
                    }
                    else if (elementType == typeof(Image).ToString())
                    {

                    }
                    else if (elementType == typeof(bool).ToString())
                    {

                    }

                    //                     Assembly[] AssembliesLoaded = AppDomain.CurrentDomain.GetAssemblies();
                    //                     Type target = AssembliesLoaded.Select(assembly => assembly.GetType(elementType))
                    //                         .Where(type => type != null)
                    //                         .FirstOrDefault();


                    ++elementNum;
                }
            }
            catch (Exception e)
            {
                MessageBox.Show("Error in ComponentName " + componentName + "\n" + e.ToString());
            }
        }

        private void CreateDefaultCategorys()
        {
            XmlDocument xmlDoc = new XmlDocument();
            XmlElement transfrom = xmlDoc.CreateElement("Category");
            XmlElement camera = xmlDoc.CreateElement("Category");
            XmlElement skybox = xmlDoc.CreateElement("Category");
            XmlElement grid = xmlDoc.CreateElement("Category");

            m_ComponentsByName = new Dictionary<string, XmlNode>();

            transfrom.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "name", "TransformComponent"));
            m_ComponentsByName["TransformComponent"] = transfrom;
            transfrom.AppendChild(CreateProperty(xmlDoc, m_TranslationProperty, m_TranslationDescription, typeof(Vector3).ToString()));
            transfrom.AppendChild(CreateProperty(xmlDoc, m_ScaleProperty, m_ScaleDescription, typeof(Vector3).ToString()));
            transfrom.AppendChild(CreateProperty(xmlDoc, m_RotationProperty, m_RotationDescription, typeof(Vector3).ToString()));

            camera.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "name", "Camera"));
            m_ComponentsByName["Camera"] = camera;
            camera.AppendChild(CreateProperty(xmlDoc, m_TranslationProperty, m_TranslationDescription, "Vector3"));
            camera.AppendChild(CreateProperty(xmlDoc, m_RotationProperty, m_RotationDescription, "Vector3"));

            skybox.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "name", "SkyboxRenderComponent"));
            m_ComponentsByName["SkyboxRenderComponent"] = skybox;
            skybox.AppendChild(CreateProperty(xmlDoc, "Color", "The color of the skybox", typeof(Color).ToString()));
            skybox.AppendChild(CreateProperty(xmlDoc, "Texture", "The cube texture of the skybox", typeof(Image).ToString()));

            grid.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "name", "GridRenderComponent"));
            m_ComponentsByName["GridRenderComponent"] = grid;
            grid.AppendChild(CreateProperty(xmlDoc, "Color", "The color of the grid", typeof(Color).ToString()));
            grid.AppendChild(CreateProperty(xmlDoc, "Texture", "The diffuse texture of the grid", typeof(Image).ToString()));
            grid.AppendChild(CreateProperty(xmlDoc, "GridSize", "The size of the grid", typeof(Size).ToString()));
            grid.AppendChild(CreateProperty(xmlDoc, "TicksInterval", "The interval of the grid", typeof(float).ToString()));
        }

        private XmlNode CreateProperty(XmlDocument xmlDoc, string name, string description, string type)
        {
            XmlNode propertyNode = xmlDoc.CreateElement("Property");
            propertyNode.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "name", name));
            propertyNode.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "description", description));
            propertyNode.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "type", type));
            return propertyNode;
        }
    }

    public class PropertyElement
    {
        public string PropertyName { get; private set; }
        public Type PropertyType { get; private set; }
        public object PropertyValue { get; set; }
        public bool PropertyReadOnly { get; private set; }
        public Attribute[] PropertyAttributes { get; private set; }

        public PropertyElement(string name, Type type, object value, bool readOnly, params Attribute[] attributes)
        {
            this.PropertyName = name;
            this.PropertyType = type;
            this.PropertyValue = value;
            this.PropertyReadOnly = readOnly;
            if (attributes != null)
            {
                this.PropertyAttributes = new Attribute[attributes.Length];
                attributes.CopyTo(this.PropertyAttributes, 0);
            }
        }
    }

    [TypeConverter(typeof(PropertyConverter))]
    class PropertyInstance
    {
        private readonly List<PropertyElement> properties = new List<PropertyElement>();
        public List<PropertyElement> Properties { get { return properties; } }

        class PropertyConverter : ExpandableObjectConverter
        {
            public override PropertyDescriptorCollection GetProperties(ITypeDescriptorContext context, object value, Attribute[] attributes)
            {
                PropertyDescriptor[] descriptors = (from element in ((PropertyInstance)value).Properties
                                                    select new InstanceDescriptor(element)).ToArray();
                return new PropertyDescriptorCollection(descriptors);
            }
        }

        #region Descriptor impl

        class InstanceDescriptor : PropertyDescriptor
        {
            private readonly PropertyElement m_Property;

            public InstanceDescriptor(PropertyElement property)
                : base(property.PropertyName, property.PropertyAttributes)
            {
                this.m_Property = property;
            }

            public override Type PropertyType
            {
                get { return m_Property.PropertyType; }
            }

            public override object GetValue(object component)
            {
                return m_Property.PropertyValue;
            }

            public override void SetValue(object component, object value)
            {
                m_Property.PropertyValue = value;
            }

            public override bool ShouldSerializeValue(object component)
            {
                return GetValue(component) != null;
            }

            public override bool CanResetValue(object component)
            {
                return true;
            }

            public override void ResetValue(object component)
            {
                SetValue(component, null);
            }

            public override bool IsReadOnly
            {
                get { return m_Property.PropertyReadOnly; }
            }

            public override Type ComponentType
            {
                get { return typeof(PropertyInstance); }
            }
        }

        #endregion
    }

    [TypeConverterAttribute(typeof(Vector3Converter))]
    public class Vector3
    {
        private float x = 0.0f;
        private float y = 0.0f;
        private float z = 0.0f;

        public Vector3(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        public float X
        {
            get { return x; }
            set { x = value; }
        }

        public float Y
        {
            get { return y; }
            set { y = value; }
        }

        public float Z
        {
            get { return z; }
            set { z = value; }
        }
    }

    public class Vector3Converter : ExpandableObjectConverter
    {
        public override bool CanConvertTo(ITypeDescriptorContext context,
                                      System.Type destinationType)
        {
            if (destinationType == typeof(Vector3))
                return true;

            return base.CanConvertTo(context, destinationType);
        }

        public override object ConvertTo(ITypeDescriptorContext context,
                                   CultureInfo culture,
                                   object value,
                                   System.Type destinationType)
        {
            if (destinationType == typeof(string) && value is Vector3)
            {

                Vector3 vec = (Vector3)value;

                return vec.X + ", " + vec.Y + ", " + vec.Z;
            }
            return base.ConvertTo(context, culture, value, destinationType);
        }

        public override bool CanConvertFrom(ITypeDescriptorContext context,
                                  System.Type sourceType)
        {
            if (sourceType == typeof(string))
                return true;

            return base.CanConvertFrom(context, sourceType);
        }

        public override object ConvertFrom(ITypeDescriptorContext context,
                                  CultureInfo culture, object value)
        {
            if (value is string)
            {
                try
                {
                    string[] elements = ((string)value).Split(',');
                    if (elements.Length == 3)
                    {
                        Vector3 vec = new Vector3(Convert.ToSingle(elements[0]), Convert.ToSingle(elements[1]), Convert.ToSingle(elements[2]));
                        return vec;
                    }
                }
                catch
                {
                    throw new ArgumentException("Can not convert '" + (string)value + "' to type Vector3");
                }
            }
            return base.ConvertFrom(context, culture, value);
        }
    }
}