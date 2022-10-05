#pragma once

class Snooper;

class ISnooperInfo
{
  public:
    virtual ~ISnooperInfo()                            = default;
    virtual void addServerStatsToSnooper(Snooper var1) = 0;

    virtual void addServerTypeToSnooper(Snooper var1) = 0;

    virtual bool isSnooperEnabled() = 0;
};
