using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;

namespace FXStudio
{
    public partial class LoadingProgressDialog : Form
    {
        private string m_SourceFileName;
        private string m_DestFileName;

        public LoadingProgressDialog()
        {
            InitializeComponent();
        }

        public string SourceFileName { set { m_SourceFileName = value; } }
        public string DestFileName { set { m_DestFileName = value; } }

        private void LoadingProgressDialog_Load(object sender, EventArgs e)
        {
            if (!backgroundWorkerLoading.IsBusy)
            {
                backgroundWorkerLoading.RunWorkerAsync();
            }
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            if (backgroundWorkerLoading.WorkerSupportsCancellation)
            {
                backgroundWorkerLoading.CancelAsync();
            }
            if (!backgroundWorkerLoading.IsBusy)
            {
                this.DialogResult = DialogResult.Cancel;
                this.Close();
            }
        }

        private void backgroundWorkerLoading_DoWork(object sender, DoWorkEventArgs e)
        {
            BackgroundWorker worker = sender as BackgroundWorker;
            RenderMethods.ImportModel(m_SourceFileName, m_DestFileName, (percent, error) =>
             {
                 try
                 {
                     worker.ReportProgress((int)(percent * 100), error);
                     e.Result = error;
                 }
                 catch (Exception ex)
                 {
                     MessageBox.Show(ex.Message);
                 }
                 return !worker.CancellationPending;
             });
        }

        private void backgroundWorkerLoading_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            string errorInfo = e.UserState as string;
            if (string.IsNullOrEmpty(errorInfo))
            {
                progressBarLoading.Value = e.ProgressPercentage;
                labelProgress.Text = e.ProgressPercentage.ToString() + " %";
            }
            else
            {
                progressBarLoading.Value = 0;
                labelProgress.Text = errorInfo;
            }
        }

        private void backgroundWorkerLoading_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (e.Cancelled)
            {
                this.DialogResult = DialogResult.Cancel;
                this.Close();
            }
            else if (e.Error != null)
            {

            }
            else
            {
                string errorInfo = e.Result as string;
                if (string.IsNullOrEmpty(errorInfo))
                {
                    this.DialogResult = DialogResult.OK;
                    this.Close();
                }
                else
                {
                    progressBarLoading.Value = 0;
                    labelProgress.Text = errorInfo;
                }
            }
        }
    }
}
