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
    public partial class AssetsView : ViewWindow
    {
        public AssetsView()
        {
            InitializeComponent();
        }

        public void UpdateAssets(string assetFile)
        {
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

        private void AddTreeNode(TreeNode parentTree, XmlNode parentXml)
        {

        }
    }
}
