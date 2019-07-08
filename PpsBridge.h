/*
 * PpsBridge.h
 *
 *  Created on: 2017-8-22
 *      Author: czn
 */

#ifndef PPS_BRIDGE_H_
#define PPS_BRIDGE_H_

#include <string>
#include <pthread.h>
#include <errno.h>
#include <gulliver.h>
#include "BasicTypeDefine.h"

#define PPS_DEFAULT_DIRECTORY		"/pps/"
#define PPS_DEFAULT_OBJECT_SIZE		32768
#define PPS_CONVERT_BUF_SIZE			33

class PpsBridge {
public:
    static const int PpsDefaultPermissions = 0666;
    enum PpsMode {
        PPS_PUBLISHER = 0,
        PPS_SUBSCRIBER = 1,
        PPS_TWO_WAY = 2,
    };

public:
    PpsBridge(std::string, PpsMode);
    PpsBridge(std::string, std::string, PpsMode);
    PpsBridge(std::string, std::string, int, PpsMode);
    virtual ~PpsBridge();

public:
    static int getValue(std::string data, std::string name, std::string* value, std::string* encoding);

public:
    int setNumeric(std::string, long);
    int setString(std::string, std::string);
    int setBoolean(std::string, int);

public:
    void setData(std::string data);
    int setValue(std::string name, std::string value);
    int setValue(std::string name, std::string value, std::string encoding);

public:
    int getValue(std::string name, std::string* value, std::string* encoding);
    int getValue(std::string name , std::string* value);

public:
    int getIntValue(std::string name, int* value);
    int getLongValue(std::string name, long* value);
   	int getStringValue(std::string name, std::string* value);

public:
    int removeValue(std::string);

public:
    int setValue(unsigned long long, std::string, std::string);
    int setValue(unsigned long long, std::string, std::string, std::string);
    int removeValue(unsigned long long, std::string);
    int removeAllValues();

    void setAutoPublish(bool);
    void setAutoRefresh(bool);
    bool getAutoPublish();
    bool getAutoRefresh();

    int publish();
    int refresh();

protected:
    PpsMode m_mode;
    int m_PpsFd;
    std::string m_objectName;
    std::string m_objectPath;
    std::string *m_PpsData;
    bool m_AutoPublish;
    bool m_AutoRefresh;
    std::string *m_PpsPublishString; 
    unsigned char m_buf[PPS_CONVERT_BUF_SIZE];
    char* m_pBufPPS;

protected:
    int setBlocking(bool);
    void init(std::string, std::string, int, PpsMode); 
    int receive();

protected:
    void freeBuf();
};

#endif /* PPS_BRIDGE_H_ */
