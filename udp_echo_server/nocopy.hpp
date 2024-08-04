#pragma once

// 不让肆意拷贝服务端
class nocopy
{
public:
    nocopy(){}
    // 拷贝构造
    nocopy(const nocopy&) = delete;
    // 复制重载
    const nocopy& operator = (const nocopy&) = delete; 
    ~nocopy(){}
};