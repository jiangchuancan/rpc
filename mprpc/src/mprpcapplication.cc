#include "mprpcapplication.h"
#include <iostream>
#include <unistd.h>
#include <string>

//对静态方法的定义
MprcpConfig MprpcApplication::m_config;

void ShowArgsHelp()
{
    //std::cout<<"format: command -i <configfile>"<<std::endl;
    LOG_INFO("format: command -i <configfile>");
}

void MprpcApplication::Init(int argc, char **argv)
{
    if(argc <2)
    {
        ShowArgsHelp();
        exit(EXIT_FAILURE);
    }

    int c = 0;
    std::string config_file;
    while((c = getopt(argc,argv,"i:")) != -1)
    {
        switch (c)
        {
        case 'i'://正确情况
            config_file = optarg;
            break;
        case '?'://-i都没有
            ShowArgsHelp();
            exit(EXIT_FAILURE);
        case ':'://有-i但是没有配置文件
            ShowArgsHelp();
            exit(EXIT_FAILURE);
        default:
            break;
        }       
    }

    //加载配置文件 rpcserver_ip rpcserver_port zookeeper_ip zookeeper_port
    m_config.LoadConfigFile(config_file.c_str());
    //std::cout<<"rpcserverip"<<m_config.Load("rpcserverip")<<std::endl;
}

MprpcApplication &MprpcApplication::GetInstance()
{
    static MprpcApplication app;
    return app;
}

MprcpConfig &MprpcApplication::GetConfig()
{
    return m_config;
}