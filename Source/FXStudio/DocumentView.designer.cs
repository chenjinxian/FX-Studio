﻿namespace FXStudio
{
    partial class DocumentView
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
            this.scintillaBox = new ScintillaNET.Scintilla();
            this.SuspendLayout();
            // 
            // scintillaBox
            // 
            this.scintillaBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.scintillaBox.IndentationGuides = ScintillaNET.IndentView.LookBoth;
            this.scintillaBox.Location = new System.Drawing.Point(0, 4);
            this.scintillaBox.Name = "scintillaBox";
            this.scintillaBox.Size = new System.Drawing.Size(448, 422);
            this.scintillaBox.TabIndex = 3;
            // 
            // DocumentView
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(448, 426);
            this.Controls.Add(this.scintillaBox);
            this.Name = "DocumentView";
            this.Padding = new System.Windows.Forms.Padding(0, 4, 0, 0);
            this.Text = "DocumentView";
            this.Load += new System.EventHandler(this.DocumentView_Load);
            this.ResumeLayout(false);

        }

        #endregion
        private ScintillaNET.Scintilla scintillaBox;
    }
}