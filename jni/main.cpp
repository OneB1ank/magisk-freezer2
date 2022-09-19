#include "tombstone/Freezer.hpp"
#include "processOptimization.hpp"

//解冻前台进程
void *configuration::ThawingFrontDeskApplication(void *arg)
{
    char processPid[FILE_CACHE_X], frozenList[FILE_CACHE_Z];
    for (;;)
    {
        FILE *fp = popen("dumpsys activity lru | grep 'TOP'|grep 'activities' |awk -F ':' '{print $2}'|awk -F ' ' '{print $NF}'", "r");
        while (fgets(processPid, sizeof(processPid), fp) != NULL)
        {
            freezer::WriteFile(thawed_path, processPid);
        }
        signal(SIGCHLD, SIG_IGN);
        pclose(fp);
        std::this_thread::sleep_for(std::chrono::milliseconds(Hconf.cycle));
    }
}

//推送前台进程
void *configuration::PushProcess(void *arg)
{
    char processPid[FILE_CACHE_X], bufferApk[FILE_CACHE_X], bufferPid[FILE_CACHE_ZX];
    for (;;)
    {
        FILE *fp = popen("dumpsys activity lru |grep 'recents' |grep -vE 'FGS|IMPF|TOP|dumpsys' |awk -F '/' '{print $1}' |awk -F ' ' '{print $NF}'", "r");
        while (fscanf(fp, "%19[^:]:%s", bufferPid, bufferApk) != EOF)
        {
            if (strstr(pushqq, bufferApk))
            {
                freezer::WriteFile(thawed_path, bufferPid);
                std::this_thread::sleep_for(std::chrono::milliseconds(600));
                freezer::WriteFile(frozen_path, bufferPid);
            }
        }
        signal(SIGCHLD, SIG_IGN);
        pclose(fp);
        std::this_thread::sleep_for(std::chrono::seconds(Hconf.frozenCycle));
    }
}

//冻结后台进程
void configuration::BackstageProcessFrozen()
{
    FILE *fp;
    char whiteNameSingleAudioProcess[3072], bufferApk[FILE_CACHE_X], bufferPid[FILE_CACHE_ZX], bufferPath[FILE_CACHE_Z];
    freezer::ExecuteShell("dumpsys activity lru |grep -E 'IMPF|FGS|LCMN|TOP|com.android' |awk -F '/' '{print $1}' |awk -F ' ' '{print $NF}'", whiteNameSingleAudioProcess);
    fp = popen("dumpsys activity lru |grep 'recents' |awk -F '/' '{print $1}' |awk -F ' ' '{print $NF}'", "r");
    while (fscanf(fp, "%19[^:]:%s", bufferPid, bufferApk) != EOF)
    {
        if (strstr(blacklist, bufferApk) == NULL && strstr(whiteNameSingleAudioProcess, bufferApk) == NULL || strstr(forcedBlacklist, bufferApk))
        {
            freezer::WriteFile(frozen_path, bufferPid);
            sprintf(bufferPath, "/proc/%d/oom_score_adj", atoi(bufferPid));
            freezer::WriteFiles(bufferPath, "-800");
        }
    }
    signal(SIGCHLD, SIG_IGN);
    pclose(fp);
}

