#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "History.h"
#include "mark.h"
#include "My_Sql.h"

History * History::m_pHistory = NULL;
std::mutex History::m_HistoryLock;

History * History::GetHistory()
{
    std::unique_lock<std::mutex> lock(m_HistoryLock);
    if(m_pHistory == NULL)
    {
        m_pHistory = new History();
    }

    return m_pHistory;
}
void History::FreeHistory()
{
    std::unique_lock<std::mutex> lock(m_HistoryLock);
    if(m_pHistory != NULL)
    {
        delete m_pHistory;
        m_pHistory = NULL;
    }
}

History::History() : m_pId(NULL),m_pPasswd(NULL),m_pStr1(NULL),m_pStr2(NULL)
{

}

History::~History()
{
    if(m_pId != NULL)
    {
        delete []m_pId;
    }

    if(m_pPasswd != NULL)
    {
        delete []m_pPasswd;
    }

    if(m_pStr1 != NULL)
    {
        delete []m_pStr1;
    }

    if(m_pStr2 != NULL)
    {
        delete []m_pStr2;
    }

}

int History::Function(int cfd)
{
    printf("History function!\n");

    char get_len[5];
    int len;
    int rev;

    memset(get_len,0,sizeof(len));
    rev = recv(cfd,get_len,4,0);
    if(rev < 0)
    {
        perror("History::id_len");
        return -1;
    }
    //printf("get_len = %s\n",get_len);

    len = atoi(get_len);
    m_pId = new char[len + 1];
    memset(m_pId, 0, len + 1);
    rev = recv(cfd, m_pId, len, 0);
    if(rev <  0)
    {
        perror("History::id:");
        return -1;
    }
    //printf("id : %s\n",m_pId);

    rev = recv(cfd, &get_len, 4, 0);
    if(rev < 0)
    {
        perror("History::str1_len");
        return -1;
    }

    len = atoi(get_len);
    m_pStr1 = new char[len + 1];
    memset(m_pStr1,0,len + 1);
    rev = recv(cfd, m_pStr1, len, 0);
    if(rev <  0)
    {
        perror("History::str1:");
        return -1;
    }

    rev = recv(cfd,&get_len,4,0);
    if(rev < 0)
    {
        perror("History::str2_len");
        return -1;
    }

    len = atoi(get_len);
    m_pStr2 = new char[len + 1];
    memset(m_pStr2,0,len+ 1);
    rev = recv(cfd,m_pStr2,len,0);
    if(rev <  0)
    {
        perror("History::str2:");
        return -1;
    }

    m_pPasswd = new char[17];
    memset(m_pPasswd,0,17);
    rev = recv(cfd, m_pPasswd, 16, 0);
    if(rev <  0)
    {
        perror("History::passwd:");
        return -1;
    }

    printf("History::id:%s\n",m_pId);
    printf("History::password:%s\n",m_pPasswd);
    printf("History::str1:%s\n",m_pStr1);
    printf("History::str2:%s\n",m_pStr2);


    int s = DataBase();
    send(cfd, &s, sizeof(s), 0);

    if(s == HIST_SUCCESS)
    {

        int len = 0;
        char *buf;
        string temp;
#if 0
        len = m_sFieldsName.size();
        buf = new char[len+1];
        strcpy(buf,m_sFieldsName.c_str());

        printf("FielsName = %s\n",m_sFieldsName.c_str());
        printf("len = %d\n",len);

        send(cfd,&len,sizeof(len),0);
        send(cfd,buf,len,0);
        delete []buf;
#endif
        for(vector<vector<string>>::iterator it = m_vData.begin(); it != m_vData.end(); ++it)
        {
            temp.clear();
            for(int i = 0; i < (*it).size(); ++i)
            {
                cout << (*it)[i] << " ";
                temp = temp + (*it)[i] + " ";
            }
            temp += '\n';
            len = temp.size();
            buf = new char[len+1];
            strcpy(buf,temp.c_str());

            //printf("Row information:%s\n",buf);

            send(cfd,&len,sizeof(len),0);
            send(cfd,buf,len,0);
            delete []buf;
        }

        len = 0;    //发送结束标志
        send(cfd,&len,sizeof(len),0);
    }

    return 0;
}

int History::DataBase()
{
    int result = 0;
    char buf[1024];
    double money = 0.0;

    My_Sql sql("BankData");

    sprintf(buf,"select *from UserInformation where ID= %s;",m_pId);
    sql.MySql_exec(buf);

    if(sql.m_iNumRows == 0)        //无帐号
    {
        result = NO_ACCOUNT;
    }
    else
    {
        sprintf(buf,"select passwd from UserInformation where ID= %s AND passwd=\"%s\";",m_pId,m_pPasswd);
        sql.MySql_exec(buf);
        if(sql.m_iNumRows == 0) //密码错误
        {
            result = PASSWD_ERROR;
        }
        else
        {
            // 历史记录查询
            sprintf(buf,"select * from _%s where Date between '%s' and '%s';"
                    ,m_pId,m_pStr1,m_pStr2);
            sql.MySql_exec(buf);
            sql.GetFieldsName(m_sFieldsName);
            sql.GetRowInformation(m_vRow);

            if(m_vRow.size() == 0)
            {
                result = NO_HISTORY;
            }
            else
            {
                sprintf(buf,"select * from _%s where Date between '%s' and '%s';"
                        ,m_pId,m_pStr1,m_pStr2);
                sql.getDatafromDB(buf,m_vData);

                result = HIST_SUCCESS;

            }
  //          std::cout << _LineInformation << std::endl;

        }

    }

    return result;
}


