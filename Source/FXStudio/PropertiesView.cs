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

        private Dictionary<string, XmlNode> m_ComponentsByName;

        public PropertiesView()
        {
            InitializeComponent();
            CreateDefaultCategorys();
        }

        public void UpdateProperties(XmlNode selectedNode)
        {
//             propertyGridView.SelectedObject = new XmlNodeWrapper(selectedNode, m_ComponentsByName);
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

    [TypeConverter(typeof(XmlNodeWrapperConverter))]
    class XmlNodeWrapper
    {
        private readonly XmlNode m_XmlNode;
        private readonly Dictionary<string, XmlNode> m_ComponentsByName;
        public XmlNodeWrapper(XmlNode node, Dictionary<string, XmlNode> components)
        {
            this.m_XmlNode = node;
            this.m_ComponentsByName = components;
        }

        class XmlNodeWrapperConverter : ExpandableObjectConverter
        {
            public override PropertyDescriptorCollection GetProperties(ITypeDescriptorContext context, object value, Attribute[] attributes)
            {
                List<PropertyDescriptor> properties = new List<PropertyDescriptor>();
                XmlNode rootNode = ((XmlNodeWrapper)value).m_XmlNode;
                Dictionary<string, XmlNode> components = ((XmlNodeWrapper)value).m_ComponentsByName;
                if (rootNode != null)
                {
                    foreach (XmlNode categoryNode in rootNode.ChildNodes)
                    {
                        XmlNode editorNode = components[categoryNode.Name];
                        if (editorNode != null && editorNode.Attributes["name"] != null)
                        {
                            string categoryName = editorNode.Attributes["name"].Value;
                            foreach (XmlNode property in editorNode.ChildNodes)
                            {
                                Attribute[] attr = { new CategoryAttribute(categoryName),
                                    new DisplayNameAttribute(property.Attributes["name"].Value),
                                    new DescriptionAttribute(property.Attributes["description"].Value) };

                                if (categoryNode.SelectSingleNode(property.Attributes["name"].Value) != null)
                                {
                                    properties.Add(new XmlNodeWrapperPropertyDescriptor(
                                        categoryNode.SelectSingleNode(property.Attributes["name"].Value), property, attr));
                                }
                            }
                        }
                    }
                }

                return new PropertyDescriptorCollection(properties.ToArray(), true);
            }

            public override object ConvertTo(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value, Type destinationType)
            {
                return destinationType == typeof(string)
                    ? ((XmlNodeWrapper)value).m_XmlNode.InnerXml
                    : base.ConvertTo(context, culture, value, destinationType);
            }
        }
        class XmlNodeWrapperPropertyDescriptor : PropertyDescriptor
        {
        private readonly XmlNode m_XmlProperty;
            private readonly XmlNode m_XmlEditor;
            public XmlNodeWrapperPropertyDescriptor(XmlNode property, XmlNode editor, Attribute[] attributes)
                : base(property.Name, attributes)
            {
                this.m_XmlProperty = property;
                this.m_XmlEditor = editor;
            }

            public override bool ShouldSerializeValue(object component)
            {
                return false;
            }
            public override void SetValue(object component, object value)
            {
                m_XmlProperty.Value = (string)value;
            }
            public override bool CanResetValue(object component)
            {
                return !IsReadOnly;
            }
            public override void ResetValue(object component)
            {
                SetValue(component, "");
            }
            public override Type PropertyType
            {
                get
                {
                    Assembly[] AssembliesLoaded = AppDomain.CurrentDomain.GetAssemblies();
                    Type target = AssembliesLoaded.Select(assembly => assembly.GetType(m_XmlEditor.Attributes["type"].Value))
                        .Where(type => type != null)
                        .FirstOrDefault();
                    return target;
                }
            }
            public override bool IsReadOnly
            {
                get
                {
                    return false;
                }
            }
            public override object GetValue(object component)
            {
                string typeString = m_XmlEditor.Attributes["type"].Value;
                if (typeString == "Vector3")
                {
                    return m_XmlProperty.Value;
                }
                else if (typeString == typeof(Color).ToString())
                {
                    float r = Convert.ToSingle(m_XmlProperty.Attributes["r"].Value);
                    float g = Convert.ToSingle(m_XmlProperty.Attributes["g"].Value);
                    float b = Convert.ToSingle(m_XmlProperty.Attributes["b"].Value);
                    float a = Convert.ToSingle(m_XmlProperty.Attributes["a"].Value);
                    return Color.FromArgb((int)(a * 255.0), (int)(r * 255.0), (int)(g * 255.0), (int)(b * 255.0));
                }
                else if (typeString == typeof(Image).ToString())
                {
                    return Image.FromFile(@"C:\Users\alan\Documents\FX Studio\Projects\Project1\Textures\DefaultTexture.dds");
                }
                else
                {
                    return m_XmlProperty.Value;
                }
            }
            public override Type ComponentType
            {
                get { return typeof(XmlNodeWrapper); }
            }
        }
    }
}