#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "Open.h"
#include "mark.h"
#include "My_Sql.h"

Open * Open::m_pOpen = NULL;
std::mutex Open::m_OpenLock;

Open * Open::GetOpen()
{
    std::unique_lock<std::mutex> lock(m_OpenLock);
    if(m_pOpen == NULL)
    {
        m_pOpen = new Open();
    }

    return m_pOpen;
}
void Open::FreeOpen()
{
    std::unique_lock<std::mutex> lock(m_OpenLock);
    if(m_pOpen != NULL)
    {
        delete m_pOpen;
        m_pOpen = NULL;
    }
}

Open::Open() : m_pName(NULL),m_pID(NULL),m_pPasswd(NULL),m_pMoney(NULL)
{

}

Open::~Open()
{
    if(m_pName != NULL)
    {
        delete m_pName;
    }
    if(m_pID != NULL)
    {
        delete m_pID;
    }
    if(m_pPasswd != NULL)
    {
        delete m_pPasswd;
    }
    if(m_pMoney != NULL)
    {
        delete m_pMoney;
    }
}

int Open::Function(int cfd)
{
    printf("open function\n\n");

    char get_len[5];
    int len;
    int rev;

    memset(get_len,0,sizeof(get_len));

    rev = recv(cfd, get_len, sizeof(get_len)-1, 0);
    if(rev < 0)
    {
        perror("Open::name_len : ");
        return -1;
    }
    len = atoi(get_len);
   // printf("len=%d\n",len);

    m_pName = new char[len + 1];
    memset(m_pName, 0, len + 1);
    rev = recv(cfd, m_pName, len, 0);
    if(rev < 0)
    {
        perror("Open::name : ");
        return -1;
    }

    rev = recv(cfd, get_len, 4, 0);
    if(rev < 0)
    {
        perror("Open :: money_len :");
        return -1;
    }

    len = atoi(get_len);
    m_pMoney = new char[len + 1];
    memset(m_pMoney, 0, len+ 1);
    rev = recv(cfd, m_pMoney, len, 0);
    if(rev < 0)
    {
        perror("Open::money : ");
        return -1;
    }

    m_pID = new char[19];
    memset(m_pID, 0, 19);
    rev = recv(cfd, m_pID, 18, 0);
    if(rev < 0)
    {
        perror("Open::ID : ");
        return -1;
    }

    m_pPasswd = new char[17];
    memset(m_pPasswd, 0, 17);
    rev = recv(cfd, m_pPasswd, 16, 0);
    if(rev < 0)
    {
        perror("Open::passwd : ");
        return -1;
    }

    printf("name:%s\n",m_pName);
    printf("id:%s\n",m_pID);
    printf("passwd:%s\n",m_pPasswd);
    printf("money:%s\n\n",m_pMoney);

    //数据读取完毕,进行数据库处理,并把处理的结果返回给客户端
    int s = DataBase();

    send(cfd, &s, sizeof(s), 0);

    if(s == REGIST_SUCCESS)
    {
        send(cfd, &m_iID, sizeof(m_iID), 0);
    }

    return 0;
}

int Open::DataBase()
{
    //printf("DataBase deal\n");
    char buf[1024];
    My_Sql sql("BankData");
    int result = 0;
    unsigned int rand_num;

    strcpy(buf,"create table if not exists UserInformation(\
           id int unsigned not null primary key,\
           name char(10) not null,\
           passwd char(16) not null,\
           identify char(18) not null,\
           money double(10,3) not null default 0);");
    sql.MySql_exec(buf);

    sprintf(buf,"select *from UserInformation where identify=\"%s\";",m_pID);
    sql.MySql_exec(buf);
    printf("row number:%d\n",sql.m_iNumRows);
    if(sql.m_iNumRows == 0)  //没有该用户,可以开户
    {
        srand(time(NULL));
        rand_num = rand()%899999 + 100000;
        //itoa(rand_num,m_aID,10);
        m_iID = rand_num;
        sprintf(buf,"insert into UserInformation values(%d,\"%s\",\"%s\",\"%s\",\"%s\");",m_iID,m_pName,m_pPasswd,m_pID,m_pMoney);
        sql.MySql_exec(buf);

        sprintf(buf,"create table _%d(\
                number int unsigned not null auto_increment primary key,\
                deposit double(10,3) not null default 0,\
                draw double(10,3) not null default 0,\
                balance double(10,3) not null,\
                other_account_id int unsigned not null default 0,\
                action char(10) not null default '0',\
                Date timestamp DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP\
                );",m_iID);
        sql.MySql_exec(buf);
        printf("create table successfully!\n");
        sprintf(buf,"insert into _%d(deposit,balance,action) values(\"%s\",\"%s\",\"%s\");",m_iID,m_pMoney,m_pMoney,OPEN);
        sql.MySql_exec(buf);
        printf("insert successfully!\n");

        result = REGIST_SUCCESS;
    }
    else
    {
        result = HAS_ACCOUNT;
    }

    return result;
}

