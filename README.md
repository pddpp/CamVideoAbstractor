# CamVideoAbstractor
 This program is implemented in 2012 with the use of Gaussian Mixture Modeling(GMM), morphology filter(openCV) and other useful functions in openCV.
This program compresses camera video from hours to minutes, while preserving the video characters, vehicles and other activities of interest to the details.

The whole process can be divided as follows: 
1. background modeling, using Gaussian mixture background modeling algorithm to monitor the regional background model
2. motion detection, the module will monitor the video event details (e.g. prospects) to extract and details for each activity unit (e.g. activities or objects) with the time stamp function.
3. video concentration(overlapper). Intelligently adjust the magnification of the video concentrated, and save the results of compression.
