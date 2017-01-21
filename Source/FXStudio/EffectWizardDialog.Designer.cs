namespace FXStudio
{
    partial class EffectWizardDialog
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(EffectWizardDialog));
            this.wizardForm = new CristiPotlog.WizardControl.Wizard();
            this.wizardPageName = new CristiPotlog.WizardControl.WizardPage();
            this.wizardPageTemplate = new CristiPotlog.WizardControl.WizardPage();
            this.wizardPageSelect = new CristiPotlog.WizardControl.WizardPage();
            this.wizardForm.SuspendLayout();
            this.SuspendLayout();
            // 
            // wizardForm
            // 
            this.wizardForm.Controls.Add(this.wizardPageTemplate);
            this.wizardForm.Controls.Add(this.wizardPageName);
            this.wizardForm.Controls.Add(this.wizardPageSelect);
            this.wizardForm.HeaderFont = new System.Drawing.Font("Microsoft YaHei UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.wizardForm.HeaderImage = ((System.Drawing.Image)(resources.GetObject("wizardForm.HeaderImage")));
            this.wizardForm.HeaderTitleFont = new System.Drawing.Font("Microsoft YaHei UI", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.wizardForm.Location = new System.Drawing.Point(0, 0);
            this.wizardForm.Name = "wizardForm";
            this.wizardForm.Pages.AddRange(new CristiPotlog.WizardControl.WizardPage[] {
            this.wizardPageSelect,
            this.wizardPageTemplate,
            this.wizardPageName});
            this.wizardForm.Size = new System.Drawing.Size(496, 318);
            this.wizardForm.TabIndex = 0;
            // 
            // wizardPageName
            // 
            this.wizardPageName.Description = "Use this page to customize the name of the Effect and Materials to be created whe" +
    "n the wizard is complete";
            this.wizardPageName.Location = new System.Drawing.Point(0, 0);
            this.wizardPageName.Name = "wizardPageName";
            this.wizardPageName.Size = new System.Drawing.Size(509, 292);
            this.wizardPageName.TabIndex = 12;
            this.wizardPageName.Title = "New Effect Name";
            // 
            // wizardPageTemplate
            // 
            this.wizardPageTemplate.CausesValidation = false;
            this.wizardPageTemplate.Description = "The imported effect will be saved with the specified  filename and location";
            this.wizardPageTemplate.Location = new System.Drawing.Point(0, 0);
            this.wizardPageTemplate.Name = "wizardPageTemplate";
            this.wizardPageTemplate.Size = new System.Drawing.Size(496, 270);
            this.wizardPageTemplate.TabIndex = 11;
            this.wizardPageTemplate.Title = "Select a HLSL FX Template";
            // 
            // wizardPageSelect
            // 
            this.wizardPageSelect.Description = "Add a new effect to the project";
            this.wizardPageSelect.Dock = System.Windows.Forms.DockStyle.Top;
            this.wizardPageSelect.Font = new System.Drawing.Font("Microsoft YaHei UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.wizardPageSelect.Location = new System.Drawing.Point(0, 0);
            this.wizardPageSelect.Name = "wizardPageSelect";
            this.wizardPageSelect.Size = new System.Drawing.Size(489, 292);
            this.wizardPageSelect.TabIndex = 10;
            this.wizardPageSelect.Title = "Add Effect";
            // 
            // EffectWizardDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(496, 318);
            this.Controls.Add(this.wizardForm);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "EffectWizardDialog";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "EffectWizardDialog";
            this.wizardForm.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private CristiPotlog.WizardControl.Wizard wizardForm;
        private CristiPotlog.WizardControl.WizardPage wizardPageSelect;
        private CristiPotlog.WizardControl.WizardPage wizardPageTemplate;
        private CristiPotlog.WizardControl.WizardPage wizardPageName;
    }
}