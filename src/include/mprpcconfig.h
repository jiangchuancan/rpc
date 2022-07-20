#pragma once

#include <string>
#include <unordered_map>

class MprcpConfig
{
public:
    //负责加载配置文件
    void LoadConfigFile(const char *config_file);
    //查询配置项信息
    std::string Load(std::string key);

private:
    std::unordered_map<std::string, std::string> m_configMap;
    // 去掉字符串前后的空格
    void Trim(std::string &src_buf);
};