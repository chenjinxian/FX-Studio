using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
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

            DocumentView doc1 = CreateNewDocument("Document1");
            DocumentView doc2 = CreateNewDocument("Document2");
            doc1.Show(this.dockPanelEdit, DockState.Document);
            doc2.Show(doc1.Pane, null);
        }

        private IDockContent FindDocument(string text)
        {
            foreach (IDockContent content in dockPanelEdit.Documents)
                if (content.DockHandler.TabText == text)
                    return content;

            return null;
        }

        private DocumentView CreateNewDocument()
        {
            DocumentView dummyDoc = new DocumentView();

            int count = 1;
            string text = $"Document{count}";
            while (FindDocument(text) != null)
            {
                count++;
                text = $"Document{count}";
            }

            dummyDoc.Text = text;
            return dummyDoc;
        }

        private DocumentView CreateNewDocument(string text)
        {
            DocumentView dummyDoc = new DocumentView();
            dummyDoc.Text = text;
            return dummyDoc;
        }

    }
}
