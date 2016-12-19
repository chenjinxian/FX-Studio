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

        public void UpdateProject(string project)
        {
            treeViewProject.Nodes.Clear();
            var rootTree = new TreeNode(Path.GetFileNameWithoutExtension(project)) { Tag = project };

            XmlDocument doc = new XmlDocument();
            doc.Load(project);
            XmlElement root = doc.DocumentElement;
            XmlNode assetNode = root.SelectSingleNode("AssetFile");
            if (assetNode != null)
            {
                var assetTreeNode = new TreeNode(Path.GetFileName(assetNode.FirstChild.Value)) { Tag = Path.GetDirectoryName(project) };
                rootTree.Nodes.Add(assetTreeNode);
            }

            foreach (XmlNode node in root.ChildNodes)
            {

            }

            treeViewProject.Nodes.Add(rootTree);
        }
    }
}
