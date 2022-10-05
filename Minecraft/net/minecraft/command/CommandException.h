#pragma once
#include <exception>
#include <string_view>
#include <vector>

class CommandException : public std::exception
{

  public:
    CommandException(std::string_view message, const std::vector<> &objects);
    std::vector<> getErrorObjects();
    virtual std::exception &fillInStackTrace();

  private:
    std::vector<> errorObjects;
};
