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
            this.pictureBoxStarted = new System.Windows.Forms.PictureBox();
            this.labelStarted = new System.Windows.Forms.Label();
            this.groupBoxStart = new System.Windows.Forms.GroupBox();
            this.linkLabelOpenProject = new System.Windows.Forms.LinkLabel();
            this.linkLabelNewProject = new System.Windows.Forms.LinkLabel();
            this.groupBoxRecent = new System.Windows.Forms.GroupBox();
            this.groupBoxIntroduction = new System.Windows.Forms.GroupBox();
            this.checkBox1 = new System.Windows.Forms.CheckBox();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxStarted)).BeginInit();
            this.groupBoxStart.SuspendLayout();
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
            this.linkLabelOpenProject.Location = new System.Drawing.Point(6, 62);
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
            this.linkLabelNewProject.Location = new System.Drawing.Point(7, 29);
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
            this.groupBoxIntroduction.Font = new System.Drawing.Font("Microsoft YaHei", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.groupBoxIntroduction.Location = new System.Drawing.Point(483, 46);
            this.groupBoxIntroduction.Name = "groupBoxIntroduction";
            this.groupBoxIntroduction.Size = new System.Drawing.Size(200, 250);
            this.groupBoxIntroduction.TabIndex = 6;
            this.groupBoxIntroduction.TabStop = false;
            this.groupBoxIntroduction.Text = "Introduction";
            // 
            // checkBox1
            // 
            this.checkBox1.AutoSize = true;
            this.checkBox1.Checked = true;
            this.checkBox1.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox1.Font = new System.Drawing.Font("Microsoft YaHei", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.checkBox1.Location = new System.Drawing.Point(12, 326);
            this.checkBox1.Name = "checkBox1";
            this.checkBox1.Size = new System.Drawing.Size(156, 21);
            this.checkBox1.TabIndex = 7;
            this.checkBox1.Text = "Show page on startup";
            this.checkBox1.UseVisualStyleBackColor = true;
            // 
            // StartPageDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.Window;
            this.ClientSize = new System.Drawing.Size(695, 354);
            this.Controls.Add(this.checkBox1);
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
        private System.Windows.Forms.CheckBox checkBox1;
    }
}