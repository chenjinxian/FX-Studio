using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.IO;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml;

namespace FXStudio
{
    public partial class AssetsView : ViewWindow
    {
        public AssetsView()
        {
            InitializeComponent();
        }

        public void UpdateAssets(string assetFile)
        {
            if (!File.Exists(assetFile))
            {
                return;
            }
            XmlDocument doc = new XmlDocument();
            doc.Load(assetFile);
            XmlElement rootXml = doc.DocumentElement;

            foreach (XmlNode node in rootXml.ChildNodes)
            {
                TreeNode treeNode = treeViewAssets.Nodes[node.Name];
                foreach (XmlNode child in node.ChildNodes)
                {
                    if (treeNode != null)
                        treeNode.Nodes.Add(new TreeNode(child.Attributes.GetNamedItem("name").Value));
                }
            }
        }

        public void AddEffect(string effectXml)
        {

        }

        private void AddTreeNode(TreeNode parentTree, XmlNode parentXml)
        {

        }

        private void treeViewAssets_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                TreeNode select = treeViewAssets.GetNodeAt(e.X, e.Y);
                if (select != null)
                {
                    treeViewAssets.SelectedNode = select;
                }
            }
        }

        private void treeViewAssets_MouseUp(object sender, MouseEventArgs e)
        {
        }
    }
}
