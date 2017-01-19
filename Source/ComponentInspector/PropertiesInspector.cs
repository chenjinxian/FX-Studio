using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;

namespace Inspector
{

    public partial class PropertiesInspector : UserControl
    {
        private int mFirstColumnWidth = 100;
        private int mItemHeight = 26;
        private BaseItem mItemSelected = null;
        private BaseItem PreviousItemSelected = null;
        private Color mTextForeColor = Color.White;
        private Dictionary<string, CategoryItem> m_CategoryByName = null;
        private bool resizeCursorOnScreen = false;
        private bool controlChangeEventLocked = true;

        public PropertiesInspector()
        {
            m_CategoryByName = new Dictionary<string, CategoryItem>();

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

        public int ColumnWidth
        {
            get
            {
                return mFirstColumnWidth;
            }
            set
            {
                mFirstColumnWidth = value;
                if (mFirstColumnWidth < 40) mFirstColumnWidth = 40;
                if (mFirstColumnWidth > this.Width) mFirstColumnWidth = this.Width;
            }
        }

        public int RowsHeight
        {
            get
            {
                return mItemHeight;
            }
            set
            {
                if (value < 19)
                    mItemHeight = 19;
                else
                    mItemHeight = (value < 64) ? mItemHeight = value :
                                                 mItemHeight = 64;
            }
        }

        public BaseItem SelectedItem
        {
            get
            {
                return mItemSelected;
            }
            set
            {
                mItemSelected = value;
            }
        }

        public int CategoryCount
        {
            get
            {
                return m_CategoryByName.Count;
            }
        }

        private string NowTimeString()
        {
            return System.DateTime.Now.ToLongTimeString() + ":" + DateTime.Now.Millisecond.ToString("000") + ") ";
        }

        private void AddStringItemControl(BaseItem item, Panel panelItem)
        {
            Label labelItem = new Label();
            labelItem.AutoSize = true;
            labelItem.ForeColor = Color.Black;
            labelItem.Location = new Point(24, 3);
            labelItem.Name = "label" + item.Name;
            labelItem.Text = item.Name;

            TextBox textItem = new TextBox();
            textItem.Location = new Point(100, 1);
            textItem.Size = new Size(this.Width - 100 - 2, mItemHeight - 1);
            textItem.Name = "text" + item.Name;
            textItem.Text = item.ValueString;
            textItem.ReadOnly = item.Enabled;

            panelItem.Controls.Add(labelItem);
            panelItem.Controls.Add(textItem);
        }

        private void AddVector3ItemControl(BaseItem item, Panel panelItem)
        {
            Label labelItem = new Label();
            labelItem.AutoSize = true;
            labelItem.ForeColor = Color.Black;
            labelItem.Location = new Point(24, 3);
            labelItem.Name = "label" + item.Name;
            labelItem.Text = item.Name;

            string[] elements = item.ValueString.Split(',');
            if (elements.Length != 3)
                return;

            const int labelWidth = 16;
            int textWidth = (this.Width - 100 - labelWidth * 3 - 2) / 3;
            int x1 = 100;

            Label labelX = new Label();
            labelX.AutoSize = true;
            labelX.ForeColor = Color.Black;
            labelX.Location = new Point(x1, 3);
            labelX.Name = "label" + item.Name + "X";
            labelX.Text = "X";

            x1 += labelWidth;
            TextBox textX = new TextBox();
            textX.Location = new Point(x1, 1);
            textX.Size = new Size(textWidth, mItemHeight - 1);
            textX.Name = "text" + item.Name + "X";
            textX.Text = elements[0];
            textX.ReadOnly = item.Enabled;

            x1 += textWidth;
            Label labelY = new Label();
            labelY.AutoSize = true;
            labelY.ForeColor = Color.Black;
            labelY.Location = new Point(x1 + 1, 3);
            labelY.Name = "label" + item.Name + "Y";
            labelY.Text = "Y";

            x1 += labelWidth;
            TextBox textY = new TextBox();
            textY.Location = new Point(x1, 1);
            textY.Size = new Size(textWidth, mItemHeight - 1);
            textY.Name = "text" + item.Name + "Y";
            textY.Text = elements[1];
            textY.ReadOnly = item.Enabled;

            x1 += textWidth;
            Label labelZ = new Label();
            labelZ.AutoSize = true;
            labelZ.ForeColor = Color.Black;
            labelZ.Location = new Point(x1 + 1, 3);
            labelZ.Name = "label" + item.Name + "Z";
            labelZ.Text = "Z";

            x1 += labelWidth;
            TextBox textZ = new TextBox();
            textZ.Location = new Point(x1, 1);
            textZ.Size = new Size(textWidth, mItemHeight - 1);
            textZ.Name = "text" + item.Name + "Z";
            textZ.Text = elements[2];
            textZ.ReadOnly = item.Enabled;

            panelItem.Controls.Add(labelItem);
            panelItem.Controls.Add(labelX);
            panelItem.Controls.Add(textX);
            panelItem.Controls.Add(labelY);
            panelItem.Controls.Add(textY);
            panelItem.Controls.Add(labelZ);
            panelItem.Controls.Add(textZ);
        }

        private void AddColorItemControl(BaseItem item, Panel panelItem)
        {
            Label labelItem = new Label();
            labelItem.AutoSize = true;
            labelItem.ForeColor = Color.Black;
            labelItem.Location = new Point(24, 3);
            labelItem.Name = "label" + item.Name;
            labelItem.Text = item.Name;

            Panel panelColor = new Panel();
            panelColor.BackColor = /*((ColorItem)item).Value*/Color.White;
            panelColor.BorderStyle = BorderStyle.FixedSingle;
            panelColor.Dock = DockStyle.None;
            panelColor.Location = new Point(100, 2);
            panelColor.Name = "panel" + item.Name;
            panelColor.Size = new Size(20, 20);

            TextBox textColor = new TextBox();
            textColor.Location = new Point(121, 1);
            textColor.Size = new Size(this.Width - 121 - 2, mItemHeight);
            textColor.Name = "text" + item.Name;
            textColor.Text = item.ValueString;
            textColor.ReadOnly = item.Enabled;

            panelItem.Controls.Add(labelItem);
            panelItem.Controls.Add(panelColor);
            panelItem.Controls.Add(textColor);
        }

        private void AddImageItemControl(BaseItem item, Panel panelItem)
        {
            Label labelItem = new Label();
            labelItem.AutoSize = true;
            labelItem.ForeColor = Color.Black;
            labelItem.Location = new Point(24, 3);
            labelItem.Name = "label" + item.Name;
            labelItem.Text = item.Name;

            PictureBox pictureImage = new PictureBox();
            pictureImage.BackColor = Color.White;
            pictureImage.BorderStyle = BorderStyle.FixedSingle;
            pictureImage.Dock = DockStyle.None;
            pictureImage.Location = new Point(100, 2);
            pictureImage.Name = "panel" + item.Name;
            pictureImage.Size = new Size(20, 20);

            TextBox textColor = new TextBox();
            textColor.Location = new Point(121, 1);
            textColor.Size = new Size(this.Width - 121 - 2, mItemHeight - 2);
            textColor.Name = "text" + item.Name;
            textColor.Text = item.ValueString;
            textColor.ReadOnly = item.Enabled;

            panelItem.Controls.Add(labelItem);
            panelItem.Controls.Add(pictureImage);
            panelItem.Controls.Add(textColor);
        }

        private void PaintItem(BaseItem item, int x1, int x2, ref int y1, ref int y2)
        {
            System.Drawing.Font propFont = null;
            if (!item.Visible) return;

            item.Rect = new Rectangle(mFirstColumnWidth, y1, this.Width - mFirstColumnWidth, y2 - y1);

            propFont = item.Changed ? new Font(this.Font, FontStyle.Bold) :
                                      new Font(this.Font, FontStyle.Regular);
            Panel panelItem = new Panel();
            panelItem.BackColor = Color.White;
            panelItem.Dock = DockStyle.None;
            panelItem.Location = new Point(0, y1);
            panelItem.Name = "panel" + item.Name;
            panelItem.Size = new Size(this.Width, mItemHeight);
            this.panelProperties.Controls.Add(panelItem);

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
            y2 += mItemHeight;
        }

        private void RefreshCursor(int xMouse)
        {
            int x1;
            int x2;

            x1 = xMouse - 3;
            x2 = xMouse + 3;
            resizeCursorOnScreen = (mFirstColumnWidth >= x1 && mFirstColumnWidth <= x2);
            if (resizeCursorOnScreen)
            {
                if (this.Cursor != Cursors.VSplit)
                {
                    UpdateControl();
                    this.Cursor = Cursors.VSplit;
                }
            }
            else
                this.Cursor = Cursors.Default;
        }

        private void VerifyMouseUpOnCategory(MouseEventArgs e)
        {
            //             Rectangle rect;
            //             CategoryItem category;
            //             BaseItem item;
            //             int k, j;
            //             int itemWidth;
            // 
            //             for (k = 0; k < m_CategoryByName.Count; k++)
            //             {
            //                 category = m_CategoryByName[k];
            //                 rect = category.Rect;
            //                 // - Category
            //                 if (e.X >= rect.X && e.X <= (rect.X + rect.Width) && e.Y >= rect.Y && e.Y <= (rect.Y + rect.Height))
            //                 {
            //                     // Expand/Collapse category
            //                     category.Expanded = !category.Expanded;
            //                     PaintControl();
            //                     return;     // Exit
            //                 }
            //                 // - Property BaseItem (search for active selected item)
            //                 if (category.Expanded)
            //                     for (j = 0; j < category.ItemList.Count; j++)
            //                     {
            //                         item = category.ItemList[j];
            //                         rect = item.Rect;
            //                         if (e.Y >= rect.Y && e.Y <= (rect.Y + rect.Height))
            //                         {
            //                             PreviousItemSelected = mItemSelected;
            //                             mItemSelected = item;
            //                             RefreshHelp(item);
            //                             PaintControl();
            //                             // Show input box (if BaseItem is enabled)
            //                             if (pnlProp.VerticalScroll.Visible)
            //                                 itemWidth = rect.Width - 20;
            //                             else
            //                                 itemWidth = rect.Width;
            //                             if (itemWidth < 8) itemWidth = 8;
            //                             if (item.Enabled)
            //                                 ShowInputControl(rect.X, rect.Y, itemWidth, mItemHeight, item);
            //                             return;     // Exit
            //                         }
            //                     }
            //             }
        }

        private void VerifyMouseUpOnItemList(MouseEventArgs e)
        {
            //             Rectangle rect;
            //             int j;
            // 
            //             for (j = 0; j < m_PropertyByName.Count; j++)
            //             {
            //                 rect = m_PropertyByName[j].Rect;
            //                 if (e.Y >= rect.Y && e.Y <= (rect.Y + rect.Height))
            //                 {
            //                     PreviousItemSelected = mItemSelected;
            //                     mItemSelected = m_PropertyByName[j];
            //                     RefreshHelp(m_PropertyByName[j]);
            //                     PaintControl();
            //                     // Show input box (if BaseItem is enabled)
            //                     if (m_PropertyByName[j].Enabled)
            //                         ShowInputControl(rect.X, rect.Y, rect.Width, mItemHeight, m_PropertyByName[j]);
            //                     return;     // Exit
            //                 }
            //             }
        }

        private void MoreButtonIsPressed()
        {
            //             DialogResult diagRet;
            //             ColorItem colItem = null;
            //             ImageItem imgItem = null;
            //             FileItem fileItem = null;
            //             StringItem StringItem = null;
            //             string oldValue = "";
            // 
            //             if (!btnMore.Visible) return;
            //             btnMore.Enabled = false;
            //             if (mItemSelected is ColorItem)
            //             {
            //                 colItem = (ColorItem)mItemSelected;
            //                 colDiag.Color = colItem.Value;
            //                 diagRet = colDiag.ShowDialog();
            //                 if (diagRet == DialogResult.OK)
            //                 {
            //                     colItem.Value = colDiag.Color;
            //                     PaintControl();
            //                 }
            //                 btnMore.Enabled = true;
            //                 return;
            //             }
            //             if (mItemSelected is ImageItem)
            //             {
            //                 imgItem = (ImageItem)mItemSelected;
            //                 fileDiag.FileName = imgItem.FileName;
            //                 fileDiag.Filter = "All pictures (*.bmp;*.png;*.jpg;*.tiff;*.gif)|*.bmp;*.png;*.jpg;*.tiff;*.gif|All files (*.*)|*.*";
            //                 fileDiag.FilterIndex = 1;
            //                 diagRet = fileDiag.ShowDialog();
            //                 if (diagRet == DialogResult.OK)
            //                 {
            //                     imgItem.FileName = fileDiag.FileName;
            //                     PaintControl();
            //                 }
            //                 btnMore.Enabled = true;
            //                 return;
            //             }
            //             if (mItemSelected is FileItem)
            //             {
            //                 fileItem = (FileItem)mItemSelected;
            //                 fileDiag.FileName = fileItem.Value;
            //                 fileDiag.Filter = fileItem.Filter;
            //                 fileDiag.FilterIndex = fileItem.FilterIndex;
            //                 diagRet = fileDiag.ShowDialog();
            //                 if (diagRet == DialogResult.OK)
            //                 {
            //                     fileItem.Value = fileDiag.FileName;
            //                     PaintControl();
            //                 }
            //                 btnMore.Enabled = true;
            //                 return;
            //             }
            //             if (mItemSelected is StringItem)
            //             {
            //                 // The more button is visible when the property
            //                 // .ShowExpandButton is true. Use this button to run 
            //                 // a user defined action like to open a dialog box.
            //                 // (The event ExpandButtonPressed will be raised; 
            //                 //  add your action into this event)
            //                 StringItem = (StringItem)mItemSelected;
            //                 oldValue = StringItem.Value;
            //                 RaiseExpandButtonPressed(StringItem);
            //                 btnMore.Enabled = true;
            //                 if (oldValue != StringItem.Value)
            //                     // Execute control refresh if the user chage a property value
            //                     RefreshControl(true);
            //                 return;
            //             }
            //             // Exit
            //             btnMore.Enabled = true;
        }

        private void txtBox_ValidateText(BaseItem item)
        {
            // Validate data!
            //             Int32Item int32Item = null;
            //             FloatItem dblItem = null;
            // 
            //             if (item == null) return;
            //             if (item is Int32Item)
            //             {
            //                 int32Item = (Int32Item)item;
            //                 int32Item.ValueString = txtBox.Text;
            //                 return;
            //             }
            //             if (item is FloatItem)
            //             {
            //                 dblItem = (FloatItem)item;
            //                 dblItem.ValueString = txtBox.Text;
            //                 if (dblItem.ValidationRangeCheck == ValidationRangeCheckType.Manual)
            //                 {
            //                     if (dblItem.Value < dblItem.Minimum || dblItem.Value > dblItem.Maximum)
            //                         System.Media.SystemSounds.Hand.Play();
            //                 }
            //                 return;
            //             }
        }

        public void UpdateControl()
        {
            panelProperties.Controls.Clear();

            int x1 = 0;
            int x2 = this.Width;
            if (x2 < 32) x2 = 32;
            int y1 = 0;
            int y2 = y1 + mItemHeight;

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
                panelCategory.Size = new Size(this.Width, mItemHeight);
                panelCategory.Controls.Add(btnExpand);
                panelCategory.Controls.Add(labelCategory);

                this.panelProperties.Controls.Add(panelCategory);

                category.Value.Rect = new Rectangle(2, y1 + 2, 20, 20);

                y1 = y2;
                y2 += mItemHeight;

                if (category.Value.Expanded)
                {
                    foreach (var item in category.Value.ItemList)
                    {
                        PaintItem(item.Value, x1, x2, ref y1, ref y2);
                    }
                }
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

        public void ItemAdd(string categoryKey, string itemKey, StringItem strItem)
        {
            if (m_CategoryByName.ContainsKey(categoryKey))
            {
                m_CategoryByName[categoryKey].ItemList.Add(itemKey, strItem);
                if (this.SelectedItem == null)
                    this.SelectedItem = strItem;
            }
        }

        public void ItemAdd(string categoryKey, string itemKey, Int32Item intItem)
        {
            if (m_CategoryByName.ContainsKey(categoryKey))
            {
                m_CategoryByName[categoryKey].ItemList.Add(itemKey, intItem);
                if (this.SelectedItem == null)
                    this.SelectedItem = intItem;
            }
        }

        public void ItemAdd(string categoryKey, string itemKey, DropDownItem cmbItem)
        {
            if (m_CategoryByName.ContainsKey(categoryKey))
            {
                m_CategoryByName[categoryKey].ItemList.Add(itemKey, cmbItem);
                if (this.SelectedItem == null)
                    this.SelectedItem = cmbItem;
            }
        }

        public void ItemAdd(string categoryKey, string itemKey, BooleanItem boolItem)
        {
            if (m_CategoryByName.ContainsKey(categoryKey))
            {
                m_CategoryByName[categoryKey].ItemList.Add(itemKey, boolItem);
                if (this.SelectedItem == null)
                    this.SelectedItem = boolItem;
            }
        }

        public void ItemAdd(string categoryKey, string itemKey, FloatItem dblItem)
        {
            if (m_CategoryByName.ContainsKey(categoryKey))
            {
                m_CategoryByName[categoryKey].ItemList.Add(itemKey, dblItem);
                if (this.SelectedItem == null)
                    this.SelectedItem = dblItem;
            }
        }

        public void ItemAdd(string categoryKey, string itemKey, Vector3Item vector3Item)
        {
            if (m_CategoryByName.ContainsKey(categoryKey))
            {
                m_CategoryByName[categoryKey].ItemList.Add(itemKey, vector3Item);
                if (this.SelectedItem == null)
                    this.SelectedItem = vector3Item;
            }
        }

        public void ItemAdd(string categoryKey, string itemKey, ColorItem colItem)
        {
            if (m_CategoryByName.ContainsKey(categoryKey))
            {
                m_CategoryByName[categoryKey].ItemList.Add(itemKey, colItem);
                if (this.SelectedItem == null)
                    this.SelectedItem = colItem;
            }
        }

        public void ItemAdd(string categoryKey, string itemKey, ImageItem imgItem)
        {
            if (m_CategoryByName.ContainsKey(categoryKey))
            {
                m_CategoryByName[categoryKey].ItemList.Add(itemKey, imgItem);
                if (this.SelectedItem == null)
                    this.SelectedItem = imgItem;
            }
        }

        public void ItemAdd(string categoryKey, string itemKey, FileItem fileItem)
        {
            if (m_CategoryByName.ContainsKey(categoryKey))
            {
                m_CategoryByName[categoryKey].ItemList.Add(itemKey, fileItem);
                if (this.SelectedItem == null)
                    this.SelectedItem = fileItem;
            }
        }

        public void CategoryAdd(string categoryKey, CategoryItem catItem)
        {
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
                m_CategoryByName[key].Name = text;
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

        private void txtBox_TextChanged(object sender, EventArgs e)
        {
            StringItem strItem = null;
            //Int32Item int32Item = null;
            //FloatItem dblItem = null;

            if (controlChangeEventLocked) return;
            if (mItemSelected == null) return;
            if (mItemSelected is StringItem)
            {
                strItem = (StringItem)mItemSelected;
                //                 strItem.Value = txtBox.Text;
                return;
            }
            /*
			if (mItemSelected is Int32Item)
			{
				int32Item = (Int32Item)mItemSelected;
				int32Item.ValueString = txtBox.Text;
				return;
			}
			if (mItemSelected is FloatItem)
			{
				dblItem = (FloatItem)mItemSelected;
				dblItem.ValueString = txtBox.Text;
				return;
			}
			*/
        }

        private void cmbBox_TextChanged(object sender, EventArgs e)
        {
            DropDownItem listItem = null;

            if (controlChangeEventLocked) return;
            if (mItemSelected == null) return;
            if (mItemSelected is DropDownItem)
            {
                listItem = (DropDownItem)mItemSelected;
                //                 listItem.Value = cmbBox.Text;
            }
        }

        private void cmbBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            DropDownItem listItem = null;

            if (controlChangeEventLocked) return;
            if (mItemSelected == null) return;
            if (mItemSelected is DropDownItem)
            {
                listItem = (DropDownItem)mItemSelected;
                //                 listItem.Value = cmbBox.Text;
            }
        }

        private void btnMore_Click(object sender, EventArgs e)
        {
            MoreButtonIsPressed();
        }

        private void txtBox_KeyPress(object sender, KeyPressEventArgs e)
        {
            //             if (e.KeyChar == (Char)13)
            //             {
            //                 txtBox_ValidateText(mItemSelected);
            //                 HideInputControls(true);
            //             }
        }

        private void cmbBox_KeyPress(object sender, KeyPressEventArgs e)
        {
            //             if (e.KeyChar == (Char)13)
            //                 HideInputControls();
        }

        private void picProp_MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left && !resizeCursorOnScreen)
            {
                VerifyMouseUpOnCategory(e);
            }
            else
            {
                controlChangeEventLocked = true;
            }
        }

        private void picProp_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left && resizeCursorOnScreen)
            {
                if (mFirstColumnWidth != e.X)
                {
                    mFirstColumnWidth = e.X;
                    UpdateControl();
                }
            }
            else
                RefreshCursor(e.X);
        }

        private void txtBox_DoubleClick(object sender, EventArgs e)
        {
            MoreButtonIsPressed();
        }

        private void picProp_DoubleClick(object sender, EventArgs e)
        {
            MoreButtonIsPressed();
        }

        private void txtBox_Leave(object sender, EventArgs e)
        {
            txtBox_ValidateText(PreviousItemSelected);
        }

        private void btnApply_Click(object sender, EventArgs e)
        {
            RaiseApplyButtonPressed();
        }

        private void PropertiesInspector_SizeChanged(object sender, EventArgs e)
        {
            UpdateControl();
        }
    }
}
