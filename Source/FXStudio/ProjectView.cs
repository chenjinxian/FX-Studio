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

            foreach (XmlNode child in root.ChildNodes)
            {
                XmlNode typeNode = child.Attributes["type"];
                if (typeNode != null)
                {
                    if (typeNode.Value == "EditorCamera")
                    {
                        var cameraTree = new TreeNode(child.Name) { Tag = child };
                        rootTree.Nodes.Add(cameraTree);
                    }
                    else if (typeNode.Value == "Scene")
                    {
                        var sceneTree = new TreeNode(child.Name) { Tag = child };

                        foreach (XmlNode childScene in child.ChildNodes)
                        {
                            sceneTree.Nodes.Add(new TreeNode(childScene.Name) { Tag = childScene });
                        }

                        rootTree.Nodes.Add(sceneTree);
                    }
                }
            }

            treeViewProject.Nodes.Add(rootTree);
            treeViewProject.EndUpdate();

            treeViewProject.ExpandAll();
            treeViewProject.SelectedNode = treeViewProject.Nodes[0];
        }

        private XmlNode CreateProjectXmlNode(XmlNode node, string projectLocation)
        {
            XmlDocument xmlDoc = new XmlDocument();

            XmlElement projectElement = xmlDoc.CreateElement("Project");
            projectElement.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "type", "Project"));

            XmlNode name = xmlDoc.CreateElement("Name");
            name.InnerText = Path.GetFileNameWithoutExtension(projectLocation);

            XmlNode location = xmlDoc.CreateElement("Location");
            location.InnerText = projectLocation;

            projectElement.AppendChild(name);
            projectElement.AppendChild(location);

            return projectElement;
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
