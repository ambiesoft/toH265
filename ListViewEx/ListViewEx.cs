using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace ListViewCustomReorder
{
    /// <summary>
    /// A slightly extended version of the standard ListView.
    /// It has two additional properties to draw an insertion line before or after an item.
    /// </summary>
    public class ListViewEx : ListView
    {
        // from WinUser.h
        private const int WM_PAINT = 0x000F;

        public ListViewEx()
        {
            // Reduce flicker
            SetStyle(ControlStyles.OptimizedDoubleBuffer, true);

            this.MouseDown += ListViewEx_MouseDown;
            this.MouseMove += ListViewEx_MouseMove;
            this.MouseUp += ListViewEx_MouseUp;
        }

      
        private int _LineBefore = -1;
        /// <summary>
        /// If set to a value >= 0, an insertion line is painted before the item with the given index.
        /// </summary>
        public int LineBefore
        {
            get { return _LineBefore; }
            set { _LineBefore = value; }
        }

        private int _LineAfter = -1;
        /// <summary>
        /// If set to a value >= 0, an insertion line is painted after the item with the given index.
        /// </summary>
        public int LineAfter
        {
            get { return _LineAfter; }
            set { _LineAfter = value; }
        }

        protected override void WndProc(ref Message m)
        {
            base.WndProc(ref m);

            // We have to take this way (instead of overriding OnPaint()) because the ListView is just a wrapper
            // around the common control ListView and unfortunately does not call the OnPaint overrides.
            if (m.Msg == WM_PAINT)
            {
                if (LineBefore >= 0 && LineBefore < Items.Count)
                {
                    Rectangle rc = Items[LineBefore].GetBounds(ItemBoundsPortion.Entire);
                    DrawInsertionLine(rc.Left, rc.Right, rc.Top);
                }
                if (LineAfter >= 0 && LineBefore < Items.Count)
                {
                    Rectangle rc = Items[LineAfter].GetBounds(ItemBoundsPortion.Entire);
                    DrawInsertionLine(rc.Left, rc.Right, rc.Bottom);
                }
            }
        }

        /// <summary>
        /// Draw a line with insertion marks at each end
        /// </summary>
        /// <param name="X1">Starting position (X) of the line</param>
        /// <param name="X2">Ending position (X) of the line</param>
        /// <param name="Y">Position (Y) of the line</param>
        private void DrawInsertionLine(int X1, int X2, int Y)
        {
            using (Graphics g = this.CreateGraphics())
            {
                g.DrawLine(Pens.Red, X1, Y, X2 - 1, Y);

                Point[] leftTriangle = new Point[3] {
                            new Point(X1,      Y-4),
                            new Point(X1 + 7,  Y),
                            new Point(X1,      Y+4)
                        };
                Point[] rightTriangle = new Point[3] {
                            new Point(X2,     Y-4),
                            new Point(X2 - 8, Y),
                            new Point(X2,     Y+4)
                        };
                g.FillPolygon(Brushes.Red, leftTriangle);
                g.FillPolygon(Brushes.Red, rightTriangle);
            }
        }

        // The LVItem being dragged
        private ListViewItem _itemDnD = null;
        private bool _mouseDowning;
        private int _mouseDownStartTick;
        private void ListViewEx_MouseDown(object sender, MouseEventArgs e)
        {
            _mouseDowning = true;
            _mouseDownStartTick = Environment.TickCount;
            _itemDnD = GetItemAt(e.X, e.Y);
        }

        private void ListViewEx_MouseMove(object sender, MouseEventArgs e)
        {
            if (_mouseDowning && (Environment.TickCount - _mouseDownStartTick) > 100)
            {

            }
            else
            {
                _itemDnD = null;
            }
            if (_itemDnD == null)
                return;

            // drag begines
            ListViewItemSorter = null;

            // Show the user that a drag operation is happening
            Cursor = Cursors.Hand;

            // calculate the bottom of the last item in the LV so that you don't have to stop your drag at the last item
            int lastItemBottom = Math.Min(e.Y, Items[Items.Count - 1].GetBounds(ItemBoundsPortion.Entire).Bottom - 1);

            // use 0 instead of e.X so that you don't have to keep inside the columns while dragging
            ListViewItem itemOver = GetItemAt(0, lastItemBottom);

            if (itemOver == null)
                return;

            Rectangle rc = itemOver.GetBounds(ItemBoundsPortion.Entire);
            if (e.Y < rc.Top + (rc.Height / 2))
            {
                LineBefore = itemOver.Index;
                LineAfter = -1;
            }
            else
            {
                LineBefore = -1;
                LineAfter = itemOver.Index;
            }

            // invalidate the LV so that the insertion line is shown
            Invalidate();
        }

        private void ListViewEx_MouseUp(object sender, MouseEventArgs e)
        {
            _mouseDowning = false;
            if (_itemDnD == null)
                return;

            try
            {
                // calculate the bottom of the last item in the LV so that you don't have to stop your drag at the last item
                int lastItemBottom = Math.Min(e.Y, Items[Items.Count - 1].GetBounds(ItemBoundsPortion.Entire).Bottom - 1);

                // use 0 instead of e.X so that you don't have to keep inside the columns while dragging
                ListViewItem itemOver = GetItemAt(0, lastItemBottom);

                if (itemOver == null)
                    return;

                Rectangle rc = itemOver.GetBounds(ItemBoundsPortion.Entire);

                // find out if we insert before or after the item the mouse is over
                bool insertBefore;
                if (e.Y < rc.Top + (rc.Height / 2))
                {
                    insertBefore = true;
                }
                else
                {
                    insertBefore = false;
                }

                if (_itemDnD != itemOver) // if we dropped the item on itself, nothing is to be done
                {
                    if (insertBefore)
                    {
                        Items.Remove(_itemDnD);
                        Items.Insert(itemOver.Index, _itemDnD);
                    }
                    else
                    {
                        Items.Remove(_itemDnD);
                        Items.Insert(itemOver.Index + 1, _itemDnD);
                    }
                }

                // clear the insertion line
                LineAfter =
                LineBefore = -1;

                Invalidate();

            }
            finally
            {
                // finish drag&drop operation
                _itemDnD = null;
                Cursor = Cursors.Default;
            }
        }

    }
}
