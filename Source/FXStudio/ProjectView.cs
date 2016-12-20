using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Windows.Forms;
using System.Xml;

namespace FXStudio
{
    public delegate void UpdatePropertiesDelegate(XmlNode selectedNode);

    public partial class ProjectView : ViewWindow
    {
        private UpdatePropertiesDelegate m_NodeDelegate;

        private static string m_NameProperty = "Name";
        private static string m_HiddenProperty = "Hidden";
        private static string m_NameDescription = "The name of this graph item";
        private static string m_HiddenDescription = "The hidden state of this graph item";

        public ProjectView()
        {
            m_NodeDelegate = null;
            InitializeComponent();
        }

        public void UpdateProject(string project, ref string assetFile, UpdatePropertiesDelegate updateProps)
        {
            m_NodeDelegate = updateProps;
            XmlDocument doc = new XmlDocument();
            doc.Load(project);
            XmlElement root = doc.DocumentElement;

            XmlNode assetNode = root.SelectSingleNode("AssetFile");
            if (assetNode != null)
            {
                assetFile = assetNode.FirstChild.Value;
            }

            treeViewProject.BeginUpdate();
            treeViewProject.Nodes.Clear();
            var rootTree = new TreeNode(Path.GetFileNameWithoutExtension(project)) { Tag = CreateProjectXmlNode(root, project) };

            foreach (XmlNode sceneXml in root.ChildNodes)
            {
                XmlNode typeNode = sceneXml.Attributes.GetNamedItem("type");
                if (typeNode != null && typeNode.Value == "Scene")
                {
                    var sceneTree = new TreeNode(sceneXml.Name);

                    foreach (XmlNode childScene in sceneXml.ChildNodes)
                    {
                        sceneTree.Nodes.Add(new TreeNode(childScene.Name));
                    }

                    rootTree.Nodes.Add(sceneTree);
                }
            }

            treeViewProject.Nodes.Add(rootTree);
            treeViewProject.EndUpdate();

            treeViewProject.ExpandAll();
            treeViewProject.SelectedNode = treeViewProject.Nodes[0];
        }

        private void SetPropertyAttribute(XmlDocument xmlDoc, string name, string description, string value, Type type, ref XmlNode xmlNode)
        {
            XmlAttribute attributeName = xmlDoc.CreateAttribute("Name");
            attributeName.Value = name;
            XmlAttribute attributeDesc = xmlDoc.CreateAttribute("Description");
            attributeDesc.Value = description;
            XmlAttribute attributeValue = xmlDoc.CreateAttribute("Value");
            attributeValue.Value = value;
            XmlAttribute attributeType = xmlDoc.CreateAttribute("Type");
            attributeType.Value = type.ToString();
            xmlNode.Attributes.Append(attributeName);
            xmlNode.Attributes.Append(attributeDesc);
            xmlNode.Attributes.Append(attributeValue);
            xmlNode.Attributes.Append(attributeType);
        }

        private XmlNode CreateProjectXmlNode(XmlNode node, string projectLocation)
        {
            XmlDocument xmlDoc = new XmlDocument();
            xmlDoc.CreateXmlDeclaration("1.0", "utf-8", "yes");

            XmlElement projectElement = xmlDoc.CreateElement("Project");
            XmlNode categoryProjects = xmlDoc.CreateElement("Category");
            XmlNode categoryProperties = xmlDoc.CreateElement("Category");
            projectElement.AppendChild(categoryProjects);
            projectElement.AppendChild(categoryProperties);

            XmlAttribute nameProjects = xmlDoc.CreateAttribute("Name");
            nameProjects.Value = "Projects";
            categoryProjects.Attributes.Append(nameProjects);
            XmlAttribute nameProperties = xmlDoc.CreateAttribute("Name");
            nameProperties.Value = "Properties";
            categoryProperties.Attributes.Append(nameProperties);

            XmlNode propertyLocation = xmlDoc.CreateElement("Property");
            XmlNode propertyName = xmlDoc.CreateElement("Property");
            XmlNode propertyHide = xmlDoc.CreateElement("Property");
            categoryProjects.AppendChild(propertyLocation);
            categoryProperties.AppendChild(propertyName);
            categoryProperties.AppendChild(propertyHide);

            SetPropertyAttribute(xmlDoc, "Projects Location", "FX Studio projects location", projectLocation, typeof(string), ref propertyLocation);
            SetPropertyAttribute(xmlDoc, m_NameProperty, m_NameDescription, node.Name, typeof(string), ref propertyName);
            SetPropertyAttribute(xmlDoc, m_HiddenProperty, m_HiddenDescription, "False", typeof(bool), ref propertyHide);

            xmlDoc.AppendChild(projectElement);

            return projectElement;
        }

        private XmlNode CreateSceneXmlNode(XmlNode node)
        {
            XmlElement element = new XmlDocument().CreateElement("Project");
            return element;
        }

        private XmlNode CreateEffectXmlNode(XmlNode node)
        {
            XmlElement element = new XmlDocument().CreateElement("Project");
            return element;
        }

        private XmlNode CreateMaterialXmlNode(XmlNode node)
        {
            XmlElement element = new XmlDocument().CreateElement("Project");
            return element;
        }

        private XmlNode CreateModelXmlNode(XmlNode node)
        {
            XmlElement element = new XmlDocument().CreateElement("Project");
            return element;
        }

        private XmlNode CreateGeometryXmlNode(XmlNode node)
        {
            XmlElement element = new XmlDocument().CreateElement("Project");
            return element;
        }

        private void treeViewProject_AfterSelect(object sender, TreeViewEventArgs e)
        {
            XmlNode element = (XmlNode)e.Node.Tag;
            if (element != null && m_NodeDelegate != null)
            {
                m_NodeDelegate(element);
            }
        }
    }
}
