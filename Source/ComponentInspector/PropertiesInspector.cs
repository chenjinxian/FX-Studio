using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;
using System.Linq;

namespace Inspector
{
    public partial class PropertiesInspector : UserControl
    {
        private readonly int m_NameLocationX = 24;
        private readonly int m_ItemHeight = 26;
        private readonly int m_SideLength = 20;
        private readonly int m_ButtonWidth = 22;
        private int m_ValueLocationX = 120;
        private Color mTextForeColor = Color.White;
        private Dictionary<string, CategoryItem> m_CategoryByName = null;

        private bool m_LButtonDown;
        private Point m_LastMousePos;

        public PropertiesInspector()
        {
            m_CategoryByName = new Dictionary<string, CategoryItem>();

            base.SetStyle(ControlStyles.AllPaintingInWmPaint, true);
            base.SetStyle(ControlStyles.DoubleBuffer, true);
            base.SetStyle(ControlStyles.ResizeRedraw, true);
            base.SetStyle(ControlStyles.UserPaint, true);
            InitializeComponent();
        }

        ~PropertiesInspector()
        {
            ItemsClear();
            m_CategoryByName = null;
        }

        [DesignerSerializationVisibility(DesignerSerializationVisibility.Visible),
        Browsable(true), EditorBrowsable(EditorBrowsableState.Always),
        Bindable(false)]
        public override string Text
        {
            get
            {
                return base.Text;
            }
            set
            {
                base.Text = value;
            }
        }

        public int CategoryCount
        {
            get
            {
                return m_CategoryByName.Count;
            }
        }

        private void ResetControl()
        {
            panelProperties.Controls.Clear();

            int x1 = 0;
            int x2 = this.Width;
            if (x2 < 32) x2 = 32;
            int y1 = 0;
            int y2 = y1 + m_ItemHeight;

            foreach (var category in m_CategoryByName)
            {
                Button btnExpand = new Button();
                btnExpand.Image = this.imageListExpand.Images[0];
                btnExpand.ImageAlign = ContentAlignment.MiddleCenter;
                btnExpand.Name = "button" + category.Key + "Expand";
                btnExpand.Text = "";
                btnExpand.Location = new Point(2, 2);
                btnExpand.Size = new Size(20, 20);
                if (category.Value.Expanded)
                    btnExpand.Image = this.imageListExpand.Images[1];
                else
                    btnExpand.Image = this.imageListExpand.Images[0];

                Label labelCategory = new Label();
                labelCategory.AutoSize = true;
                labelCategory.ForeColor = Color.Black;
                labelCategory.Location = new Point(24, 3);
                labelCategory.Name = "label" + category.Key;
                labelCategory.AutoSize = true;
                labelCategory.Text = category.Key;

                Panel panelCategory = new Panel();
                panelCategory.BackColor = Color.FromArgb(240, 240, 240);
                panelCategory.Dock = DockStyle.None;
                panelCategory.Location = new Point(0, y1);
                panelCategory.Name = "panel" + category.Key;
                panelCategory.Size = new Size(this.Width, m_ItemHeight);
                panelCategory.Controls.Add(btnExpand);
                panelCategory.Controls.Add(labelCategory);

                this.panelProperties.Controls.Add(panelCategory);

                y1 = y2;
                y2 += m_ItemHeight;

                if (category.Value.Expanded)
                {
                    foreach (var item in category.Value.ItemList)
                    {
                        PaintItem(item.Value, x1, x2, ref y1, ref y2);
                    }
                }
            }
        }

        private void UpdateValue()
        {
            foreach (var category in m_CategoryByName)
            {
                foreach (var item in category.Value.ItemList)
                {
                    BaseItem child = item.Value;
                    if (child is Vector3Item)
                    {
                        ControlCollection controls = panelProperties.Controls[child.CategoryName + "_" + child.ItemName + "_" + "panel"].Controls;
                        string[] elements = child.ValueString.Split(',');
                        if (elements.Length != 3)
                            return;
                        for (int i = 0; i < 3; i++)
                        {
                            string name = i == 0 ? "X" : (i == 1 ? "Y" : "Z");
                            string controlName = child.CategoryName + "_" + child.ItemName + "_" + "labelValue_" + name;
                            Label labelValue = controls[controlName] as Label;
                            if (labelValue != null)
                                labelValue.Text = elements[i];
                        }
                    }
                    else if (child is ColorItem)
                    {
                        ControlCollection controls = panelProperties.Controls[child.CategoryName + "_" + child.ItemName + "_" + "panel"].Controls;

                        string controlName = child.CategoryName + "_" + child.ItemName + "_" + "panelColor";
                        Panel panelColor = controls[controlName] as Panel;
                        if (panelColor != null)
                            panelColor.BackColor = ((ColorItem)child).Value;

                        controlName = child.CategoryName + "_" + child.ItemName + "_" + "textColor";
                        TextBox textColor = controls[controlName] as TextBox;
                        if (textColor != null)
                            textColor.Text = child.ValueString;
                    }
                }
            }
        }