//删除已关闭后台进程
void configuration::RemoveTheBackground()
{
    FILE *fpp;
    char ProcPid[FILE_CACHE_Z];
    // fp = popen("dumpsys activity lru |grep 'recents' |awk -F '[:/]+' '{print $2}' |awk -F ' ' '{print $NF}'", "r");
    // fread(buffer, sizeof(char), sizeof(buffer), fp);
    // signal(SIGCHLD, SIG_IGN);
    // pclose(fp);
    fpp = fopen(thawed_path, "r");
    while (fgets(ProcPid, sizeof(ProcPid), fpp) != NULL)
    {
        // if (strstr(buffer, ProcPid) == NULL)
        //{
        freezer::WriteFile(thawed_path, ProcPid);
        //}
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    fclose(fpp);
}

//判断音频进程是否被冻结
void configuration::CheckTheAudioProcess()
{
    char audioProcess[FILE_CACHE_ZX], frozenList[FILE_CACHE_D];
    freezer::ReadFile(frozen_path, frozenList);
    FILE *fp = popen("dumpsys activity lru |grep 'FGS' |awk -F '[/:]+' '{print $3}'|awk 'a[$0]++'|awk 'NR==1'|xargs pidof", "r");
    fread(audioProcess, sizeof(char), sizeof(audioProcess), fp);
    signal(SIGCHLD, SIG_IGN);
    if (strlen(audioProcess) > 2 && strstr(frozenList, audioProcess))
    {
        //有音频进程就解冻
        freezer::WriteFile(thawed_path, audioProcess);
    }
    pclose(fp);
}

//日志记录
void configuration::WriteALog()
{
    // freezer::ExcutingAnOrder("touch /data/media/0/Android/Hc_tombstone/日志.log");
    remove(log_path);
    configuration::FileExist(log_path);
    freezer::WriteFile(log_path, configuration::GetTime());
    configuration::WriteLog(log_path, "已加载白名单: ", blacklist);
    configuration::WriteLog(log_path, "已加载强制黑名单: ", forcedBlacklist);
    if (strcmp(Hconf.openPushAndReceive, "开") == 0)
    {
        configuration::WriteLog(log_path, "已加载推送名单: ", pushqq);
    }
    if (strcmp(Hconf.wellBehavedMode, "开") == 0)
    {
        configuration::WriteLog(log_path, "已加载乖巧名单: ", goodlist);
    }
}

void configuration::GoodList()
{
    if (strcmp(Hconf.wellBehavedMode, "开") == 0)
    {
        //乖巧模式
        char bufferApk[FILE_CACHE_X], bufferPid[FILE_CACHE_ZX], bufferqt[FILE_CACHE_X],str[6];
        configuration::FrontDeskPidF(bufferqt);
        FILE *fp = popen("dumpsys activity lru | grep -vE 'recents|dumpsys|com.android|com.miui|com.xiaomi|:system|Other:|:\\.|Activities:|com.google' |awk -F '/' '{print $1}' |awk -F ' ' '{print $NF}' |grep '.*:.*:'", "r");
        while (fscanf(fp, "%19[^:]:%s", bufferPid, bufferApk) != EOF)
        {
            if (strstr(goodlist, bufferApk) && strlen(bufferPid) > 2)
            {
                snprintf(str,6,"%s",bufferApk);
                if (std::string (bufferqt).find(std::string (str)) == std::string::npos)
                {
                    kill(atoi(bufferPid), SIGKILL);
                }
            }
        }
        signal(SIGCHLD, SIG_IGN);
        pclose(fp);
    }
}

int main()
{
    char previousPid[FILE_CACHE_X], postPid[FILE_CACHE_X];
    pthread_t hcread[2];
    //初始化冰箱机制
    freezer::InitializationKernel("/data/adb/mchildpidodules/Hc_tombstone/refrigerator");
    configuration::ReadConfiguration();
    configuration::TombstoneVersion();
    //初始化黑白名单
    configuration::ReadConf(blacklist_path, blacklist);
    configuration::ReadConf(forcedBlacklist_path, forcedBlacklist);
    //读取乖巧模式配置文件
    configuration::ReadKILL(good_path, goodlist);
    //初始化墓碑版本
    if (strcmp(Hconf.openPushAndReceive, "开") == 0)
    {
        configuration::ReadConf(push_path, pushqq);
        //启动推送线程
        // std::thread(configuration::PushProcess).detach();
        pthread_create(&hcread[1], NULL, configuration::PushProcess, NULL);
        pthread_detach(hcread[1]);
    }
    configuration::WriteALog();
    //启动解冻线程
    // std::thread(configuration::ThawingFrontDeskApplication).detach();
    pthread_create(&hcread[0], NULL, configuration::ThawingFrontDeskApplication, NULL);
    pthread_detach(hcread[0]);
    configuration::FrontDeskPid(postPid);
    for (;;)
    {
        //获取前台应用的pid
        std::this_thread::sleep_for(std::chrono::seconds(1));
        configuration::FrontDeskPid(previousPid);
        //判断previousPid和postPid是否相等
        if (strcmp(previousPid, postPid) != 0)
        {
            std::this_thread::sleep_for(std::chrono::seconds(Hconf.frozenCycle));
            configuration::RemoveTheBackground();
            configuration::BackstageProcessFrozen();
            //configuration::CheckTheAudioProcess();
            std::this_thread::sleep_for(std::chrono::seconds(1));
            configuration::GoodList();
        }
        //把previousPid赋值给postPid
        strcpy(postPid, previousPid);
    }
}