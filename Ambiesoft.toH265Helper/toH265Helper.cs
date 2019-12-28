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

    public static class FFMpegHelper
    {
        static System.Text.RegularExpressions.Regex regFFMpeg_ = new System.Text.RegularExpressions.Regex(
				// "frame=.*fps=.*size=.*time=(?<time>.*)\\.\\d\\d.*speed=\\s*(?<speed>.*)"
				"frame=.*fps=.*size=.*time=(?<time>.*)\\s+bitrate=.*speed=\\s*(?<speed>.*)"
			);

        public static bool GetInfoFromFFMpegoutput(string text, out TimeSpan tsTime, out double dblSpeed)
        {
            if (regFFMpeg_.IsMatch(text))
            {
                System.Text.RegularExpressions.Match match = regFFMpeg_.Match(text);
                string timeValue = match.Groups["time"].Value;
                if (!string.IsNullOrEmpty(timeValue) && timeValue[0] == '-')
                    timeValue = "00:00:00.00";

                //DateTime dtTime = DateTime::ParseExact(timeValue, L"hh:mm:ss.ff",
                //	System::Globalization::CultureInfo::InvariantCulture);
                //tsTime = dtTime - dtTime.Date;

                tsTime = TimeSpan.Parse(timeValue);

                string speedValue = match.Groups["speed"].Value;
                if (speedValue == "N/A")
                {
                    dblSpeed = 0;
                }
                else
                {
                    speedValue = speedValue.Trim().TrimEnd('x');
                    if (!double.TryParse(speedValue, out dblSpeed))
                        return false;
                }
                //UpdateTitleTS(tsTime, dblSpeed);
                //txtLogOut->Text = text;
                return true;
            }
            tsTime = TimeSpan.Zero;
            dblSpeed = 0;
            return false;
        }
    };
}
