#include "mprpcconfig.h"
#include "logger.h"
#include <iostream>
//负责加载配置文件
void MprcpConfig::LoadConfigFile(const char *config_file)
{
    FILE *p = fopen(config_file, "r");
    if (p == nullptr)
    {
        // std::cout << "config_file error!!!" << std::endl;
        LOG_ERR("config_file error!!!");
        exit(EXIT_FAILURE);
    }

    while (!feof(p))
    {
        char buf[512] = {};
        fgets(buf, 512, p);

        //去掉前面的空格
        std::string str_buf(buf);
        Trim(str_buf);

        //去掉#号和整行为空格的情况
        if (str_buf[0] == '#' || str_buf.empty())
        {
            continue;
        }

        //解析配置文件
        int idx = str_buf.find('=');
        if (idx == -1)
        {
            //配置不合法
            continue;
        }

        std::string key;
        std::string value;
        key = str_buf.substr(0, idx);
        Trim(key);
        int endidx = str_buf.find('\n', idx);
        value = str_buf.substr(idx + 1, endidx - idx - 1);
        Trim(value);
        m_configMap.insert({key, value});
    }
}

//查询配置项信息
std::string MprcpConfig::Load(std::string key)
{
    auto it = m_configMap.find(key);
    if (it == m_configMap.end())
    {
        return "";
    }
    return m_configMap[key];
}

// 去掉字符串前后的空格
void MprcpConfig::Trim(std::string &src_buf)
{
    int idx = src_buf.find_first_not_of(' ');
    if (idx != -1)
    {
        // 说明字符串前面有空格
        src_buf = src_buf.substr(idx, src_buf.size() - idx);
    }
    // 去掉字符串后面多余的空格
    idx = src_buf.find_last_not_of(' ');
    if (idx != -1)
    {
        // 说明字符串后面有空格
        src_buf = src_buf.substr(0, idx + 1);
    }
}