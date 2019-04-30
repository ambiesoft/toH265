using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace Ambiesoft
{
    public class toH265Helper
    {
        public static string GetCommonFilename(List<string> all)
        {
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
            return outfilename;
        }

        public static string human_format(string format)
        {
            if (format == null)
                return null;

            return format.Split(',')[0];
        }
    }
}
