using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Xml;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Configuration;
using WeifenLuo.WinFormsUI.Docking;

namespace FXStudio
{
    public partial class FXStudioForm : Form
    {
        private MessageHandler m_messageHandler;
        private string m_DefaultLocation;
        private string m_ProjectLocation;

        private DeserializeDockContent m_dockContent;
        private AssetsView m_AssetsView;
        private ProjectView m_ProjectView;
        private EditorView m_editorView;
        private RenderView m_renderView;
        private PropertiesView m_propertiesView;
        private TaskListView m_taskView;
        private OutputView m_outputView;

        private readonly ToolStripRenderer renderEx = new ToolStripExRenderer();

        public FXStudioForm()
        {
            m_DefaultLocation = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments) + @"\FX Studio\Projects";
            if (!Directory.Exists(m_DefaultLocation))
            {
                Directory.CreateDirectory(m_DefaultLocation);
            }

            InitializeComponent();

            CreateStandardViews();
            m_dockContent = new DeserializeDockContent(GetContentFromString);
            toolStripEx.DefaultRenderer = renderEx;
            SetScheme();

            var panel = m_renderView.GetRenderPanel();
            if (panel != null)
            {
                m_messageHandler = new MessageHandler(this, m_renderView.GetRenderPanel());
            }
        }

        public MessageHandler GetMessageHandler()
        {
            return m_messageHandler;
        }

        public string GetDefaultLocation()
        {
            return m_DefaultLocation;
        }

        public void PickActor(Point cursor)
        {
            uint actorId = RenderMethods.GetPickedActor(cursor.X, cursor.Y);
            m_ProjectView.SelectActorNode(actorId);
        }

        private void OpenProject(string project)
        {
            RenderMethods.OpenProject(project);
            m_ProjectLocation = Path.GetDirectoryName(project);

            string assetFile = string.Empty;
            m_ProjectView.UpdateProject(project, ref assetFile, UpdateProjectPropertiesView);
            if (string.Empty != assetFile)
                m_AssetsView.UpdateAssets(m_ProjectLocation + @"\" + assetFile, m_propertiesView.UpdateAssetProperties);

            EnableControlView(true);
        }

        private void UpdateProjectPropertiesView(XmlNode selectedNode)
        {
            if (m_propertiesView.UpdateProjectProperties(selectedNode))
            {
                int selectAcotrId = m_ProjectView.GetSelectActor();
                if (selectAcotrId > 2)
                    RenderMethods.SetPickedActor((uint)selectAcotrId);
                else
                    RenderMethods.SetPickedActor(0);
            }
            else
                RenderMethods.SetPickedActor(0);
        }

        private void EnableControlView(bool enable)
        {
            toolStripButtonImport.Enabled = enable;
            toolStripButtonSaveAll.Enabled = enable;
            toolStripButtonUndo.Enabled = enable;
            toolStripButtonRedo.Enabled = enable;
            toolStripButtonEffect.Enabled = enable;
            toolStripButtonReBuild.Enabled = enable;
            toolStripButtonCompile.Enabled = enable;
            toolStripButtonTeapot.Enabled = enable;
            toolStripButtonCube.Enabled = enable;
            toolStripButtonSphere.Enabled = enable;
            toolStripButtonCylinder.Enabled = enable;
            toolStripButtonPlane.Enabled = enable;
            toolStripButtonDirectional.Enabled = enable;
            toolStripButtonPoint.Enabled = enable;
            toolStripButtonSpot.Enabled = enable;
            toolStripButtonPlay.Enabled = enable;
            toolStripButtonStop.Enabled = enable;
        }

        private void CreateStandardViews()
        {
            m_AssetsView = new AssetsView();
            m_ProjectView = new ProjectView();
            m_propertiesView = new PropertiesView();
            m_outputView = new OutputView();
            m_taskView = new TaskListView();
            m_editorView = new EditorView();
            m_renderView = new RenderView();
        }

        private void DestoryStandardViews()
        {
            m_AssetsView.DockPanel = null;
            m_ProjectView.DockPanel = null;
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
                return m_AssetsView;
            }
            else if (viewString == typeof(ProjectView).ToString())
            {
                return m_ProjectView;
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

            m_AssetsView.Show(panelAllView, DockState.DockLeft);
            m_ProjectView.Show(panelAllView, DockState.DockLeft);
            m_propertiesView.Show(panelAllView, DockState.DockRight);
            m_editorView.Show(panelAllView, DockState.Document);
            m_renderView.Show(panelAllView, DockState.Document);
            m_taskView.Show(m_renderView.Pane, DockAlignment.Bottom, 0.15);
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

            panelAllView.DockLeftPortion = panelAllView.Width * 0.18d;
            panelAllView.DockRightPortion = panelAllView.DockLeftPortion;

            IntPtr hInstance = Marshal.GetHINSTANCE(this.GetType().Module);
            var panel = m_renderView.GetRenderPanel();
            m_messageHandler.ResetRenderPanel(panel);
            RenderMethods.CreateInstance(hInstance, IntPtr.Zero, panel.Handle, 1, panel.Width, panel.Height);

            if (panel.Width != 0 && panel.Height != 0)
                RenderMethods.ResizeWnd(panel.Width, panel.Height);
        }

        private void FXStudioForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            string configFile = Path.Combine(Path.GetDirectoryName(Application.ExecutablePath), "FXStudio.Layout.config");
            panelAllView.SaveAsXml(configFile);
        }

