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
    // class (eg. PropertyItemString) but the base class must be always PropertyItemGeneric

    #region ProgressBar property item class

    /// <summary>
    /// ProgressBar property item class.
    /// </summary>
    public class PropertyItemProgressBar : PropertyItemGeneric
    {

        #region Private internal var./properties

        private System.Drawing.Color mBackgroundColor = System.Drawing.Color.LightGray;
        private System.Drawing.Color mBarColorNormalStart = System.Drawing.Color.LightBlue;
        private System.Drawing.Color mBarColorNormalEnd = System.Drawing.Color.Blue;
        private System.Drawing.Color mBarColorAtMaximumStart = System.Drawing.Color.LightGreen;
        private System.Drawing.Color mBarColorAtMaximumEnd = System.Drawing.Color.Green;
        private bool mGradientFill = true;
        private int mValue = 0;
        private int mDefaultValue = 0;
        private int mMinValue = 0;
        private int mMaxValue = 100;

        #endregion

        #region Constructors

        /// <summary>
        /// Constructor.
        /// </summary>
        public PropertyItemProgressBar()
        {
            this.Text = "New progress bar item";
            this.SetValidationRange(0, 100);
            this.Format = "%";
        }

        /// <summary>
        /// Constructor. The DefaultValue is the same as Value.
        /// </summary>
        public PropertyItemProgressBar(string text, int value)
        {
            this.Text = text;
            this.SetValidationRange(0, 100);
            this.DefaultValue = value;
            this.Value = value;
            this.Format = "%";
        }

        /// <summary>
        /// Constructor.
        /// </summary>
        public PropertyItemProgressBar(string text, int value, int defaultValue)
        {
            this.Text = text;
            this.SetValidationRange(0, 100);
            this.DefaultValue = defaultValue;
            this.Value = value;
            this.Format = "%";
        }

        #endregion

        #region Public properties

        /// <summary>
        /// Gets/sets the progress bar value
        /// </summary>
        public int Value
        {
            get
            {
                return mValue;
            }
            set
            {
                int oldValue = mValue;

                // Set only a valid value!
                if (value < mMinValue)
                    mValue = mMinValue;
                else
                    if (value > mMaxValue)
                        mValue = mMaxValue;
                    else
                        mValue = value;
                this.Changed = (oldValue != mValue);
            }
        }

        /// <summary>
        /// Gets/sets the default progressbar value. To sets the current Value to the DefaultValue use the SetDefaultValue() method.
        /// </summary>
        public int DefaultValue
        {
            get
            {
                return mDefaultValue;
            }
            set
            {
                // Set only a valid value!
                if (value < mMinValue)
                    mDefaultValue = mMinValue;
                else
                    if (value > mMaxValue)
                        mDefaultValue = mMaxValue;
                    else
                        mDefaultValue = value;
            }
        }

        /// <summary>
        /// Get the value as a formatted string. If the Format property is "%" return a perc.string value.
        /// </summary>
        public override string ValueString
        {
            get
            {
                if (this.Format == "%")
                    return ValuePercent.ToString("0") + " %";
                else
                    return Value.ToString(this.Format);
            }
        }

        /// <summary>
        /// Get the minimum valid value if ValidationRangeCheck is true
        /// </summary>
        public int Minimum
        {
            get
            {
                return mMinValue;
            }
        }

        /// <summary>
        /// Get the maximum valid value if ValidationRangeCheck is true
        /// </summary>
        public int Maximum
        {
            get
            {
                return mMaxValue;
            }
        }

        /// <summary>
        /// Gets/sets the current percent value
        /// </summary>
        public double ValuePercent
        {
            get
            {
                double perc;

                perc = (double)(this.Value - this.Minimum) / (double)(this.Maximum - this.Minimum);
                perc *= 100.0;
                return perc;
            }
            set
            {
                double perc;
                int intValue;

                if (value < 0.0)
                    perc = 0.0;
                else
                    if (value > 100.0)
                        perc = 100.0;
                    else
                        perc = value;
                intValue = this.Minimum + (int)((double)((this.Maximum - this.Minimum) / 100.0) * perc);
                //intValue = mMinimum + (int)(perc * (double)(mMaximum - mMinimum));
                this.Value = intValue;
            }
        }


        /// <summary>
        /// Get the current bar color / start color for gradient fill (depending from the current bar value)
        /// </summary>
        public System.Drawing.Color BarColorStart
        {
            get
            {
                if (this.Value == this.Maximum)
                    return this.BarColorAtMaximumStart;
                else
                    return this.BarColorNormalStart;
            }
        }

        /// <summary>
        /// Get the current bar final color for gradient fill (depending from the current bar value)
        /// </summary>
        public System.Drawing.Color BarColorEnd
        {
            get
            {
                if (this.Value == this.Maximum)
                    return this.BarColorAtMaximumEnd;
                else
                    return this.BarColorNormalEnd;
            }
        }

        /// <summary>
        /// Gets/sets the bar background color.
        /// </summary>
        public System.Drawing.Color BackgroundColor
        {
            get
            {
                return mBackgroundColor;
            }
            set
            {
                mBackgroundColor = value;
            }
        }

        /// <summary>
        /// Gets/sets the bar color (with partial progress done) for solid fill or the start color for gradient fill
        /// </summary>
        public System.Drawing.Color BarColorNormalStart
        {
            get
            {
                return mBarColorNormalStart;
            }
            set
            {
                mBarColorNormalStart = value;
            }
        }

        /// <summary>
        /// Gets/sets the bar color (with partial progress done) for gradient fill
        /// </summary>
        public System.Drawing.Color BarColorNormalEnd
        {
            get
            {
                return mBarColorNormalEnd;
            }
            set
            {
                mBarColorNormalEnd = value;
            }
        }

        /// <summary>
        /// Gets/sets the bar color (100% completed, full progress) for solid fill or the final (end) color for the gradient fill
        /// </summary>
        public System.Drawing.Color BarColorAtMaximumStart
        {
            get
            {
                return mBarColorAtMaximumStart;
            }
            set
            {
                mBarColorAtMaximumStart = value;
            }
        }

        /// <summary>
        /// Gets/sets the bar color (100% completed, full progress) for the gradient fill
        /// </summary>
        public System.Drawing.Color BarColorAtMaximumEnd
        {
            get
            {
                return mBarColorAtMaximumEnd;
            }
            set
            {
                mBarColorAtMaximumEnd = value;
            }
        }


        /// <summary>
        /// Gets/sets the if the bar color is a gradient color (true) or a standard solid color (false)
        /// </summary>
        public bool GradientFill
        {
            get
            {
                return mGradientFill;
            }
            set
            {
                mGradientFill = value;
            }
        }

        #endregion

        #region Public Methods

        /// <summary>
        /// Sets the validation range parameters. An exception accour if the minValue is equal to the maxValue.
        /// </summary>
        public void SetValidationRange(int minValue, int maxValue)
        {
            if (minValue == maxValue)
                // Error
                throw new ArgumentException("Parameters minValue and MaxValue can't be equal!", "minValue, maxValue");
            else
            {
                if (minValue > maxValue)
                {
                    // Swap values!
                    mMinValue = maxValue;
                    mMaxValue = minValue;
                }
                else
                {
                    mMinValue = minValue;
                    mMaxValue = maxValue;
                }
                // Correct invalid value!
                if (mValue < mMinValue) mValue = mMinValue;
                if (mValue > mMaxValue) mValue = mMaxValue;
            }
        }

        /// <summary>
        /// Sets the current default int value. To sets the current Value to the DefaultValue use the SetDefaultValue() method.
        /// </summary>
        public override void SetDefaultValue()
        {
            this.Value = this.DefaultValue;
        }


        #endregion

    }

    #endregion

}
