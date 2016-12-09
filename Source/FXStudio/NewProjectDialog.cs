using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace FXStudio
{
    public partial class NewProjectDialog : Form
    {
        public NewProjectDialog()
        {
            InitializeComponent();

            this.textBoxName.Text = "Project1";
            this.textBoxLocation.Text = @"C:\Users\chenjinxian\Documents\FX Studio\Projects\Project1";
        }

        private void buttonBrowse_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog dialog = new FolderBrowserDialog();
            dialog.SelectedPath = this.labelLocation.Name;
            dialog.ShowNewFolderButton = true;
            dialog.Description = "Select a folder";

            if (dialog.ShowDialog() == DialogResult.OK)
            {
            }
        }
    }
}
