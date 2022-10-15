using System;
using System.Collections.Generic;

using Microsoft.VisualStudio.TestTools.UnitTesting;

using Ambiesoft;

namespace toH265Test
{
    [TestClass]
    public class UnitTestHelper
    {
        [TestMethod]
        public void TestGetTSFromHMS()
        {
            {
                TimeSpan ts = toH265Helper.GetTSFromHMS("00:00:00");
                Assert.AreEqual(ts, TimeSpan.Zero);
            }
            {
                TimeSpan ts = toH265Helper.GetTSFromHMS("01:00:00");
                Assert.AreNotEqual(ts, TimeSpan.Zero);
                Assert.AreEqual(ts.Hours, 1);
                Assert.AreEqual(ts.Minutes, 0);
                Assert.AreEqual(ts.Seconds, 0);
            }
            {
                TimeSpan ts = toH265Helper.GetTSFromHMS("30:30:30");
                Assert.AreNotEqual(ts, TimeSpan.Zero);
                Assert.AreEqual(ts.Days, 1);
                Assert.AreEqual(ts.Hours, 6);
                Assert.AreEqual(ts.Minutes, 30);
                Assert.AreEqual(ts.Seconds, 30);
            }
            {
                TimeSpan ts = toH265Helper.GetTSFromHMS("00:00:00.00");
                Assert.AreEqual(ts, TimeSpan.Zero);
            }
            {
                TimeSpan ts = toH265Helper.GetTSFromHMS("00:00:00.00000111");
                Assert.AreEqual(ts, TimeSpan.Zero);
            }
            {
                TimeSpan ts = toH265Helper.GetTSFromHMS("00:00:00.11");
                Assert.AreNotEqual(ts, TimeSpan.Zero);
                Assert.AreEqual(ts.Milliseconds, 110);
            }
            {
                TimeSpan ts = toH265Helper.GetTSFromHMS("25:01:02.1234");
                Assert.AreNotEqual(ts, TimeSpan.Zero);
                Assert.AreEqual(ts.Days, 1);
                Assert.AreEqual(ts.Hours, 1);
                Assert.AreEqual(ts.Minutes, 1);
                Assert.AreEqual(ts.Seconds, 2);
                Assert.AreEqual(ts.Milliseconds, 123);
            }
        }
    }
}
