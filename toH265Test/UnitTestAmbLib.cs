using System;
using System.Collections.Generic;

using Microsoft.VisualStudio.TestTools.UnitTesting;
using Ambiesoft;

namespace toH265Test
{
    [TestClass]
    public class UnitTestAmbLib
    {
        [TestMethod]
        public void TestAmbLib()
        {
            Assert.AreEqual<string>("100", AmbLib.GetRatioString(100, 100));
            Assert.AreEqual<string>("1", AmbLib.GetRatioString(1, 100));
            Assert.IsTrue(AmbLib.GetRatioString(821874467, 4017871354).StartsWith("20"));
            Assert.IsTrue(AmbLib.GetRatioString((double)821874467, (double)4017871354).StartsWith("20"));
            Assert.IsTrue(AmbLib.GetRatioString(821874467L, 4017871354L).StartsWith("20"));

            string s = AmbLib.GetRatioString(1, 0);
            Assert.IsTrue(s.Contains("∞") || s.Contains("Infinity"));
        }
	}
}
