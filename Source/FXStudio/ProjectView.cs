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
        public ProjectView()
        {
            InitializeComponent();
        }

        public void UpdateProject(string project, ref string assetFile)
        {
            treeViewProject.BeginUpdate();
            treeViewProject.Nodes.Clear();
            var rootTree = new TreeNode(Path.GetFileNameWithoutExtension(project)) { Tag = project };

            XmlDocument doc = new XmlDocument();
            doc.Load(project);
            XmlElement root = doc.DocumentElement;

            XmlNode assetNode = root.SelectSingleNode("AssetFile");
            if (assetNode != null)
            {
                assetFile = assetNode.FirstChild.Value;
            }

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
        }
    }
}
