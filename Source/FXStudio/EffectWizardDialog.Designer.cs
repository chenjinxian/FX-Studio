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
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.ListViewItem listViewItem1 = new System.Windows.Forms.ListViewItem("Empty", 0);
            System.Windows.Forms.ListViewItem listViewItem2 = new System.Windows.Forms.ListViewItem("Blinn", 1);
            System.Windows.Forms.ListViewItem listViewItem3 = new System.Windows.Forms.ListViewItem("Blinn bump reflect", 2);
            System.Windows.Forms.ListViewItem listViewItem4 = new System.Windows.Forms.ListViewItem("Gooch", 3);
            System.Windows.Forms.ListViewItem listViewItem5 = new System.Windows.Forms.ListViewItem("Gooch bump reflect", 4);
            System.Windows.Forms.ListViewItem listViewItem6 = new System.Windows.Forms.ListViewItem("Lambert", 5);
            System.Windows.Forms.ListViewItem listViewItem7 = new System.Windows.Forms.ListViewItem("Lambert bump reflect", 6);
            System.Windows.Forms.ListViewItem listViewItem8 = new System.Windows.Forms.ListViewItem("Phong", 7);
            System.Windows.Forms.ListViewItem listViewItem9 = new System.Windows.Forms.ListViewItem("Phong bump reflect", 8);
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(EffectWizardDialog));
            this.wizardForm = new CristiPotlog.WizardControl.Wizard();
            this.wizardPageSelect = new CristiPotlog.WizardControl.WizardPage();
            this.groupBoxSelect = new System.Windows.Forms.GroupBox();
            this.labelHlslInfo = new System.Windows.Forms.Label();
            this.labelFileInfo = new System.Windows.Forms.Label();
            this.radioButtonHlsl = new System.Windows.Forms.RadioButton();
            this.radioButtonFile = new System.Windows.Forms.RadioButton();
            this.wizardPageFile = new CristiPotlog.WizardControl.WizardPage();
            this.buttonFile = new System.Windows.Forms.Button();
            this.textBoxFile = new System.Windows.Forms.TextBox();
            this.labelFile = new System.Windows.Forms.Label();
            this.wizardPageTemplate = new CristiPotlog.WizardControl.WizardPage();
            this.buttonLocation = new System.Windows.Forms.Button();
            this.textBoxLocation = new System.Windows.Forms.TextBox();
            this.labelLocation = new System.Windows.Forms.Label();
            this.textBoxName = new System.Windows.Forms.TextBox();
            this.labelName = new System.Windows.Forms.Label();
            this.listViewEffect = new System.Windows.Forms.ListView();
            this.imageListEffect = new System.Windows.Forms.ImageList(this.components);
            this.wizardPageName = new CristiPotlog.WizardControl.WizardPage();
            this.textBoxEffect = new System.Windows.Forms.TextBox();
            this.labelEffect = new System.Windows.Forms.Label();
            this.wizardForm.SuspendLayout();
            this.wizardPageSelect.SuspendLayout();
            this.groupBoxSelect.SuspendLayout();
            this.wizardPageFile.SuspendLayout();
            this.wizardPageTemplate.SuspendLayout();
            this.wizardPageName.SuspendLayout();
            this.SuspendLayout();
            // 
            // wizardForm
            // 
            this.wizardForm.Controls.Add(this.wizardPageName);
            this.wizardForm.Controls.Add(this.wizardPageTemplate);
            this.wizardForm.Controls.Add(this.wizardPageFile);
            this.wizardForm.Controls.Add(this.wizardPageSelect);
            this.wizardForm.HeaderFont = new System.Drawing.Font("Microsoft YaHei UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.wizardForm.HeaderImage = ((System.Drawing.Image)(resources.GetObject("wizardForm.HeaderImage")));
            this.wizardForm.HeaderTitleFont = new System.Drawing.Font("Microsoft YaHei UI", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.wizardForm.Location = new System.Drawing.Point(0, 0);
            this.wizardForm.Name = "wizardForm";
            this.wizardForm.Pages.AddRange(new CristiPotlog.WizardControl.WizardPage[] {
            this.wizardPageSelect,
            this.wizardPageFile,
            this.wizardPageTemplate,
            this.wizardPageName});
            this.wizardForm.Size = new System.Drawing.Size(496, 340);
            this.wizardForm.TabIndex = 0;
            this.wizardForm.BeforeSwitchPages += new CristiPotlog.WizardControl.Wizard.BeforeSwitchPagesEventHandler(this.wizardForm_BeforeSwitchPages);
            this.wizardForm.AfterSwitchPages += new CristiPotlog.WizardControl.Wizard.AfterSwitchPagesEventHandler(this.wizardForm_AfterSwitchPages);
            this.wizardForm.Finish += new System.EventHandler(this.wizardForm_Finish);
            this.wizardForm.Load += new System.EventHandler(this.wizardForm_Load);
            // 
            // wizardPageSelect
            // 
            this.wizardPageSelect.Controls.Add(this.groupBoxSelect);
            this.wizardPageSelect.Description = "Add a new effect to the project";
            this.wizardPageSelect.Dock = System.Windows.Forms.DockStyle.Top;
            this.wizardPageSelect.Font = new System.Drawing.Font("Microsoft YaHei UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.wizardPageSelect.Location = new System.Drawing.Point(0, 0);
            this.wizardPageSelect.Name = "wizardPageSelect";
            this.wizardPageSelect.Size = new System.Drawing.Size(420, 186);
            this.wizardPageSelect.TabIndex = 10;
            this.wizardPageSelect.Title = "Add Effect";
            // 
            // groupBoxSelect
            // 
            this.groupBoxSelect.Controls.Add(this.labelHlslInfo);
            this.groupBoxSelect.Controls.Add(this.labelFileInfo);
            this.groupBoxSelect.Controls.Add(this.radioButtonHlsl);
            this.groupBoxSelect.Controls.Add(this.radioButtonFile);
            this.groupBoxSelect.Location = new System.Drawing.Point(12, 73);
            this.groupBoxSelect.Name = "groupBoxSelect";
            this.groupBoxSelect.Size = new System.Drawing.Size(472, 193);
            this.groupBoxSelect.TabIndex = 1;
            this.groupBoxSelect.TabStop = false;
            this.groupBoxSelect.Text = "Profiles to create";
            // 
            // labelHlslInfo
            // 
            this.labelHlslInfo.AutoSize = true;
            this.labelHlslInfo.ForeColor = System.Drawing.SystemColors.GrayText;
            this.labelHlslInfo.Location = new System.Drawing.Point(23, 87);
            this.labelHlslInfo.MaximumSize = new System.Drawing.Size(430, 0);
            this.labelHlslInfo.Name = "labelHlslInfo";
            this.labelHlslInfo.Size = new System.Drawing.Size(426, 51);
            this.labelHlslInfo.TabIndex = 5;
            this.labelHlslInfo.Text = "High Level Shading Language (HLSL) is a shading language developed by Microsoft f" +
    "or use with the Direct3D API. Use this profile to create an effect that referenc" +
    "e a Microsoft Direct3D 11 FX file.";
            // 
            // labelFileInfo
            // 
            this.labelFileInfo.AutoSize = true;
            this.labelFileInfo.ForeColor = System.Drawing.SystemColors.GrayText;
            this.labelFileInfo.Location = new System.Drawing.Point(23, 44);
            this.labelFileInfo.Name = "labelFileInfo";
            this.labelFileInfo.Size = new System.Drawing.Size(159, 17);
            this.labelFileInfo.TabIndex = 4;
            this.labelFileInfo.Text = "Adds an effect from a file.";
            // 
            // radioButtonHlsl
            // 
            this.radioButtonHlsl.AutoSize = true;
            this.radioButtonHlsl.Font = new System.Drawing.Font("Microsoft YaHei UI", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.radioButtonHlsl.Location = new System.Drawing.Point(6, 65);
            this.radioButtonHlsl.Name = "radioButtonHlsl";
            this.radioButtonHlsl.Size = new System.Drawing.Size(78, 23);
            this.radioButtonHlsl.TabIndex = 2;
            this.radioButtonHlsl.TabStop = true;
            this.radioButtonHlsl.Text = "HLSL FX";
            this.radioButtonHlsl.UseVisualStyleBackColor = true;
            this.radioButtonHlsl.CheckedChanged += new System.EventHandler(this.radioButton_CheckedChanged);
            // 
            // radioButtonFile
            // 
            this.radioButtonFile.AutoSize = true;
            this.radioButtonFile.Font = new System.Drawing.Font("Microsoft YaHei UI", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.radioButtonFile.Location = new System.Drawing.Point(6, 22);
            this.radioButtonFile.Name = "radioButtonFile";
            this.radioButtonFile.Size = new System.Drawing.Size(121, 23);
            this.radioButtonFile.TabIndex = 1;
            this.radioButtonFile.TabStop = true;
            this.radioButtonFile.Text = "Effect from File";
            this.radioButtonFile.UseVisualStyleBackColor = true;
            this.radioButtonFile.CheckedChanged += new System.EventHandler(this.radioButton_CheckedChanged);
            // 
            // wizardPageFile
            // 
            this.wizardPageFile.Controls.Add(this.buttonFile);
            this.wizardPageFile.Controls.Add(this.textBoxFile);
            this.wizardPageFile.Controls.Add(this.labelFile);
            this.wizardPageFile.Description = "Use this page to import an existing effect";
            this.wizardPageFile.Location = new System.Drawing.Point(0, 0);
            this.wizardPageFile.Name = "wizardPageFile";
            this.wizardPageFile.Size = new System.Drawing.Size(420, 186);
            this.wizardPageFile.TabIndex = 13;
            this.wizardPageFile.Title = "New Effect from File";
            // 
            // buttonFile
            // 
            this.buttonFile.Location = new System.Drawing.Point(409, 74);
            this.buttonFile.Name = "buttonFile";
            this.buttonFile.Size = new System.Drawing.Size(75, 23);
            this.buttonFile.TabIndex = 2;
            this.buttonFile.Text = "Browse...";
            this.buttonFile.UseVisualStyleBackColor = true;
            this.buttonFile.Click += new System.EventHandler(this.buttonFile_Click);
            // 
            // textBoxFile
            // 
            this.textBoxFile.Location = new System.Drawing.Point(80, 75);
            this.textBoxFile.Name = "textBoxFile";
            this.textBoxFile.Size = new System.Drawing.Size(322, 21);
            this.textBoxFile.TabIndex = 1;
            this.textBoxFile.TextChanged += new System.EventHandler(this.textBoxFile_TextChanged);
            // 
            // labelFile
            // 
            this.labelFile.AutoSize = true;
            this.labelFile.Font = new System.Drawing.Font("Microsoft YaHei UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.labelFile.Location = new System.Drawing.Point(12, 77);
            this.labelFile.Name = "labelFile";
            this.labelFile.Size = new System.Drawing.Size(66, 17);
            this.labelFile.TabIndex = 0;
            this.labelFile.Text = "Effect File:";
            // 
            // wizardPageTemplate
            // 
            this.wizardPageTemplate.CausesValidation = false;
            this.wizardPageTemplate.Controls.Add(this.buttonLocation);
            this.wizardPageTemplate.Controls.Add(this.textBoxLocation);
            this.wizardPageTemplate.Controls.Add(this.labelLocation);
            this.wizardPageTemplate.Controls.Add(this.textBoxName);
            this.wizardPageTemplate.Controls.Add(this.labelName);
            this.wizardPageTemplate.Controls.Add(this.listViewEffect);
            this.wizardPageTemplate.Description = "The imported effect will be saved with the specified  filename and location";
            this.wizardPageTemplate.Location = new System.Drawing.Point(0, 0);
            this.wizardPageTemplate.Name = "wizardPageTemplate";
            this.wizardPageTemplate.Size = new System.Drawing.Size(420, 186);
            this.wizardPageTemplate.TabIndex = 11;
            this.wizardPageTemplate.Title = "Select a HLSL FX Template";
            // 
            // buttonLocation
            // 
            this.buttonLocation.Location = new System.Drawing.Point(412, 258);
            this.buttonLocation.Name = "buttonLocation";
            this.buttonLocation.Size = new System.Drawing.Size(75, 23);
            this.buttonLocation.TabIndex = 8;
            this.buttonLocation.Text = "Browse...";
            this.buttonLocation.UseVisualStyleBackColor = true;
            this.buttonLocation.Click += new System.EventHandler(this.buttonLocation_Click);
            // 
            // textBoxLocation
            // 
            this.textBoxLocation.Location = new System.Drawing.Point(78, 260);
            this.textBoxLocation.Name = "textBoxLocation";
            this.textBoxLocation.Size = new System.Drawing.Size(328, 21);
            this.textBoxLocation.TabIndex = 7;
            // 
            // labelLocation
            // 
            this.labelLocation.AutoSize = true;
            this.labelLocation.Font = new System.Drawing.Font("Microsoft YaHei UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.labelLocation.Location = new System.Drawing.Point(12, 263);
            this.labelLocation.Name = "labelLocation";
            this.labelLocation.Size = new System.Drawing.Size(60, 17);
            this.labelLocation.TabIndex = 6;
            this.labelLocation.Text = "Location:";
            // 
            // textBoxName
            // 
            this.textBoxName.Location = new System.Drawing.Point(78, 236);
            this.textBoxName.Name = "textBoxName";
            this.textBoxName.Size = new System.Drawing.Size(328, 21);
            this.textBoxName.TabIndex = 4;
            this.textBoxName.TextChanged += new System.EventHandler(this.textBoxName_TextChanged);
            // 
            // labelName
            // 
            this.labelName.AutoSize = true;
            this.labelName.Font = new System.Drawing.Font("Microsoft YaHei UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.labelName.Location = new System.Drawing.Point(12, 239);
            this.labelName.Name = "labelName";
            this.labelName.Size = new System.Drawing.Size(46, 17);
            this.labelName.TabIndex = 3;
            this.labelName.Text = "Name:";
            // 
            // listViewEffect
            // 
            listViewItem1.Tag = "Empty";
            listViewItem2.Tag = "Blinn";
            listViewItem3.Tag = "BlinnBumpReflect";
            listViewItem4.Tag = "Gooch";
            listViewItem5.Tag = "GoochBumpReflect";
            listViewItem6.Tag = "Lambert";
            listViewItem7.Tag = "LambertBumpReflect";
            listViewItem8.Tag = "Phong";
            listViewItem9.Tag = "PhongBumpReflect";
            this.listViewEffect.Items.AddRange(new System.Windows.Forms.ListViewItem[] {
            listViewItem1,
            listViewItem2,
            listViewItem3,
            listViewItem4,
            listViewItem5,
            listViewItem6,
            listViewItem7,
            listViewItem8,
            listViewItem9});
            this.listViewEffect.LargeImageList = this.imageListEffect;
            this.listViewEffect.Location = new System.Drawing.Point(12, 69);
            this.listViewEffect.MultiSelect = false;
            this.listViewEffect.Name = "listViewEffect";
            this.listViewEffect.Size = new System.Drawing.Size(472, 158);
            this.listViewEffect.TabIndex = 0;
            this.listViewEffect.TileSize = new System.Drawing.Size(128, 32);
            this.listViewEffect.UseCompatibleStateImageBehavior = false;
            this.listViewEffect.SelectedIndexChanged += new System.EventHandler(this.listViewEffect_SelectedIndexChanged);
            // 
            // imageListEffect
            // 
            this.imageListEffect.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageListEffect.ImageStream")));
            this.imageListEffect.TransparentColor = System.Drawing.Color.Transparent;
            this.imageListEffect.Images.SetKeyName(0, "00035[32x32x8BPP].png");
            this.imageListEffect.Images.SetKeyName(1, "blinn.png");
            this.imageListEffect.Images.SetKeyName(2, "blinn_bump_reflect.png");
            this.imageListEffect.Images.SetKeyName(3, "gooch.png");
            this.imageListEffect.Images.SetKeyName(4, "gooch_bump_reflect.png");
            this.imageListEffect.Images.SetKeyName(5, "lambert.png");
            this.imageListEffect.Images.SetKeyName(6, "lambert_bump_reflect.png");
            this.imageListEffect.Images.SetKeyName(7, "phong.png");
            this.imageListEffect.Images.SetKeyName(8, "phong_bump_reflect.png");
            // 
            // wizardPageName
            // 
            this.wizardPageName.Controls.Add(this.textBoxEffect);
            this.wizardPageName.Controls.Add(this.labelEffect);
            this.wizardPageName.Description = "Use this page to customize the name of the Effect and to be created when the wiza" +
    "rd is complete";
            this.wizardPageName.Location = new System.Drawing.Point(0, 0);
            this.wizardPageName.Name = "wizardPageName";
            this.wizardPageName.Size = new System.Drawing.Size(496, 292);
            this.wizardPageName.TabIndex = 12;
            this.wizardPageName.Title = "New Effect Name";
            // 
            // textBoxEffect
            // 
            this.textBoxEffect.Location = new System.Drawing.Point(115, 83);
            this.textBoxEffect.Name = "textBoxEffect";
            this.textBoxEffect.Size = new System.Drawing.Size(369, 21);
            this.textBoxEffect.TabIndex = 6;
            this.textBoxEffect.TextChanged += new System.EventHandler(this.textBoxEffect_TextChanged);
            // 
            // labelEffect
            // 
            this.labelEffect.AutoSize = true;
            this.labelEffect.Font = new System.Drawing.Font("Microsoft YaHei UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.labelEffect.Location = new System.Drawing.Point(12, 85);
            this.labelEffect.Name = "labelEffect";
            this.labelEffect.Size = new System.Drawing.Size(82, 17);
            this.labelEffect.TabIndex = 5;
            this.labelEffect.Text = "Effect Name:";
            // 
            // EffectWizardDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(496, 340);
            this.Controls.Add(this.wizardForm);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "EffectWizardDialog";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "EffectWizardDialog";
            this.wizardForm.ResumeLayout(false);
            this.wizardPageSelect.ResumeLayout(false);
            this.groupBoxSelect.ResumeLayout(false);
            this.groupBoxSelect.PerformLayout();
            this.wizardPageFile.ResumeLayout(false);
            this.wizardPageFile.PerformLayout();
            this.wizardPageTemplate.ResumeLayout(false);
            this.wizardPageTemplate.PerformLayout();
            this.wizardPageName.ResumeLayout(false);
            this.wizardPageName.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private CristiPotlog.WizardControl.Wizard wizardForm;
        private CristiPotlog.WizardControl.WizardPage wizardPageSelect;
        private CristiPotlog.WizardControl.WizardPage wizardPageTemplate;
        private CristiPotlog.WizardControl.WizardPage wizardPageName;
        private System.Windows.Forms.GroupBox groupBoxSelect;
        private System.Windows.Forms.RadioButton radioButtonHlsl;
        private System.Windows.Forms.RadioButton radioButtonFile;
        private CristiPotlog.WizardControl.WizardPage wizardPageFile;
        private System.Windows.Forms.Button buttonFile;
        private System.Windows.Forms.TextBox textBoxFile;
        private System.Windows.Forms.Label labelFile;
        private System.Windows.Forms.ListView listViewEffect;
        private System.Windows.Forms.ImageList imageListEffect;
        private System.Windows.Forms.Button buttonLocation;
        private System.Windows.Forms.TextBox textBoxLocation;
        private System.Windows.Forms.Label labelLocation;
        private System.Windows.Forms.TextBox textBoxName;
        private System.Windows.Forms.Label labelName;
        private System.Windows.Forms.TextBox textBoxEffect;
        private System.Windows.Forms.Label labelEffect;
        private System.Windows.Forms.Label labelHlslInfo;
        private System.Windows.Forms.Label labelFileInfo;
    }
}