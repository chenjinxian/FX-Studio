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
    public partial class NewProjectDialog : Form
    {
        public NewProjectDialog(string defaultLocation)
        {
            InitializeComponent();

            this.textBoxName.Text = "Project1";
            this.textBoxLocation.Text = defaultLocation;
        }

        public string GetProjectName()
        {
            return this.textBoxName.Text;
        }

        public string GetProjectLocation()
        {
            if (checkBoxCreateDir.Checked)
                return this.textBoxLocation.Text;
            else
                return this.textBoxLocation.Text + @"\" + this.textBoxName;
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
    }
}
