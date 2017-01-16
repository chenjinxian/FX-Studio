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

		#region Private internal var./properties

		// Properties
		private int mFirstColumnWidth = 100;                // (Pixel)
		private int mItemHeight = 19;                       // (Pixel)
		private BaseItem mItemSelected = null;
		private BaseItem PreviousItemSelected = null;           // Used for validating data in the controls leave event (User Interface).
		private bool mShowItemsAsCategory = true;
		private bool mHelpVisible = true;
		private bool mToolbarVisible = true;
		private bool mTextVisible = false;
		private bool mButtonShowTextVisible = false;
		private bool mButtonApplyVisible = false;
		private Color mTextForeColor = Color.White;
		private bool mTextBackgroundColorIsGradient = true;
		private Color mTextBackgroundGradientColorStart = Color.LightBlue;
		private Color mTextBackgroundGradientColorFinish = Color.Blue;
		private Color mTextBackgroundColor = Color.LightBlue;
		private bool mTraceLogEnabled = true;
		// This collections contains the same values (but with different order)
		// Use public methods to access values into collections
		private CategoryCollection CategoryList = null;     // Order by category
		private ItemCollection ItemList = null;             // Order by item
															// Internal var.
		private bool resizeCursorOnScreen = false;
		private System.Drawing.Bitmap bmpPropGrid = null;   // Ultima immagine della property grid rigenerata dal metodo PaintControl()
		private bool controlChangeEventLocked = true;
		private bool toolbarLocked = false;
		private bool traceLogOpen = false;
		private bool printDemoLabel = false;

		#endregion

		#region Control constructor/destructor

		/// <summary>
		/// Control constructor (initialize properties and objects).
		/// </summary>
		public PropertiesInspector()
		{
			CategoryList = new CategoryCollection();
			ItemList = new ItemCollection();

			InitializeComponent();
			// Start trace log (debug utility)
			TraceLogStart();
			TraceWriteLine("PropertiesInspector() Constructor");
			// TODO:Add any licence control here 
			// (only for commercial product version)
			//TraceWriteLine("License manager started");
			// Initialize var./properties
			txtBox.BorderStyle = BorderStyle.None;
			lblHelpCaption.Text = "";
			lblHelpText.Text = "";
			// Adjust controls size
			ResizePropertyPanel();
		}

		/// <summary>
		/// Control destructor (clear memory).
		/// </summary>
		~PropertiesInspector()
		{
			ItemsClear();
			CategoryList = null;
			ItemList = null;
			if (bmpPropGrid != null) bmpPropGrid.Dispose();
			TraceWriteLine("PropertiesInspector() Destructor");
			TraceLogStop();
		}

		#endregion

		#region Public properties

		/// <summary>
		/// Gets an BaseItem stored into the property grid. Return null if the BaseItem is not found.
		/// This property is equivalent to the method GetItem().
		/// </summary>
		public BaseItem this[string itemKey]
		{
			get
			{
				if (ItemList.Contains(itemKey))
					return ItemList[itemKey];
				else
					return null;
			}
		}

		/// <summary>
		/// Gets/sets the control text label
		/// </summary>
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
				PaintText(value);
			}
		}

		/// <summary>
		/// Gets/sets the property grid toolbar visible status.
		/// </summary>
		public bool ToolbarVisible
		{
			get
			{
				return mToolbarVisible;
			}
			set
			{
				mToolbarVisible = value;
				topBar.Visible = mToolbarVisible;
				SetPnlPropTopPosition();
			}
		}

		/// <summary>
		/// Gets/sets the property grid text visible status.
		/// </summary>
		public bool TextVisible
		{
			get
			{
				return mTextVisible;
			}
			set
			{
				mTextVisible = value;
				pnlText.Visible = mTextVisible;
				SetPnlPropTopPosition();
				// Change button status on the toolbar (this properties could be 
				// set also by program code, not only by user action on the 
				// toolbar)
				toolbarLocked = true;
				btnShowText.CheckState = mTextVisible ? CheckState.Checked : CheckState.Unchecked;
				toolbarLocked = false;
				ResizePropertyPanel();
			}
		}

		/// <summary>
		/// Gets/sets the if user button on the toolbar that show/hide the text caption is visible or not.
		/// </summary>
		public bool ButtonShowTextVisible
		{
			get
			{
				return mButtonShowTextVisible;
			}
			set
			{
				mButtonShowTextVisible = value;
				btnShowText.Visible = mButtonShowTextVisible;
			}
		}

		/// <summary>
		/// Gets/sets the if apply button on the toolbar is visible or not.
		/// </summary>
		public bool ButtonApplyVisible
		{
			get
			{
				return mButtonApplyVisible;
			}
			set
			{

				mButtonApplyVisible = value;
				btnApply.Visible = mButtonApplyVisible;
			}
		}

		/// <summary>
		/// Gets/sets the property grid help panel visible status.
		/// </summary>
		public bool HelpVisible
		{
			get
			{
				return mHelpVisible;
			}
			set
			{
				mHelpVisible = value;
				pnlHelp.Visible = mHelpVisible;
				splitBottom.Visible = mHelpVisible;
				// Change button status on the toolbar (this properties could be 
				// set also by program code, not only by user action on the 
				// toolbar)
				toolbarLocked = true;
				btnShowHelp.CheckState = mHelpVisible ? CheckState.Checked : CheckState.Unchecked;
				toolbarLocked = false;
				ResizePropertyPanel();
			}
		}

		/// <summary>
		/// Gets/sets the grid first column width (pixel). Minimum valid value is 40 pixels.
		/// </summary>
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

		/// <summary>
		/// Gets/sets the grid rows height (pixel). Valid range is : 19..64 pixels.
		/// </summary>
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

		/// <summary>
		/// Gets or sets the current selected item. Return null if no BaseItem is selected.
		/// </summary>
		public BaseItem SelectedItem
		{
			get
			{
				return mItemSelected;
			}
			set
			{
				mItemSelected = value;
				RefreshHelp(mItemSelected);
			}
		}

		/// <summary>
		/// Gets/sets how to show items into the grid. true=show as category, false=show as a list sorted alphabetically
		/// </summary>
		public bool ShowItemsAsCategory
		{
			get
			{
				return mShowItemsAsCategory;
			}
			set
			{
				mShowItemsAsCategory = value;
				RefreshControl(true);
				// Change button status on the toolbar (this properties could be set by
				// program code and not by user action on the toolbar)
				if (mShowItemsAsCategory)
				{
					btnCategorized.CheckState = CheckState.Checked;
					btnAlphabetical.CheckState = CheckState.Unchecked;
				}
				else
				{
					btnCategorized.CheckState = CheckState.Unchecked;
					btnAlphabetical.CheckState = CheckState.Checked;
				}
			}
		}

		/// <summary>
		/// Gets/sets the trace log status (true=enabled, for control/program debugging).
		/// </summary>
		public bool TraceLogEnabled
		{
			get
			{
				return mTraceLogEnabled;
			}
			set
			{
				mTraceLogEnabled = value;
				if (mTraceLogEnabled)
					TraceLogStart();
				else
					TraceLogStop();
			}
		}

		/// <summary>
		/// Gets/sets the text background style (true=gradient fill, false=solid fill)
		/// </summary>
		public bool TextBackgroundColorIsGradient
		{
			get
			{
				return mTextBackgroundColorIsGradient;
			}
			set
			{
				mTextBackgroundColorIsGradient = value;
			}
		}

		/// <summary>
		/// Gets/sets the text background color (initial color) while using the gradient fill style.
		/// </summary>
		public Color TextBackgroundGradientColorStart
		{
			get
			{
				return mTextBackgroundGradientColorStart;
			}
			set
			{
				mTextBackgroundGradientColorStart = value;
			}
		}

		/// <summary>
		/// Gets/sets the text background color (final color) while using the gradient fill style.
		/// </summary>
		public Color TextBackgroundGradientColorFinish
		{
			get
			{
				return mTextBackgroundGradientColorFinish;
			}
			set
			{
				mTextBackgroundGradientColorFinish = value;
			}
		}

		/// <summary>
		/// Gets/sets the text background color while using the solid fill style.
		/// </summary>
		public Color TextBackgroundColor
		{
			get
			{
				return mTextBackgroundColor;
			}
			set
			{
				mTextBackgroundColor = value;
			}
		}

		/// <summary>
		/// Gets the items count (while order by items)
		/// </summary>
		public int ItemsCount
		{
			get
			{
				return ItemList.Count;
			}
		}

		/// <summary>
		/// Return true if a specific BaseItem exist into the property grid.
		/// </summary>
		/// <param name="itemKey">Unique key for a fast BaseItem reference.</param>
		/// <returns>true=Item exist  false=the property grid do not contains the item.</returns>
		public bool ItemExist(string itemKey)
		{
			// Add BaseItem to category list and BaseItem list
			return ItemList.Contains(itemKey);
		}


		/// <summary>
		/// Gets the category count (while order by category)
		/// </summary>
		public int CategoryCount
		{
			get
			{
				return CategoryList.Count;
			}
		}

		#endregion

		#region Private internal functions

		/// <summary>
		/// Set the panel (with contain the property grid) top position.
		/// The top position depend from the visibile status of the topBar and pnlText control. 
		/// </summary>
		private void SetPnlPropTopPosition()
		{
			int pnlPropTopPosition = 0;

			if (topBar.Visible)
				pnlPropTopPosition += topBar.Height;
			if (pnlText.Visible)
				pnlPropTopPosition += pnlText.Height;
			pnlProp.Top = pnlPropTopPosition;
		}

		/// <summary>
		/// Return the current full time as string (hh:mm:ss:xxx) xxx=milli-seconds
		/// </summary>
		private string NowTimeString()
		{
			return System.DateTime.Now.ToLongTimeString() + ":" + DateTime.Now.Millisecond.ToString("000") + ") ";
		}

		/// <summary>
		/// Write a trace debug line string. Return true if the line is written successfully. 
		/// </summary>
		private bool TraceWriteLine(string strLine)
		{
			if (!(mTraceLogEnabled && traceLogOpen)) return false;
			try
			{
				//Trace.WriteLine(NowTimeString() + strLine);
				Trace.WriteLine(strLine);
				return true;
			}
			catch
			{
				mTraceLogEnabled = false;
				return false;
			}
		}

		/// <summary>
		/// Close the log file.
		/// </summary>
		private void TraceLogStop()
		{
			try
			{
				if (traceLogOpen && Trace.Listeners.Count > 0)
				{
					TraceWriteLine("Trace log : stop");
					Trace.Flush();
					Trace.Close();
					Trace.Listeners.Clear();
					traceLogOpen = false;
				}
			}
			catch
			{
				mTraceLogEnabled = false;
			}
		}

		/// <summary>
		/// Open the trace debug file. Return true if the trace log is started successfully.
		/// </summary>
		private bool TraceLogStart()
		{
			if (!mTraceLogEnabled) return false;
			try
			{
				TraceLogStop();
				Trace.Listeners.Add(new EventLogTraceListener("Inspector"));
				Trace.AutoFlush = true;
				traceLogOpen = true;
				return TraceWriteLine("Trace log : start");
			}
			catch
			{
				mTraceLogEnabled = false;
				return false;
			}
		}

		/// <summary>
		/// Print the PropertiesInspector text title.
		/// </summary>
		private void PaintText(string text)
		{
			Graphics g;
			System.Drawing.Drawing2D.LinearGradientBrush gradBrush;
			Rectangle rect;
			Font font;
			Brush brush;
			float y;

			if (!TextVisible) return;
			brush = new SolidBrush(mTextForeColor);
			font = new Font("Arial", 12.0F, FontStyle.Bold);
			g = this.pnlText.CreateGraphics();
			y = ((float)pnlText.Height / 2.0F) - (g.MeasureString(text, font).Height / 2.0F);
			if (!mTextBackgroundColorIsGradient)
				// Solid color
				g.Clear(mTextBackgroundColor);
			else
			{
				// Fill background with gradient colors
				rect = new Rectangle(0, 0, pnlText.Width, pnlText.Height);
				gradBrush = new System.Drawing.Drawing2D.LinearGradientBrush(rect,
								mTextBackgroundGradientColorStart,
								mTextBackgroundGradientColorFinish, 90.5F);
				g.FillRectangle(gradBrush, rect);
				gradBrush.Dispose();
			}
			g.DrawString(text, font, brush, 3F, y);
			g.Dispose();
			brush.Dispose();
			font.Dispose();
		}

		/// <summary>
		/// Print a single Property Grid Item.
		/// </summary>
		private void PaintItem(BaseItem item, Graphics g, Pen pen, Brush brush, Brush txtBrush, int x1, int x2, ref int y1, ref int y2)
		{
			BooleanItem boolItem = null;
			ColorItem colItem = null;
			SolidBrush fillBrush = null;
			SolidBrush txtDisabledBrush = null;
			System.Drawing.Font propFont = null;
			FloatItem dblItem;
			Brush warnBrush = null;
			//System.Media.SoundPlayer sp = null;

			//TODO:Add any new BaseItem management here in this function...
			if (!item.Visible) return;
			// txtDisabledBrush = new SolidBrush(Color.FromArgb(this.ForeColor.R / 2, this.ForeColor.G / 2, this.ForeColor.B / 2));
			txtDisabledBrush = new SolidBrush(Color.DarkGray);
			item.rectangle = new Rectangle(mFirstColumnWidth, y1, this.Width - mFirstColumnWidth, y2 - y1);
			if (item == mItemSelected)
				g.FillRectangle(brush, x1 + 14, y1, mFirstColumnWidth - 14, mItemHeight);
			g.DrawLine(pen, x1, y2, x2, y2);
			// Property title (text)
			g.Clip = new Region(new Rectangle(x1, y1, mFirstColumnWidth, y2 - y1));
			g.DrawString(item.Text, this.Font, txtBrush, (float)x1 + 16F, (float)y1 + 2);
			g.ResetClip();
			// Property value (view as bold font if values is been changed by user)
			propFont = item.Changed ? new Font(this.Font, FontStyle.Bold) :
									  new Font(this.Font, FontStyle.Regular);
			if (item is BooleanItem)
			{
				boolItem = (BooleanItem)item;
				if (boolItem.Value)
					g.DrawImageUnscaled(imgList.Images[3], mFirstColumnWidth + 2, y1 + 2);
				else
					g.DrawImageUnscaled(imgList.Images[2], mFirstColumnWidth + 2, y1 + 2);
				g.Clip = new Region(item.rectangle);
				if (item.Enabled)
					g.DrawString(item.ValueString, propFont, txtBrush, (float)mFirstColumnWidth + 19, (float)y1 + 2);
				else
					g.DrawString(item.ValueString, propFont, txtDisabledBrush, (float)mFirstColumnWidth + 19, (float)y1 + 2);
			}
			else
				if (item is ColorItem)
			{
				colItem = (ColorItem)item;
				fillBrush = new SolidBrush(colItem.Value);
				g.FillRectangle(fillBrush, mFirstColumnWidth + 2, y1 + 2, 16, y2 - y1 - 6);
				g.DrawRectangle(new Pen(Color.Black), mFirstColumnWidth + 2, y1 + 2, 16, y2 - y1 - 6);
				g.Clip = new Region(item.rectangle);
				if (item.Enabled)
					g.DrawString(item.ValueString, propFont, txtBrush, (float)mFirstColumnWidth + 28, (float)y1 + 2);
				else
					g.DrawString(item.ValueString, propFont, txtDisabledBrush, (float)mFirstColumnWidth + 28, (float)y1 + 2);
			}
			else
			if (item is FloatItem)
			{
				dblItem = (FloatItem)item;
				warnBrush = new SolidBrush(Color.Red);
				if (item.Enabled)
				{
					if (dblItem.ValidationRangeCheck == ValidationRangeCheckType.Manual &&
					   !(dblItem.Value >= dblItem.Minimum && dblItem.Value <= dblItem.Maximum))
					{
						//TODO:Play sound only while setting a new value
						g.DrawString(item.ValueString, propFont, warnBrush, (float)mFirstColumnWidth, (float)y1 + 2);
						//sp = new System.Media.SoundPlayer(@"c:\windows\media\Ringin.wav");
						//sp.Play();
					}
					else
						g.DrawString(item.ValueString, propFont, txtBrush, (float)mFirstColumnWidth, (float)y1 + 2);
				}
				else
					g.DrawString(item.ValueString, propFont, txtDisabledBrush, (float)mFirstColumnWidth, (float)y1 + 2);
			}
			else
			{
				// Any other item...
				g.Clip = new Region(item.rectangle);
				if (item.Enabled)
					g.DrawString(item.ValueString, propFont, txtBrush, (float)mFirstColumnWidth, (float)y1 + 2);
				else
					g.DrawString(item.ValueString, propFont, txtDisabledBrush, (float)mFirstColumnWidth, (float)y1 + 2);
			}
			g.ResetClip();
			y1 = y2;
			y2 += mItemHeight;
		}

		/// <summary>
		/// Print on screen a category item.
		/// </summary>
		private void PaintAsCategory(Graphics g, Pen pen, Brush brush, Brush catBrush, Brush txtBrush, int x1, int x2, int y1, ref int y2)
		{
			CategoryItem category = null;
			BaseItem item = null;
			int k, j;

			for (k = 0; k < CategoryList.Count; k++)
			{
				// - Category
				category = CategoryList[k];
				// Property title (text)
				g.FillRectangle(catBrush, x1 + 14, y1, this.Width, mItemHeight);
				g.DrawString(category.Text, this.Font, txtBrush, (float)x1 + 16F, (float)y1 + 2);
				// Print plus/minus
				category.rectangle = new Rectangle(2, y1 + 2, 13, 13);

				if (category.Expanded)
					g.DrawImageUnscaled(imgList.Images[0], 2, y1 + 2);
				else
					g.DrawImageUnscaled(imgList.Images[1], 2, y1 + 2);
				y1 = y2;
				y2 += mItemHeight;
				// - Property Item
				if (category.Expanded)
					for (j = 0; j < category.ItemList.Count; j++)
					{
						item = category.ItemList[j];
						PaintItem(item, g, pen, brush, txtBrush, x1, x2, ref y1, ref y2);
					}
			}
		}

		/// <summary>
		/// Print on screen a list of property items.
		/// </summary>
		private void PaintAsItemList(Graphics g, Pen pen, Brush brush, Brush txtBrush, int x1, int x2, int y1, ref int y2)
		{
			int j;

			for (j = 0; j < ItemList.Count; j++)
				PaintItem(ItemList[j], g, pen, brush, txtBrush, x1, x2, ref y1, ref y2);
		}

		/// <summary>
		/// Paint property grid control.
		/// </summary>
		private void PaintControl()
		{
			int x1, x2, y1, y2;
			Graphics g;
			Pen pen;
			Brush brush;
			Brush catBrush;
			Brush txtBrush;
			string text;

			TraceWriteLine("PaintControl()");
			SetPnlPropTopPosition();
			pnlProp.AutoScroll = false;
			x1 = 0;
			x2 = this.Width;
			if (x2 < 32) x2 = 32;
			y2 = mItemHeight * (ItemList.Count + CategoryList.Count + 1);
			if (y2 <= 0) y2 = this.Height;
			if (y2 < 32) y2 = 32;
			if (mShowItemsAsCategory)
				picProp.Height = (CategoryList.Count + ItemList.Count) * mItemHeight;
			else
				picProp.Height = ItemList.Count * mItemHeight;
			// Per evitare blink faccio il disegno in un oggetto
			// bitmap e poi lo disegno nell'oggetto G con una 
			// singola istruzione (double buffer technology)
			if (bmpPropGrid != null) bmpPropGrid.Dispose();
			bmpPropGrid = new System.Drawing.Bitmap(x2, y2);
			g = Graphics.FromImage(bmpPropGrid);
			g.Clear(this.BackColor);
			y1 = 0;
			y2 = y1 + mItemHeight;
			// Hide input controls
			HideInputControls();
			// Barra grigia a sx
			brush = new SolidBrush(Color.LightGray);
			g.FillRectangle(brush, 0, 0, 14, bmpPropGrid.Height);
			brush.Dispose();
			// Riga verticale separatore colonne
			pen = new Pen(Color.LightGray);
			g.DrawLine(pen, mFirstColumnWidth, 0, mFirstColumnWidth, bmpPropGrid.Height);
			// Disegna proprietà raggruppate per categoria o come semplice lista
			brush = new SolidBrush(Color.LightBlue);
			txtBrush = new SolidBrush(Color.Black);
			catBrush = new SolidBrush(Color.LightGray);
			if (mShowItemsAsCategory)
				PaintAsCategory(g, pen, brush, catBrush, txtBrush, x1, x2, y1, ref y2);
			else
				PaintAsItemList(g, pen, brush, txtBrush, x1, x2, y1, ref y2);
			// Copy image from memory to screen (to avoid flick)
			picProp.BackgroundImage = bmpPropGrid;
			// Set exact image size
			y2 -= mItemHeight;
			if (y2 < 32) y2 = 32;
			picProp.Height = y2;
			// Print text background
			text = printDemoLabel ? "DEMO" : this.Text;
			PaintText(text);
			// Refresh done, free memory
			pen.Dispose();
			brush.Dispose();
			txtBrush.Dispose();
			catBrush.Dispose();
			g.Dispose();
			pnlProp.AutoScroll = true;
		}

		/// <summary>
		/// Set mouse pointer according to the current mouse pointer position.
		/// </summary>
		/// <param name="xMouse"></param>
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
					PaintControl();
					this.Cursor = Cursors.VSplit;
				}
			}
			else
				this.Cursor = Cursors.Default;
		}

		/// <summary>
		/// Print help info on the screen for a specific property item.
		/// </summary>
		/// <param name="item"></param>
		private void RefreshHelp(BaseItem item)
		{
			if (item == null) return;
			if (item.HelpVisible)
			{
				lblHelpCaption.Text = item.HelpCaption;
				lblHelpText.Text = item.HelpText;
			}
			else
			{
				lblHelpCaption.Text = "";
				lblHelpText.Text = "";
			}
		}

		/// <summary>
		/// Check if the mouse pointer is over a [+]/[-] category expand/collapse.
		/// (Call this function when a mouse click is detected)
		/// </summary>
		private void VerifyMouseUpOnCategory(MouseEventArgs e)
		{
			Rectangle rect;
			CategoryItem category;
			BaseItem item;
			int k, j;
			int itemWidth;

			for (k = 0; k < CategoryList.Count; k++)
			{
				category = CategoryList[k];
				rect = category.rectangle;
				// - Category
				if (e.X >= rect.X && e.X <= (rect.X + rect.Width) && e.Y >= rect.Y && e.Y <= (rect.Y + rect.Height))
				{
					// Expand/Collapse category
					category.Expanded = !category.Expanded;
					PaintControl();
					return;     // Exit
				}
				// - Property BaseItem (search for active selected item)
				if (category.Expanded)
					for (j = 0; j < category.ItemList.Count; j++)
					{
						item = category.ItemList[j];
						rect = item.rectangle;
						if (e.Y >= rect.Y && e.Y <= (rect.Y + rect.Height))
						{
							PreviousItemSelected = mItemSelected;
							mItemSelected = item;
							RefreshHelp(item);
							PaintControl();
							// Show input box (if BaseItem is enabled)
							if (pnlProp.VerticalScroll.Visible)
								itemWidth = rect.Width - 20;
							else
								itemWidth = rect.Width;
							if (itemWidth < 8) itemWidth = 8;
							if (item.Enabled)
								ShowInputControl(rect.X, rect.Y, itemWidth, mItemHeight, item);
							return;     // Exit
						}
					}
			}
		}

		/// <summary>
		/// Check if the mouse pointer is over a property item as set is as the selected item.
		/// (Call this function when a mouse click is detected)
		/// </summary>
		private void VerifyMouseUpOnItemList(MouseEventArgs e)
		{
			Rectangle rect;
			int j;

			for (j = 0; j < ItemList.Count; j++)
			{
				rect = ItemList[j].rectangle;
				if (e.Y >= rect.Y && e.Y <= (rect.Y + rect.Height))
				{
					PreviousItemSelected = mItemSelected;
					mItemSelected = ItemList[j];
					RefreshHelp(ItemList[j]);
					PaintControl();
					// Show input box (if BaseItem is enabled)
					if (ItemList[j].Enabled)
						ShowInputControl(rect.X, rect.Y, rect.Width, mItemHeight, ItemList[j]);
					return;     // Exit
				}
			}
		}

		/// <summary>
		/// Show the combo box control (eg. For editing a DropDownItem)
		/// </summary>
		private void ShowComboBoxControl(int X, int Y, int width, int height, DropDownItem listItem)
		{
			string[] list;
			int t;
			int xOffset, yOffset;

			xOffset = pnlProp.HorizontalScroll.Value;
			yOffset = pnlProp.VerticalScroll.Value;
			cmbBox.Left = X + 1 - xOffset;
			cmbBox.Top = Y - yOffset;
			cmbBox.Width = width - 8;
			cmbBox.Height = height;
			controlChangeEventLocked = true;
			cmbBox.DropDownStyle = (ComboBoxStyle)listItem.DropDownStyle;
			cmbBox.Items.Clear();
			list = listItem.GetDropDownList();
			for (t = 0; t < list.Length; t++)
				cmbBox.Items.Add(list[t]);
			cmbBox.Text = listItem.Value;
			cmbBox.SelectAll();
			cmbBox.Visible = true;
			controlChangeEventLocked = false;
			cmbBox.Focus();
		}

		/// <summary>
		/// Show the check box control (eg. For editing a BooleanItem)
		/// </summary>
		private void ShowCheckBoxControl(int X, int Y, int width, int height, BooleanItem boolItem)
		{
			int xOffset, yOffset;

			xOffset = pnlProp.HorizontalScroll.Value;
			yOffset = pnlProp.VerticalScroll.Value;
			chkBox.Left = X + 3 - xOffset;
			chkBox.Top = Y + 2 - yOffset;
			chkBox.Width = width - 8;
			chkBox.Height = height - 3;
			controlChangeEventLocked = true;
			chkBox.Text = boolItem.ValueString;
			chkBox.Checked = boolItem.Value;
			chkBox.Visible = true;
			controlChangeEventLocked = false;
			chkBox.Focus();
		}

		/// <summary>
		/// Show the text box control (eg. For editing a StringItem)
		/// </summary>
		private void ShowTextBoxControl(int X, int Y, int width, int height, BaseItem item)
		{
			int xOffset;
			int yOffset;
			int moreButtonWidth = 0;

			// Per il tipo testo semplice è possibile visualizzare il pulsante
			// "..." (more) in modo che l'utente possa aprire una finestra personalizzata
			if (item.ShowExpandButton)
			{
				ShowMoreButtonControl(X, Y, width, height);
				moreButtonWidth = btnMore.Width;
			}
			xOffset = pnlProp.HorizontalScroll.Value;
			yOffset = pnlProp.VerticalScroll.Value;
			txtBox.Left = X + 3 - xOffset;
			txtBox.Top = Y + 3 - yOffset;
			txtBox.Width = width - 8 - moreButtonWidth;
			txtBox.Height = height;
			controlChangeEventLocked = true;
			txtBox.Text = item.ValueString;
			txtBox.SelectAll();
			txtBox.Visible = true;
			controlChangeEventLocked = false;
			txtBox.Focus();
		}

		/// <summary>
		/// Show the numeric up/down text box control (eg. For editing a Int32Item)
		/// </summary>
		private void ShowNumericUpDownControl(int X, int Y, int width, int height, Int32Item intItem)
		{
			int xOffset, yOffset;

			xOffset = pnlProp.HorizontalScroll.Value;
			yOffset = pnlProp.VerticalScroll.Value;
			numUpDn.Left = X + 1 - xOffset;
			numUpDn.Top = Y + 1 - yOffset;
			numUpDn.Width = width - 8;
			numUpDn.Height = height - 1;
			controlChangeEventLocked = true;
			numUpDn.Minimum = intItem.Minimum;
			numUpDn.Maximum = intItem.Maximum;
			numUpDn.Increment = intItem.Increment;
			if (intItem.Value < intItem.Minimum) intItem.Value = intItem.Minimum;
			if (intItem.Value > intItem.Maximum) intItem.Value = intItem.Maximum;
			numUpDn.Value = intItem.Value;
			numUpDn.DecimalPlaces = 0;
			numUpDn.Select(0, numUpDn.Value.ToString().Length);
			numUpDn.Hexadecimal = intItem.Hexadecimal;
			numUpDn.Visible = true;
			controlChangeEventLocked = false;
			numUpDn.Focus();
		}

		/// <summary>
		/// Show the numeric up/down text box control (eg. For editing a FloatItem)
		/// </summary>
		private void ShowNumericUpDownControl(int X, int Y, int width, int height, FloatItem dblItem)
		{
			int xOffset, yOffset;

			try
			{
				// An exception could be generated while casting from double to decimal
				xOffset = pnlProp.HorizontalScroll.Value;
				yOffset = pnlProp.VerticalScroll.Value;
				numUpDn.Left = X + 1 - xOffset;
				numUpDn.Top = Y + 1 - yOffset;
				numUpDn.Width = width - 8;
				numUpDn.Height = height - 1;
				controlChangeEventLocked = true;
				numUpDn.DecimalPlaces = dblItem.DecimalPlaces;
				numUpDn.Minimum = (decimal)dblItem.Minimum;
				numUpDn.Maximum = (decimal)dblItem.Maximum;
				numUpDn.Increment = (decimal)dblItem.Increment;
				if (dblItem.Value < dblItem.Minimum) dblItem.Value = dblItem.Minimum;
				if (dblItem.Value > dblItem.Maximum) dblItem.Value = dblItem.Maximum;
				numUpDn.Value = (decimal)dblItem.Value;
				numUpDn.Select(0, numUpDn.Value.ToString().Length);
				numUpDn.Visible = true;
				controlChangeEventLocked = false;
				numUpDn.Focus();
			}
			catch
			{
				// Error! Do not show input control!
				numUpDn.Visible = false;
				controlChangeEventLocked = false;
			}
		}

		/// <summary>
		/// Show the more button (to open a dialog box)
		/// </summary>
		private void ShowMoreButtonControl(int X, int Y, int width, int height)
		{
			int xOffset, yOffset;

			xOffset = pnlProp.HorizontalScroll.Value;
			yOffset = pnlProp.VerticalScroll.Value;
			btnMore.Left = X + width - xOffset - btnMore.Width - 4;
			btnMore.Top = Y - yOffset;
			btnMore.Visible = true;
			if (btnMore.Enabled)
				btnMore.Focus();
		}

		/// <summary>
		/// Show a cencel button.
		/// </summary>
		private void ShowCancelButtonControl(int X, int Y, int width, int height)
		{
			int xOffset, yOffset;

			xOffset = pnlProp.HorizontalScroll.Value;
			yOffset = pnlProp.VerticalScroll.Value;
			btnCancel.Left = X + width - xOffset - btnMore.Width - 4;
			btnCancel.Top = Y - yOffset;
			btnCancel.Visible = true;
			if (btnCancel.Enabled)
				btnCancel.Focus();
		}

		/// <summary>
		/// Show the image preview form.
		/// </summary>
		private void ShowImagePreviewControl(int X, int Y, Image image, string imageFileName)
		{
			// 			Point formPos;
			// 			Rectangle rect;
			// 			
			// 			try
			// 			{
			// 				if (frmImagePreview == null)
			// 					frmImagePreview = new FrmImagePreview();
			// 				rect = Screen.PrimaryScreen.WorkingArea;
			// 				// Show the form at mouse position
			// 				//formPos = Control.MousePosition;      
			// 				// Show the preview form under the property line that rappresent the object ImageItem
			// 				// (or on the topo of the line, depending from the form height, in a way to ensure the entire form is visible)
			// 				X -= pnlProp.HorizontalScroll.Value;
			// 				Y -= pnlProp.VerticalScroll.Value;
			// 				if (ToolbarVisible)
			// 					Y += topBar.Height;
			// 				if (TextVisible)
			// 					Y += pnlText.Height;
			// 				formPos = new Point(X, Y);
			// 				formPos = this.PointToScreen(formPos);
			// 				// Adjust coord. to fix the preview form in the screen
			// 				if (formPos.Y + frmImagePreview.Height > rect.Height)
			// 					formPos.Y -= (this.RowsHeight + frmImagePreview.Height);
			// 				if (formPos.X + frmImagePreview.Width > rect.Width)
			// 					formPos.X = rect.Width - frmImagePreview.Width;
			// 				// Show image
			// 				frmImagePreview.BackgroundImage = image;
			// 				frmImagePreview.ImageFileName = imageFileName;
			// 				frmImagePreview.Show();
			// 				frmImagePreview.Location = formPos;
			// 			}
			// 			catch
			// 			{
			// 				frmImagePreview = null;
			// 			}   
		}

		/// <summary>
		/// Show the correct input control (with focus) depending from a specific BaseItem type
		/// </summary>
		private void ShowInputControl(int X, int Y, int width, int height, BaseItem item)
		{
			int xOffset, yOffset;
			DropDownItem listItem = null;
			Int32Item intItem = null;
			BooleanItem boolItem = null;
			FloatItem dblItem = null;
			ImageItem imgItem = null;

			//TODO:Add any new BaseItem management here in this function...
			xOffset = pnlProp.HorizontalScroll.Value;
			yOffset = pnlProp.VerticalScroll.Value;
			HideInputControls();
			// Show only the correct input control for the BaseItem management
			// - Cominciare dai tipi ultimi derivati; ad esempio se c'e questo
			//   tipo di eredità "FileItem :  StringItem : Item" deve essere verificato
			//   per primo il tipo FileItem per evitare che questo venga gestito 
			//   erroneamente come un tipo  StringItem da cui è derivato
			if (item is FileItem)
			{
				// NOTA IMPORTANTE : Lasciare questo IF in questo punto, prima della 
				// verifica del tipo  StringItem
				btnMore.Enabled = item.Enabled;
				ShowMoreButtonControl(X, Y, width, height);
				return;
			}
			if (item is DropDownItem)
			{
				// NOTA IMPORTANTE : Lasciare questo IF in questo punto, prima della 
				// verifica del tipo  StringItem
				listItem = (DropDownItem)item;
				ShowComboBoxControl(X, Y, width, height, listItem);
				return;
			}
			if (item is Int32Item)
			{
				intItem = (Int32Item)item;
				if (intItem.ValidationRangeCheck)
					ShowNumericUpDownControl(X, Y, width, height, intItem);
				else
					ShowTextBoxControl(X, Y, width, height, item);
				return;
			}
			if (item is StringItem)
			{
				ShowTextBoxControl(X, Y, width, height, item);
				return;
			}
			if (item is BooleanItem)
			{
				boolItem = (BooleanItem)item;
				ShowCheckBoxControl(X, Y, width, height, boolItem);
				return;
			}
			if (item is FloatItem)
			{
				dblItem = (FloatItem)item;
				if (dblItem.ValidationRangeCheck == ValidationRangeCheckType.Automatic)
					ShowNumericUpDownControl(X, Y, width, height, dblItem);
				else
					ShowTextBoxControl(X, Y, width, height, item);
				return;
			}
			if (item is ColorItem)
			{
				ShowMoreButtonControl(X, Y, width, height);
				return;
			}
			if (item is ImageItem)
			{
				//ShowMoreButtonControl(X, Y, width, height);
				imgItem = (ImageItem)item;
				ShowMoreButtonControl(X, Y, width, height);
				imgItem.Refresh();
				ShowImagePreviewControl(X, Y + height, imgItem.Value, imgItem.FileName);
				return;
			}
		}

		/// <summary>
		/// Hide all input controls.
		/// </summary>
		private void HideInputControls(bool repaintAll)
		{
			txtBox.Visible = false;
			cmbBox.Visible = false;
			numUpDn.Visible = false;
			chkBox.Visible = false;
			btnMore.Visible = false;
			btnCancel.Visible = false;
			dtTmBox.Visible = false;
			//TODO:Add any new control hide action here...
			if (repaintAll)
				PaintControl();
		}

		/// <summary>
		/// Hide all input controls.
		/// </summary>
		private void HideInputControls()
		{
			HideInputControls(false);
		}

		/// <summary>
		/// Resize property panel.
		/// </summary>
		private void ResizePropertyPanel()
		{
			int height;

			if (splitBottom.Visible)
				height = splitBottom.Top - pnlProp.Top;
			else
				height = this.Height - pnlProp.Top;
			if (height < 32) height = 32;
			pnlProp.Height = height;
		}

		/// <summary>
		/// Run this function when the btnMore button is pressed or when the user
		/// double-click the mouse on any BaseItem (only if the btnMore is visibile)
		/// </summary>
		private void MoreButtonIsPressed()
		{
			DialogResult diagRet;
			ColorItem colItem = null;
			ImageItem imgItem = null;
			FileItem fileItem = null;
			StringItem StringItem = null;
			string oldValue = "";

			if (!btnMore.Visible) return;
			btnMore.Enabled = false;
			if (mItemSelected is ColorItem)
			{
				colItem = (ColorItem)mItemSelected;
				colDiag.Color = colItem.Value;
				diagRet = colDiag.ShowDialog();
				if (diagRet == DialogResult.OK)
				{
					colItem.Value = colDiag.Color;
					PaintControl();
				}
				btnMore.Enabled = true;
				return;
			}
			if (mItemSelected is ImageItem)
			{
				imgItem = (ImageItem)mItemSelected;
				fileDiag.FileName = imgItem.FileName;
				fileDiag.Filter = "All pictures (*.bmp;*.png;*.jpg;*.tiff;*.gif)|*.bmp;*.png;*.jpg;*.tiff;*.gif|All files (*.*)|*.*";
				fileDiag.FilterIndex = 1;
				diagRet = fileDiag.ShowDialog();
				if (diagRet == DialogResult.OK)
				{
					imgItem.FileName = fileDiag.FileName;
					PaintControl();
				}
				btnMore.Enabled = true;
				return;
			}
			if (mItemSelected is FileItem)
			{
				fileItem = (FileItem)mItemSelected;
				fileDiag.FileName = fileItem.Value;
				fileDiag.Filter = fileItem.Filter;
				fileDiag.FilterIndex = fileItem.FilterIndex;
				diagRet = fileDiag.ShowDialog();
				if (diagRet == DialogResult.OK)
				{
					fileItem.Value = fileDiag.FileName;
					PaintControl();
				}
				btnMore.Enabled = true;
				return;
			}
			if (mItemSelected is StringItem)
			{
				// The more button is visible when the property
				// .ShowExpandButton is true. Use this button to run 
				// a user defined action like to open a dialog box.
				// (The event ExpandButtonPressed will be raised; 
				//  add your action into this event)
				StringItem = (StringItem)mItemSelected;
				oldValue = StringItem.Value;
				RaiseExpandButtonPressed(StringItem);
				btnMore.Enabled = true;
				if (oldValue != StringItem.Value)
					// Execute control refresh if the user chage a property value
					RefreshControl(true);
				return;
			}
			// Exit
			btnMore.Enabled = true;
		}

		/// <summary>
		/// Validate text
		/// </summary>
		private void txtBox_ValidateText(BaseItem item)
		{
			// Validate data!
			Int32Item int32Item = null;
			FloatItem dblItem = null;

			if (item == null) return;
			if (item is Int32Item)
			{
				int32Item = (Int32Item)item;
				int32Item.ValueString = txtBox.Text;
				return;
			}
			if (item is FloatItem)
			{
				dblItem = (FloatItem)item;
				dblItem.ValueString = txtBox.Text;
				if (dblItem.ValidationRangeCheck == ValidationRangeCheckType.Manual)
				{
					if (dblItem.Value < dblItem.Minimum || dblItem.Value > dblItem.Maximum)
						System.Media.SystemSounds.Hand.Play();
				}
				return;
			}
		}

		#endregion

		#region Public methods

		/// <summary>
		/// Sets the default values for all items into the property grid. Redraw (RefreshControl) is automatic.
		/// </summary>
		public void SetDefaultValues()
		{
			int t;

			// Set all items to the default value
			if (ItemList == null) return;
			for (t = 0; t < ItemList.Count; t++)
				ItemList[t].SetDefaultValue();
			RefreshControl(true);
		}

		/// <summary>
		/// Call this method to refresh the control on the screen. For fast printing set repaint to false.
		/// Set repaint to true if you want to redraw all items (example after an BaseItem property change).
		/// </summary>
		/// <param name="repaint">true:redraw all BaseItem by item. false:redraw using the last saved memory bitmap.</param>
		public void RefreshControl(bool repaint)
		{
			TraceWriteLine("RefreshControl(repaint=" + repaint.ToString() + ")");
			if (repaint || bmpPropGrid == null)
			{
				PaintControl();
				RefreshHelp(this.SelectedItem);
				return;
			}
			// Copy last image from memory to screen (to avoid flick)
			pnlProp.AutoScroll = false;
			picProp.BackgroundImage = bmpPropGrid;
			pnlProp.AutoScroll = true;
		}

		/// <summary>
		/// Set the Changed property of each BaseItem to false.
		/// </summary>
		public void ResetChanges()
		{
			int t;

			TraceWriteLine("ResetChanges()");
			if (ItemList == null) return;
			for (t = 0; t < ItemList.Count; t++)
				ItemList[t].Changed = false;
			RefreshControl(true);
		}

		/// <summary>
		/// Remove all items from the property grid.
		/// </summary>
		public void ItemsClear()
		{
			int t;

			TraceWriteLine("ItemsClear()");
			// Remove all category and related items
			if (CategoryList != null)
			{
				for (t = 0; t < CategoryList.Count; t++)
					if (CategoryList[t].ItemList != null)
					{
						CategoryList[t].ItemList.Clear();
						CategoryList[t].ItemList = null;
					}
				CategoryList.Clear();
			}
			// Remove all items
			if (ItemList != null)
				ItemList.Clear();
			RefreshControl(true);
		}

		/// <summary>
		/// Add a specific BaseItem to the property grid. Be sure to use a unique key (itemKey).
		/// </summary>
		/// <param name="categoryKey">The category where to append the item.</param>
		/// <param name="itemKey">Unique key for a fast BaseItem reference.</param>
		/// <param name="strItem">String item.</param>
		public void ItemAdd(string categoryKey, string itemKey, StringItem strItem)
		{
			// Add BaseItem to category list and BaseItem list
			if (CategoryList.Contains(categoryKey))
			{
				CategoryList[categoryKey].ItemList.Add(itemKey, strItem);
				ItemList.Add(itemKey, strItem);
				if (this.SelectedItem == null)
					this.SelectedItem = strItem;
			}
		}

		/// <summary>
		/// Add a specific BaseItem to the property grid. Be sure to use a unique key (itemKey).
		/// </summary>
		/// <param name="categoryKey">The category where to append the item.</param>
		/// <param name="itemKey">Unique key for a fast BaseItem reference.</param>
		/// <param name="intItem">Int32 item.</param>
		public void ItemAdd(string categoryKey, string itemKey, Int32Item intItem)
		{
			// Add BaseItem to category list and BaseItem list
			if (CategoryList.Contains(categoryKey))
			{
				CategoryList[categoryKey].ItemList.Add(itemKey, intItem);
				ItemList.Add(itemKey, intItem);
				if (this.SelectedItem == null)
					this.SelectedItem = intItem;
			}
		}

		/// <summary>
		/// Add a specific BaseItem to the property grid. Be sure to use a unique key (itemKey).
		/// </summary>
		/// <param name="categoryKey">The category where to append the item.</param>
		/// <param name="itemKey">Unique key for a fast BaseItem reference.</param>
		/// <param name="cmbItem">Drop down list item.</param>
		public void ItemAdd(string categoryKey, string itemKey, DropDownItem cmbItem)
		{
			// Add BaseItem to category list and BaseItem list
			if (CategoryList.Contains(categoryKey))
			{
				CategoryList[categoryKey].ItemList.Add(itemKey, cmbItem);
				ItemList.Add(itemKey, cmbItem);
				if (this.SelectedItem == null)
					this.SelectedItem = cmbItem;
			}
		}

		/// <summary>
		/// Add a specific BaseItem to the property grid. Be sure to use a unique key (itemKey).
		/// </summary>
		/// <param name="categoryKey">The category where to append the item.</param>
		/// <param name="itemKey">Unique key for a fast BaseItem reference.</param>
		/// <param name="boolItem">Boolean item.</param>
		public void ItemAdd(string categoryKey, string itemKey, BooleanItem boolItem)
		{
			// Add BaseItem to category list and BaseItem list
			if (CategoryList.Contains(categoryKey))
			{
				CategoryList[categoryKey].ItemList.Add(itemKey, boolItem);
				ItemList.Add(itemKey, boolItem);
				if (this.SelectedItem == null)
					this.SelectedItem = boolItem;
			}
		}

		/// <summary>
		/// Add a specific BaseItem to the property grid. Be sure to use a unique key (itemKey).
		/// </summary>
		/// <param name="categoryKey">The category where to append the item.</param>
		/// <param name="itemKey">Unique key for a fast BaseItem reference.</param>
		/// <param name="dblItem">Double tem.</param>
		public void ItemAdd(string categoryKey, string itemKey, FloatItem dblItem)
		{
			// Add BaseItem to category list and BaseItem list
			if (CategoryList.Contains(categoryKey))
			{
				CategoryList[categoryKey].ItemList.Add(itemKey, dblItem);
				ItemList.Add(itemKey, dblItem);
				if (this.SelectedItem == null)
					this.SelectedItem = dblItem;
			}
		}

		/// <summary>
		/// Add a specific BaseItem to the property grid. Be sure to use a unique key (itemKey).
		/// </summary>
		/// <param name="categoryKey">The category where to append the item.</param>
		/// <param name="itemKey">Unique key for a fast BaseItem reference.</param>
		/// <param name="colItem">Color item.</param>
		public void ItemAdd(string categoryKey, string itemKey, ColorItem colItem)
		{
			// Add BaseItem to category list and BaseItem list
			if (CategoryList.Contains(categoryKey))
			{
				CategoryList[categoryKey].ItemList.Add(itemKey, colItem);
				ItemList.Add(itemKey, colItem);
				if (this.SelectedItem == null)
					this.SelectedItem = colItem;
			}
		}

		/// <summary>
		/// Add a specific BaseItem to the property grid. Be sure to use a unique key (itemKey).
		/// </summary>
		/// <param name="categoryKey">The category where to append the item.</param>
		/// <param name="itemKey">Unique key for a fast BaseItem reference.</param>
		/// <param name="imgItem">Image (picture) item.</param>
		public void ItemAdd(string categoryKey, string itemKey, ImageItem imgItem)
		{
			// Add BaseItem to category list and BaseItem list
			if (CategoryList.Contains(categoryKey))
			{
				CategoryList[categoryKey].ItemList.Add(itemKey, imgItem);
				ItemList.Add(itemKey, imgItem);
				if (this.SelectedItem == null)
					this.SelectedItem = imgItem;
			}
		}

		/// <summary>
		/// Add a specific BaseItem to the property grid. Be sure to use a unique key (itemKey).
		/// </summary>
		/// <param name="categoryKey">The category where to append the item.</param>
		/// <param name="itemKey">Unique key for a fast BaseItem reference.</param>
		/// <param name="fileItem">File (file name) item.</param>
		public void ItemAdd(string categoryKey, string itemKey, FileItem fileItem)
		{
			// Add BaseItem to category list and BaseItem list
			if (CategoryList.Contains(categoryKey))
			{
				CategoryList[categoryKey].ItemList.Add(itemKey, fileItem);
				ItemList.Add(itemKey, fileItem);
				if (this.SelectedItem == null)
					this.SelectedItem = fileItem;
			}
		}


		//TODO:Add any new BaseItem management here... (ItemAdd() function)


		/// <summary>
		/// Add a specific category BaseItem to the property grid. Be sure to use a unique key (categoryKey).
		/// </summary>
		/// <param name="categoryKey">Category BaseItem key reference.</param>
		/// <param name="catItem">Category item.</param>
		public void CategoryAdd(string categoryKey, CategoryItem catItem)
		{
			TraceWriteLine("CategoryAdd(categoryKey=" + categoryKey + ")");
			CategoryList.Add(categoryKey, catItem);
		}

		/// <summary>
		/// Return an BaseItem value as a string. Return null if the BaseItem is not found.
		/// </summary>
		/// <param name="itemKey">Unique key for a fast BaseItem reference.</param>
		/// <returns>String value rappresentation.</returns>
		public string GetItemValue(string itemKey)
		{
			if (ItemList.Contains(itemKey))
				return ItemList[itemKey].ValueString;
			else
				return null;
		}

		/// <summary>
		/// Return an BaseItem double value. Return 0.0 if the BaseItem is not found.
		/// </summary>
		/// <param name="itemKey">Unique key for a fast BaseItem reference.</param>
		/// <returns>Double value.</returns>
		public double GetPropertyItemDoubleValue(string itemKey)
		{
			FloatItem dblItem = null;

			if (ItemList.Contains(itemKey) && ItemList[itemKey] is FloatItem)
			{
				dblItem = (FloatItem)ItemList[itemKey];
				return dblItem.Value;
			}
			else
				return 0.0;
		}

		/// <summary>
		/// Sets a BaseItem string value. Only some items ( StringItem, DropDownItem, ...) support this method.
		/// </summary>
		public void SetItemValue(string itemKey, string value)
		{

			if (ItemList.Contains(itemKey) && (ItemList[itemKey] is StringItem ||
				ItemList[itemKey] is DropDownItem))
				// Set value
				ItemList[itemKey].ValueString = value;
		}

		/// <summary>
		/// Sets a BaseItem double value. Only the FloatItem object support this method.
		/// </summary>
		public void SetItemValue(string itemKey, double value)
		{
			string format;

			if (ItemList.Contains(itemKey) && ItemList[itemKey] is FloatItem)
			{
				format = ItemList[itemKey].Format;
				ItemList[itemKey].ValueString = value.ToString(format);
			}
		}

		/// <summary>
		/// Sets a BaseItem int value. Only the Int32Item object support this method.
		/// </summary>
		public void SetItemValue(string itemKey, int value)
		{
			if (ItemList.Contains(itemKey) && ItemList[itemKey] is Int32Item)
				ItemList[itemKey].ValueString = value.ToString();
		}

		/// <summary>
		/// Return the text property of an item. Return null if the BaseItem is not found.
		/// </summary>
		/// <param name="itemKey">Unique key for a fast BaseItem reference.</param>
		/// <returns>String Text of an item.</returns>
		public string GetItemText(string itemKey)
		{
			if (ItemList.Contains(itemKey))
				return ItemList[itemKey].Text;
			else
				return null;
		}

		/// <summary>
		/// Return an BaseItem stored into the property grid. Return null if the BaseItem is not found.
		/// </summary>
		/// <param name="itemKey">Unique key for a fast BaseItem reference.</param>
		/// <returns>Generic BaseItem object.</returns>
		public BaseItem GetItem(string itemKey)
		{
			if (ItemList.Contains(itemKey))
				return ItemList[itemKey];
			else
				return null;
		}

		/// <summary>
		/// Return a CategoryItem.
		/// </summary>
		public CategoryItem GetCategory(string key)
		{
			if (CategoryList.Contains(key))
				return CategoryList[key];
			else
				return null;
		}

		/// <summary>
		/// Return a CategoryItem.
		/// </summary>
		public void SetCategoryText(string key, string text)
		{
			if (CategoryList.Contains(key))
				CategoryList[key].Text = text;
		}

		#endregion

		#region Public events

		/// <summary>
		/// Delegate for event ExpandButtonPressed.
		/// </summary>
		/// <param name="sender">Sender object.</param>
		/// <param name="item">String item.</param>
		public delegate void ExpandButtonPressedHandle(object sender, StringItem item);

		/// <summary>
		/// This event accour while the user press the expand button ("...", more button).
		/// </summary>
		public event ExpandButtonPressedHandle ExpandButtonPressed;

		private void RaiseExpandButtonPressed(StringItem item)
		{
			if (ExpandButtonPressed != null)
				// Raise event
				ExpandButtonPressed(this, item);
		}

		/// <summary>
		/// Delegate for event ApplyButtonPressed.
		/// </summary>
		/// <param name="sender">Sender object.</param>
		/// <param name="cancel">Set to false if you don't want to apply changes.</param>
		public delegate void ApplyButtonPressedHandle(object sender, ref bool cancel);

		/// <summary>
		/// This event accour while the user press the apply button on the toolbar.
		/// </summary>
		public event ApplyButtonPressedHandle ApplyButtonPressed;

		private void RaiseApplyButtonPressed()
		{
			bool cancel = false;

			if (ApplyButtonPressed != null)
			{
				// Raise event
				ApplyButtonPressed(this, ref cancel);
				if (!cancel)
					ResetChanges();
			}
		}

		#endregion

		#region Private internal events generated by user interface

		private void txtBox_TextChanged(object sender, EventArgs e)
		{
			StringItem strItem = null;
			//Int32Item int32Item = null;
			//FloatItem dblItem = null;

			TraceWriteLine("txtBox_TextChanged()");
			if (controlChangeEventLocked) return;
			if (mItemSelected == null) return;
			if (mItemSelected is StringItem)
			{
				strItem = (StringItem)mItemSelected;
				strItem.Value = txtBox.Text;
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

			TraceWriteLine("cmbBox_TextChanged()");
			if (controlChangeEventLocked) return;
			if (mItemSelected == null) return;
			if (mItemSelected is DropDownItem)
			{
				listItem = (DropDownItem)mItemSelected;
				listItem.Value = cmbBox.Text;
			}
		}

		private void cmbBox_SelectedIndexChanged(object sender, EventArgs e)
		{
			DropDownItem listItem = null;

			TraceWriteLine("cmbBox_SelectedIndexChanged()");
			if (controlChangeEventLocked) return;
			if (mItemSelected == null) return;
			if (mItemSelected is DropDownItem)
			{
				listItem = (DropDownItem)mItemSelected;
				listItem.Value = cmbBox.Text;
			}
		}

		private void numUpDn_ValueChanged(object sender, EventArgs e)
		{
			Int32Item int32Item = null;
			int intValue;
			FloatItem dblItem = null;
			double dblValue;

			TraceWriteLine("numUpDn_ValueChanged()");
			if (controlChangeEventLocked) return;
			if (mItemSelected == null) return;
			if (mItemSelected is Int32Item)
			{
				int32Item = (Int32Item)mItemSelected;
				try
				{
					intValue = (int)numUpDn.Value;
					int32Item.Value = intValue;
				}
				catch
				{
					// Ignore data convert error! Do not set item value!
				}
				return;
			}
			if (mItemSelected is FloatItem)
			{
				dblItem = (FloatItem)mItemSelected;
				try
				{
					dblValue = (double)numUpDn.Value;
					dblItem.Value = dblValue;
				}
				catch
				{
					// Ignore data convert error! Do not set item value!
				}
				return;
			}
		}

		private void chkBox_Click(object sender, EventArgs e)
		{
			BooleanItem boolItem = null;

			TraceWriteLine("chkBox_Click()");
			if (mItemSelected is BooleanItem)
			{
				boolItem = (BooleanItem)mItemSelected;
				boolItem.Value = chkBox.Checked;
				chkBox.Text = boolItem.ValueString;
			}
		}

		private void btnMore_Click(object sender, EventArgs e)
		{
			TraceWriteLine("btnMore_Click()");
			MoreButtonIsPressed();
		}

		private void txtBox_KeyPress(object sender, KeyPressEventArgs e)
		{
			if (e.KeyChar == (Char)13)
			{
				txtBox_ValidateText(mItemSelected);
				HideInputControls(true);
			}
		}

		private void cmbBox_KeyPress(object sender, KeyPressEventArgs e)
		{
			if (e.KeyChar == (Char)13)
				HideInputControls();
		}

		private void numUpDn_KeyPress(object sender, KeyPressEventArgs e)
		{
			if (e.KeyChar == (Char)13)
				HideInputControls(true);
		}

		private void chkBox_KeyPress(object sender, KeyPressEventArgs e)
		{
			if (e.KeyChar == (Char)13)
				HideInputControls(true);
		}

		private void PropertyGrid_Paint(object sender, PaintEventArgs e)
		{
			TraceWriteLine("PropertyGrid_Paint()");
			//PaintControl();
			//RefreshControl(false);
		}

		private void picProp_MouseUp(object sender, MouseEventArgs e)
		{
			if (e.Button == MouseButtons.Left && !resizeCursorOnScreen)
			{
				if (mShowItemsAsCategory)
					VerifyMouseUpOnCategory(e);
				else
					VerifyMouseUpOnItemList(e);
			}
			else
			{
				controlChangeEventLocked = true;
				HideInputControls();
			}
		}

		private void picProp_MouseMove(object sender, MouseEventArgs e)
		{
			if (e.Button == MouseButtons.Left && resizeCursorOnScreen)
			{
				if (mFirstColumnWidth != e.X)
				{
					mFirstColumnWidth = e.X;
					PaintControl();
				}
			}
			else
				RefreshCursor(e.X);
		}

		private void splitBottom_SplitterMoved(object sender, SplitterEventArgs e)
		{
			ResizePropertyPanel();
		}

		private void PropertyGrid_SizeChanged(object sender, EventArgs e)
		{
			ResizePropertyPanel();
			RefreshControl(true);
		}

		private void btnAlphabetical_Click(object sender, EventArgs e)
		{
			this.ShowItemsAsCategory = false;
		}

		private void btnCategorized_Click(object sender, EventArgs e)
		{
			this.ShowItemsAsCategory = true;
		}

		private void btnShowHelp_Click(object sender, EventArgs e)
		{
			if (toolbarLocked) return;
			this.HelpVisible = (btnShowHelp.CheckState == CheckState.Checked);
		}

		private void btnShowText_Click(object sender, EventArgs e)
		{
			if (toolbarLocked) return;
			this.TextVisible = (btnShowText.CheckState == CheckState.Checked);
			PaintText(this.Text);
		}

		private void txtBox_DoubleClick(object sender, EventArgs e)
		{
			MoreButtonIsPressed();
		}

		private void picProp_DoubleClick(object sender, EventArgs e)
		{
			MoreButtonIsPressed();
		}

		private void pnlHelp_DoubleClick(object sender, EventArgs e)
		{
		}

		private void lblHelpText_MouseDoubleClick(object sender, MouseEventArgs e)
		{
		}

		private void lblHelpCaption_MouseDoubleClick(object sender, MouseEventArgs e)
		{
		}

		private void PropertyGrid_Load(object sender, EventArgs e)
		{
			TraceWriteLine("PropertyGrid_Load()");
			TraceWriteLine("DesignMode = " + this.DesignMode.ToString());
			this.Text = this.Name;
		}

		private void txtBox_Leave(object sender, EventArgs e)
		{
			txtBox_ValidateText(PreviousItemSelected);
			HideInputControls(true);
		}

		private void btnApply_Click(object sender, EventArgs e)
		{
			RaiseApplyButtonPressed();
		}

		#endregion

	}
}
