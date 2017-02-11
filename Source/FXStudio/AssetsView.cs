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
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace FXStudio
{
    public partial class AssetsView : ViewWindow
    {
        private string m_ProjectLocation;
        private UpdatePropertiesDelegate m_NodeDelegate = null;
        private UpdateOutputDelegate m_OuputDeleagate = null;
        private string m_AssetPath;
        private XmlDocument m_XmlDoc;

        public AssetsView()
        {
            m_XmlDoc = new XmlDocument();
            InitializeComponent();
        }

        public void UpdateAssets(string assetFile, UpdatePropertiesDelegate updateProps, UpdateOutputDelegate updateOutput)
        {
            if (!File.Exists(assetFile))
            {
                return;
            }

            m_AssetPath = assetFile;
            m_ProjectLocation = Path.GetDirectoryName(assetFile);
            m_NodeDelegate = updateProps;
            m_OuputDeleagate = updateOutput;

            m_XmlDoc.Load(assetFile);
            XmlElement rootXml = m_XmlDoc.DocumentElement;

            treeViewAssets.BeginUpdate();

            foreach (XmlNode node in rootXml.ChildNodes)
            {
                TreeNode treeNode = treeViewAssets.Nodes[node.Name];
                if (treeNode != null)
                {
                    treeNode.Tag = node;
                    treeNode.Nodes.Clear();
                    foreach (XmlNode child in node.ChildNodes)
                    {
                        if (node.Name == "Effects")
                        {
                            AddEffect(treeNode, child);
                        }
                        else if (node.Name == "Materials")
                        {
                            AddMaterial(treeNode, child);
                        }
                        else
                        {
                            XmlNode nameNode = child.Attributes["name"];
                            if (nameNode != null)
                                treeNode.Nodes.Add(new TreeNode(nameNode.Value) { Tag = child });
                        }
                    }
                }
            }

            treeViewAssets.EndUpdate();
            treeViewAssets.ExpandAll();
        }

        public void SaveAssetsFile()
        {
            m_XmlDoc.Save(XmlWriter.Create(m_AssetPath, new XmlWriterSettings()
            {
                Indent = true,
                IndentChars = "\t",
                OmitXmlDeclaration = false
            }));
        }

        public void AddEffect(string sourceFileName, string effectName, string materialName, bool fromExist)
        {
            string destOjbect = m_ProjectLocation + @"\Effects\" + Path.GetFileNameWithoutExtension(sourceFileName) + ".fxo";
            var processInfo = new ProcessStartInfo
            {
                FileName = "fxc.exe",
#if (Debug)
                Arguments = "/Od /Zi /T fx_5_0 /nologo /Fo \"" + destOjbect + "\" \"" + sourceFileName + "\"",
#else
                    Arguments = "/T fx_5_0 /nologo /Fo \"" + destOjbect + "\" \"" + sourceFileName + "\"",
#endif
                CreateNoWindow = true,
                UseShellExecute = false,
                RedirectStandardOutput = true,
                RedirectStandardError = true,
                WindowStyle = ProcessWindowStyle.Hidden
            };
            var fxcProcess = Process.Start(processInfo);
            string compileInfo = fxcProcess.StandardOutput.ReadToEnd();
            string errorInfo = fxcProcess.StandardError.ReadToEnd();
            fxcProcess.WaitForExit();

            m_OuputDeleagate?.Invoke(compileInfo, errorInfo);

            {
                string effectXml = string.Format(
                    @"<Effect name=""{0}"" object=""{1}"">{2}</Effect>", effectName, @"Effects\" + Path.GetFileName(destOjbect), sourceFileName);
                XmlDocument effectDoc = new XmlDocument();
                effectDoc.LoadXml(effectXml);
                XmlElement effectXmlNode = effectDoc.DocumentElement;

                XmlNode effectRoot = m_XmlDoc.DocumentElement.SelectSingleNode("Effects");
                if (effectRoot != null)
                {
                    effectRoot.AppendChild(m_XmlDoc.ImportNode(effectXmlNode, true));
                    TreeNode effectsTreeNode = treeViewAssets.Nodes["Effects"];
                    if (effectsTreeNode != null)
                    {
                        AddEffect(effectsTreeNode, effectRoot.LastChild);
                    }
                }
            }

            if (!string.IsNullOrEmpty(compileInfo) && !string.IsNullOrEmpty(materialName))
            {
                uint size = RenderMethods.AddMaterial(@"Effects\" + Path.GetFileName(destOjbect), effectName, materialName);
                if (size > 0)
                {
                    StringBuilder materialtXml = new StringBuilder((int)size);
                    RenderMethods.GetMaterialXml(@"Effects\" + Path.GetFileName(destOjbect), materialtXml, size);

                    XmlDocument materialDoc = new XmlDocument();
                    materialDoc.LoadXml(materialtXml.ToString());
                    XmlElement materialXmlNode = materialDoc.DocumentElement;

                    XmlNode materialRoot = m_XmlDoc.DocumentElement.SelectSingleNode("Materials");
                    if (materialRoot != null)
                    {
                        materialRoot.AppendChild(m_XmlDoc.ImportNode(materialXmlNode, true));
                        TreeNode materialsTreeNode = treeViewAssets.Nodes["Materials"];
                        if (materialsTreeNode != null)
                        {
                            AddMaterial(materialsTreeNode, materialRoot.LastChild);
                        }
                    }
                }
            }
        }

        private void AddEffect(TreeNode treeNode, XmlNode effectNode)
        {
            XmlNode nameNode = effectNode.Attributes["name"];
            if (nameNode != null)
            {
                treeNode.Nodes.Add(new TreeNode(nameNode.Value) { Tag = effectNode });
            }
        }

        private void AddMaterial(TreeNode treeNode, XmlNode materialNode)
        {
            XmlNode nameNode = materialNode.Attributes["name"];
            if (nameNode != null)
            {
                TreeNode materialRoot = new TreeNode(nameNode.Value) { Tag = materialNode };
                treeNode.Nodes.Add(materialRoot);

                XmlNode effectNode = materialNode.Attributes["effect"];
                TreeNode effectRoot = new TreeNode(effectNode.Value) { Tag = effectNode };
                materialRoot.Nodes.Add(effectRoot);

                XmlNode techniqueRoot = materialNode.SelectSingleNode("Techniques");
                TreeNode apiNode = new TreeNode("Direct3D11") { Tag = techniqueRoot };
                effectRoot.Nodes.Add(apiNode);

                foreach (XmlNode techniqueChild in techniqueRoot)
                {
                    apiNode.Nodes.Add(new TreeNode(techniqueChild.Attributes["name"].Value) { Tag = techniqueChild });
                }
            }
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

        private void treeViewAssets_AfterSelect(object sender, TreeViewEventArgs e)
        {
            XmlNode element = (XmlNode)e.Node.Tag;
            if (element != null)
            {
                m_NodeDelegate?.Invoke(element);
            }
        }

        private void treeViewAssets_VisibleChanged(object sender, EventArgs e)
        {
            //             if (treeViewAssets.SelectedNode != null)
            //             {
            //                 XmlNode element = (XmlNode)treeViewAssets.SelectedNode.Tag;
            //                 if (element != null)
            //                 {
            //                     m_NodeDelegate?.Invoke(element);
            //                 }
            //             }
        }

        private void treeViewAssets_DragEnter(object sender, DragEventArgs e)
        {
            string[] formats = e.Data.GetFormats();
            if (formats.Contains(DataFormats.FileDrop))
            {
                e.Effect = DragDropEffects.Copy;
            }
        }

        private void treeViewAssets_DragDrop(object sender, DragEventArgs e)
        {
            string[] fileList = (string[])e.Data.GetData(DataFormats.FileDrop, false);
            foreach (string file in fileList)
            {
                if (Path.GetExtension(file) == ".fx")
                {
                    AddEffect(file, Path.GetFileNameWithoutExtension(file), Path.GetFileNameWithoutExtension(file) + "_Material", true);
                }
            }
        }

        private void treeViewAssets_ItemDrag(object sender, ItemDragEventArgs e)
        {
            TreeNode node = (TreeNode)e.Item;
            if (node.Parent != null && node.Parent.Name == "Materials")
                DoDragDrop(e.Item, DragDropEffects.Copy);
        }

        private void treeViewAssets_DragOver(object sender, DragEventArgs e)
        {

        }
    }
}
