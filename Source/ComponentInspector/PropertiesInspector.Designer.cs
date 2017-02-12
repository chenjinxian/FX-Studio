namespace Inspector
{
    partial class PropertiesInspector
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(PropertiesInspector));
            this.panelProperties = new System.Windows.Forms.Panel();
            this.imageListExpand = new System.Windows.Forms.ImageList(this.components);
            this.SuspendLayout();
            // 
            // panelProperties
            // 
            this.panelProperties.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelProperties.Font = new System.Drawing.Font("Microsoft YaHei UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.panelProperties.Location = new System.Drawing.Point(0, 0);
            this.panelProperties.Name = "panelProperties";
            this.panelProperties.Size = new System.Drawing.Size(352, 672);
            this.panelProperties.TabIndex = 1;
            // 
            // imageListExpand
            // 
            this.imageListExpand.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageListExpand.ImageStream")));
            this.imageListExpand.TransparentColor = System.Drawing.Color.Transparent;
            this.imageListExpand.Images.SetKeyName(0, "Expand_16xLG.png");
            this.imageListExpand.Images.SetKeyName(1, "Collapse_16xLG.png");
            // 
            // PropertiesInspector
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.Window;
            this.Controls.Add(this.panelProperties);
            this.Name = "PropertiesInspector";
            this.Size = new System.Drawing.Size(352, 672);
            this.SizeChanged += new System.EventHandler(this.PropertiesInspector_SizeChanged);
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.Panel panelProperties;
        private System.Windows.Forms.ImageList imageListExpand;
    }
}
