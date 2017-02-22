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
        private TreeNode m_EditorCameraNode = null;
        private string m_ProjectPath;
        private XmlDocument m_XmlDoc;

        public ProjectView()
        {
            InitializeComponent();
            m_XmlDoc = new XmlDocument();
        }

        public void UpdateProject(string project, UpdatePropertiesDelegate updateProps)
        {
            m_ProjectPath = project;
            m_NodeDelegate = updateProps;

            m_XmlDoc.Load(project);
            XmlElement root = m_XmlDoc.DocumentElement;

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
                        m_EditorCameraNode = new TreeNode(child.Name) { Tag = child };
                        rootTree.Nodes.Add(m_EditorCameraNode);
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
            treeViewProject.SelectedNode = rootTree.FirstNode;
            treeViewProject.EndUpdate();
            treeViewProject.ExpandAll();
        }

        public void SaveProjectFile()
        {
            m_XmlDoc.Save(XmlWriter.Create(m_ProjectPath, new XmlWriterSettings()
            {
                Indent = true,
                IndentChars = "\t"
            }));
        }

        public void AddActorNode(XmlNode node)
        {
            if (m_SceneNode != null)
            {
                m_SceneNode.Nodes.Add(new TreeNode(node.Name) { Tag = node });
                treeViewProject.SelectedNode = m_SceneNode.Nodes[m_SceneNode.Nodes.Count - 1];
                try
                {
                    m_XmlDoc.DocumentElement.SelectSingleNode("DefaultScene").AppendChild(m_XmlDoc.ImportNode(node, true));
                }
                catch (Exception)
                {

                }
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
                treeViewProject.SelectedNode = treeViewProject.Nodes[0].FirstNode;
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

        public void ModifyTransformXml(string actorXml)
        {
            XmlDocument doc = new XmlDocument();
            doc.LoadXml(actorXml);

            XmlElement root = doc.DocumentElement;
            int actorId = int.Parse(root.Attributes["id"].Value);
            if (actorId == treeViewProject.SelectedNode.Index + 1)
            {
                // Actor node, child of DefaultScene
                XmlNode selectNode = (XmlNode)treeViewProject.SelectedNode.Tag;
                XmlNode oldNode = selectNode.SelectSingleNode(root.FirstChild.Name);
                oldNode.InnerXml = root.FirstChild.InnerXml;
                treeViewProject.SelectedNode.Tag = selectNode;

                try
                {
                    XmlNode docNode = m_XmlDoc.DocumentElement.SelectSingleNode("DefaultScene").ChildNodes[actorId - 1];
                    docNode.InnerXml = selectNode.InnerXml;
                }
                catch (Exception)
                {

                }
                m_NodeDelegate?.Invoke((XmlNode)treeViewProject.SelectedNode.Tag);
            }
            else
            {
                // Editor camera node
                XmlNode selectNode = (XmlNode)m_EditorCameraNode.Tag;
                selectNode.InnerXml = root.FirstChild.InnerXml;
                m_EditorCameraNode.Tag = selectNode;

                try
                {
                    m_XmlDoc.DocumentElement.SelectSingleNode("EditorCamera").InnerXml = selectNode.InnerXml;
                }
                catch (Exception)
                {

                }
                m_NodeDelegate?.Invoke((XmlNode)m_EditorCameraNode.Tag);
            }
        }

        public void ModifyMaterialXml(int actorId, int meshIndex, string material)
        {
            XmlNode selectNode = m_SceneNode.Nodes[actorId - 1].Tag as XmlNode;
            if (selectNode != null)
            {
                string path = string.Format("//Material[{0}]", meshIndex + 1);
                XmlNode node = selectNode.SelectSingleNode(path);
                if (node != null)
                {
                    node.InnerText = material;
                    m_SceneNode.Nodes[actorId - 1].Tag = selectNode;

                    try
                    {
                        XmlNode docNode = m_XmlDoc.DocumentElement.SelectSingleNode("DefaultScene").ChildNodes[actorId - 1];
                        docNode.InnerXml = selectNode.InnerXml;
                    }
                    catch (Exception)
                    {

                    }
                    m_NodeDelegate?.Invoke((XmlNode)treeViewProject.SelectedNode.Tag);
                }
            }
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

        private void treeViewProject_NodeMouseDoubleClick(object sender, TreeNodeMouseClickEventArgs e)
        {

        }
    }
}
