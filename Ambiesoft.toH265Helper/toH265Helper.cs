using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Ambiesoft
{
    public class toH265Helper
    {
        public static string GetCommonFilename(string[] all)
        {
            if (all.Length == 0)
                return null;
            string ext = Path.GetExtension(all[0]);
            string outfilename = null;
            foreach (string file in all)
            {
                FileInfo fi = new FileInfo(file);
                if (outfilename == null)
                    outfilename = fi.Name;
                else
                {
                    int isame = 0;
                    try
                    {
                        for (int i = 0; i < fi.Name.Length; ++i)
                        {
                            if (fi.Name[i] == outfilename[i])
                                isame = i + 1;
                            else
                                break;
                        }
                    }
                    catch (Exception) { }

                    outfilename = outfilename.Substring(0, isame);
                }
            }
            return outfilename == null ? null : outfilename + ext;
        }

        public static string human_format(string format)
        {
            if (format == null)
                return null;

            return format.Split(',')[0];
        }
        /// <summary>
        /// Format fps like "29.97"
        /// </summary>
        /// <param name="fps"></param>
        /// <returns></returns>
        public static string FormatFPS(double fps)
        {
            return Ambiesoft.AmbLib.FormatSize(fps);
        }



        public static Rectangle GetSubItemBounds(
            ListView lv, ListViewItem Item, int SubItem,
            int[] order)
        {
            Rectangle subItemRect = Rectangle.Empty;

            if (Item == null)
                throw new ArgumentNullException("Item");

            //int[] order = GetColumnOrder();
            //if (order == null) // No Columns
            //    return subItemRect;

            if (SubItem >= order.Length)
                throw new IndexOutOfRangeException("SubItem " + SubItem + " out of range");

            // Retrieve the bounds of the entire ListViewItem (all subitems)
            Rectangle lviBounds = Item.GetBounds(ItemBoundsPortion.Entire);
            int subItemX = lviBounds.Left;

            // Calculate the X position of the SubItem.
            // Because the columns can be reordered we have to use Columns[order[i]] instead of Columns[i] !
            ColumnHeader col;
            int i;
            for (i = 0; i < order.Length; i++)
            {
                col = lv.Columns[order[i]];
                if (col.Index == SubItem)
                    break;
                subItemX += col.Width;
            }

            subItemRect = new Rectangle(subItemX, lviBounds.Top, lv.Columns[order[i]].Width, lviBounds.Height);

            return subItemRect;
        }

        // https://stackoverflow.com/a/4669343
        public static void setupToolTip(ToolTip toolTip, Font font)
        {
            toolTip.OwnerDraw = true;
            toolTip.Draw += ToolTip_Draw;
            toolTip.Popup += ToolTip_Popup;
            toolTip.UseAnimation = true;
            toolTip.AutoPopDelay = 500;
            toolTip.AutomaticDelay = 500;
            toolTip.Tag = font;
        }

        private static readonly int TOOLTIP_XOFFSET = 2;
        private static readonly int TOOLTIP_YOFFSET = 2;
        private static void ToolTip_Popup(object sender, PopupEventArgs e)
        {
            ToolTip toolTip = (ToolTip)sender;
            e.ToolTipSize = TextRenderer.MeasureText(toolTip.ToolTipTitle,
                (Font)toolTip.Tag);
            e.ToolTipSize = new Size(
                e.ToolTipSize.Width + TOOLTIP_XOFFSET,
                e.ToolTipSize.Height + TOOLTIP_YOFFSET);
        }

        private static void ToolTip_Draw(object sender, DrawToolTipEventArgs e)
        {
            ToolTip toolTip = (ToolTip)sender;
            Rectangle bounds = e.Bounds;
            bounds.Offset(TOOLTIP_XOFFSET, TOOLTIP_YOFFSET);
            DrawToolTipEventArgs newArgs = new DrawToolTipEventArgs(
                e.Graphics, 
                e.AssociatedWindow,
                e.AssociatedControl, 
                bounds, 
                e.ToolTipText, 
                toolTip.BackColor,
                toolTip.ForeColor,
                (Font)toolTip.Tag);
            newArgs.DrawBackground();
            newArgs.DrawBorder();
            newArgs.DrawText(TextFormatFlags.TextBoxControl);
        }

        public static TimeSpan GetTSFromHMS(string strHMS)
        {
            string[] arrayHMS = strHMS.Split(':');
            return new TimeSpan(
                int.Parse(arrayHMS[0]),
                int.Parse(arrayHMS[1]),
                int.Parse(arrayHMS[2]));
        }
    }
}
