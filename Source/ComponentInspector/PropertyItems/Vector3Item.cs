using System;
using System.Collections.Generic;
using System.Linq;

namespace Inspector
{
    public class Vector3
    {
        private float x = 0.0f;
        private float y = 0.0f;
        private float z = 0.0f;

        public Vector3(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        public float X { get { return x; } set { x = value; } }

        public float Y { get { return y; } set { y = value; } }

        public float Z { get { return z; } set { z = value; } }

        public override bool Equals(object obj)
        {
            if (obj == null || (this.GetType() != obj.GetType()))
                return false;

            return (this.X == ((Vector3)obj).X) && (this.Y == ((Vector3)obj).Y) && (this.Z == ((Vector3)obj).Z);
        }

        public override int GetHashCode()
        {
            return base.GetHashCode();
        }
    }

    public class Vector3Item : BaseItem
    {
        private Vector3 m_Value;

        public Vector3Item(string categoryKey, string itemKey, string variable, Vector3 value)
        {
            this.CategoryName = categoryKey;
            this.UIName = itemKey;
            this.VariableName = variable;
            this.Value = value;
        }

        public Vector3 Value
        {
            get
            {
                return m_Value;
            }
            set
            {
                m_Value = value;
                RaiseValueChanged(m_Value);
            }
        }

        public override string ValueString
        {
            get
            {
                return m_Value.X + "," + m_Value.Y + "," + m_Value.Z;
            }
            set
            {
                try
                {
                    string[] elements = ((string)value).Split(',');
                    if (elements.Length == 3)
                    {
                        m_Value = new Vector3(Convert.ToSingle(elements[0]), Convert.ToSingle(elements[1]), Convert.ToSingle(elements[2]));
                    }
                }
                catch
                {
                    throw new ArgumentException("Can not convert '" + (string)value + "' to type Vector3");
                }
            }
        }

        public delegate void ValueChangedHandle(object sender, Vector3 value);

        public event ValueChangedHandle ValueChanged;

        private void RaiseValueChanged(Vector3 value)
        {
            if (ValueChanged != null)
                ValueChanged(this, value);
        }
    }
}
