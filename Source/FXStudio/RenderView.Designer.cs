namespace FXStudio
{
    partial class RenderView
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(RenderView));
            this.toolStripRender = new System.Windows.Forms.ToolStrip();
            this.panelRender = new System.Windows.Forms.Panel();
            this.toolStripButtonInstance = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonNavigation = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripButtonZoomObjects = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonReset = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonZoomScene = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripButtonSelect = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonTranslate = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonRotate = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonScale = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripButtonBackground = new System.Windows.Forms.ToolStripButton();
            this.toolStripRender.SuspendLayout();
            this.SuspendLayout();
            // 
            // toolStripRender
            // 
            this.toolStripRender.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.toolStripRender.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripButtonInstance,
            this.toolStripButtonNavigation,
            this.toolStripSeparator1,
            this.toolStripButtonZoomObjects,
            this.toolStripButtonReset,
            this.toolStripButtonZoomScene,
            this.toolStripSeparator2,
            this.toolStripButtonSelect,
            this.toolStripButtonTranslate,
            this.toolStripButtonRotate,
            this.toolStripButtonScale,
            this.toolStripSeparator3,
            this.toolStripButtonBackground});
            this.toolStripRender.Location = new System.Drawing.Point(0, 0);
            this.toolStripRender.Name = "toolStripRender";
            this.toolStripRender.Size = new System.Drawing.Size(756, 25);
            this.toolStripRender.TabIndex = 0;
            this.toolStripRender.Text = "toolStripRender";
            // 
            // panelRender
            // 
            this.panelRender.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelRender.Location = new System.Drawing.Point(0, 25);
            this.panelRender.Name = "panelRender";
            this.panelRender.Size = new System.Drawing.Size(756, 523);
            this.panelRender.TabIndex = 1;
            // 
            // toolStripButtonInstance
            // 
            this.toolStripButtonInstance.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonInstance.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonInstance.Image")));
            this.toolStripButtonInstance.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonInstance.Name = "toolStripButtonInstance";
            this.toolStripButtonInstance.Size = new System.Drawing.Size(23, 22);
            this.toolStripButtonInstance.Text = "Instance Cameras";
            // 
            // toolStripButtonNavigation
            // 
            this.toolStripButtonNavigation.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonNavigation.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonNavigation.Image")));
            this.toolStripButtonNavigation.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonNavigation.Name = "toolStripButtonNavigation";
            this.toolStripButtonNavigation.Size = new System.Drawing.Size(23, 22);
            this.toolStripButtonNavigation.Text = "Navigation Mode";
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
            // 
            // toolStripButtonZoomObjects
            // 
            this.toolStripButtonZoomObjects.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonZoomObjects.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonZoomObjects.Image")));
            this.toolStripButtonZoomObjects.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonZoomObjects.Name = "toolStripButtonZoomObjects";
            this.toolStripButtonZoomObjects.Size = new System.Drawing.Size(23, 22);
            this.toolStripButtonZoomObjects.Text = "Zoom Selected Object Extents";
            // 
            // toolStripButtonReset
            // 
            this.toolStripButtonReset.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonReset.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonReset.Image")));
            this.toolStripButtonReset.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonReset.Name = "toolStripButtonReset";
            this.toolStripButtonReset.Size = new System.Drawing.Size(23, 22);
            this.toolStripButtonReset.Text = "Reset Camera Rotation";
            // 
            // toolStripButtonZoomScene
            // 
            this.toolStripButtonZoomScene.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonZoomScene.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonZoomScene.Image")));
            this.toolStripButtonZoomScene.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonZoomScene.Name = "toolStripButtonZoomScene";
            this.toolStripButtonZoomScene.Size = new System.Drawing.Size(23, 22);
            this.toolStripButtonZoomScene.Text = "Zoom Scene Extents";
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 25);
            // 
            // toolStripButtonSelect
            // 
            this.toolStripButtonSelect.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonSelect.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonSelect.Image")));
            this.toolStripButtonSelect.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonSelect.Name = "toolStripButtonSelect";
            this.toolStripButtonSelect.Size = new System.Drawing.Size(23, 22);
            this.toolStripButtonSelect.Text = "Select Object";
            // 
            // toolStripButtonTranslate
            // 
            this.toolStripButtonTranslate.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonTranslate.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonTranslate.Image")));
            this.toolStripButtonTranslate.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonTranslate.Name = "toolStripButtonTranslate";
            this.toolStripButtonTranslate.Size = new System.Drawing.Size(23, 22);
            this.toolStripButtonTranslate.Text = "Translate Object";
            // 
            // toolStripButtonRotate
            // 
            this.toolStripButtonRotate.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonRotate.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonRotate.Image")));
            this.toolStripButtonRotate.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonRotate.Name = "toolStripButtonRotate";
            this.toolStripButtonRotate.Size = new System.Drawing.Size(23, 22);
            this.toolStripButtonRotate.Text = "Rotate Object";
            // 
            // toolStripButtonScale
            // 
            this.toolStripButtonScale.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonScale.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonScale.Image")));
            this.toolStripButtonScale.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonScale.Name = "toolStripButtonScale";
            this.toolStripButtonScale.Size = new System.Drawing.Size(23, 22);
            this.toolStripButtonScale.Text = "Scale Object";
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(6, 25);
            // 
            // toolStripButtonBackground
            // 
            this.toolStripButtonBackground.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonBackground.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonBackground.Image")));
            this.toolStripButtonBackground.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonBackground.Name = "toolStripButtonBackground";
            this.toolStripButtonBackground.Size = new System.Drawing.Size(23, 22);
            this.toolStripButtonBackground.Text = "Set the background color";
            this.toolStripButtonBackground.ToolTipText = "Set the background color (for scenes without a full screen effect)";
            // 
            // RenderView
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(756, 548);
            this.Controls.Add(this.panelRender);
            this.Controls.Add(this.toolStripRender);
            this.Name = "RenderView";
            this.TabText = "Render";
            this.Text = "RenderView";
            this.toolStripRender.ResumeLayout(false);
            this.toolStripRender.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ToolStrip toolStripRender;
        private System.Windows.Forms.Panel panelRender;
        private System.Windows.Forms.ToolStripButton toolStripButtonInstance;
        private System.Windows.Forms.ToolStripButton toolStripButtonNavigation;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripButton toolStripButtonZoomObjects;
        private System.Windows.Forms.ToolStripButton toolStripButtonReset;
        private System.Windows.Forms.ToolStripButton toolStripButtonZoomScene;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripButton toolStripButtonSelect;
        private System.Windows.Forms.ToolStripButton toolStripButtonTranslate;
        private System.Windows.Forms.ToolStripButton toolStripButtonRotate;
        private System.Windows.Forms.ToolStripButton toolStripButtonScale;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripButton toolStripButtonBackground;
    }
}