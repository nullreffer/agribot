
using System.Net;
using System.Net.Http;
using System.Web.Http;
using Swashbuckle.Swagger.Annotations;
using System.Threading.Tasks;
using System.Collections.Generic;
using System;
using System.Drawing;
using AutoFarmApi.Models;
using System.IO;

namespace AutoFarmApi.Controllers
{
    public class ActionDetectionController : ApiController
    {
        public object WebImage { get; private set; }

        // POST api/values
       
        [HttpGet]
        public HttpResponseMessage Get()
        {
            return Request.CreateResponse(HttpStatusCode.OK, "Hello");
        }

        // POST api/values
        [HttpPost]
        public HttpResponseMessage Post()
        {
            Dictionary<string, object> dict = new Dictionary<string, object>();
            string action = "NO_ACTION";
            try
            {

                var httpRequest = System.Web.HttpContext.Current.Request;

                foreach (string file in httpRequest.Files)
                {
                    HttpResponseMessage response = Request.CreateResponse(HttpStatusCode.Created);

                    var postedFile = httpRequest.Files[file];
                    if (postedFile != null && postedFile.ContentLength > 0)
                    {

                        int MaxContentLength = 32 * 32 * 1; //Size = 1 MB

                        IList<string> AllowedFileExtensions = new List<string> { ".jpg", ".gif", ".png" };
                        var ext = postedFile.FileName.Substring(postedFile.FileName.LastIndexOf('.'));
                        var extension = ext.ToLower();
                        if (!AllowedFileExtensions.Contains(extension))
                        {

                            var message = string.Format("Please Upload image of type .jpg,.gif,.png.");

                            dict.Add("error", message);
                            return Request.CreateResponse(HttpStatusCode.BadRequest, dict);
                        }
                        else if (postedFile.ContentLength > MaxContentLength)
                        {

                            var message = string.Format("Please Upload a file upto 1 mb.");

                            dict.Add("error", message);
                            return Request.CreateResponse(HttpStatusCode.BadRequest, dict);
                        }
                        else
                        {
                            var i = Image.FromStream(postedFile.InputStream, true, true);
                                
                            var bitmap = new Bitmap(i);
                            HarvestDetectionModel model = new HarvestDetectionModel();
                            action = model.GetAction(bitmap);
                            return Request.CreateResponse(HttpStatusCode.OK, action);
                        }
                    }
                }
                
                return Request.CreateResponse(HttpStatusCode.NotFound, dict);
            }
            catch (Exception ex)
            {
                var res = string.Format("some Message " + ex.ToString());
                dict.Add("error", res);
                return Request.CreateResponse(HttpStatusCode.NotFound, dict);
            }
        }
            
    }
}
