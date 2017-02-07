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
    public partial class ProjectView : ViewWindow
    {
        private UpdatePropertiesDelegate m_NodeDelegate = null;
        private TreeNode m_SceneNode = null;

        public ProjectView()
        {
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
                        m_SceneNode = new TreeNode(child.Name) { Tag = child };

                        foreach (XmlNode childScene in child.ChildNodes)
                        {
                            m_SceneNode.Nodes.Add(new TreeNode(childScene.Name) { Tag = childScene });
                        }

                        rootTree.Nodes.Add(m_SceneNode);
                    }
                }
            }

            treeViewProject.Nodes.Add(rootTree);
            treeViewProject.SelectedNode = m_SceneNode;
            treeViewProject.EndUpdate();
            treeViewProject.ExpandAll();
        }

        public void AddActorNode(XmlNode node)
        {
            if (m_SceneNode != null)
            {
                m_SceneNode.Nodes.Add(new TreeNode(node.Name) { Tag = node });
                treeViewProject.SelectedNode = m_SceneNode.Nodes[m_SceneNode.Nodes.Count - 1];
            }
        }

        public void SelectActorNode(uint actorId)
        {
            if (actorId != 0)
            {
                treeViewProject.SelectedNode = m_SceneNode.Nodes[(int)(actorId - 1)];
            }
            else
            {
                treeViewProject.SelectedNode = m_SceneNode;
            }
        }

        public int GetSelectActorId()
        {
            if (treeViewProject.SelectedNode.Parent != m_SceneNode)
                return 0;
            else
                return treeViewProject.SelectedNode.Index + 1;
        }

        public XmlNode GetSelectActorXml()
        {
            return (XmlNode)treeViewProject.SelectedNode.Tag;
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
            if (element != null)
            {
                m_NodeDelegate?.Invoke(element);
            }
        }

        private void treeViewProject_VisibleChanged(object sender, EventArgs e)
        {
            if (treeViewProject.SelectedNode != null)
            {
                XmlNode element = (XmlNode)treeViewProject.SelectedNode.Tag;
                if (element != null)
                {
                    m_NodeDelegate?.Invoke(element);
                }
            }
        }
    }
}