        public void UpdateControl(bool reset = true)
        {
            if (reset)
            {
                ResetControl();
            }
            else
            {
                UpdateValue();
            }
        }

        public void ItemsClear()
        {
            if (m_CategoryByName != null)
            {
                foreach (var category in m_CategoryByName)
                {
                    if (category.Value.ItemList != null)
                    {
                        category.Value.ItemList.Clear();
                        category.Value.ItemList = null;
                    }
                }
                m_CategoryByName.Clear();
            }

            UpdateControl();
        }

        public void ItemAdd(BaseItem baseItem)
        {
            if (m_CategoryByName.ContainsKey(baseItem.CategoryName))
            {
                if (m_CategoryByName[baseItem.CategoryName].ItemList.ContainsKey(baseItem.ItemName))
                    m_CategoryByName[baseItem.CategoryName].ItemList[baseItem.ItemName] = baseItem;
                else
                    m_CategoryByName[baseItem.CategoryName].ItemList.Add(baseItem.ItemName, baseItem);
            }
        }

        public void CategoryAdd(string categoryKey, CategoryItem catItem)
        {
            if (m_CategoryByName.ContainsKey(categoryKey))
                m_CategoryByName[categoryKey] = catItem;
            else
                m_CategoryByName.Add(categoryKey, catItem);
        }

        public CategoryItem GetCategory(string key)
        {
            if (m_CategoryByName.ContainsKey(key))
                return m_CategoryByName[key];
            else
                return null;
        }

        public void SetCategoryText(string key, string text)
        {
            if (m_CategoryByName.ContainsKey(key))
                m_CategoryByName[key].CategoryName = text;
        }

        public delegate void ExpandButtonPressedHandle(object sender, StringItem item);

        public event ExpandButtonPressedHandle ExpandButtonPressed;

        private void RaiseExpandButtonPressed(StringItem item)
        {
            if (ExpandButtonPressed != null)
                ExpandButtonPressed(this, item);
        }

        public delegate void ApplyButtonPressedHandle(object sender, ref bool cancel);

        public event ApplyButtonPressedHandle ApplyButtonPressed;

        private void AddStringItemControl(BaseItem item, Panel panelItem)
        {
            Label labelItem = new Label();
            labelItem.AutoSize = true;
            labelItem.ForeColor = Color.Black;
            labelItem.Location = new Point(m_NameLocationX, 3);
            labelItem.MaximumSize = new Size(m_ValueLocationX - m_NameLocationX, 20);
            labelItem.Name = item.CategoryName + "_" + item.ItemName + "_" + "label";
            labelItem.Text = item.ItemName;

            TextBox textItem = new TextBox();
            textItem.Location = new Point(m_ValueLocationX, 1);
            textItem.Size = new Size(this.Width - m_ValueLocationX - 2, m_ItemHeight - 1);
            textItem.Name = item.CategoryName + "_" + item.ItemName + "_" + "text";
            textItem.Text = item.ValueString;
            textItem.Enabled = item.Enabled;

            panelItem.Controls.Add(labelItem);
            panelItem.Controls.Add(textItem);
        }

        private void AddVector3ItemControl(BaseItem item, Panel panelItem)
        {
            Label labelItem = new Label();
            labelItem.AutoSize = true;
            labelItem.ForeColor = Color.Black;
            labelItem.Location = new Point(m_NameLocationX, 3);
            labelItem.MaximumSize = new Size(m_ValueLocationX - m_NameLocationX, 20);
            labelItem.Name = item.CategoryName + "_" + item.ItemName + "_" + "label";
            labelItem.Text = item.ItemName;

            panelItem.Controls.Add(labelItem);

            string[] elements = item.ValueString.Split(',');
            if (elements.Length != 3)
                return;

            const int labelWidth = 16;
            int textWidth = (this.Width - m_ValueLocationX - labelWidth * 3 - 2) / 3;
            int x1 = m_ValueLocationX;

            for (int i = 0; i < 3; i++)
            {
                string name = i == 0 ? "X" : (i == 1 ? "Y" : "Z");

                Label labelName = new Label();
                labelName.AutoSize = true;
                labelName.ForeColor = Color.Black;
                labelName.Location = new Point(x1, 3);
                labelName.Name = item.CategoryName + "_" + item.ItemName + "_" + "labelName_" + name;
                labelName.Text = name;

                x1 += labelWidth;
                Label labelValue = new Label();
                labelValue.BackColor = Color.CornflowerBlue;
                labelValue.BorderStyle = BorderStyle.FixedSingle;
                labelValue.Enabled = item.Enabled;
                labelValue.ForeColor = Color.White;
                labelValue.Location = new Point(x1, 1);
                labelValue.Name = item.CategoryName + "_" + item.ItemName + "_" + "labelValue_" + name;
                labelValue.Size = new Size(textWidth, m_ItemHeight - 4);
                labelValue.Tag = item;
                labelValue.Text = elements[i];
                labelValue.TextAlign = ContentAlignment.MiddleCenter;
                labelValue.MouseDown += LabelValue_MouseDown;
                labelValue.MouseUp += LabelValue_MouseUp;
                labelValue.MouseMove += LabelValue_MouseMove;

                panelItem.Controls.Add(labelName);
                panelItem.Controls.Add(labelValue);
                x1 += textWidth;
            }
        }

