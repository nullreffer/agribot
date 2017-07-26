using System.Drawing;
using AutoFarmApi.Models;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using AutoFarmApi.Controllers;
using System.Net;
using System.IO;
using System.Net.Http;

namespace AutoFarmApi.Tests
{
    [DeploymentItem(@"Resources\z3-400.model")]
    [TestClass]
     public class TestOutputClass
    {
        [TestMethod]
        public void TestNoPlantOutOutValue()
        {
            string imageName = @"C:\Users\jadesai\Pictures\128_128IMG_1356.jpg"; // @"C:\Users\nirama\Documents\Visual Studio 2015\Projects\MINST_Eval\64_64IMG_1351.JPG";
            Bitmap bmp = new Bitmap(Bitmap.FromFile(imageName));

            HarvestDetectionModel model = new HarvestDetectionModel();

            string val = model.GetAction(bmp);
            Assert.Equals(val, "NO_PLANT");
        }

        [TestMethod]
        public void TestWebService()
        {
            string filename = @"C:\Users\nirama\Documents\Visual Studio 2015\Projects\MINST_Eval\64_64IMG_1351.JPG";
            string address = "http://localhost:1208/api/actiondetection";
            using (WebClient client = new WebClient())
            {
                client.UploadFile(address, filename);
            }
           
        }
    }
}
