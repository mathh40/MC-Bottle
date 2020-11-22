#pragma once
#include <string>
#include <thread>

void setName(std::thread & thread,const std::string& name);
std::string getName(std::thread & thread);