        private void LabelValue_MouseDown(object sender, MouseEventArgs e)
        {
            Label label = sender as Label;
            if (label != null && e.Button == MouseButtons.Left)
            {
                this.Cursor = Cursors.Cross;
                label.BackColor = Color.DarkBlue;
                m_LastMousePos = e.Location;
                m_LButtonDown = true;
            }
        }

        private void LabelValue_MouseUp(object sender, MouseEventArgs e)
        {
            Label label = sender as Label;
            if (label != null && e.Button == MouseButtons.Left)
            {
                this.Cursor = Cursors.Default;
                label.BackColor = Color.CornflowerBlue;
                m_LButtonDown = false;
            }
        }

        private void LabelValue_MouseMove(object sender, MouseEventArgs e)
        {
            Label label = sender as Label;
            if (m_LButtonDown && label != null && label.Tag != null)
            {
                if (m_LastMousePos != e.Location)
                {
                    string[] names = label.Name.Split('_');
                    if (names.Count() < 4)
                        return;

                    float delta = (float)(e.Location.X - m_LastMousePos.X);
                    if (names[1] == "Translation")
                        delta *= 0.01F;
                    else if (names[1] == "Scale")
                        delta *= 0.1F;
                    else if (names[1] == "Rotation")
                        delta *= 0.25F;

                    Single value = Single.Parse(label.Text) + delta;
                    Vector3Item item = label.Tag as Vector3Item;
                    switch (label.Name.Last())
                    {
                        case 'X': item.Value.X = value; break;
                        case 'Y': item.Value.Y = value; break;
                        case 'Z': item.Value.Z = value; break;
                        default: break;
                    }
                    item.Value = item.Value;
                    label.Text = value.ToString("F");

                    m_LastMousePos = e.Location;
                }
            }
        }

        private void AddColorItemControl(BaseItem item, Panel panelItem)
        {
            Label labelItem = new Label();
            labelItem.AutoSize = true;
            labelItem.ForeColor = Color.Black;
            labelItem.Location = new Point(m_NameLocationX, 3);
            labelItem.MaximumSize = new Size(m_ValueLocationX - m_NameLocationX, 20);
            labelItem.Name = item.CategoryName + "_" + item.ItemName + "_" + "label";
            labelItem.Text = item.ItemName;

            Panel panelColor = new Panel();
            panelColor.BackColor = ((ColorItem)item).Value;
            panelColor.BorderStyle = BorderStyle.FixedSingle;
            panelColor.Dock = DockStyle.None;
            panelColor.Location = new Point(m_ValueLocationX, 2);
            panelColor.Name = item.CategoryName + "_" + item.ItemName + "_" + "panelColor";
            panelColor.Size = new Size(m_SideLength, m_SideLength);

            TextBox textColor = new TextBox();
            textColor.Location = new Point(m_ValueLocationX + m_SideLength + 1, 1);
            textColor.Size = new Size(this.Width - m_ValueLocationX - m_SideLength - m_ButtonWidth - 2 - 2, m_ItemHeight);
            textColor.Name = item.CategoryName + "_" + item.ItemName + "_" + "textColor";
            textColor.Text = item.ValueString;

            Button btnColor = new Button();
            btnColor.Location = new Point(this.Width - m_ButtonWidth - 2, 1);
            btnColor.Size = new Size(m_ButtonWidth, m_ButtonWidth);
            btnColor.Name = item.CategoryName + "_" + item.ItemName + "_" + "button";
            btnColor.Text = "..";
            btnColor.UseVisualStyleBackColor = true;
            btnColor.Tag = item;
            btnColor.Click += BtnColor_Click;

            panelItem.Controls.Add(labelItem);
            panelItem.Controls.Add(panelColor);
            panelItem.Controls.Add(textColor);
            panelItem.Controls.Add(btnColor);
        }

