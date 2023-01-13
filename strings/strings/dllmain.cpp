// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "sys/stat.h"
#include "climits"
#include "fstream"


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

char* buffer=0;
unsigned long length=0;

//提取文件字符串
extern "C" _declspec(dllexport) char* GetString(char* fname);

//获取字符串长度
extern "C" _declspec(dllexport) unsigned long GetStringLength();

//释放缓冲区
extern "C" _declspec(dllexport) void RemoveString();



// 通过stat结构体 获得文件大小,单位字节
size_t getFileSize(const char* fileName) {

    if (fileName == NULL) {
        return 0;
    }

    // 这是一个存储文件(夹)信息的结构体，其中有文件大小和创建时间、访问时间、修改时间等
    struct stat statbuf;

    // 提供文件名字符串，获得文件属性结构体
    stat(fileName, &statbuf);

    // 获取文件大小
    size_t filesize = statbuf.st_size;

    return filesize;
}

using namespace std;

//获取处理过的字符串
char* GetString(char* fname)
{
    char ch;

    size_t fileSize = getFileSize(fname);
    if (fileSize > ULONG_MAX)   //文件太大,截断为4GB
        fileSize = ULONG_MAX;

    //文件大小为0
    if (!fileSize)
    {
        length = 0;
        return 0;
    }
   
    //打开文件
    ifstream file;
    file.open(fname, ios_base::binary);
    if (!file.is_open())    //文件打开失败
    {
        length = 0;
        return 0;
    }

    //申请缓冲区
    buffer = new char[fileSize+1];  //多申请一个字节,用于首个字符不可显示
    memset(buffer,0, fileSize+1);

    length = 1;

    //读取字节
    ch=file.get();
    while (file)
    {
        //判断数据
        if (ch > 31 && ch < 128)  //可显字符
        {
            buffer[length] = ch;
            length++;   //索引、长度+1
        }
        else if (buffer[length - 1] != '\n')    //考虑第一个字符不可显,多申请了一字节
        {
            buffer[length] = '\n';
            length++;
        }     

        ch = file.get();//读取下一个字符
       
    }
    if (!file.eof())
        length = 0;
    //关闭文件
    file.close();
    length--;  

    return buffer+1;    //指向首个字符

}

unsigned long GetStringLength()
{
    return length;
}


void RemoveString()
{
    delete [] buffer;
    buffer = 0;
    length = 0;
}

