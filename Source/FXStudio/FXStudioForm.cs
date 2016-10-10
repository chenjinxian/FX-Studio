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
using System.Runtime.InteropServices;
using WeifenLuo.WinFormsUI.Docking;

namespace FXStudio
{
    public partial class FXStudioForm : Form
    {
        private MessageHandler m_messageHandler;

        private DeserializeDockContent m_dockContent;
        private AssetsView m_assetsView;
        private ProjectView m_projectView;
        private EditorView m_editorView;
        private RenderView m_renderView;
        private PropertiesView m_propertiesView;
        private TaskListView m_taskView;
        private OutputView m_outputView;

        private readonly ToolStripRenderer renderEx = new ToolStripExRenderer();

        public FXStudioForm()
        {
            InitializeComponent();
            m_messageHandler = new MessageHandler(this);

            CreateStandardViews();
            m_dockContent = new DeserializeDockContent(GetContentFromString);

            toolStripEx.DefaultRenderer = renderEx;
            SetScheme();
        }

        public MessageHandler GetMessageHandler()
        {
            return m_messageHandler;
        }

        private void ShutDown()
        {
            RenderMethods.DestroyInstance();
            Application.Exit();
        }

        private void CreateStandardViews()
        {
            m_assetsView = new AssetsView();
            m_projectView = new ProjectView();
            m_propertiesView = new PropertiesView();
            m_outputView = new OutputView();
            m_taskView = new TaskListView();
            m_editorView = new EditorView();
            m_renderView = new RenderView();
            m_messageHandler.SetRenderPanel(m_renderView.GetRenderPanel());
        }

        private void DestoryStandardViews()
        {
            m_assetsView.DockPanel = null;
            m_projectView.DockPanel = null;
            m_renderView.DockPanel = null;
            m_propertiesView.DockPanel = null;
            m_outputView.DockPanel = null;
            m_taskView.DockPanel = null;
            m_editorView.DockPanel = null;

            foreach (IDockContent document in panelAllView.DocumentsToArray())
            {
                document.DockHandler.DockPanel = null;
                document.DockHandler.Close();
            }

            System.Diagnostics.Debug.Assert(panelAllView.Panes.Count == 0);
        }

        private IDockContent GetContentFromString(string viewString)
        {
            if (viewString == typeof(AssetsView).ToString())
            {
                return m_assetsView;
            }
            else if (viewString == typeof(ProjectView).ToString())
            {
                return m_projectView;
            }
            else if (viewString == typeof(EditorView).ToString())
            {
                return m_editorView;
            }
            else if (viewString == typeof(RenderView).ToString())
            {
                return m_renderView;
            }
            else if (viewString == typeof(PropertiesView).ToString())
            {
                return m_propertiesView;
            }
            else if (viewString == typeof(OutputView).ToString())
            {
                return m_outputView;
            }
            else if (viewString == typeof(TaskListView).ToString())
            {
                return m_taskView;
            }
            else
            {
                return null;
            }
        }

        private void SetScheme()
        {
            toolStripEx.SetStyle(this.menuStripMain);
            toolStripEx.SetStyle(this.toolStripMain);
            panelTop.BackColor = panelAllView.Theme.Skin.ColorPalette.MainWindowActive.Background;
            panelBottom.BackColor = panelAllView.Theme.Skin.ColorPalette.MainWindowActive.Background;
            statusStripMain.BackColor = panelAllView.Theme.Skin.ColorPalette.MainWindowStatusBarDefault.Background;
        }

        private void ResetLayout()
        {
            panelAllView.SuspendLayout(true);
            DestoryStandardViews();
            CreateStandardViews();

            m_assetsView.Show(panelAllView, DockState.DockLeft);
            m_projectView.Show(panelAllView, DockState.DockLeft);
            m_propertiesView.Show(panelAllView, DockState.DockRight);
            m_editorView.Show(panelAllView, DockState.Document);
            m_renderView.Show(panelAllView, DockState.Document);
            m_taskView.Show(m_renderView.Pane, DockAlignment.Bottom, 0.25);
            m_outputView.Show(m_taskView.Pane, null);

            panelAllView.ResumeLayout(true, true);
        }

        private void FXStudioForm_Load(object sender, EventArgs e)
        {
            string configFile = Path.Combine(Path.GetDirectoryName(Application.ExecutablePath), "FXStudio.Layout.config");

            if (File.Exists(configFile))
            {
                panelAllView.LoadFromXml(configFile, m_dockContent);
            }
            else
            {
                ResetLayout();
            }
        }

        private void FXStudioForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            string configFile = Path.Combine(Path.GetDirectoryName(Application.ExecutablePath), "FXStudio.Layout.config");
            panelAllView.SaveAsXml(configFile);
        }

        private void toolStripButtonOpen_Click(object sender, EventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog();

            var currentDirectory = Directory.GetCurrentDirectory();
            var parentDirectory = Directory.GetParent(currentDirectory);
            dialog.InitialDirectory = parentDirectory.FullName;
            dialog.Filter = "XML files (*.xml)|*.xml";
            dialog.FilterIndex = 1;
            dialog.RestoreDirectory = true;

            if (dialog.ShowDialog() == DialogResult.OK)
            {
                RenderMethods.OpenLevel(dialog.FileName);
            }
        }
    }
}
