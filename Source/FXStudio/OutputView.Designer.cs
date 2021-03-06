﻿namespace FXStudio
{
    partial class OutputView
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(OutputView));
            this.toolStripOutput = new System.Windows.Forms.ToolStrip();
            this.toolStripLabelHelp = new System.Windows.Forms.ToolStripLabel();
            this.toolStripComboBoxType = new System.Windows.Forms.ToolStripComboBox();
            this.toolStripButtonClear = new System.Windows.Forms.ToolStripButton();
            this.richTextBoxOutput = new System.Windows.Forms.RichTextBox();
            this.toolStripOutput.SuspendLayout();
            this.SuspendLayout();
            // 
            // toolStripOutput
            // 
            this.toolStripOutput.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.toolStripOutput.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripLabelHelp,
            this.toolStripComboBoxType,
            this.toolStripButtonClear});
            this.toolStripOutput.Location = new System.Drawing.Point(0, 0);
            this.toolStripOutput.Name = "toolStripOutput";
            this.toolStripOutput.Size = new System.Drawing.Size(637, 25);
            this.toolStripOutput.TabIndex = 0;
            this.toolStripOutput.Text = "toolStripOutput";
            // 
            // toolStripLabelHelp
            // 
            this.toolStripLabelHelp.Name = "toolStripLabelHelp";
            this.toolStripLabelHelp.Size = new System.Drawing.Size(107, 22);
            this.toolStripLabelHelp.Text = "Show output from:";
            // 
            // toolStripComboBoxType
            // 
            this.toolStripComboBoxType.Items.AddRange(new object[] {
            "Log",
            "Build"});
            this.toolStripComboBoxType.Name = "toolStripComboBoxType";
            this.toolStripComboBoxType.Size = new System.Drawing.Size(121, 25);
            this.toolStripComboBoxType.Text = "Log";
            // 
            // toolStripButtonClear
            // 
            this.toolStripButtonClear.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonClear.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonClear.Image")));
            this.toolStripButtonClear.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonClear.Name = "toolStripButtonClear";
            this.toolStripButtonClear.Size = new System.Drawing.Size(23, 22);
            this.toolStripButtonClear.Text = "toolStripButtonClear";
            this.toolStripButtonClear.ToolTipText = "Clear All";
            // 
            // richTextBoxOutput
            // 
            this.richTextBoxOutput.Dock = System.Windows.Forms.DockStyle.Fill;
            this.richTextBoxOutput.Location = new System.Drawing.Point(0, 25);
            this.richTextBoxOutput.Name = "richTextBoxOutput";
            this.richTextBoxOutput.ReadOnly = true;
            this.richTextBoxOutput.Size = new System.Drawing.Size(637, 465);
            this.richTextBoxOutput.TabIndex = 1;
            this.richTextBoxOutput.Text = "";
            // 
            // OutputView
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(637, 490);
            this.Controls.Add(this.richTextBoxOutput);
            this.Controls.Add(this.toolStripOutput);
            this.Name = "OutputView";
            this.TabText = "Output";
            this.Text = "OutputView";
            this.toolStripOutput.ResumeLayout(false);
            this.toolStripOutput.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ToolStrip toolStripOutput;
        private System.Windows.Forms.RichTextBox richTextBoxOutput;
        private System.Windows.Forms.ToolStripLabel toolStripLabelHelp;
        private System.Windows.Forms.ToolStripComboBox toolStripComboBoxType;
        private System.Windows.Forms.ToolStripButton toolStripButtonClear;
    }
}