#include "StrHC.h"
#ifndef FREE_ZER_H
#define FREE_ZER_H
// Language: cpp
namespace freezer
{
    
    //判断文件是否存在
    bool IsFileExists(const std::string &name)
    {
        return (access(name.c_str(), F_OK) == -1);
    }

    //判断目录是否存在
    bool IsDirExist(const std::string &path)
    {
        struct stat info;
        if (stat(path.c_str(), &info) != 0)
        {
            return false;
        }
        return (info.st_mode & S_IFDIR) != 0;
    }

    //创建文件夹
    bool MakePath(const std::string &path)
    {
        //权限777
        mode_t mode = 0777;
        int ret = mkdir(path.c_str(), mode);
        if (ret == 0)
            return true;
        switch (errno)
        {
        case ENOENT:
        {
            int pos = path.find_last_of('/');
            if (pos == std::string::npos)
                return false;
            if (!MakePath(path.substr(0, pos)))
                return false;
        }
            //创建目录
            return 0 == mkdir(path.c_str(), mode);
        case EEXIST:
            //判断目录存在
            return IsDirExist(path);
        default:
            return false;
        }
    }

    // popen执行shell命令 不返回结果
    void ExcutingAnOrder(const char *shell)
    {
        FILE *fp = popen(shell, "r");
        //关闭僵尸进程
        signal(SIGCHLD, SIG_IGN);
        pclose(fp);
    }

    // popen执行shell命令 返回结果
    void ExecuteShell(const char *shell, char *result)
    {
        FILE *fp;
        char buffer[FILE_CACHE_ZX];
        fp = popen(shell, "r");
        fread(buffer, sizeof(char),sizeof(buffer), fp);
        //关闭僵尸进程
        signal(SIGCHLD, SIG_IGN);
        strcpy(result, buffer);
        pclose(fp);
    }

    //写入文件
    void WriteFile(const char *path,const char data[])
    {
        // FILE *fp = fopen(path.c_str(), "a"); 经过汇编分析发现此代码与下面代码无差异
        FILE *fp;
        fp = fopen(path, "wb");
        fwrite(data, strlen(data), 1, fp);
        fclose(fp);
    }

    //写入文件 判断是否打开成功
    void WriteFiles(const char *path,const char data[])
    {
        FILE *fp = fopen(path, "wb");
        if (fp == NULL)
        {
            return;
        }
        fwrite(data, strlen(data), 1, fp);
        fclose(fp);
    }

    //读取文件 返回文件内容
    void ReadFile(const char *fileName, char *data)
    {
        char buffer[1024] = {0};
        FILE *fp = fopen(fileName, "r");
        fread(buffer, sizeof(char), sizeof(buffer), fp);
        strcpy(data, buffer);
        fclose(fp);
    }

    //kill进程
    void KillProcess(const char *name)
    {
        char buffer[FILE_CACHE_X];
        sprintf(buffer, "kill -9 %d", atoi(name));
        ExcutingAnOrder(buffer);
    }

    //初始化内核
    void InitializationKernel(const char *path)
    {
        //判断文件是否存在 不存在则创建并且初始化freezer
        if (IsFileExists(path))
        {
            MakePath("/dev/hchai");
            ExcutingAnOrder("device_config put activity_manager_native_boot use_freezer true");
            ExcutingAnOrder("device_config put activity_manager_native_boot freeze_debounce_timeout 600000");
            ExcutingAnOrder("settings put global cached_apps_freezer default");
            ExcutingAnOrder("mount -t tmfs HCcgroup /dev/hchai");
            MakePath("/dev/hchai/freezer");
            MakePath("/dev/hchai/freezer2");
            ExcutingAnOrder("mount -t cgroup -o freezer HCfreezer /dev/hchai/freezer");
            ExcutingAnOrder("mount -t cgroup2 -o nosuid,nodev,noexec none /dev/hchai/freezer2");
            MakePath("/dev/hchai/freezer2/jie");
            MakePath("/dev/hchai/freezer2/dong");
            WriteFile("/dev/hchai/freezer2/dong/cgroup.freeze", "1");
            MakePath("/dev/hchai/freezer/perf/frozen");
            WriteFile("/dev/hchai/freezer/perf/frozen/freezer.state", "FROZEN");
            MakePath("/dev/hchai/freezer/perf/thawed");
            WriteFile("/dev/hchai/freezer/perf/thawed/freezer.state", "THAWED");
            WriteFile("/data/adb/modules/Hc_tombstone/refrigerator", "运行+1\n");
        }
    }

} // namespace freezer
#endif // FREE_ZER_H