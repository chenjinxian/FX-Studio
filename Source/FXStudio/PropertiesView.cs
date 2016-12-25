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
                m_PropertyInstance.Properties.Add(new PropertyElement(m_NameProperty, typeof(string),
                    new CategoryAttribute("Properties"), new DescriptionAttribute(m_NameDescription)));
                m_PropertyInstance.Properties.Add(new PropertyElement(m_HiddenProperty, typeof(bool),
                    new CategoryAttribute("Properties"), new DescriptionAttribute(m_HiddenDescription)));
            }
            propertyGridView.SelectedObject = m_PropertyInstance;
        }

        private void AddCompoentProperties(XmlNode actorComponentValues, XmlNode editorComponentValues)
        {
            string componentName = actorComponentValues.Name.ToString();
            string componentXpath = XPathUtility.GetXPathToNode(actorComponentValues);
            try
            {
                int elementNum = 0;
                foreach (XmlNode inputField in editorComponentValues)
                {
                    string xpath = XPathUtility.GetXPathToNode(inputField);
                    string elementName = inputField.Attributes["name"].Value;
                    string elementType = inputField.Attributes["type"].Value;

                    XmlNode actorValues = actorComponentValues.ChildNodes[elementNum];

                    Assembly[] AssembliesLoaded = AppDomain.CurrentDomain.GetAssemblies();
                    Type target = AssembliesLoaded.Select(assembly => assembly.GetType(elementType))
                        .Where(type => type != null)
                        .FirstOrDefault();

                    m_PropertyInstance.Properties.Add(new PropertyElement(elementName, target,
                        new CategoryAttribute(componentName)));

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

            transfrom.Attributes.Append(CreateAttribute(xmlDoc, "name", "TransformComponent"));
            m_ComponentsByName["TransformComponent"] = transfrom;
            transfrom.AppendChild(CreateProperty(xmlDoc, m_TranslationProperty, m_TranslationDescription, "Vector3"));
            transfrom.AppendChild(CreateProperty(xmlDoc, m_ScaleProperty, m_ScaleDescription, "Vector3"));
            transfrom.AppendChild(CreateProperty(xmlDoc, m_RotationProperty, m_RotationDescription, "Vector3"));

            camera.Attributes.Append(CreateAttribute(xmlDoc, "name", "Camera"));
            m_ComponentsByName["Camera"] = camera;
            camera.AppendChild(CreateProperty(xmlDoc, m_TranslationProperty, m_TranslationDescription, "Vector3"));
            camera.AppendChild(CreateProperty(xmlDoc, m_RotationProperty, m_RotationDescription, "Vector3"));

            skybox.Attributes.Append(CreateAttribute(xmlDoc, "name", "SkyboxRenderComponent"));
            m_ComponentsByName["SkyboxRenderComponent"] = skybox;
            skybox.AppendChild(CreateProperty(xmlDoc, "Color", "The color of the skybox", typeof(Color).ToString()));
            skybox.AppendChild(CreateProperty(xmlDoc, "Texture", "The cube texture of the skybox", typeof(Image).ToString()));

            grid.Attributes.Append(CreateAttribute(xmlDoc, "name", "GridRenderComponent"));
            m_ComponentsByName["GridRenderComponent"] = grid;
            grid.AppendChild(CreateProperty(xmlDoc, "Color", "The color of the skybox", typeof(Color).ToString()));
            grid.AppendChild(CreateProperty(xmlDoc, "Texture", "The cube texture of the skybox", typeof(Image).ToString()));
            grid.AppendChild(CreateProperty(xmlDoc, "Size", "The cube texture of the skybox", typeof(Size).ToString()));
        }

        private XmlNode CreateProperty(XmlDocument xmlDoc, string name, string description, string type)
        {
            XmlNode propertyNode = xmlDoc.CreateElement("Property");
            propertyNode.Attributes.Append(CreateAttribute(xmlDoc, "name", name));
            propertyNode.Attributes.Append(CreateAttribute(xmlDoc, "description", description));
            propertyNode.Attributes.Append(CreateAttribute(xmlDoc, "type", type));
            return propertyNode;
        }

        private XmlAttribute CreateAttribute(XmlDocument xmlDoc, string name, string value)
        {
            XmlAttribute attribute = xmlDoc.CreateAttribute(name);
            attribute.Value = value;
            return attribute;
        }
    }

    public class PropertyElement
    {
        public string PropertyName { get; private set; }
        public Type PropertyType { get; private set; }
        public object PropertyValue { get; set; }
        public bool PropertyReadOnly { get; private set; }
        public Attribute[] PropertyAttributes { get; private set; }

        public PropertyElement(string name, Type type, params Attribute[] attributes)
        {
            this.PropertyName = name;
            this.PropertyType = type;
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
}