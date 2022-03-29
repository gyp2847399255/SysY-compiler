//
// Created by gyp on 2021/10/25.
//

#ifndef SYSYCOMPILER_CFG_LIB_H
#define SYSYCOMPILER_CFG_LIB_H
#include "Quaternion/Quaternion.h"
#include "Quaternion/PhiFunction.h"
#include "Quaternion/Binary.h"
#include "RegisterAllocate.h"
#include "BasicBlock.h"
#include "Quaternion/NotEqual.h"
#include "Quaternion/Equal.h"
#include "Quaternion/LessThan.h"
#include "Quaternion/GreaterThan.h"
#include "Quaternion/LessOrEqual.h"
#include "Quaternion/GreaterOrEqual.h"
#include "Quaternion/Plus.h"
#include "Quaternion/Minus.h"
#include "Quaternion/Multiply.h"
#include "Quaternion/Divide.h"
#include "Quaternion/Mod.h"
#include "Quaternion/FuncCall.h"
#include "Quaternion/Jump.h"
#include "Quaternion/JumpWhenTrue.h"
#include "Quaternion/JumpWhenFalse.h"
#include "Quaternion/NotLogic.h"
#include "Quaternion/Negate.h"
#include "Quaternion/StoreWord.h"
#include "Quaternion/LoadWord.h"
#include "Quaternion/LoadPointer.h"
#include "Quaternion/CreatePointer.h"
#include "Quaternion/Assign.h"
#include "Quaternion/PrintString.h"
#include "Quaternion/PrintInteger.h"
#include "Quaternion/Return.h"
#include "Quaternion/Getint.h"
#include "Quaternion/ReadInt.h"
#include "Quaternion/GetReturn.h"
#include "Quaternion/PushParam.h"
#include "Quaternion/LoadParam.h"
#include "ControlFlowGraphBuilder.h"
#endif //SYSYCOMPILER_CFG_LIB_H