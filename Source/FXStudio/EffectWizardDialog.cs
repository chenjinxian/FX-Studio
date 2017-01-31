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
using CristiPotlog.WizardControl;

namespace FXStudio
{
    public partial class EffectWizardDialog : Form
    {
        private string m_FileName;
        private string m_EffectName;

        private string m_EffectDir;
        private string m_TemplatePath;

        public EffectWizardDialog(string projectLocation)
        {
            InitializeComponent();

            m_EffectDir = projectLocation + @"\Effects";
            m_TemplatePath = Directory.GetCurrentDirectory() + @"\Templates\";
        }

        public string FileName { get { return m_FileName; } set { m_FileName = value; } }

        public string EffectName { get { return m_EffectName; } set { m_EffectName = value; } }

        public bool IsEffectFromExist() { return radioButtonFile.Checked; }

        private void wizardForm_AfterSwitchPages(object sender, CristiPotlog.WizardControl.Wizard.AfterSwitchPagesEventArgs e)
        {
            WizardPage newPage = wizardForm.Pages[e.NewIndex];

            if (newPage == wizardPageFile)
            {
                wizardForm.NextEnabled = (textBoxFile.Text != string.Empty) && File.Exists(textBoxFile.Text);
            }
            else if (newPage == wizardPageTemplate)
            {
                wizardForm.NextEnabled = !string.IsNullOrEmpty(textBoxName.Text);
            }
            else if (newPage == wizardPageName)
            {
                textBoxEffect.Text = m_EffectName;
                if (radioButtonFile.Checked)
                {
                    m_FileName = textBoxFile.Text;
                }
                else if (radioButtonHlsl.Checked)
                {
                    m_FileName = textBoxLocation.Text + @"\" + textBoxName.Text;
                }
            }
        }

        private void wizardForm_BeforeSwitchPages(object sender, CristiPotlog.WizardControl.Wizard.BeforeSwitchPagesEventArgs e)
        {
            WizardPage oldPage = wizardForm.Pages[e.OldIndex];

            if (oldPage == wizardPageSelect && e.NewIndex > e.OldIndex)
            {
                if (!radioButtonFile.Checked)
                {
                    e.NewIndex++;
                }
            }
            else if (oldPage == wizardPageTemplate && e.NewIndex < e.OldIndex)
            {
                if (!radioButtonFile.Checked)
                {
                    e.NewIndex--;
                }
            }
            else if (oldPage == wizardPageFile && e.NewIndex > e.OldIndex)
            {
                e.NewIndex++;
            }
            else if (oldPage == wizardPageName && e.NewIndex < e.OldIndex)
            {
                if (radioButtonFile.Checked)
                {
                    e.NewIndex--;
                }
            }
        }

        private void radioButton_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButtonEmpty.Checked)
            {
                wizardForm.NextEnabled = false;
                wizardForm.FinishEnabled = true;
            }
            else
            {
                wizardForm.NextEnabled = true;
                wizardForm.FinishEnabled = false;
            }
        }

        private void wizardForm_Load(object sender, EventArgs e)
        {
            wizardForm.SelectedPage = wizardPageSelect;
            wizardForm.NextEnabled = false;
            wizardForm.FinishEnabled = false;
            textBoxLocation.Text = m_EffectDir;
        }

        private void textBoxFile_TextChanged(object sender, EventArgs e)
        {
            wizardForm.NextEnabled = (!string.IsNullOrEmpty(textBoxFile.Text)) && File.Exists(textBoxFile.Text);
        }

        private void buttonFile_Click(object sender, EventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog();

            dialog.Filter = "HLSL Effect Files (*.fx)|*.fx";
            dialog.FilterIndex = 1;
            dialog.RestoreDirectory = true;

            if (dialog.ShowDialog() == DialogResult.OK)
            {
                textBoxFile.Text = dialog.FileName;
                m_EffectName = Path.GetFileNameWithoutExtension(dialog.FileName);
            }
        }

        private void listViewEffect_SelectedIndexChanged(object sender, EventArgs e)
        {
            int count = listViewEffect.SelectedItems.Count;
            if (listViewEffect.SelectedIndices.Count > 0)
            {
                textBoxName.Text = (string)listViewEffect.SelectedItems[0].Tag + ".fx";
            }
            else
            {
                textBoxName.Text = "";
                wizardForm.FinishEnabled = false;
            }
        }

        private void buttonLocation_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog dialog = new FolderBrowserDialog();
            dialog.SelectedPath = textBoxLocation.Text;
            dialog.ShowNewFolderButton = true;
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                textBoxLocation.Text = dialog.SelectedPath;
            }
        }

        private void textBoxEffect_TextChanged(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(textBoxEffect.Text))
                wizardForm.FinishEnabled = false;
        }

        private void textBoxName_TextChanged(object sender, EventArgs e)
        {
            wizardForm.NextEnabled = !string.IsNullOrEmpty(textBoxName.Text);
            m_EffectName = Path.GetFileNameWithoutExtension(textBoxName.Text);
        }

        private void wizardForm_Finish(object sender, EventArgs e)
        {
            if (!File.Exists(m_FileName))
            {
                string destFileName = m_TemplatePath + (string)listViewEffect.SelectedItems[0].Tag + ".fx";
                File.Copy(destFileName, m_FileName);
            }
        }
    }
}
