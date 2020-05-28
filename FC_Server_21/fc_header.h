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

//part three -- profile about
//#define FC_ACCOUNT_LENGTH unsigned(6)
#define FC_PROFILE unsigned(1<<29)
#define FC_SIGN_IN unsigned(FC_PROFILE | 1<<3)  //login request


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
