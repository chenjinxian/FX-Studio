namespace FXStudio
{
    partial class PropertiesView
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
            this.inspectorComponent = new Inspector.PropertiesInspector();
            this.SuspendLayout();
            // 
            // inspectorComponent
            // 
            this.inspectorComponent.ButtonApplyVisible = false;
            this.inspectorComponent.ButtonShowTextVisible = false;
            this.inspectorComponent.ColumnWidth = 100;
            this.inspectorComponent.Dock = System.Windows.Forms.DockStyle.Fill;
            this.inspectorComponent.HelpVisible = true;
            this.inspectorComponent.Location = new System.Drawing.Point(0, 0);
            this.inspectorComponent.Name = "inspectorComponent";
            this.inspectorComponent.RowsHeight = 19;
            this.inspectorComponent.SelectedItem = null;
            this.inspectorComponent.ShowItemsAsCategory = true;
            this.inspectorComponent.Size = new System.Drawing.Size(424, 901);
            this.inspectorComponent.TabIndex = 0;
            this.inspectorComponent.Text = "PropertiesInspector";
            this.inspectorComponent.TextBackgroundColor = System.Drawing.Color.LightBlue;
            this.inspectorComponent.TextBackgroundColorIsGradient = true;
            this.inspectorComponent.TextBackgroundGradientColorFinish = System.Drawing.Color.Blue;
            this.inspectorComponent.TextBackgroundGradientColorStart = System.Drawing.Color.LightBlue;
            this.inspectorComponent.TextVisible = false;
            this.inspectorComponent.ToolbarVisible = true;
            this.inspectorComponent.TraceLogEnabled = false;
            // 
            // PropertiesView
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(424, 901);
            this.Controls.Add(this.inspectorComponent);
            this.Name = "PropertiesView";
            this.TabText = "Properties";
            this.Text = "PropertiesView";
            this.ResumeLayout(false);

        }

        #endregion

        private Inspector.PropertiesInspector inspectorComponent;
    }
}