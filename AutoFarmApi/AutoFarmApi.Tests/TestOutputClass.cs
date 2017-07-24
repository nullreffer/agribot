using System;
using System.Drawing;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.Web;
using AutoFarmApi.Models;

namespace AutoFarmApi.Tests
{
    [TestClass]
     public class TestOutputClass
    {
        [TestMethod]
        public void TestNoPlantOutOutValue()
        {
            string imageName = @"C:\Users\nirama\Documents\Visual Studio 2015\Projects\MINST_Eval\128_128IMG_1349.JPG";
           

            HarvestDetectionModel model = new HarvestDetectionModel();

            string val = model.GetAction(imageName);
            Assert.Equals(val, "NO_PLANT");
        }
    }
}
