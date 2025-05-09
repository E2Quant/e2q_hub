/*
 * =====================================================================================
 *
 *       Filename:  E2Zts.cpp
 *
 *    Description:  E2Zts
 *
 *        Version:  1.0
 *        Created:  2025/02/07 11时42分06秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  vyouzhi (vz), vyouzhi@gmail.com
 *   Organization:  Etomc2.com
 *        LICENSE:  BSD-3-Clause license
 *
 *  Copyright (c) 2019-2022, vyouzhi
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  * Redistributions of source code must retain the above copyright
 *  notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *  notice, this list of conditions and the following disclaimer in the
 *  documentation and/or other materials provided with the distribution.
 *  * Neither the name of vyouzhi and/or the DataFrame nor the
 *  names of its contributors may be used to endorse or promote products
 *  derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL vyouzhi BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * =====================================================================================
 */
#include "E2Zts.hpp"

#include <memory>

#include "E2LScript/ExternClazz.hpp"
#include "OMSPack/SessionGlobal.hpp"
#include "Toolkit/GlobalConfig.hpp"
#include "ZtsMatch.hpp"
namespace e2q {
// #ifdef ZTS_TRADER
using namespace E2ZTS;

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ZMatcher
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
void ZMatcher(const char* config)
{
    if (GlobalMatcher == nullptr) {
        GlobalMatcher = std::make_shared<ZTSMatch>(config);
    }
} /* -----  end of function ZMatcher  ----- */
/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CallE2Zts
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
void CallE2Zts(std::vector<e2q::E2lFun_t>& funs)
{
    AddFunExt(funs, E2ZTS, ZMatcher, 1, "ZMatcher", E2L_NORETURN, "(config);");
} /* -----  end of function CallE2Zts  ----- */
// #endif
}  // namespace e2q
