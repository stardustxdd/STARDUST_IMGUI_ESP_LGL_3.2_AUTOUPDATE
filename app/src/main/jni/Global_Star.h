#ifndef GLOBAL_STAR_H
#define GLOBAL_STAR_H

bool g_Initialized;
bool isLibLoaded;

int glWidth, glHeight = 0;
void *m_EGL = NULL;

// HERE WE DEFINE OUR ESP VARIABLES


// Global variables for control
float radius = 50.0f; 
float circleThickness = 1.5f;
float esplineThickness = 1.5f;
float boxlineThickness = 1.5f; 
float crosshairThickness = 1.5f;
float healthBarWidth = 5.0f; 

float crosshairSize = 20.0f; 
float textSize = 20.0f; 
ImU32 boxColor = IM_COL32(255, 0, 0, 255); 
ImU32 lineColor = IM_COL32(255, 255, 255, 255);
ImU32 textColor = IM_COL32(255, 255, 255, 255);
ImU32 crosshairColor = IM_COL32(255, 0, 0, 255); 
ImU32 distanceTextColor = IM_COL32(255, 255, 0, 255); 


bool isESP = false;
bool isESPLine = false;
bool isESPBox = false;
bool isESPCircle = false;
bool isFPSCounterEnabled = false;
bool isShowPlayerCount = false;
bool isCrosshairEnabled = false;
bool isESPName = false;
bool isESPHealth = false;
bool isESPDistance = false;
bool isESPSkeleton = false;

#endif
