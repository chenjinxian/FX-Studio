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
        private OpenEffectFile m_OpenEffect = null;
        private string m_AssetPath;
        private XmlDocument m_XmlDoc;

        public AssetsView()
        {
            m_XmlDoc = new XmlDocument();
            InitializeComponent();
        }

        public void UpdateAssets(string assetFile,
            UpdatePropertiesDelegate updateProps, UpdateOutputDelegate updateOutput, OpenEffectFile openEffect)
        {
            if (!File.Exists(assetFile))
            {
                return;
            }

            m_AssetPath = assetFile;
            m_ProjectLocation = Path.GetDirectoryName(assetFile);
            m_NodeDelegate = updateProps;
            m_OuputDeleagate = updateOutput;
            m_OpenEffect = openEffect;

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
                OmitXmlDeclaration = true
            }));
        }

        public void ModifyMaterial(string name, string value)
        {
            XmlNode materialNode = treeViewAssets.SelectedNode.Tag as XmlNode;
            if (materialNode != null)
            {
                XmlNode variable = materialNode.SelectSingleNode(name);
                if (variable == null)
                    return;

                variable.InnerText = value;

                XmlDocument materialDoc = new XmlDocument();
                materialDoc.LoadXml(materialNode.OuterXml);
                XmlWriter writer = XmlWriter.Create(treeViewAssets.SelectedNode.Name, new XmlWriterSettings()
                {
                    Indent = true,
                    IndentChars = "\t",
                    OmitXmlDeclaration = true
                });
                materialDoc.Save(writer);
                writer.Flush();
                writer.Close();

                treeViewAssets.SelectedNode.Tag = materialNode;
                RenderMethods.ModifyMaterial(@"Materials\" + Path.GetFileName(treeViewAssets.SelectedNode.Name), false);
            }
        }

        public void ModifyEffect(XmlNode effectNode)
        {
            string sourceFileName = effectNode.InnerText;
            if (!File.Exists(sourceFileName) && File.Exists(m_ProjectLocation + @"\" + sourceFileName))
            {
                sourceFileName = m_ProjectLocation + @"\" + sourceFileName;
            }

            string effectName = effectNode.Attributes["name"].Value;

            string destOjbect = m_ProjectLocation + @"\Effects\" + Path.GetFileNameWithoutExtension(sourceFileName) + ".fxo";
            if (!CompileEffect(sourceFileName, destOjbect))
                return;

            uint size = RenderMethods.ModifyEffect(@"Effects\" + Path.GetFileName(destOjbect), effectName);
            if (size > 0)
            {
                StringBuilder materialtString = new StringBuilder((int)size);
                RenderMethods.GetMaterialXml(@"Effects\" + Path.GetFileName(destOjbect), materialtString, size);

                XmlDocument materialDoc = new XmlDocument();
                materialDoc.LoadXml(materialtString.ToString());
                XmlElement newMaterialNode = materialDoc.DocumentElement;

                ReSetMaterial(newMaterialNode, effectName);
            }
        }

        public void AddEffect(string sourceFileName, string effectName, string materialName, bool fromExist)
        {
            string destOjbect = m_ProjectLocation + @"\Effects\" + Path.GetFileNameWithoutExtension(sourceFileName) + ".fxo";
            bool success = CompileEffect(sourceFileName, destOjbect);

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

            if (success && !string.IsNullOrEmpty(materialName))
            {
                uint size = RenderMethods.AddEffect(@"Effects\" + Path.GetFileName(destOjbect), effectName);
                if (size > 0)
                {
                    StringBuilder materialtString = new StringBuilder((int)size);
                    RenderMethods.GetMaterialXml(@"Effects\" + Path.GetFileName(destOjbect), materialtString, size);
                    File.WriteAllText(m_ProjectLocation + @"\Materials\" + materialName + ".mat", materialtString.ToString());

                    string materialtXml = string.Format(
                        @"<Material name=""{0}"">{1}</Material>", materialName, @"Materials\" + materialName + ".mat");
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

        public bool CompileEffect(string sourceFileName, string destOjbect)
        {
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
            return !string.IsNullOrEmpty(compileInfo);
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
                string materialFile = m_ProjectLocation + @"\" + materialNode.InnerText;
                if (!File.Exists(materialFile))
                {
                    MessageBox.Show(materialFile + "not exist, the asset file may be some errors", "FXSTudio");
                    return;
                }

                XmlDocument materialDoc = new XmlDocument();
                materialDoc.Load(materialFile);
                XmlElement materialXmlNode = materialDoc.DocumentElement;

                TreeNode materialRoot = new TreeNode(nameNode.Value) { Name = materialFile, Tag = materialXmlNode };
                treeNode.Nodes.Add(materialRoot);

                XmlNode effectNode = materialXmlNode.Attributes["effect"];
                TreeNode effectRoot = new TreeNode(effectNode.Value) { Name = materialXmlNode.Attributes["object"].Value, Tag = effectNode };
                materialRoot.Nodes.Add(effectRoot);

                XmlNode techniqueRoot = materialXmlNode.SelectSingleNode("Techniques");
                TreeNode apiNode = new TreeNode("Direct3D11") { Tag = techniqueRoot };
                effectRoot.Nodes.Add(apiNode);

                foreach (XmlNode techniqueChild in techniqueRoot)
                {
                    apiNode.Nodes.Add(new TreeNode(techniqueChild.Attributes["name"].Value) { Tag = techniqueChild });
                }
            }
        }

        private void ReSetMaterial(XmlElement newMaterialNode, string effectName)
        {
            TreeNode materialRoot = treeViewAssets.Nodes["Materials"];
            if (materialRoot == null)
                return;

            foreach (TreeNode child in materialRoot.Nodes)
            {
                if (child.FirstNode.Text != effectName)
                {
                    continue;
                }

                XmlNode oldMaterialNode = child.Tag as XmlNode;
                if (oldMaterialNode == null)
                    continue;

                XmlNode variables = newMaterialNode.SelectSingleNode("Variables");
                foreach (XmlNode childVar in variables)
                {
                    string varName = childVar.Name;
                    XmlNode varNode = oldMaterialNode.SelectSingleNode(varName);
                    if (varNode != null)
                    {
                        childVar.InnerText = varNode.InnerText;
                        if (childVar.Attributes["ResourceName"] != null && varNode.Attributes["ResourceName"] != null)
                            childVar.Attributes["ResourceName"].Value = varNode.Attributes["ResourceName"].Value;
                    }
                }

                XmlDocument materialDoc = new XmlDocument();
                materialDoc.LoadXml(newMaterialNode.OuterXml);
                XmlWriter writer = XmlWriter.Create(child.Name, new XmlWriterSettings()
                {
                    Indent = true,
                    IndentChars = "\t",
                    OmitXmlDeclaration = true
                });
                materialDoc.Save(writer);
                writer.Flush();
                writer.Close();

                child.Tag = newMaterialNode;
                RenderMethods.ModifyMaterial(@"Materials\" + Path.GetFileName(child.Name), true);
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
            if (node.Parent != null && node.Parent.Text == "Materials")
                DoDragDrop(e.Item, DragDropEffects.Copy);
        }

        private void treeViewAssets_DragOver(object sender, DragEventArgs e)
        {

        }

        private void treeViewAssets_NodeMouseDoubleClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            TreeNode node = e.Node;
            if (node.Parent != null && node.Parent.Text == "Effects")
            {
                XmlNode element = (XmlNode)e.Node.Tag;
                if (element != null)
                {
                    m_OpenEffect?.Invoke(element);
                }
            }
        }
    }
}
