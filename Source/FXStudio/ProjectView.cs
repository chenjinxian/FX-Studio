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
        private static string m_TranslationProperty = "Translation";
        private static string m_ScaleProperty = "Scale";
        private static string m_RotationProperty = "Rotation";
        private static string m_NameDescription = "The name of this graph item";
        private static string m_HiddenDescription = "The hidden state of this graph item";
        private static string m_TranslationDescription = "The translation of this node";
        private static string m_ScaleDescription = "The uniform scale of this node";
        private static string m_RotationDescription = "The rotation of this node";

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
                XmlNode typeNode = sceneXml.Attributes["type"];
                if (typeNode != null && typeNode.Value == "Scene")
                {
                    var sceneTree = new TreeNode(sceneXml.Name) { Tag = CreateSceneXmlNode(sceneXml) };

                    foreach (XmlNode childScene in sceneXml.ChildNodes)
                    {
                        XmlNode sceneType = childScene.Attributes["type"];
                        if (sceneType != null)
                        {
                            switch (sceneType.Value)
                            {
                                case "Camera":
                                    sceneTree.Nodes.Add(new TreeNode(childScene.Name) { Tag = CreateCameraXmlNode(childScene) });
                                    break;
                                case "Skybox":
                                    sceneTree.Nodes.Add(new TreeNode(childScene.Name) { Tag = CreateSkyboxXmlNode(childScene) });
                                    break;
                                case "Grid":
                                    sceneTree.Nodes.Add(new TreeNode(childScene.Name) { Tag = CreateGridXmlNode(childScene) });
                                    break;
                                case "Lights":
                                    sceneTree.Nodes.Add(new TreeNode(childScene.Name) { Tag = CreateLightsXmlNode(childScene) });
                                    break;
                                case "Geometry":
                                    sceneTree.Nodes.Add(new TreeNode(childScene.Name) { Tag = CreateGeometryXmlNode(childScene) });
                                    break;
                                default:
                                    sceneTree.Nodes.Add(new TreeNode(childScene.Name) { Tag = new XmlDocument().CreateElement("Unknown") });
                                    break;
                            }
                        }
                    }

                    rootTree.Nodes.Add(sceneTree);
                }
            }

            treeViewProject.Nodes.Add(rootTree);
            treeViewProject.EndUpdate();

            treeViewProject.ExpandAll();
            treeViewProject.SelectedNode = treeViewProject.Nodes[0];
        }

        private void CreateProperty(
            XmlDocument xmlDoc, string name, string description, string value, Type type, bool isReadOnly, ref XmlNode catetoryNode)
        {
            XmlNode propertyNode = xmlDoc.CreateElement("Property");
            catetoryNode.AppendChild(propertyNode);

            XmlAttribute attributeName = xmlDoc.CreateAttribute("Name");
            attributeName.Value = name;
            XmlAttribute attributeDesc = xmlDoc.CreateAttribute("Description");
            attributeDesc.Value = description;
            XmlAttribute attributeValue = xmlDoc.CreateAttribute("Value");
            attributeValue.Value = value;
            XmlAttribute attributeType = xmlDoc.CreateAttribute("Type");
            attributeType.Value = type.ToString();
            XmlAttribute attributeReadOnly = xmlDoc.CreateAttribute("ReadOnly");
            attributeReadOnly.Value = isReadOnly.ToString();
            propertyNode.Attributes.Append(attributeName);
            propertyNode.Attributes.Append(attributeDesc);
            propertyNode.Attributes.Append(attributeValue);
            propertyNode.Attributes.Append(attributeType);
            propertyNode.Attributes.Append(attributeReadOnly);
        }

        private XmlNode CreateProjectXmlNode(XmlNode node, string projectLocation)
        {
            XmlDocument xmlDoc = new XmlDocument();

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

            CreateProperty(xmlDoc, "Projects Location", "FX Studio projects location",
                projectLocation, typeof(string), false, ref categoryProjects);
            CreateProperty(xmlDoc, m_NameProperty, m_NameDescription, node.Name, typeof(string), false, ref categoryProperties);
            CreateProperty(xmlDoc, m_HiddenProperty, m_HiddenDescription, "False", typeof(bool), true, ref categoryProperties);

            return projectElement;
        }

        private XmlNode CreateSceneXmlNode(XmlNode node)
        {
            XmlDocument xmlDoc = new XmlDocument();

            XmlElement projectElement = xmlDoc.CreateElement("Scene");
            XmlNode categoryProperties = xmlDoc.CreateElement("Category");
            projectElement.AppendChild(categoryProperties);

            XmlAttribute nameProperties = xmlDoc.CreateAttribute("Name");
            nameProperties.Value = "Properties";
            categoryProperties.Attributes.Append(nameProperties);

            CreateProperty(xmlDoc, m_NameProperty, m_NameDescription, node.Name, typeof(string), false, ref categoryProperties);
            CreateProperty(xmlDoc, m_HiddenProperty, m_HiddenDescription, "False", typeof(bool), true, ref categoryProperties);

            return projectElement;
        }

        private XmlNode CreateCameraXmlNode(XmlNode node)
        {
            XmlDocument xmlDoc = new XmlDocument();

            XmlElement projectElement = xmlDoc.CreateElement("Camera");
            XmlNode categoryProperties = xmlDoc.CreateElement("Category");
            projectElement.AppendChild(categoryProperties);

            XmlAttribute nameProperties = xmlDoc.CreateAttribute("Name");
            nameProperties.Value = "Properties";
            categoryProperties.Attributes.Append(nameProperties);

            CreateProperty(xmlDoc, m_NameProperty, m_NameDescription, node.Name, typeof(string), false, ref categoryProperties);
            CreateProperty(xmlDoc, m_HiddenProperty, m_HiddenDescription, "False", typeof(bool), true, ref categoryProperties);
            CreateProperty(xmlDoc, "Show Cameras", "Show the cameras in the scene view", "True", typeof(bool), false, ref categoryProperties);
            CreateProperty(xmlDoc, m_TranslationProperty, m_TranslationDescription, "True", typeof(bool), false, ref categoryProperties);
            CreateProperty(xmlDoc, m_RotationProperty, m_RotationDescription, "True", typeof(bool), false, ref categoryProperties);

            return projectElement;
        }

        private XmlNode CreateSkyboxXmlNode(XmlNode node)
        {
            XmlDocument xmlDoc = new XmlDocument();

            XmlElement projectElement = xmlDoc.CreateElement("Camera");
            XmlNode categoryProperties = xmlDoc.CreateElement("Category");
            projectElement.AppendChild(categoryProperties);

            XmlAttribute nameProperties = xmlDoc.CreateAttribute("Name");
            nameProperties.Value = "Properties";
            categoryProperties.Attributes.Append(nameProperties);

            CreateProperty(xmlDoc, m_NameProperty, m_NameDescription, node.Name, typeof(string), false, ref categoryProperties);
            CreateProperty(xmlDoc, m_HiddenProperty, m_HiddenDescription, "False", typeof(bool), true, ref categoryProperties);
            CreateProperty(xmlDoc, "Show Skybox", "Show the skybox in the scene view", "True", typeof(bool), false, ref categoryProperties);
            CreateProperty(xmlDoc, m_TranslationProperty, m_TranslationDescription, "True", typeof(bool), false, ref categoryProperties);
            CreateProperty(xmlDoc, m_RotationProperty, m_RotationDescription, "True", typeof(bool), false, ref categoryProperties);

            return projectElement;
        }

        private XmlNode CreateGridXmlNode(XmlNode node)
        {
            XmlDocument xmlDoc = new XmlDocument();

            XmlElement projectElement = xmlDoc.CreateElement("Camera");
            XmlNode categoryProperties = xmlDoc.CreateElement("Category");
            projectElement.AppendChild(categoryProperties);

            XmlAttribute nameProperties = xmlDoc.CreateAttribute("Name");
            nameProperties.Value = "Properties";
            categoryProperties.Attributes.Append(nameProperties);

            CreateProperty(xmlDoc, m_NameProperty, m_NameDescription, node.Name, typeof(string), false, ref categoryProperties);
            CreateProperty(xmlDoc, m_HiddenProperty, m_HiddenDescription, "False", typeof(bool), true, ref categoryProperties);
            CreateProperty(xmlDoc, "Show Grid", "Show the grid in the scene view", "True", typeof(bool), false, ref categoryProperties);
            CreateProperty(xmlDoc, m_TranslationProperty, m_TranslationDescription, "True", typeof(bool), false, ref categoryProperties);
            CreateProperty(xmlDoc, m_RotationProperty, m_RotationDescription, "True", typeof(bool), false, ref categoryProperties);

            return projectElement;
        }

        private XmlNode CreateLightsXmlNode(XmlNode node)
        {
            XmlElement element = new XmlDocument().CreateElement("Project");
//             CreateProperty(xmlDoc, "Show Lights", "Show the lights in the scene view", "True", typeof(bool), false, ref categoryProperties);
            return element;
        }

        private XmlNode CreateGeometryXmlNode(XmlNode node)
        {
            XmlElement element = new XmlDocument().CreateElement("Project");
//             CreateProperty(xmlDoc, "Show Geometry", "Show the geometry in the scene view", "True", typeof(bool), false, ref categoryProperties);
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
