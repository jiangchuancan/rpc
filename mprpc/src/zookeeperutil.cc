#include "zookeeperutil.h"
#include "mprpcapplication.h"
#include "logger.h"
#include <iostream>

void global_watcher(zhandle_t *zh, int type,
                    int state, const char *path, void *watcherCtx)
{
    if (type == ZOO_SESSION_EVENT)
    {
        if (state == ZOO_CONNECTED_STATE)
        {
            sem_t *sem = (sem_t *)zoo_get_context(zh);
            //将sem加1
            sem_post(sem);
        }
    }
}

ZKClient::ZKClient() : m_zhandle(nullptr)
{
}

ZKClient::~ZKClient()
{
    if (m_zhandle != nullptr)
    {
        m_zhandle == nullptr;
    }
}

// zkclient启动连接zkserver
void ZKClient::Start()
{
    std::string host = MprpcApplication::GetInstance().GetConfig().Load("zookeeperip");
    std::string port = MprpcApplication::GetInstance().GetConfig().Load("zookeeperport");
    std::string connstr = host + ":" + port;

    /*
    zookeeper_mt是多线程版本
    zookeeper的API客户端程序提供了三个线程
    API调用线程
    网络IO线程 pthread_create poll
    watcher回调线程pthread_create
    */
    //                           zookeeper的服务端  回调函数      timeout
    m_zhandle = zookeeper_init(connstr.c_str(), global_watcher, 30000, nullptr, nullptr, 0);
    if (m_zhandle == nullptr)
    {
        std::cout << "zookeeprt_init error!!!" << std::endl;
        LOG_ERR("zookeeprt_init error!!!");
        exit(EXIT_FAILURE);
    }

    //定义一个信号量
    sem_t sem;
    sem_init(&sem, 0, 0);
    //为m_zhandle添加sem信息
    zoo_set_context(m_zhandle, &sem);

    //等待回调函数内部唤醒
    sem_wait(&sem);
    std::cout << "zookeeprt_init success!!!" << std::endl;
}

//在zkserver上面根据指定的path创建znode节点
void ZKClient::Create(const char *path, const char *data, int datalen, int state)
{
    char path_buffer[128];
    int bufferlen = sizeof(path_buffer);
    int flag;

    //先判断path表示的znode是否存在，如果存在，就不再重复创建
    flag = zoo_exists(m_zhandle, path, 0, nullptr);
    if (flag == ZNONODE) //不存在才会创建znode
    {
        //创建指定path的znode节点
        flag = zoo_create(m_zhandle, path, data, datalen, &ZOO_OPEN_ACL_UNSAFE, state, path_buffer, bufferlen);
        if (flag == ZOK)
        {
            std::cout << "znode create success!!!" << std::endl;
            LOG_INFO("znode create success!!!");
        }
        else
        {
            std::cout << "flag:" << flag << std::endl;
            LOG_INFO("flag:%d", flag);
            std::cout << "zonde create error!!!" << std::endl;
            LOG_ERR("zonde create error!!!");
            exit(EXIT_FAILURE);
        }
    }
}

//根据参数指定的znode节点路径，获取znode节点的值
std::string ZKClient::GetData(const char *path)
{
    char buffer[64];
    int bufferlen = sizeof(buffer);
    int flag = zoo_get(m_zhandle,path,0,buffer,&bufferlen,nullptr);
    if(flag != ZOK)
    {
        std::cout<<"zoo_get error!!!"<<std::endl;
        return "";
    }
    else
    {
        return buffer;
    }
}