        private void toolStripButtonOpen_Click(object sender, EventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog();

            dialog.InitialDirectory = m_DefaultLocation;
            dialog.Filter = "FX Studio files (*.fxsproj)|*.fxsproj";
            dialog.FilterIndex = 1;
            dialog.RestoreDirectory = true;

            if (dialog.ShowDialog() == DialogResult.OK)
            {
                OpenProject(dialog.FileName);
            }
        }

        private void FXStudioForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            RenderMethods.DestroyInstance();
        }

        private void FXStudioForm_Shown(object sender, EventArgs e)
        {
            Configuration appConfig = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);

            bool isShow = true;
            if (appConfig.AppSettings.Settings["show"] != null)
            {
                isShow = bool.Parse(appConfig.AppSettings.Settings["show"].Value);
            }
            else
            {
                appConfig.AppSettings.Settings.Add("show", bool.TrueString);
                appConfig.Save(ConfigurationSaveMode.Modified);
                isShow = true;
            }

            if (isShow)
            {
                StartPageDialog startForm = new StartPageDialog("", m_DefaultLocation);
                if (startForm.ShowDialog(this) == DialogResult.OK)
                {
                    OpenProject(startForm.GetProjectPath());
                }
            }
        }

        private void FXStudioForm_Resize(object sender, EventArgs e)
        {
            if (panelAllView.Width != 0 && panelAllView.Height != 0)
            {
                panelAllView.DockLeftPortion = panelAllView.Width * 0.18d;
                panelAllView.DockRightPortion = panelAllView.DockLeftPortion;
            }
        }

        private void toolStripButtonNew_Click(object sender, EventArgs e)
        {
            NewProjectDialog dialog = new NewProjectDialog(m_DefaultLocation);
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                string location = dialog.GetProjectLocation();
                Directory.CreateDirectory(location);
                string projectFile = location + @"\" + dialog.GetProjectName() + @".fxsproj";
                RenderMethods.CreateNewProject(projectFile);

                string sourcePath = Directory.GetCurrentDirectory() + @"\Data";
                StartPageDialog.CopyDefaultData(sourcePath, location);

                OpenProject(projectFile);
            }
        }

        private void toolStripButtonTeapot_Click(object sender, EventArgs e)
        {
            XmlDocument xmlDoc = new XmlDocument();
            XmlElement geometryElement = xmlDoc.CreateElement("Geometry");

            geometryElement.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "type", "Teapot"));
            geometryElement.AppendChild(XmlUtility.CreateTransformComponent(xmlDoc, "0", "0.5"));
            geometryElement.AppendChild(XmlUtility.CreateTeapotRenderComponent(xmlDoc));

            if (RenderMethods.AddActor(geometryElement.OuterXml) > 0)
            {
                m_ProjectView.AddActorNode(geometryElement);
            }
        }

        private void toolStripButtonCube_Click(object sender, EventArgs e)
        {
            XmlDocument xmlDoc = new XmlDocument();
            XmlElement geometryElement = xmlDoc.CreateElement("Geometry");

            geometryElement.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "type", "Cube"));
            geometryElement.AppendChild(XmlUtility.CreateTransformComponent(xmlDoc, "-2", "0.5"));
            geometryElement.AppendChild(XmlUtility.CreateCubeRenderComponent(xmlDoc));

            if (RenderMethods.AddActor(geometryElement.OuterXml) > 0)
            {
                m_ProjectView.AddActorNode(geometryElement);
            }
        }

        private void toolStripButtonSphere_Click(object sender, EventArgs e)
        {
            XmlDocument xmlDoc = new XmlDocument();
            XmlElement geometryElement = xmlDoc.CreateElement("Geometry");

            geometryElement.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "type", "Sphere"));
            geometryElement.AppendChild(XmlUtility.CreateTransformComponent(xmlDoc));
            geometryElement.AppendChild(XmlUtility.CreateSphereRenderComponent(xmlDoc));

            if (RenderMethods.AddActor(geometryElement.OuterXml) > 0)
            {
                m_ProjectView.AddActorNode(geometryElement);
            }
        }

        private void toolStripButtonCylinder_Click(object sender, EventArgs e)
        {
            XmlDocument xmlDoc = new XmlDocument();
            XmlElement geometryElement = xmlDoc.CreateElement("Geometry");

            geometryElement.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "type", "Cylinder"));
            geometryElement.AppendChild(XmlUtility.CreateTransformComponent(xmlDoc, "2", "0.5"));
            geometryElement.AppendChild(XmlUtility.CreateCylinderRenderComponent(xmlDoc));

            if (RenderMethods.AddActor(geometryElement.OuterXml) > 0)
            {
                m_ProjectView.AddActorNode(geometryElement);
            }
        }

        private void toolStripButtonPlane_Click(object sender, EventArgs e)
        {

        }

        private void toolStripButtonImport_Click(object sender, EventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog();

            dialog.Filter = "All supported formats (*.fbx; *.dae; *.blend; *.3ds; *.ase; *.obj; *.ply; *.x; *.ms3d;)|" +
                "*.fbx; *.dae; *.blend; *.3ds; *.ase; *.obj; *.ply; *.x; *.ms3d;|" +
                "Autodesk FBX (*.fbx)|*.fbx|" + "Autodesk 3DS (*.3ds)|*.3ds|" + "Autodesk ASE (*.ase)|*.ase|" +
                "Collada Model (*.dae)|*.dae|" + "Blender 3D (*.blend)|*.blend|" + "Stanford Polygon Library (*.ply)|*.ply|" +
                "Wavefront Object (*.obj)|*.obj|" + "Milkshape 3D (*.ms3d)|*.ms3d";
            dialog.FilterIndex = 1;
            dialog.RestoreDirectory = true;

            if (dialog.ShowDialog() == DialogResult.OK)
            {
                string destFileName = m_ProjectLocation + @"\Models\" + Path.GetFileName(dialog.FileName);
                if (destFileName != dialog.FileName)
                    File.Copy(dialog.FileName, destFileName, true);

                string fileName = Path.GetFileNameWithoutExtension(destFileName);
                fileName = fileName.First().ToString().ToUpper() + fileName.Substring(1);
                XmlDocument xmlDoc = new XmlDocument();
                XmlElement modelElement = xmlDoc.CreateElement(fileName);

                modelElement.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "type", "Model"));
                modelElement.AppendChild(XmlUtility.CreateTransformComponent(xmlDoc));
                modelElement.AppendChild(XmlUtility.CreateModelRenderComponent(xmlDoc, Path.GetFileName(destFileName)));

                if (RenderMethods.AddActor(modelElement.OuterXml) > 0)
                {
                    m_ProjectView.AddActorNode(modelElement);
                }
            }
        }

        private void toolStripButtonEffect_Click(object sender, EventArgs e)
        {
            EffectWizardDialog dialog = new EffectWizardDialog();
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                m_AssetsView.AddEffect(dialog.FileName, dialog.EffectName, dialog.IsEffectFromExist());
            }
        }
    }
}
