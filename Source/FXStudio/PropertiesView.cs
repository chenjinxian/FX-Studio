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
//             propertyGridView.SelectedObject = new XmlNodeWrapper(selectedNode);
        }
    }

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
//                 XmlElement rootNode = ((XmlNodeWrapper)value).m_Node as XmlElement;
//                 if (rootNode != null)
//                 {
//                     foreach (XmlAttribute attr in rootNode.Attributes)
//                     {
//                         props.Add(new XmlNodePropertyDescriptor(attr));
//                     }
//                 }
                foreach (XmlNode child in ((XmlNodeWrapper)value).m_Node.ChildNodes)
                {
                    props.Add(new XmlNodePropertyDescriptor(child));
                }
                return new PropertyDescriptorCollection(props.ToArray(), true);
            }
        }

        class XmlNodePropertyDescriptor : PropertyDescriptor
        {
            private static readonly Attribute[] nix = new Attribute[0];
            private readonly XmlNode node;
            public XmlNodePropertyDescriptor(XmlNode node)
                : base(GetName(node), nix)
            {
                this.node = node;
            }
            static string GetName(XmlNode node)
            {
                switch (node.NodeType)
                {
                    case XmlNodeType.Attribute: return "@" + node.Name;
                    case XmlNodeType.Element: return node.Name;
                    case XmlNodeType.Comment: return "<!-- -->";
                    case XmlNodeType.Text: return "(text)";
                    default: return node.NodeType + ":" + node.Name;
                }
            }
            public override string Category { get { return "Properties"; } }
            public override string Description { get { return "Test Description"; } }
            public override bool ShouldSerializeValue(object component)
            {
                return false;
            }
            public override void SetValue(object component, object value)
            {
                node.Value = (string)value;
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
                    switch (node.NodeType)
                    {
                        case XmlNodeType.Element:
                            return typeof(XmlNodeWrapper);
                        default:
                            return typeof(string);
                    }
                }
            }
            public override bool IsReadOnly
            {
                get
                {
                    switch (node.NodeType)
                    {
                        case XmlNodeType.Attribute:
                        case XmlNodeType.Text:
                            return false;
                        default:
                            return true;
                    }
                }
            }
            public override object GetValue(object component)
            {
                switch (node.NodeType)
                {
                    case XmlNodeType.Element:
                        return new XmlNodeWrapper(node);
                    default:
                        return node.Value;
                }
            }
            public override Type ComponentType
            {
                get { return typeof(XmlNodeWrapper); }
            }
        }
    }
}
