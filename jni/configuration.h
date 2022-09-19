#include "tombstone/HeadFile.h"

//冰箱路径
static char *frozen_path = "/dev/hchai/freezer/perf/frozen/cgroup.procs";
static char *thawed_path = "/dev/hchai/freezer/perf/thawed/cgroup.procs";
static const char *blacklist_path = "/data/media/0/Android/Hc_tombstone/白名单.conf";
static const char *confList_path = "/data/media/0/Android/Hc_tombstone/配置.conf";
static const char *forcedBlacklist_path = "/data/media/0/Android/Hc_tombstone/强制黑名单.conf";
static const char *push_path = "/data/media/0/Android/Hc_tombstone/推送名单.conf";
static const char *good_path = "/data/media/0/Android/Hc_tombstone/乖巧名单.conf";
static const char *log_path = "/data/media/0/Android/Hc_tombstone/日志.log";
static char blacklist[20480], forcedBlacklist[4096], pushqq[FILE_CACHE_D], goodlist[4096];

void freezers(int freezerv)
{
    if (freezerv == 2)
    {
        frozen_path = "/dev/hchai/freezer2/dong/cgroup.procs";
        thawed_path = "/dev/hchai/freezer2/jie/cgroup.procs";
    }
    else
    {
        frozen_path = "/dev/hchai/freezer/perf/frozen/cgroup.procs";
        thawed_path = "/dev/hchai/freezer/perf/thawed/cgroup.procs";
    }
}

struct parameter
{
    int cycle, frozenCycle;
    char home[FILE_CACHE_ZXX], openPushAndReceive[FILE_CACHE_ZXX], wellBehavedMode[FILE_CACHE_ZXX], freezers[FILE_CACHE_ZXX];
};
parameter Hconf{0};

//配置文件
namespace configuration
{
    //冻结后台进程
    void BackstageProcessFrozen();
    //删除已关闭后台进程
    void RemoveTheBackground();
    //解冻前台进程
    void *ThawingFrontDeskApplication(void *arg);
    //判断音频进程是否被冻结
    void CheckTheAudioProcess();
    //推送进程
    void *PushProcess(void *arg);
    //日志
    void WriteALog();
    //乖巧名单
    void GoodList();
    //读取配置文件参数
    void ReadConfiguration()
    {
        char buffer[5120];
        FILE *fp = fopen(confList_path, "r");
        while (fgets(buffer, sizeof(buffer), fp) != NULL)
        {
            sscanf(buffer, "cycle=%d", &Hconf.cycle);
            sscanf(buffer, "home=%s", Hconf.home);
            sscanf(buffer, "frozenCycle=%d", &Hconf.frozenCycle);
            sscanf(buffer, "openPushAndReceive=%s", Hconf.openPushAndReceive);
            sscanf(buffer, "wellBehavedMode=%s", Hconf.wellBehavedMode);
            sscanf(buffer, "freezers=%s", Hconf.freezers);
        }
        fclose(fp);
    }
    //墓碑版本
    void TombstoneVersion()
    {
        if (strcmp(Hconf.freezers, "自动") == 0)
        {
            if (freezer::IsFileExists("/dev/hchai/freezer2/dong/cgroup.procs"))
            {
                freezers(2);
            }
            else
            {
                freezers(1);
            }
        }
        else if (strcmp(Hconf.freezers, "v2") == 0)
        {
            freezers(2);
        }
        else
        {
            freezers(1);
        }
    }

    void FrontDeskPid(char *storagePid)
    {
        char buffer[FILE_CACHE_X];
        FILE *fp = popen("dumpsys activity lru | grep ' TOP' |awk -F '[:/]+' '{print $3}' |awk -F ' ' '{print $NF}'", "r");
        fgets(buffer, sizeof(buffer), fp);
        signal(SIGCHLD, SIG_IGN);
        strcpy(storagePid, buffer);
        pclose(fp);
    }

    void FrontDeskPidF(char *storagePid)
    {
        char buffer[FILE_CACHE_X];
        FILE *fp = popen("dumpsys activity lru | grep ' TOP' |awk -F '[:/]+' '{print $3}' |awk -F ' ' '{print $NF}'", "r");
        fread(buffer, sizeof(char), sizeof(buffer), fp);
        signal(SIGCHLD, SIG_IGN);
        strcpy(storagePid, buffer);
        pclose(fp);
    }

    //写入日志文件
    void WriteLog(const char *path, const char *content, const char *content2)
    {
        FILE *fp = fopen(path, "a+");
        fprintf(fp, "%s\n%s\n\n", content, content2);
        fclose(fp);
    }

    //读取文件 过滤掉空行和注释行
    void ReadConf(const char *path, char *storage)
    {
        char buffer[FILE_CACHE_Z];
        FILE *fp = fopen(path, "r");
        while (fgets(buffer, sizeof(buffer), fp) != NULL)
        {
            if (buffer[0] == '#' || buffer[0] == '\n')
                continue;
            strcat(storage, buffer);
        }
        fclose(fp);
    }

    //获取时间 年月日时分秒 格式为：2019-01-01 12:12:12 返回字符串
    char *GetTime()
    {
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        char *time = new char[FILE_CACHE_X];
        sprintf(time, "%d-%02d-%02d %02d:%02d:%02d | Freezer:v2 已启动 | 作者: HChai\n", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
        return time;
    }

    //判断文件是否存在 不存在则创建
    void FileExist(const char *path)
    {
        FILE *fp = fopen(path, "a+");
        if (fp == NULL)
        {
            fp = fopen(path, "w");
            fclose(fp);
        }
        fclose(fp);
    }

    //读取文件 过滤掉非KILL行
    void ReadKILL(const char *path, char *storage)
    {
        char buffer[FILE_CACHE_Z];
        FILE *fp = fopen(path, "r");
        while (fgets(buffer, sizeof(buffer), fp) != NULL)
        {
            if (buffer[0] != 'K')
                continue;
            strcat(storage, buffer);
        }
        fclose(fp);
    }

} // namespace configuration