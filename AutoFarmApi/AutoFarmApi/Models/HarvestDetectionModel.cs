using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using CNTK;
using AutoFarmApi.Extentions;
using System.Globalization;
using System.IO;

namespace AutoFarmApi.Models
{
    public class HarvestDetectionModel
    {
        Dictionary<int, string> classesMap = new Dictionary<int, string>();
        string domainBaseDirectory = AppDomain.CurrentDomain.BaseDirectory;

        public HarvestDetectionModel()
        {
            classesMap.Add(0, "HARVEST");
            classesMap.Add(1, "NO_HARVEST");
            classesMap.Add(2, "NO_PLANT");
        }

        public string GetAction()
        {
            return "NO_PLANT";
        }

        public string GetAction(Bitmap bmp)
        {
            string modelFilePath = Path.Combine(domainBaseDirectory, @"Resources\MixDetection800-07252017.model");
            Function modelFunc = Function.Load(modelFilePath, DeviceDescriptor.CPUDevice);

            Variable inputVar = modelFunc.Arguments.Single();

            // Get shape data for the input variable
            NDShape inputShape = inputVar.Shape;
            int imageWidth = inputShape[0];
            int imageHeight = inputShape[1];
            int imageChannels = inputShape[2];
            int imageSize = inputShape.TotalSize;

            // The model has only one output.
            // If the model have more than one output, use the following way to get output variable by name.
            // Variable outputVar = modelFunc.Outputs.Where(variable => string.Equals(variable.Name, outputName)).Single();
            Variable outputVar = modelFunc.Output;

            var inputDataMap = new Dictionary<Variable, Value>();
            var outputDataMap = new Dictionary<Variable, Value>();
            
            var resized = bmp.Resize((int)imageWidth, (int)imageHeight, true);
            List<float> resizedCHW = resized.ParallelExtractCHW();

            // Create input data map
            var inputVal = Value.CreateBatch(inputVar.Shape, resizedCHW, DeviceDescriptor.CPUDevice);
            inputDataMap.Add(inputVar, inputVal);

            // Create ouput data map. Using null as Value to indicate using system allocated memory.
            // Alternatively, create a Value object and add it to the data map.
            outputDataMap.Add(outputVar, null);

            // Start evaluation on the device
            modelFunc.Evaluate(inputDataMap, outputDataMap, DeviceDescriptor.CPUDevice);

            // Get evaluate result as dense output
            var outputVal = outputDataMap[outputVar];
            var outputData = outputVal.GetDenseData<float>(outputVar);
            
            return GetClassFromSequence(outputVar.Shape.TotalSize, outputData);
        }

        /// <summary>
        /// Print out the evalaution results.
        /// </summary>
        /// <typeparam name="T">The data value type</typeparam>
        /// <param name="sampleSize">The size of each sample.</param>
        /// <param name="outputBuffer">The evaluation result data.</param>
        internal string GetClassFromSequence<T>(int sampleSize, IList<IList<T>> outputBuffer)
        {
            string outClass = "";
            int outputSampleSize = sampleSize;
            int championSeq = -1;

            foreach (var seq in outputBuffer)
            {
                if (seq.Count % outputSampleSize != 0)
                {
                    outClass = "NO_ACTION";
                }

                int sampleNo = 0;
                int highestPositive = 0;


                foreach (var element in seq)
                {
                    int value = 0;
                    value = (int)float.Parse(element.ToString(), CultureInfo.InvariantCulture.NumberFormat);

                    if (value > 0)
                    {
                        if (value > highestPositive)
                        {
                            highestPositive = value;
                            championSeq = sampleNo;
                        }
                    }
                    sampleNo++;
                }
            }

            if (championSeq == -1)
            {
                outClass = "NO_ACTION";
            }

            outClass = classesMap[championSeq];
            return outClass;
        }

        private static void ThrowIfFileNotExist(string sampleImage, string v)
        {

        }
    }
}
