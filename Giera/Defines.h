#pragma once
#define ANDROID_BUILD 1
#if ANDROID_BUILD
#define SAVE_FILES_PATH "save_files"
#else
#define SAVE_FILES_PATH "../../save_files"
#endif