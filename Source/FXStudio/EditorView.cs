using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml;
using WeifenLuo.WinFormsUI.Docking;

namespace FXStudio
{
    public partial class EditorView : ViewWindow
    {
        public EditorView()
        {
            InitializeComponent();

            panelTop.BackColor = dockPanelEdit.Theme.ColorPalette.MainWindowActive.Background;
            panelBottom.BackColor = dockPanelEdit.Theme.ColorPalette.MainWindowActive.Background;
        }

        public void ShowEffectDoc(XmlNode effectNode, string projectLocation)
        {
            string effectPath = effectNode.InnerText;
            if (!File.Exists(effectPath) && File.Exists(projectLocation + @"\" + effectPath))
            {
                effectPath = projectLocation + @"\" + effectPath;
            }

            string fileName = Path.GetFileName(effectPath);
            DocumentView content = FindDocument(effectPath) as DocumentView;
            if (content != null)
            {
                this.dockPanelEdit.ActiveDocumentPane.ActiveContent = content;
            }
            else
            {
                DocumentView docView = new DocumentView();
                docView.TabText = fileName;
                docView.ToolTipText = effectPath;
                docView.Tag = effectNode;
                docView.Show(this.dockPanelEdit);
            }
        }

        public XmlNode GetSelectedDocument()
        {
            if (this.dockPanelEdit.ActiveDocumentPane != null)
            {
                DocumentView content = this.dockPanelEdit.ActiveDocumentPane.ActiveContent as DocumentView;
                if (content != null)
                {
                    content.SaveFile();
                    return (content.Tag as XmlNode);
                }
            }

            return null;
        }

        private IDockContent FindDocument(string fullPath)
        {
            foreach (IDockContent content in dockPanelEdit.Documents)
            {
                if (content.DockHandler.ToolTipText == fullPath)
                    return content;
            }

            return null;
        }
    }
}
