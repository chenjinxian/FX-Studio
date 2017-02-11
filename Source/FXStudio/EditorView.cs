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
using WeifenLuo.WinFormsUI.Docking;

namespace FXStudio
{
    public partial class EditorView : ViewWindow
    {
        public EditorView()
        {
            InitializeComponent();

            panelTop.BackColor = dockPanelEdit.Theme.Skin.ColorPalette.MainWindowActive.Background;
            panelBottom.BackColor = dockPanelEdit.Theme.Skin.ColorPalette.MainWindowActive.Background;
        }

        public void ShowEffectDoc(string effectPath)
        {
            string fileName = Path.GetFileName(effectPath);
            DocumentView content = FindDocument(effectPath) as DocumentView;
            if (content != null)
            {
                this.dockPanelEdit.ActiveDocumentPane.ActiveContent = content;
            }
            else
            {
                DocumentView docView = new DocumentView();
                docView.DockHandler.TabText = fileName;
                docView.DockHandler.ToolTipText = effectPath;
                docView.ToolTipText = effectPath;
                docView.Show(this.dockPanelEdit);
            }
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
