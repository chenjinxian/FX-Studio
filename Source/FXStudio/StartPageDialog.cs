using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace FXStudio
{
    public partial class StartPageDialog : Form
    {
        private string m_DefaultLocation;
        private string m_ProjectPath;
        public StartPageDialog(string recentProjects, string defaultLocation)
        {
            InitializeComponent();

            m_DefaultLocation = defaultLocation;
        }

        public string GetProjectPath()
        {
            return m_ProjectPath;
        }

        private void CopyDefaultData(string sourceDir, string destDir)
        {
            foreach (string dirPath in Directory.GetDirectories(sourceDir))
            {
                Directory.CreateDirectory(dirPath.Replace(sourceDir, destDir));

                foreach (string sourceFile in Directory.GetFiles(dirPath))
                {
                    File.Copy(sourceFile, sourceFile.Replace(sourceDir, destDir), true);
                }
            }
        }

        private void linkLabelNewProject_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            NewProjectDialog dialog = new NewProjectDialog(m_DefaultLocation);
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                string location = dialog.GetProjectLocation();
                Directory.CreateDirectory(location);
                m_ProjectPath = location + @"\" + dialog.GetProjectName() + @".fxsproj";
                RenderMethods.CreateNewProject(m_ProjectPath);

                string sourcePath = Directory.GetCurrentDirectory() + @"\Data";
                CopyDefaultData(sourcePath, location);

                this.DialogResult = DialogResult.OK;
                this.Close();
            }
        }

        private void linkLabelOpenProject_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog();

            var currentDirectory = Directory.GetCurrentDirectory();
            var parentDirectory = Directory.GetParent(currentDirectory);
            dialog.InitialDirectory = parentDirectory.FullName;
            dialog.Filter = "FX Studio files (*.fxsproj)|*.fxsproj";
            dialog.FilterIndex = 1;
            dialog.RestoreDirectory = true;

            if (dialog.ShowDialog() == DialogResult.OK)
            {
                m_ProjectPath = dialog.FileName;
                this.DialogResult = DialogResult.OK;
                this.Close();
            }
        }
    }
}
