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

namespace FXStudio
{
    public partial class PropertiesView : ViewWindow
    {
        public PropertiesView()
        {
            InitializeComponent();
        }

        public void UpdateProperties(XmlNode selectedNode)
        {
            propertyGridView.SelectedObject = new XmlNodeWrapper(selectedNode);
        }
    }

    [TypeConverter(typeof(XmlNodeConverter))]
    class XmlNodeWrapper
    {
        private readonly XmlNode m_Node;

        public XmlNodeWrapper(XmlNode node)
        {
            this.m_Node = node;
        }

        class XmlNodeConverter : ExpandableObjectConverter
        {
            public override PropertyDescriptorCollection GetProperties(ITypeDescriptorContext context, object value, Attribute[] attributes)
            {
                List<PropertyDescriptor> props = new List<PropertyDescriptor>();
                foreach (XmlNode category in ((XmlNodeWrapper)value).m_Node.ChildNodes)
                {
                    string categoryName = category.Attributes["Name"].Value;
                    foreach (XmlNode property in category.ChildNodes)
                    {
                        Attribute[] attr = { new CategoryAttribute(categoryName),
                            new DisplayNameAttribute(property.Attributes["Name"].Value),
                            new DescriptionAttribute(property.Attributes["Description"].Value) };
                        props.Add(new XmlNodePropertyDescriptor(property, attr));
                    }
                }
                return new PropertyDescriptorCollection(props.ToArray(), true);
            }
        }

        class XmlNodePropertyDescriptor : PropertyDescriptor
        {
            private readonly XmlNode m_Property;
            public XmlNodePropertyDescriptor(XmlNode node, Attribute[] attributes)
                : base(node.Attributes["Name"].Value, attributes)
            {
                this.m_Property = node;
            }

            public override bool ShouldSerializeValue(object component)
            {
                return false;
            }
            public override object GetValue(object component)
            {
                return m_Property.Attributes["Value"].Value;
            }
            public override void SetValue(object component, object value)
            {
                m_Property.Attributes["Value"].Value = (string)value;
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
                    return Type.GetType(m_Property.Attributes["Type"].Value);
                }
            }
            public override bool IsReadOnly
            {
                get { return false; }
            }
            public override Type ComponentType
            {
                get { return typeof(XmlNodeWrapper); }
            }
        }
    }
}
