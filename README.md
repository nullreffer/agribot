# Description
Agribot was created as a hackthon project. We modded an RC car hooked up a raspberry PI and arduino to demo an auto-harvesting bot.

# Components
- API: The API that takes in an image and determines if there is a plant in the image and if it is ready to be harvested. It contains a model that we trained using CNTK.
- BotController: The arduino code which receives signals from the raspberry PI and acts upon it, such as moving forward, moving the arms, taking images, etc.
- FarmController: The raspberry PI code which bridges the input from camera and API to send signals and manage state of the hardware