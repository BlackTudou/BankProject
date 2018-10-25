#include "DynBase.h"
#include "FuncFactory.h"

REGISTER_CLASS(regi);
REGISTER_CLASS(pass);

REGISTER_CLASS(open);
REGISTER_CLASS(dpst);
REGISTER_CLASS(draw);
REGISTER_CLASS(tran);
REGISTER_CLASS(inqu);
REGISTER_CLASS(chpw);
REGISTER_CLASS(dest);
REGISTER_CLASS(hist);

AB_Function* pass::CreateFunc()
{
    printf("PassFactory\n");
    return Pass::GetPass();
}

AB_Function* regi::CreateFunc()
{
    printf("RegFactory\n");
    return Reg::GetReg();
}

AB_Function * open::CreateFunc()
{
    return Open::GetOpen();
}

AB_Function * dpst::CreateFunc()
{
    return Deposit::GetDeposit();
}

AB_Function * draw::CreateFunc()
{
    return Withdraw::GetWithdraw();
}

AB_Function * tran::CreateFunc()
{
    return Transfer::GetTransfer();
}

AB_Function * inqu::CreateFunc()
{
    return Inquiry::GetInquiry();
}

AB_Function * chpw::CreateFunc()
{
    return ChPasswd::GetChPasswd();
}

AB_Function * dest::CreateFunc()
{
    return Destroy::GetDestroy();
}

AB_Function * hist::CreateFunc()
{
    return History::GetHistory();
}

