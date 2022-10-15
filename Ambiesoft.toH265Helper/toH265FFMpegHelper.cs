using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Ambiesoft
{
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
                return true;
            }
            tsTime = TimeSpan.Zero;
            dblSpeed = 0;
            return false;
        }

        public static TimeSpan GetTSFromHMS(string strHMS)
        {
            string[] arrayHMS = strHMS.Split(':');
            return new TimeSpan(
                int.Parse(arrayHMS[0]),
                int.Parse(arrayHMS[1]),
                int.Parse(arrayHMS[2]));
        }
    };
}
