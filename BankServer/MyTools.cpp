#include "MyTools.h"

int MyTools::DataHandle(vector<string>& temp)
{
    vector<string>::iterator iter;
    for(iter = temp.begin(); iter != temp.end(); iter++)
    {
        if(*iter == "1")
        {
            return 1;
        }
    }

    return 0;
}
