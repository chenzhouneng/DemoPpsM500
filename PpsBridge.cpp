#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sstream>
#include <cstring>
#include "PpsBridge.h"

using namespace std;

PpsBridge::PpsBridge(string objectName, PpsMode mode) {
    init(PPS_DEFAULT_DIRECTORY, objectName, PpsDefaultPermissions, mode);
}

PpsBridge::PpsBridge(string dir, string objectName, PpsMode mode) {
    init(dir, objectName, PpsDefaultPermissions, mode);
}

PpsBridge::PpsBridge(string dir, string objectName, int permissions,
  PpsMode mode) {
    init(dir, objectName, permissions, mode);
}

void PpsBridge::init(string dir, string objectName, int permissions,
  PpsMode mode) {
    m_PpsFd = -1;
    m_mode = mode;
    m_PpsData = new string();
    m_PpsData->reserve(PPS_DEFAULT_OBJECT_SIZE);
    m_PpsPublishString = new string();
    m_PpsPublishString->reserve(PPS_DEFAULT_OBJECT_SIZE);
    m_AutoPublish = false;
    m_AutoRefresh = true;
    m_objectName = objectName;
    memset(m_buf, 0, PPS_CONVERT_BUF_SIZE);
    m_pBufPPS = new char[PPS_DEFAULT_OBJECT_SIZE];

    size_t found = m_objectName.find_last_of("/\\");
    if (found != string::npos) {
        m_objectName = m_objectName.substr(found+1);
    }

    m_objectPath = dir;
    if (*(m_objectPath.rbegin()) != '/') {
        m_objectPath += "/";
    }
     m_objectPath += objectName;

    switch (mode) {
        case PPS_PUBLISHER:
            m_PpsFd = open(m_objectPath.c_str(), O_WRONLY | O_CREAT,
              permissions);
            break;
        case PPS_SUBSCRIBER:
            m_PpsFd = open(m_objectPath.c_str(), O_RDONLY);
            break;
        case PPS_TWO_WAY:
            m_PpsFd = open(m_objectPath.c_str(), O_RDWR | O_CREAT,
              permissions);
            break;
        default:
            m_PpsFd = -1;
            break;
    }
    
    if (m_PpsFd == -1) {
    	freeBuf();
    }
}

PpsBridge::~PpsBridge() {
    close(m_PpsFd);
    m_PpsFd = -1;
    freeBuf();
}

void PpsBridge::freeBuf()
{
	if(m_PpsData) {
		m_PpsData->clear();
		m_PpsData->reserve();
		delete m_PpsData;
		m_PpsData = NULL;
	}

	if(m_PpsPublishString) {
		m_PpsPublishString->clear();
		m_PpsPublishString->reserve();
		delete m_PpsPublishString;
		m_PpsPublishString = NULL;
	}

	if(m_pBufPPS) {
		delete m_pBufPPS;
		m_pBufPPS = NULL;
	}
}

int PpsBridge::setNumeric(std::string name, long value)
{
    return setValue(name, ltoa(value, (char*)m_buf, 10), "n");
}

int PpsBridge::setString(std::string name, std::string value)
{
    return setValue(name, value);
}

int PpsBridge::setBoolean(std::string name, int value)
{
	 return setValue(name, itoa(value, (char*)m_buf, 10), "b");
}

void PpsBridge::setData(string data) {
	m_PpsData->assign(data);
}

int PpsBridge::setValue(string name, string value) {
    return setValue(name, value, string());
}

int PpsBridge::removeValue(string name) {
    return setValue("-" + name, string(), string());
}

int PpsBridge::removeAllValues()
{
    return ftruncate(m_PpsFd, 0);
}

int PpsBridge::setValue(string name, string value, string encoding) {
    string line;

    if (name.empty()) {
        return EINVAL;
    }
    if (m_PpsFd == -1) {
        return EBADF;
    }
    if (m_mode == PPS_SUBSCRIBER) {
        return EROFS;
    }

    if (value.empty() && (name.at(0)) == '-') {
        if (encoding.empty() == false) {
            return EINVAL;
        }
    }
    line = string(name + ":" + encoding + ":" + value + "\n");

    *m_PpsPublishString += line;
    if (getAutoPublish() == true) {
        if (publish() != EOK) {
            return EIO;
        }
    }
    
    return EOK;
}


int PpsBridge::publish() {
    const char *str = (*m_PpsPublishString).c_str();
    int len = strlen(str);

    if (len == 0 || (*m_PpsPublishString).empty()) {
        return EINVAL;
    }

    if (m_PpsFd == -1) {
        return EBADF;
    }

    if (write(m_PpsFd, str, len) != len) {
        (*m_PpsPublishString).clear();
        return EIO;
    }

    (*m_PpsPublishString).clear();
    return EOK;
}

