using System;
using System.Collections.Generic;
using System.Text;
/*
 * 10/08/2008
 * 
 * Part of the open source project Inspector
 * Developer : mbr ® (Massimiliano Brugnerotto)
 *  
 */
namespace Inspector
{
    // All classes like "PropertyItem..." inherit directly from PropertyItemGeneric or another
    // class (eg. PropertyItemString) but the base class must be PropertyItemGeneric

    #region Date/Time property item class

    /// <summary>
    /// Date/Time property item class.
    /// </summary>
    public class PropertyItemDateTime : PropertyItemGeneric
    {

        #region Private internal var./properties

        private System.DateTime mValue = System.DateTime.Now;
        private System.DateTime mDefaultValue;
        private DateTimeType mStyle = DateTimeType.DateAndTime;

        #endregion

        #region Constructors

        /// <summary>
        /// Constructor. The DefaultValue is the current date/time.
        /// </summary>
        public PropertyItemDateTime()
        {
            this.Text = "New date and time item";
            this.Format = "";            // Show date and time
            this.DefaultValue = System.DateTime.Now;
        }

        /// <summary>
        /// Constructor. The DefaultValue is the current date/time.
        /// </summary>
        public PropertyItemDateTime(string text, System.DateTime value, DateTimeType style)
        {
            this.Text = text;
            this.Value = value;
            this.DefaultValue = System.DateTime.Now;
            this.Style = style;            // Show date and/or time depending from selected style
        }

        /// <summary>
        /// Constructor. The DefaultValue can be different from Value.
        /// </summary>
        public PropertyItemDateTime(string text, System.DateTime value, System.DateTime defaultValue, DateTimeType style)
        {
            this.Text = text;
            this.Value = value;
            this.DefaultValue = defaultValue;
            this.Style = style;            // Show date and/or time depending from selected style
        }

        #endregion

        #region Public Properties

        /// <summary>
        /// Gets/sets a date and time value.
        /// </summary>
        public System.DateTime Value
        {
            get
            {
                return mValue;
            }
            set
            {
                System.DateTime oldValue = mValue;

                mValue = value;
                this.Changed = (oldValue.ToBinary() != mValue.ToBinary());
                if (this.Changed)
                    RaiseValueChanged(mValue);
            }
        }

        /// <summary>
        /// Gets/sets the date and time default value. Use the method SetDefaultValue() to the the current value to the default value.
        /// </summary>
        public System.DateTime DefaultValue
        {
            get
            {
                return mDefaultValue;
            }
            set
            {
                mDefaultValue = value;
            }
        }

        /// <summary>
        /// Gets a human readable text rappresesentation of the date and time value.
        /// </summary>
        public override string ValueString
        {
            get
            {
                string dateTimeStr = "";

                dateTimeStr = mValue.ToString(Format);
                return dateTimeStr;
            }
        }

        /// <summary>
        /// Gets/sets the date and/or time style (Only date, only time, date and time).
        /// </summary>
        public DateTimeType Style
        {
            get
            {
                return mStyle;
            }
            set
            {
                mStyle = value;
                Format = "";
                switch (mStyle)
                {
                    case DateTimeType.OnlyDate:
                        Format = "dd/MM/yyyy";
                        break;
                    case DateTimeType.OnlyTime:
                        Format = "HH:mm:ss";
                        break;
                    case DateTimeType.DateAndTime:
                        Format = "HH:mm:ss - dd/MM/yyyy";
                        break;
                }
            }
        }

        #endregion

        #region Public Methods

        /// <summary>
        /// Sets the Value property as the current DefaultValue property.
        /// </summary>
        public override void SetDefaultValue()
        {
            this.Value = this.DefaultValue;
        }

        #endregion

        #region Public events

        /// <summary>
        /// Delegate for ValueChanged event.
        /// </summary>
        /// <param name="sender">Object sender.</param>
        /// <param name="value">Last DateTime value.</param>
        public delegate void ValueChangedHandle(object sender, System.DateTime value);

        /// <summary>
        /// This event accour when the DateTime value property change.
        /// </summary>
        public event ValueChangedHandle ValueChanged;

        private void RaiseValueChanged(System.DateTime value)
        {
            if (ValueChanged != null)
                // Raise event
                ValueChanged(this, value);
        }

        #endregion

    }

    #endregion

}