        private void BtnColor_Click(object sender, EventArgs e)
        {
            ColorItem item = ((Button)sender).Tag as ColorItem;
            if (item == null)
                return;

            ColorDialog dialog = new ColorDialog();
            dialog.Color = item.Value;
            dialog.FullOpen = true;
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                item.Value = dialog.Color;
                UpdateValue();
            }
        }

        private void AddImageItemControl(BaseItem item, Panel panelItem)
        {
            Label labelItem = new Label();
            labelItem.AutoSize = true;
            labelItem.ForeColor = Color.Black;
            labelItem.Location = new Point(m_NameLocationX, 3);
            labelItem.MaximumSize = new Size(m_ValueLocationX - m_NameLocationX, 20);
            labelItem.Name = item.CategoryName + "_" + item.ItemName + "_" + "label";
            labelItem.Text = item.ItemName;

            PictureBox pictureImage = new PictureBox();
            pictureImage.BackColor = Color.White;
            pictureImage.BorderStyle = BorderStyle.FixedSingle;
            pictureImage.Dock = DockStyle.None;
            pictureImage.Location = new Point(m_ValueLocationX, 2);
            pictureImage.Name = item.CategoryName + "_" + item.ItemName + "_" + "panel";
            pictureImage.Size = new Size(m_SideLength, m_SideLength);

            TextBox textColor = new TextBox();
            textColor.Location = new Point(m_ValueLocationX + m_SideLength + 1, 1);
            textColor.Size = new Size(this.Width - m_ValueLocationX - m_SideLength - m_ButtonWidth - 2 - 2, m_ItemHeight);
            textColor.Name = item.CategoryName + "_" + item.ItemName + "_" + "text";
            textColor.Text = item.ValueString;

            Button btnImage = new Button();
            btnImage.Location = new Point(this.Width - m_ButtonWidth - 2, 1);
            btnImage.Size = new Size(m_ButtonWidth, m_ButtonWidth);
            btnImage.Name = item.CategoryName + "_" + item.ItemName + "_" + "button";
            btnImage.Text = "..";
            btnImage.UseVisualStyleBackColor = true;

            panelItem.Controls.Add(labelItem);
            panelItem.Controls.Add(pictureImage);
            panelItem.Controls.Add(textColor);
            panelItem.Controls.Add(btnImage);
        }

        private void AddFloatItemControl(BaseItem item, Panel panelItem)
        {
            Label labelItem = new Label();
            labelItem.AutoSize = true;
            labelItem.ForeColor = Color.Black;
            labelItem.Location = new Point(m_NameLocationX, 3);
            labelItem.MaximumSize = new Size(m_ValueLocationX - m_NameLocationX, 20);
            labelItem.Name = item.CategoryName + "_" + item.ItemName + "_" + "label";
            labelItem.Text = item.ItemName;

            TextBox textItem = new TextBox();
            textItem.Location = new Point(m_ValueLocationX, 1);
            textItem.Size = new Size(this.Width - m_ValueLocationX - 2, m_ItemHeight - 1);
            textItem.Name = item.CategoryName + "_" + item.ItemName + "_" + "text";
            textItem.Text = item.ValueString;

            panelItem.Controls.Add(labelItem);
            panelItem.Controls.Add(textItem);
        }

        private void PaintItem(BaseItem item, int x1, int x2, ref int y1, ref int y2)
        {
            System.Drawing.Font propFont = null;
            if (!item.Visible) return;

            propFont = item.Changed ? new Font(this.Font, FontStyle.Bold) :
                                      new Font(this.Font, FontStyle.Regular);
            Panel panelItem = new Panel();
            panelItem.BackColor = Color.White;
            panelItem.Dock = DockStyle.None;
            panelItem.Location = new Point(0, y1);
            panelItem.Name = item.CategoryName + "_" + item.ItemName + "_" + "panel";
            panelItem.Size = new Size(this.Width, m_ItemHeight);
            this.panelProperties.Controls.Add(panelItem);

            if (item is FloatItem)
            {
                AddFloatItemControl(item, panelItem);
            }
            if (item is StringItem)
            {
                AddStringItemControl(item, panelItem);
            }
            else if (item is Vector3Item)
            {
                AddVector3ItemControl(item, panelItem);
            }
            else if (item is ColorItem)
            {
                AddColorItemControl(item, panelItem);
            }
            else if (item is ImageItem)
            {
                AddImageItemControl(item, panelItem);
            }

            y1 = y2;
            y2 += m_ItemHeight;
        }

        private void RaiseApplyButtonPressed()
        {
            bool cancel = false;

            if (ApplyButtonPressed != null)
            {
                // Raise event
                ApplyButtonPressed(this, ref cancel);
                //                 if (!cancel)
                //                     ResetChanges();
            }
        }

        private void PropertiesInspector_SizeChanged(object sender, EventArgs e)
        {
            m_ValueLocationX = (int)(this.Width * 0.4);
            UpdateControl();
        }
    }
}
