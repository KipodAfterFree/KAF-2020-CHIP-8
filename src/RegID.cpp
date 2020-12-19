#include "RegID.h"

std::ostream &operator<<(std::ostream &stream, const RegID reg)
{
    const char *regRep = nullptr;
    switch (reg)
    {
        case RegID::V0:
            regRep = "V0";
            break;
        case RegID::V1:
            regRep = "V1";
            break;
        case RegID::V2:
            regRep = "V2";
            break;
        case RegID::V3:
            regRep = "V3";
            break;
        case RegID::V4:
            regRep = "V4";
            break;
        case RegID::V5:
            regRep = "V5";
            break;
        case RegID::V6:
            regRep = "V6";
            break;
        case RegID::V7:
            regRep = "V7";
            break;
        case RegID::V8:
            regRep = "V8";
            break;
        case RegID::V9:
            regRep = "V9";
            break;
        case RegID::VA:
            regRep = "VA";
            break;
        case RegID::VB:
            regRep = "VB";
            break;
        case RegID::VC:
            regRep = "VC";
            break;
        case RegID::VD:
            regRep = "VD";
            break;
        case RegID::VE:
            regRep = "VE";
            break;
        case RegID::VF:
            regRep = "VF";
            break;
    }
    return stream << regRep;
}