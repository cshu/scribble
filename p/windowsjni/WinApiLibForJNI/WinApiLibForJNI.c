#define UNICODE

#include <jni.h>
#include <windows.h>

JNIEXPORT jbyteArray JNICALL Java_griddoor_util_WinApiWrapper_GetFileTime
  (JNIEnv *env, jclass jclassv, jbyteArray fullFilenameTerminated)
{
    jbyteArray timeCAWObject = (*env)->NewByteArray(env,24);
    jbyte *timeCreateAccessWrite = (*env)->GetByteArrayElements(env, timeCAWObject, NULL);
    jbyte *fft = (*env)->GetByteArrayElements(env, fullFilenameTerminated, NULL);
    HANDLE hFile;
    hFile = CreateFile((LPCSTR)fft, GENERIC_READ, FILE_SHARE_READ, NULL,
        OPEN_EXISTING, 0, NULL);
    if(hFile == INVALID_HANDLE_VALUE)
    {
        memset(timeCreateAccessWrite,0,24);
    }
    else{
        FILETIME ftcr, ftac, ftwr;
        if(GetFileTime(hFile, &ftcr, &ftac, &ftwr)){
            memcpy(timeCreateAccessWrite, &ftcr, 8);
            memcpy((LPFILETIME)timeCreateAccessWrite+1, &ftac, 8);
            memcpy((LPFILETIME)timeCreateAccessWrite+2, &ftwr, 8);
        } else {
            memset(timeCreateAccessWrite,0,24);
        }
        CloseHandle(hFile);
    }
    (*env)->ReleaseByteArrayElements(env, timeCAWObject, timeCreateAccessWrite, 0);
    return timeCAWObject;
}