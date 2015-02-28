//
//  MyLog.h
//  DetectMyFace
//
//  Created by Werck Ayrton on 02/08/2014.
//  Copyright (c) 2014 Athosone. All rights reserved.
//

#ifndef __DetectMyFace__MyLog__
#define __DetectMyFace__MyLog__

#include <iostream>


typedef enum e_logType
{
    INFO,
    ERROR,
    DEBUGMODE,
    NOTHING
} logType;

class MyLog
{
    std::ostream *m_out; // use pointer so you can change it at any point
    bool          m_owner;
public:
    MyLog();
    // constructor is trivial (and ommited)
    ~MyLog();
    void setStream( std::ostream* stream, bool owner );
    template<typename T>
    MyLog& operator << (const T& object);
    void    log(const std::string &str, logType lt);
    
    
    
};



#endif /* defined(__DetectMyFace__MyLog__) */
