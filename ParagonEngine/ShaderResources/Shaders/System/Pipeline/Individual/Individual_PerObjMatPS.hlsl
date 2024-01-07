#include "../../Libraries/System_PerObjMatLayouts.hlsli"

POutPerObjMat main(VOutPerObjMat input)
{
    POutPerObjMat output;
    
    //<uint2>
    //Object ID 전달.
    output.pout_ObjMat.x = input.vout1st_ObjID;
    //Material ID 전달.
    output.pout_ObjMat.y = input.vout1st_MatID;
    
    return output;
}