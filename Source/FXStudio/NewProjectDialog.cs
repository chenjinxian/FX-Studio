using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace FXStudio
{
    public partial class NewProjectDialog : Form
    {
        public NewProjectDialog(string defaultLocation)
        {
            InitializeComponent();

            string newProject = "Project1";
            int i = 2;
            while (Directory.Exists(defaultLocation + @"\" + newProject))
            {
                newProject = "Project" + i.ToString();
                i++;
            }
            
            this.textBoxName.Text = newProject;
            this.textBoxLocation.Text = defaultLocation;
        }

        public string GetProjectName()
        {
            return this.textBoxName.Text;
        }

        public string GetProjectLocation()
        {
            if (checkBoxCreateDir.Checked)
                return this.textBoxLocation.Text + @"\" + this.textBoxName.Text;
            else
                return this.textBoxLocation.Text;
        }

        private void buttonBrowse_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog dialog = new FolderBrowserDialog();
            dialog.SelectedPath = this.textBoxLocation.Text;
            dialog.ShowNewFolderButton = true;
            dialog.Description = "Select a folder";

            if (dialog.ShowDialog() == DialogResult.OK)
            {
                this.textBoxLocation.Text = dialog.SelectedPath;
            }
        }

        private void buttonOK_Click(object sender, EventArgs e)
        {
            string project = GetProjectLocation() + @"\" + GetProjectName() + @".fxsproj";
            if (File.Exists(project))
            {
                string message = "Selected directory already contains an FX Studio Project\r\n" + project +
                    "\r\n\r\nClick OK to overwrite the file (Not Recommended)\r\nClick Cancel to choose a different project location";
                if (DialogResult.Cancel == MessageBox.Show(this.Owner, message, "FX Studio",
                    MessageBoxButtons.OKCancel, MessageBoxIcon.Warning, MessageBoxDefaultButton.Button2))
                {
                    return;
                }
            }

            this.DialogResult = DialogResult.OK;
            this.Close();
        }
    }
}
