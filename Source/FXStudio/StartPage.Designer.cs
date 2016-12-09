namespace FXStudio
{
    partial class StartPage
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
            this.linkLabel1 = new System.Windows.Forms.LinkLabel();
            this.linkLabel2 = new System.Windows.Forms.LinkLabel();
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
            this.groupBoxStart.Controls.Add(this.linkLabel2);
            this.groupBoxStart.Controls.Add(this.linkLabel1);
            this.groupBoxStart.Font = new System.Drawing.Font("Microsoft YaHei", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.groupBoxStart.Location = new System.Drawing.Point(12, 46);
            this.groupBoxStart.Name = "groupBoxStart";
            this.groupBoxStart.Size = new System.Drawing.Size(200, 250);
            this.groupBoxStart.TabIndex = 4;
            this.groupBoxStart.TabStop = false;
            this.groupBoxStart.Text = "Start";
            // 
            // linkLabel1
            // 
            this.linkLabel1.AutoSize = true;
            this.linkLabel1.Font = new System.Drawing.Font("Microsoft YaHei", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.linkLabel1.Location = new System.Drawing.Point(7, 29);
            this.linkLabel1.Name = "linkLabel1";
            this.linkLabel1.Size = new System.Drawing.Size(99, 20);
            this.linkLabel1.TabIndex = 0;
            this.linkLabel1.TabStop = true;
            this.linkLabel1.Text = "New Project...";
            // 
            // linkLabel2
            // 
            this.linkLabel2.AutoSize = true;
            this.linkLabel2.Font = new System.Drawing.Font("Microsoft YaHei", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.linkLabel2.Location = new System.Drawing.Point(6, 62);
            this.linkLabel2.Name = "linkLabel2";
            this.linkLabel2.Size = new System.Drawing.Size(106, 20);
            this.linkLabel2.TabIndex = 1;
            this.linkLabel2.TabStop = true;
            this.linkLabel2.Text = "Open Project...";
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
            this.checkBox1.Font = new System.Drawing.Font("Microsoft YaHei", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.checkBox1.Location = new System.Drawing.Point(12, 326);
            this.checkBox1.Name = "checkBox1";
            this.checkBox1.Size = new System.Drawing.Size(156, 21);
            this.checkBox1.TabIndex = 7;
            this.checkBox1.Text = "Show page on startup";
            this.checkBox1.UseVisualStyleBackColor = true;
            // 
            // StartPage
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
            this.Name = "StartPage";
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
        private System.Windows.Forms.LinkLabel linkLabel2;
        private System.Windows.Forms.LinkLabel linkLabel1;
        private System.Windows.Forms.GroupBox groupBoxRecent;
        private System.Windows.Forms.GroupBox groupBoxIntroduction;
        private System.Windows.Forms.CheckBox checkBox1;
    }
}