int PpsBridge::refresh()
{
	return receive();
}

int PpsBridge::setBlocking(bool blocking) {
    int flags;
    if (m_PpsFd == -1) {
        return EBADF;
    }
    flags = fcntl(m_PpsFd, F_GETFL);
    if (blocking) {
        flags &= ~O_NONBLOCK;
    } else {
        flags |= O_NONBLOCK;
    }

    if (fcntl(m_PpsFd, F_SETFL, flags) == -1) {
        return EIO;
    }
    return EOK;
}

int PpsBridge::getValue(string name, string *value) {
    return getValue(name, value, NULL);
}

int PpsBridge::getValue(string name, string *value, string *encoding) {
    int rc;
    if (m_PpsFd == -1) {
        return EBADF;
    }

    if(getAutoRefresh()) {
		rc = receive();
		if (rc != EOK) {
			return rc;
		}
    }

    return getValue(*m_PpsData, name, value, encoding);
}

int PpsBridge::getValue(string data, string name, string *value,
  string *encoding) {
    size_t nameStart, nameLen;
    size_t encodingStart, encodingLen;
    size_t valueStart, valueLen;

    if (name.empty() || data.empty() || value == NULL) {
        return EINVAL;
    }

    nameStart = data.find("\n" + name + ":");
    if (nameStart == string::npos) {
        return EINVAL;
    }
    nameStart++;
    nameLen = name.size();

    encodingStart = nameStart + nameLen + 1;
    encodingLen = data.find(":", encodingStart) - encodingStart + 1;
    if (encodingLen == string::npos) {
        return EINVAL;
    }
    encodingLen--;

    valueStart = encodingStart + encodingLen + 1;
    valueLen = data.find("\n", valueStart) - valueStart;
    if (valueLen == string::npos) {
        return EINVAL;
    }

    if (encoding != NULL) {
        if (encodingLen == 0) {
            *encoding = string();
        } else if (encodingLen != 0 && encoding != NULL) {
            *encoding = data.substr(encodingStart, encodingLen);
        }
    }
    
    *value = data.substr(valueStart, valueLen);    

    return EOK;
}

int PpsBridge::getIntValue(std::string name, int * nVal)
{
	int nRet;
	std::string value;
	std::string encoding = "n";
	nRet = getValue(name, &value, &encoding);
	if(EOK == nRet) {
		*nVal = atoi(value.c_str());
	}

	return nRet;
}

int PpsBridge::getLongValue(std::string name, long * lVal)
{
	int nRet;
	std::string value;
	std::string encoding = "n";
	nRet = getValue(name, &value, &encoding);
	if(EOK == nRet) {
		*lVal = atol(value.c_str());
	}

	return nRet;
}

int PpsBridge::getStringValue(std::string name, std::string * value)
{
	return getValue(name, value);
}

int PpsBridge::setValue(unsigned long long client, string name,
  string value) {
    stringstream ss;
    ss << client;
    return setValue("@" + m_objectName + "." + ss.str() + "\n" + name, value);
}

int PpsBridge::setValue(unsigned long long client, string name,
  string value, string encoding) {
    if (client == 0) {
        return setValue(name, value, encoding);
    }
    stringstream ss;
    ss << client;
    return setValue("@" + m_objectName + "." + ss.str() + "\n" + name, value,
      encoding);
}

int PpsBridge::removeValue(unsigned long long client, string name) {
    if (client == 0) {
        return removeValue(name);
    }
    stringstream ss;
    ss << client;
    return removeValue("@" + m_objectName + "." + ss.str() + "\n" + name);
}

int PpsBridge::receive() {

    memset((void*)m_pBufPPS, 0, PPS_DEFAULT_OBJECT_SIZE);
    int nRead;
    if (m_PpsFd == -1) {
        return EBADF;
    }

    nRead = read(m_PpsFd, (void*)m_pBufPPS, PPS_DEFAULT_OBJECT_SIZE);
    if (nRead == -1) {
        return EIO;
    }

    if (nRead > 0) {
        m_PpsData->assign((const char*)m_pBufPPS, nRead);
    }

    return EOK;
}

void PpsBridge::setAutoPublish(bool autoPublish) {
    m_AutoPublish = autoPublish;
}

void PpsBridge::setAutoRefresh(bool autoRefresh) {
    m_AutoRefresh = autoRefresh;
}

bool PpsBridge::getAutoPublish() {
    return m_AutoPublish;
}

bool PpsBridge::getAutoRefresh() {
    return m_AutoRefresh;
}

