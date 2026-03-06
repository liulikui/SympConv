#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include <string>
#include <vector>
#include <sstream>

// 命令行参数解析类
class Commandline
{
public:
    // 构造函数，接受命令行参数字符串
    Commandline(const std::string& cmdLine) : mCmdLine(cmdLine) {}
    
    // 构造函数，接受argc和argv
    Commandline(int argc, char* argv[])
    {
        std::ostringstream cmdLineStream;
        for (int i = 1; i < argc; ++i)
        {
            cmdLineStream << argv[i];
            if (i < argc - 1)
            {
                cmdLineStream << " ";
            }
        }
        mCmdLine = cmdLineStream.str();
    }
    
    // 检查是否存在指定参数
    bool Find(const char* param) const
    {
        return mCmdLine.find(param) != std::string::npos;
    }
    
    // 获取整数类型参数值
    bool Get(const char* param, int& value, int defaultValue) const
    {
        size_t pos = mCmdLine.find(param);
        if (pos == std::string::npos)
        {
            value = defaultValue;
            return false;
        }
        
        size_t start = pos + strlen(param);
        size_t end = mCmdLine.find(' ', start);
        if (end == std::string::npos)
        {
            end = mCmdLine.length();
        }
        
        std::string valueStr = mCmdLine.substr(start, end - start);
        try
        {
            value = std::stoi(valueStr);
            return true;
        }
        catch (...)
        {
            value = defaultValue;
            return false;
        }
    }
    
    // 获取浮点数类型参数值
    bool Get(const char* param, float& value, float defaultValue) const
    {
        size_t pos = mCmdLine.find(param);
        if (pos == std::string::npos)
        {
            value = defaultValue;
            return false;
        }
        
        size_t start = pos + strlen(param);
        size_t end = mCmdLine.find(' ', start);
        if (end == std::string::npos)
        {
            end = mCmdLine.length();
        }
        
        std::string valueStr = mCmdLine.substr(start, end - start);
        try
        {
            value = std::stof(valueStr);
            return true;
        }
        catch (...)
        {
            value = defaultValue;
            return false;
        }
    }
    
    // 获取布尔类型参数值
    bool Get(const char* param, bool& value, bool defaultValue) const
    {
        size_t pos = mCmdLine.find(param);
        if (pos == std::string::npos)
        {
            value = defaultValue;
            return false;
        }
        
        size_t start = pos + strlen(param);
        size_t end = mCmdLine.find(' ', start);
        if (end == std::string::npos)
        {
            end = mCmdLine.length();
        }
        
        std::string valueStr = mCmdLine.substr(start, end - start);
        
        if (valueStr == "false" || valueStr == "0" || valueStr == "no")
        {
            value = false;
        }
        else if (valueStr == "true" || valueStr == "1" || valueStr == "yes")
        {
            value = true;
        }
        else
        {
            value = defaultValue;
            return false;
        }
        
        return true;
    }
    
    // 获取无符号整数类型参数值
    bool Get(const char* param, uint32_t& value, uint32_t defaultValue) const
    {
        size_t pos = mCmdLine.find(param);
        if (pos == std::string::npos)
        {
            value = defaultValue;
            return false;
        }
        
        size_t start = pos + strlen(param);
        size_t end = mCmdLine.find(' ', start);
        if (end == std::string::npos)
        {
            end = mCmdLine.length();
        }
        
        std::string valueStr = mCmdLine.substr(start, end - start);
        try
        {
            value = static_cast<uint32_t>(std::stoi(valueStr));
            return true;
        } 
        catch (...)
        {
            value = defaultValue;
            return false;
        }
    }
    
    // 获取字符串类型参数值
    bool Get(const char* param, std::string& value, const std::string& defaultValue) const
    {
        size_t pos = mCmdLine.find(param);
        if (pos == std::string::npos)
        {
            value = defaultValue;
            return false;
        }
        
        size_t start = pos + strlen(param);
        size_t end = mCmdLine.find(' ', start);
        if (end == std::string::npos) 
        {
            end = mCmdLine.length();
        }
        
        value = mCmdLine.substr(start, end - start);
        return true;
    }
    
    // 获取完整的命令行字符串
    const std::string& GetCommandLineString() const
    {
        return mCmdLine;
    }
    
private:
    std::string mCmdLine; // 存储命令行参数字符串
};

#endif // COMMANDLINE_H
