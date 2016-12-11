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

        private void linkLabelNewProject_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            NewProjectDialog dialog = new NewProjectDialog(m_DefaultLocation);
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                string location = dialog.GetProjectLocation();
                Directory.CreateDirectory(location);
                m_ProjectPath = location + @"\" + dialog.GetProjectName() + @".fxsproj";

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
            dialog.Filter = "XML files (*.xml)|*.xml";
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
