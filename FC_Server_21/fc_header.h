#ifndef FC_HEADER_H
#define FC_HEADER_H

//==============================================
//  Network protocol design
//==============================================
/*
    const unsigned FC_PROFILE = 1<<29;
    const unsigned FC_SIGN_IN = FC_PROFILE | 1<<3;
 */
//most body length
#define FC_MESSAGE_BODY_LENGTH 1024


//part one -- message about
#define FC_MESSAGES unsigned(1<<31)
#define FC_TEXT_MEG unsigned(FC_MESSAGES | 1<<1)

//part two -- friend about
#define FC_FRIENDS              unsigned(1<<30)
#define FC_FRIENDS_SEARCH       unsigned(FC_FRIENDS | 1<<1)
#define FC_FRIENDS_SEARCH_R     unsigned(FC_FRIENDS | 1<<2)
#define FC_FRIENDS_ADD          unsigned(FC_FRIENDS | 1<<3)
#define FC_FRIENDS_ADD_R        unsigned(FC_FRIENDS | 1<<4)
#define FC_DELETE_FRIENDS       unsigned(FC_FRIENDS | 1<<5)
#define FC_FRIENDS_MESSAGE      unsigned(FC_FRIENDS | 1<<6)
#define FC_FRIENDS_MES          unsigned(FC_FRIENDS | 1<<7) //好友信息（备注，分组）
#define FC_FRIENDS_REMARK       unsigned(FC_FRIENDS_MES | 1<<0) //备注
//#define FC_FRIENDS_REMARK_R       unsigned(FC_FRIENDS_MES | 1<<0) //备注返回信息
#define FC_FRIENDS_GROUP        unsigned(FC_FRIENDS_MES | 1<<1) //分组
#define FC_FRIENDS_TEST         unsigned(FC_FRIENDS | 1<<0) //测试数据

//part three -- profile about
//#define FC_ACCOUNT_LENGTH unsigned(6)
#define FC_PROFILE              unsigned(1<<29)
#define FC_SIGN_IN              unsigned(FC_PROFILE | 1<<3)       //login request
#define FC_SIGN_IN_R            unsigned(FC_PROFILE | 1<<4)       //login result
#define FC_SELF_MES             unsigned(FC_PROFILE | 1<<5)       //self message
#define FC_UPDATE_NICK          unsigned(FC_PROFILE | 1<<6)       //修改网名
#define FC_UPDATE_HEAD          unsigned(FC_PROFILE | 1<<7)       //修改头像
#define FC_UPDATE_SEX           unsigned(FC_PROFILE | 1<<8)       //修改性别


//part four -- dynamic about
#define FC_DYNAMICS unsigned(1<<28)

//part five -- undifined
#define FC_UNDIFINED unsigned(1<<27)

//other
#define FC_ACC_LEN 6 //account length


#endif // HEADER_H


//==============================================
//  private function
//==============================================
