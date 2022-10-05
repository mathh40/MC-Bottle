#pragma once
#include <string>

class IAttribute
{
  public:
    virtual std::string getName() const = 0;

    virtual double clampValue(double var1) const = 0;

    virtual double getDefaultValue() const = 0;

    virtual bool getShouldWatch() const = 0;

    virtual IAttribute *getParent() const = 0;
};
