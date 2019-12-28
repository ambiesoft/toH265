using System;
using System.Collections.Generic;

using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace toH265Test
{
    [TestClass]
    public class UnitTestFFMpegRegex
    {
        [TestMethod]
        public void TestFFMpeg()
        {
			//List<string> ls = new List<string>();
			//ls.Add();
			//ls.Add();
			TimeSpan ts;
			double dbSpeed;

			{
				Ambiesoft.FFMpegHelper.GetInfoFromFFMpegoutput(
					"frame=   61 fps= 18 q=-0.0 size=       0kB time=00:00:02.08 bitrate=   0.2kbits/s dup=1 drop=0 speed=0.619x",
					out ts,
					out dbSpeed);
				Assert.AreEqual(ts.TotalMilliseconds, 2080);
				Assert.AreEqual(dbSpeed, 0.619);
			}
			{
				Ambiesoft.FFMpegHelper.GetInfoFromFFMpegoutput(
					"frame=    0 fps=0.0 q=0.0 size=       0kB time=-577014:32:22.77 bitrate=  -0.0kbits/s speed=N/A",
					out ts,
					out dbSpeed);
				Assert.AreEqual(ts.TotalMilliseconds, 0);
				Assert.AreEqual(dbSpeed, 0);
			}
		}
	}
}
