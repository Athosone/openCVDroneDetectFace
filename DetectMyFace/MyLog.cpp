//
//  MyLog.cpp
//  DetectMyFace
//
//  Created by Werck Ayrton on 02/08/2014.
//  Copyright (c) 2014 Athosone. All rights reserved.
//

#include   <ctime>
#include "MyLog.h"

MyLog::MyLog()
{
    
}

MyLog::~MyLog()
{
    setStream(0, false);
}
void MyLog::setStream( std::ostream* stream, bool owner )
{
    if(m_owner && m_out != NULL)
        delete m_out;
    m_out = stream;
    m_owner = owner;
}
template<typename T>
MyLog& MyLog::operator << (const T& object)
{
    
    if(!m_out)
        throw std::runtime_error("No stream set for MyLog class");
    
    (*m_out) << object;
    return *this;
}
void    MyLog::log(const std::string &str, logType lt)
{
    if (lt == INFO)
        (*m_out) << "[INFO] " + str << std::endl;
    else if (lt == ERROR)
        (*m_out) << "[ERROR] " + str << std::endl;
    else
        (*m_out) <<"[DEBUG] " + str << std::endl;
}
