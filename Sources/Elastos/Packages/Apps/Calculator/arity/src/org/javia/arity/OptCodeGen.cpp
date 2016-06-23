
#include "org/javia/arity/OptCodeGen.h"
#include "org/javia/arity/Lexer.h"
#include "org/javia/arity/VM.h"
#include <elastos/core/Math.h>
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;

namespace Org {
namespace Javia {
namespace Arity {

OptCodeGen::OptCodeGen()
    : SimpleCodeGen()
    , mSp(0)
    , mIntrinsicArity(0)
    , mIsPercent(FALSE)
{
    mContext = new EvalContext();
    AutoPtr<ArrayOf<IComplex*> > mStack = mContext->mStackComplex;
    AutoPtr<ArrayOf<Double> > mTraceConstsRe = ArrayOf<Double>::Alloc(1);
    AutoPtr<ArrayOf<Double> > mTraceConstsIm = ArrayOf<Double>::Alloc(1);
    AutoPtr<ArrayOf<IFunction*> > mTraceFuncs = ArrayOf<IFunction*>(1);
    AutoPtr<ArrayOf<Byte> > mTraceCode = ArrayOf<Byte>(1);
    CCompiledFunction::NewByFriend(0, mTraceCode, mTraceConstsRe, mTraceConstsIm, mTraceFuncs,
            (CCompiledFunction**)&mTracer);
}

void OptCodeGen::Start()
{
    SimpleCodeGen::Start();
    mSp = -1;
    mIntrinsicArity = 0;
    mIsPercent = FALSE;
}

ECode OptCodeGen::Push(
    /* [in] */ Token* token)
{
    // System.err.println("state " + getFun(0) + "; token " + token);
    Boolean prevWasPercent = mIsPercent;
    mIsPercent = FALSE;
    Byte op;
    switch (token->mId) {
    case Lexer::NUMBER:
        op = VM::CONST;
        (*mTraceConstsRe)[0] = token->mValue;
        (*mTraceConstsIm)[0] = 0;
        break;

    case Lexer::CONST:
    case Lexer::CALL: {
        AutoPtr<Symbol> symbol;
        FAIL_RETURN(GetSymbol(token, (Symbol**)&symbol))
        if (token->IsDerivative()) {
            op = VM::CALL;
            AutoPtr<IFunction> temp
            symbol->mFun->GetDerivative((IFunction**)&temp);
            mTraceFuncs->Set(0, temp);
        }
        else if (symbol->mOp > 0) { // built-in
            op = symbol->mOp;
            if (op >= VM::LOAD0 && op <= VM::LOAD4) {
                Int32 arg = op - VM::LOAD0;
                if (arg + 1 > mIntrinsicArity) {
                    mIntrinsicArity = arg + 1;
                }
                ((CComplex*)(*mStack)[++mSp].Get())->mRe = Elastos::Core::Math::DOUBLE_NAN;
                ((CComplex*)(*mStack)[mSp].Get())->mIm = 0;
                mCode->Push(op);
                //System.out.println("op " + VM.opcodeName[op] + "; sp " + sp + "; top " + stack[sp]);
                return NOERROR;
            }
        }
        else if (symbol->mFun != NULL) { // function call
            op = VM::CALL;
            mTraceFuncs->Set(0, symbol->mFun);
        }
        else { // variable reference
            op = VM::CONST;
            (*mTraceConstsRe)[0] = symbol->mValueRe;
            (*mTraceConstsIm)[0] = symbol->mValueIm;
        }
        break;
    }

    default:
        op = token->mVmop;
        if (op <= 0) {
            Slogger::E("OptCodeGen", "wrong vmop: %d", op);
            return E_SYNTAX_EXCEPTION;
        }
        if (op == VM::PERCENT) {
            mIsPercent = TRUE;
        }
    }
    Int32 oldSP = mSp;
    (*mTraceCode)[0] = op;
    if (op != VM::RND) {
        mTracer->ExecWithoutCheckComplex(context, sp, prevWasPercent ? -1 : -2, &mSp);
    }
    else {
        ((CComplex*)(*mStack)[++mSp].Get())->mRe = Elastos::Core::Math::DOUBLE_NAN;
        ((CComplex*)(*mStack)[mSp].Get())->mIm = 0;
    }

    //System.out.println("op " + VM.opcodeName[op] + "; old " + oldSP + "; sp " + sp + "; top " + stack[sp] + " " + stack[0]);

    //constant folding
    Boolean isNaN;
    if (((*mStack)[mSp]->IsNaN(&isNaN), !isNaN) || op == VM::CONST) {
        Int32 nPopCode;
        if (op == VM::CALL) {
            (*mTraceFuncs)[0]->GetArity(&nPopCode);
        }
        else {
            nPopCode = (*VM::ARITY)[op];
        }
        while (nPopCode > 0) {
            Byte pop = mCode->Pop();
            if (pop == VM::CONST) {
                mConsts->Pop();
            }
            else if (pop == VM::CALL) {
                AutoPtr<IFunction> f = mFuncs->Pop();
                Int32 arity;
                f->Arity(&arity);
                nPopCode += arity - 1;
            }
            else {
                nPopCode += (*VM::ARITY)[pop];
            }
            --nPopCode;
        }
        mConsts->Push(((CComplex*)(*mStack)[mSp].Get())->mRe, ((CComplex*)(*mStack)[mSp].Get())->mIm);
        op = VM::CONST;
    }
    else if (op == VM::CALL) {
        mFuncs->Push((*mTraceFuncs)[0]);
    }
    mCode->Push(op);
    return NOERROR;
}

AutoPtr<CCompiledFunction> OptCodeGen::GetFun(
    /* [in] */ Int32 arity)
{
    AutoPtr<CCompiledFunction> func;
    CCompiledFunction::NewByFriend(arity, mCode->ToArray(), mConsts->GetRe(),
            mConsts->GetIm(), mFuncs->ToArray(), (CCompiledFunction**)&func);
    return func;
}

} // namespace Arity
} // namespace Javia
} // namespace Org