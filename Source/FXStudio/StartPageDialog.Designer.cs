namespace FXStudio
{
    partial class StartPageDialog
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.pictureBoxStarted = new System.Windows.Forms.PictureBox();
            this.labelStarted = new System.Windows.Forms.Label();
            this.groupBoxStart = new System.Windows.Forms.GroupBox();
            this.linkLabelOpenProject = new System.Windows.Forms.LinkLabel();
            this.linkLabelNewProject = new System.Windows.Forms.LinkLabel();
            this.groupBoxRecent = new System.Windows.Forms.GroupBox();
            this.groupBoxIntroduction = new System.Windows.Forms.GroupBox();
            this.linkLabelContact = new System.Windows.Forms.LinkLabel();
            this.labelContact = new System.Windows.Forms.Label();
            this.linkLabelGitHub = new System.Windows.Forms.LinkLabel();
            this.labelGitHub = new System.Windows.Forms.Label();
            this.linkLabelBlog = new System.Windows.Forms.LinkLabel();
            this.labelBlog = new System.Windows.Forms.Label();
            this.checkBoxShow = new System.Windows.Forms.CheckBox();
            this.toolTipBlog = new System.Windows.Forms.ToolTip(this.components);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxStarted)).BeginInit();
            this.groupBoxStart.SuspendLayout();
            this.groupBoxIntroduction.SuspendLayout();
            this.SuspendLayout();
            // 
            // pictureBoxStarted
            // 
            this.pictureBoxStarted.BackColor = System.Drawing.SystemColors.Highlight;
            this.pictureBoxStarted.Dock = System.Windows.Forms.DockStyle.Top;
            this.pictureBoxStarted.Location = new System.Drawing.Point(0, 0);
            this.pictureBoxStarted.Name = "pictureBoxStarted";
            this.pictureBoxStarted.Size = new System.Drawing.Size(695, 40);
            this.pictureBoxStarted.TabIndex = 1;
            this.pictureBoxStarted.TabStop = false;
            // 
            // labelStarted
            // 
            this.labelStarted.AutoSize = true;
            this.labelStarted.BackColor = System.Drawing.SystemColors.Highlight;
            this.labelStarted.Font = new System.Drawing.Font("Microsoft YaHei", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.labelStarted.ForeColor = System.Drawing.SystemColors.Control;
            this.labelStarted.Location = new System.Drawing.Point(2, 6);
            this.labelStarted.Name = "labelStarted";
            this.labelStarted.Size = new System.Drawing.Size(167, 28);
            this.labelStarted.TabIndex = 3;
            this.labelStarted.Text = "Getting Started";
            // 
            // groupBoxStart
            // 
            this.groupBoxStart.Controls.Add(this.linkLabelOpenProject);
            this.groupBoxStart.Controls.Add(this.linkLabelNewProject);
            this.groupBoxStart.Font = new System.Drawing.Font("Microsoft YaHei", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.groupBoxStart.Location = new System.Drawing.Point(12, 46);
            this.groupBoxStart.Name = "groupBoxStart";
            this.groupBoxStart.Size = new System.Drawing.Size(200, 250);
            this.groupBoxStart.TabIndex = 4;
            this.groupBoxStart.TabStop = false;
            this.groupBoxStart.Text = "Start";
            // 
            // linkLabelOpenProject
            // 
            this.linkLabelOpenProject.AutoSize = true;
            this.linkLabelOpenProject.Font = new System.Drawing.Font("Microsoft YaHei", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.linkLabelOpenProject.LinkBehavior = System.Windows.Forms.LinkBehavior.HoverUnderline;
            this.linkLabelOpenProject.Location = new System.Drawing.Point(6, 64);
            this.linkLabelOpenProject.Name = "linkLabelOpenProject";
            this.linkLabelOpenProject.Size = new System.Drawing.Size(106, 20);
            this.linkLabelOpenProject.TabIndex = 1;
            this.linkLabelOpenProject.TabStop = true;
            this.linkLabelOpenProject.Text = "Open Project...";
            this.linkLabelOpenProject.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.linkLabelOpenProject_LinkClicked);
            // 
            // linkLabelNewProject
            // 
            this.linkLabelNewProject.AutoSize = true;
            this.linkLabelNewProject.Font = new System.Drawing.Font("Microsoft YaHei", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.linkLabelNewProject.LinkBehavior = System.Windows.Forms.LinkBehavior.HoverUnderline;
            this.linkLabelNewProject.Location = new System.Drawing.Point(6, 30);
            this.linkLabelNewProject.Name = "linkLabelNewProject";
            this.linkLabelNewProject.Size = new System.Drawing.Size(99, 20);
            this.linkLabelNewProject.TabIndex = 0;
            this.linkLabelNewProject.TabStop = true;
            this.linkLabelNewProject.Text = "New Project...";
            this.linkLabelNewProject.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.linkLabelNewProject_LinkClicked);
            // 
            // groupBoxRecent
            // 
            this.groupBoxRecent.Font = new System.Drawing.Font("Microsoft YaHei", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.groupBoxRecent.Location = new System.Drawing.Point(247, 46);
            this.groupBoxRecent.Name = "groupBoxRecent";
            this.groupBoxRecent.Size = new System.Drawing.Size(200, 250);
            this.groupBoxRecent.TabIndex = 5;
            this.groupBoxRecent.TabStop = false;
            this.groupBoxRecent.Text = "Recent";
            // 
            // groupBoxIntroduction
            // 
            this.groupBoxIntroduction.Controls.Add(this.linkLabelContact);
            this.groupBoxIntroduction.Controls.Add(this.labelContact);
            this.groupBoxIntroduction.Controls.Add(this.linkLabelGitHub);
            this.groupBoxIntroduction.Controls.Add(this.labelGitHub);
            this.groupBoxIntroduction.Controls.Add(this.linkLabelBlog);
            this.groupBoxIntroduction.Controls.Add(this.labelBlog);
            this.groupBoxIntroduction.Font = new System.Drawing.Font("Microsoft YaHei", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.groupBoxIntroduction.Location = new System.Drawing.Point(483, 46);
            this.groupBoxIntroduction.Name = "groupBoxIntroduction";
            this.groupBoxIntroduction.Size = new System.Drawing.Size(200, 250);
            this.groupBoxIntroduction.TabIndex = 6;
            this.groupBoxIntroduction.TabStop = false;
            this.groupBoxIntroduction.Text = "Introduction";
            // 
            // linkLabelContact
            // 
            this.linkLabelContact.AutoSize = true;
            this.linkLabelContact.Enabled = false;
            this.linkLabelContact.Font = new System.Drawing.Font("Microsoft YaHei", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.linkLabelContact.LinkBehavior = System.Windows.Forms.LinkBehavior.NeverUnderline;
            this.linkLabelContact.Location = new System.Drawing.Point(6, 176);
            this.linkLabelContact.Name = "linkLabelContact";
            this.linkLabelContact.Size = new System.Drawing.Size(153, 20);
            this.linkLabelContact.TabIndex = 5;
            this.linkLabelContact.TabStop = true;
            this.linkLabelContact.Text = "2438923268@qq.com";
            this.toolTipBlog.SetToolTip(this.linkLabelContact, "2438923268@qq.com");
            // 
            // labelContact
            // 
            this.labelContact.AutoSize = true;
            this.labelContact.Font = new System.Drawing.Font("Microsoft YaHei", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.labelContact.Location = new System.Drawing.Point(6, 150);
            this.labelContact.Name = "labelContact";
            this.labelContact.Size = new System.Drawing.Size(64, 20);
            this.labelContact.TabIndex = 4;
            this.labelContact.Text = "Contact:";
            // 
            // linkLabelGitHub
            // 
            this.linkLabelGitHub.AutoSize = true;
            this.linkLabelGitHub.Font = new System.Drawing.Font("Microsoft YaHei", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.linkLabelGitHub.LinkBehavior = System.Windows.Forms.LinkBehavior.HoverUnderline;
            this.linkLabelGitHub.Location = new System.Drawing.Point(6, 56);
            this.linkLabelGitHub.Name = "linkLabelGitHub";
            this.linkLabelGitHub.Size = new System.Drawing.Size(170, 20);
            this.linkLabelGitHub.TabIndex = 3;
            this.linkLabelGitHub.TabStop = true;
            this.linkLabelGitHub.Text = "github.com/chenjinxian";
            this.toolTipBlog.SetToolTip(this.linkLabelGitHub, "https://github.com/chenjinxian/FX-Studio");
            this.linkLabelGitHub.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.linkLabelGitHub_LinkClicked);
            // 
            // labelGitHub
            // 
            this.labelGitHub.AutoSize = true;
            this.labelGitHub.Font = new System.Drawing.Font("Microsoft YaHei", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.labelGitHub.Location = new System.Drawing.Point(6, 30);
            this.labelGitHub.Name = "labelGitHub";
            this.labelGitHub.Size = new System.Drawing.Size(60, 20);
            this.labelGitHub.TabIndex = 2;
            this.labelGitHub.Text = "GitHub:";
            // 
            // linkLabelBlog
            // 
            this.linkLabelBlog.AutoSize = true;
            this.linkLabelBlog.Font = new System.Drawing.Font("Microsoft YaHei", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.linkLabelBlog.LinkBehavior = System.Windows.Forms.LinkBehavior.HoverUnderline;
            this.linkLabelBlog.Location = new System.Drawing.Point(6, 116);
            this.linkLabelBlog.Name = "linkLabelBlog";
            this.linkLabelBlog.Size = new System.Drawing.Size(110, 20);
            this.linkLabelBlog.TabIndex = 1;
            this.linkLabelBlog.TabStop = true;
            this.linkLabelBlog.Text = "chenjinxian_3d";
            this.toolTipBlog.SetToolTip(this.linkLabelBlog, "http://blog.csdn.net/chenjinxian_3d");
            this.linkLabelBlog.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.linkLabelBlog_LinkClicked);
            // 
            // labelBlog
            // 
            this.labelBlog.AutoSize = true;
            this.labelBlog.Font = new System.Drawing.Font("Microsoft YaHei", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.labelBlog.Location = new System.Drawing.Point(6, 90);
            this.labelBlog.Name = "labelBlog";
            this.labelBlog.Size = new System.Drawing.Size(86, 20);
            this.labelBlog.TabIndex = 0;
            this.labelBlog.Text = "CSDN Blog:";
            // 
            // checkBoxShow
            // 
            this.checkBoxShow.AutoSize = true;
            this.checkBoxShow.Checked = true;
            this.checkBoxShow.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxShow.Font = new System.Drawing.Font("Microsoft YaHei", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.checkBoxShow.Location = new System.Drawing.Point(12, 326);
            this.checkBoxShow.Name = "checkBoxShow";
            this.checkBoxShow.Size = new System.Drawing.Size(156, 21);
            this.checkBoxShow.TabIndex = 7;
            this.checkBoxShow.Text = "Show page on startup";
            this.checkBoxShow.UseVisualStyleBackColor = true;
            this.checkBoxShow.CheckedChanged += new System.EventHandler(this.checkBoxShow_CheckedChanged);
            // 
            // StartPageDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.Window;
            this.ClientSize = new System.Drawing.Size(695, 354);
            this.Controls.Add(this.checkBoxShow);
            this.Controls.Add(this.groupBoxIntroduction);
            this.Controls.Add(this.groupBoxRecent);
            this.Controls.Add(this.groupBoxStart);
            this.Controls.Add(this.labelStarted);
            this.Controls.Add(this.pictureBoxStarted);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "StartPageDialog";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Fx Studio Start Page";
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxStarted)).EndInit();
            this.groupBoxStart.ResumeLayout(false);
            this.groupBoxStart.PerformLayout();
            this.groupBoxIntroduction.ResumeLayout(false);
            this.groupBoxIntroduction.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBoxStarted;
        private System.Windows.Forms.Label labelStarted;
        private System.Windows.Forms.GroupBox groupBoxStart;
        private System.Windows.Forms.LinkLabel linkLabelOpenProject;
        private System.Windows.Forms.LinkLabel linkLabelNewProject;
        private System.Windows.Forms.GroupBox groupBoxRecent;
        private System.Windows.Forms.GroupBox groupBoxIntroduction;
        private System.Windows.Forms.CheckBox checkBoxShow;
        private System.Windows.Forms.LinkLabel linkLabelBlog;
        private System.Windows.Forms.Label labelBlog;
        private System.Windows.Forms.ToolTip toolTipBlog;
        private System.Windows.Forms.LinkLabel linkLabelGitHub;
        private System.Windows.Forms.Label labelGitHub;
        private System.Windows.Forms.LinkLabel linkLabelContact;
        private System.Windows.Forms.Label labelContact;
    }
}