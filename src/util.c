// This is a fix for NVIDIA Optimus laptop switching during development
// 1 = discrete GPU (NVIDIA Optimus). 0 = integrated GPU
#define USE_DISCRETE_GPU 0

#ifdef _WIN32
__declspec(dllexport) unsigned long NvOptimusEnablement = USE_DISCRETE_GPU;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = USE_DISCRETE_GPU;
#endif
