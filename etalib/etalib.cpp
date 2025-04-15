/*
 * =====================================================================================
 *
 *       Filename:  etalib.cpp
 *
 *    Description:  etalib
 *
 *        Version:  1.0
 *        Created:  2024年07月30日 13时59分07秒
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

#include "etalib.hpp"

#include <ta-lib/ta_common.h>
#include <ta-lib/ta_defs.h>
#include <ta-lib/ta_func.h>

#include <array>
#include <cstddef>
#include <cstdio>
#include <cstdlib>

#include "E2L/E2LType.hpp"
#include "E2LScript/util_inline.hpp"
#include "assembler/BaseType.hpp"

namespace ETALIB {
using namespace e2q;

// #ifdef E2_TALIB
//  Overlap Studies

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  BBANDS
 *  ->  e2::Int_e *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Bool BBANDS(e2::Int_e array_id, e2::Int_e timeperiodi, e2::Int_e ret_id)
{
    log::echo("array_id:", array_id);
    e2::Bool ret = e2::Bool::B_FALSE;
    return ret;

} /* -----  end of function BBANDS  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  BaseMA
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Bool BaseMA(e2::Int_e array_id, e2::Int_e timeperiod, e2::Int_e ret_id,
                TA_MAType mtype)
{
    e2::Bool ret = e2q::e2_share_array.update(ret_id, 0, 0);

    std::size_t shift = NUMBERVAL(timeperiod);

    TA_Real* ptr = (TA_Real*)calloc(shift, sizeof(TA_Real));
    TA_Real out[1];
    TA_Real outfama[1];
    bool e = e2q::e2_share_array.Ptr(array_id, &ptr, shift);
    if (e) {
        TA_Integer outBeg;
        TA_Integer outNbElement;

        TA_RetCode retcode;

        if (mtype == TA_MAType_MAMA) {
            retcode = TA_MAMA(0, (shift - 1), ptr, 0, 0, &outBeg, &outNbElement,
                              out, outfama);
        }
        else if (mtype == TA_MAType_T3) {
            retcode = TA_T3(0, (shift - 1), ptr, shift, 0, &outBeg,
                            &outNbElement, out);
        }
        else {
            retcode = TA_MA(0, (shift - 1), ptr, shift, mtype, &outBeg,
                            &outNbElement, out);
        }
        if (retcode == TA_SUCCESS) {
            ret = e2::Bool::B_TRUE;

            e2::Int_e val = VALNUMBER(out[0]);
            e2_share_array.update(ret_id, 0, 1);
            e2_share_array.update(ret_id, 1, val);

            if (e2_share_array.length(ret_id) == 3) {
                val = VALNUMBER(outfama[0]);
                e2_share_array.update(ret_id, 2, val);
            }
        }
    }

    if (!ptr) {
        free(ptr);
    }

    return ret;
} /* -----  end of function BaseMA  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  MA
 *  ->  e2::Int_e *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Bool MA(e2::Int_e array_id, e2::Int_e timeperiod, e2::Int_e ret_id)
{
    e2::Bool ret = BaseMA(array_id, timeperiod, ret_id, TA_MAType_SMA);

    return ret;
} /* -----  end of function MA  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  SMA
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Bool SMA(e2::Int_e array_id, e2::Int_e timeperiod, e2::Int_e ret_id)
{
    e2::Bool ret = BaseMA(array_id, timeperiod, ret_id, TA_MAType_SMA);

    return ret;
} /* -----  end of function SMA  ----- */
/*
 * ===  FUNCTION  =============================
 *
 *         Name:  EMA
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Bool EMA(e2::Int_e array_id, e2::Int_e timeperiod, e2::Int_e ret_id)
{
    e2::Bool ret = BaseMA(array_id, timeperiod, ret_id, TA_MAType_EMA);

    return ret;
} /* -----  end of function EMA  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  WMA
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Bool WMA(e2::Int_e array_id, e2::Int_e timeperiod, e2::Int_e ret_id)
{
    e2::Bool ret = BaseMA(array_id, timeperiod, ret_id, TA_MAType_WMA);

    return ret;
} /* -----  end of function WMA  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  DEMA
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Bool DEMA(e2::Int_e array_id, e2::Int_e timeperiod, e2::Int_e ret_id)
{
    e2::Bool ret = BaseMA(array_id, timeperiod, ret_id, TA_MAType_DEMA);

    return ret;
} /* -----  end of function DEMA  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  TEMA
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Bool TEMA(e2::Int_e array_id, e2::Int_e timeperiod, e2::Int_e ret_id)
{
    e2::Bool ret = BaseMA(array_id, timeperiod, ret_id, TA_MAType_TEMA);

    return ret;
} /* -----  end of function TEMA  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  TRIMA
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Bool TRIMA(e2::Int_e array_id, e2::Int_e timeperiod, e2::Int_e ret_id)
{
    e2::Bool ret = BaseMA(array_id, timeperiod, ret_id, TA_MAType_TRIMA);

    return ret;
} /* -----  end of function TRIMA  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  KAMA
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Bool KAMA(e2::Int_e array_id, e2::Int_e timeperiod, e2::Int_e ret_id)
{
    e2::Bool ret = BaseMA(array_id, timeperiod, ret_id, TA_MAType_KAMA);

    return ret;
} /* -----  end of function KAMA  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  MAMA
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Bool MAMA(e2::Int_e array_id, e2::Int_e ret_id)
{
    e2::Bool ret = BaseMA(array_id, 0, ret_id, TA_MAType_MAMA);

    return ret;
} /* -----  end of function MAMA  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  T3
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Bool T3(e2::Int_e array_id, e2::Int_e timeperiod, e2::Int_e ret_id)
{
    e2::Bool ret = BaseMA(array_id, timeperiod, ret_id, TA_MAType_T3);

    return ret;
} /* -----  end of function T3  ----- */
// Momentum Indicators

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  MACD
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *  https://github.com/TA-Lib/ta-lib-python/issues/65
 *  You can see the C code that implements MACD here:
 *http://sourceforge.net/p/ta-lib/code/HEAD/tree/trunk/ta-lib/c/src/ta_func/ta_MACD.c
 *They define the lookback before values are created for the 26/12/9 MACD as 33
 *because it adds 8 (9-1) and 25 (26-1). You could think of implementing MACD
 *yourself here:
 * ============================================
 */
e2::Bool MACD(e2::Int_e array_id, e2::Int_e fastperiod, e2::Int_e slowperiod,
              e2::Int_e signalperiod, e2::Int_e ret_id)
{
    std::size_t fast = NUMBERVAL(fastperiod);
    std::size_t slow = NUMBERVAL(slowperiod);
    std::size_t sig = NUMBERVAL(signalperiod);

    e2::Bool ret = e2::Bool::B_FALSE;

    std::size_t insize = e2_share_array.length(array_id);

    TA_Real* inReal = (TA_Real*)calloc(insize, sizeof(TA_Real));
    TA_Real outMacd[1];
    TA_Real outSig[1];
    TA_Real outHist[1];

    bool e = e2_share_array.Ptr(array_id, &inReal, insize);
    if (e) {
        TA_Integer outBeg;
        TA_Integer outNbElement;

        TA_RetCode retcode =
            TA_MACD(0, (insize - 1), inReal, fast, slow, sig, &outBeg,
                    &outNbElement, outMacd, outSig, outHist);
        if (retcode == TA_SUCCESS && outNbElement > 0) {
            ret = e2::Bool::B_TRUE;

            e2::Int_e val_macd = VALNUMBER(outMacd[0]);
            e2::Int_e val_sig = VALNUMBER(outSig[0]);
            e2::Int_e val_hist = VALNUMBER(outHist[0]);

            e2_share_array.update(ret_id, 0, 1);
            e2_share_array.update(ret_id, 1, val_macd);
            e2_share_array.update(ret_id, 2, val_sig);
            e2_share_array.update(ret_id, 3, val_hist);
        }
    }

    if (!inReal) {
        free(inReal);
    }

    return ret;
} /* -----  end of function MACD  ----- */

// Pattern Recognition
// bar array id, if id == 0 default bar

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLInit
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Bool CDLInit(e2::Int_e id, e2::TimeFrames timeframe, e2::Int_e array_id)
{
    std::size_t _id = NUMBERVAL(id);
    std::size_t _timeframe = (std::size_t)NUMBERVAL(timeframe);

    std::array<std::array<e2q::SeqType, ohlc_column>, cdl_rows> bar;
    e2::Int_e ret = 0;
    int endIdx = cdl_rows - 1;

    e2::Bool b = e2::Bool::B_FALSE;

    if (e2q::e2l_cnt == nullptr) {
        return e2::Bool::B_FALSE;
    }

    if (cdl_time == ticket_now) {
        return e2::Bool::B_TRUE;
    }
    if (array_id == 0) {
        // default bar
        ret = e2q::e2l_cnt->data_ptr->writed(_id, _timeframe);
        if (ret < endIdx) {
            return e2::Bool::B_FALSE;
        }
        ret = e2q::e2l_cnt->data_ptr->reads(bar, _id, _timeframe);
        if (ret == -1) {
            return e2::Bool::B_FALSE;
        }

        cdl_time = bar[0][e2q::OHLC_T::ohlc_t];

        std::size_t m = 0;

        for (auto it : bar) {
            cdl_inOpen[m] = (double)NUMBERVAL(it[e2q::OHLC_T::open_t]);
            cdl_inHigh[m] = (double)NUMBERVAL(it[e2q::OHLC_T::high_t]);
            cdl_inLow[m] = (double)NUMBERVAL(it[e2q::OHLC_T::low_t]);
            cdl_inClose[m] = (double)NUMBERVAL(it[e2q::OHLC_T::close_t]);
            m++;
        }

        b = e2::Bool::B_TRUE;
    }
    else {
        // user define
    }

    return b;
} /* -----  end of function CDLInit  ----- */
/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDL2CROWS
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDL2CROWS()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDL2CROWS(startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0],
                           &cdl_inLow[0], &cdl_inClose[0], &outBeg,
                           &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDL2CROWS  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDL3BLACKCROWS
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDL3BLACKCROWS()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDL3BLACKCROWS(startIdx, endIdx, &cdl_inOpen[0],
                                &cdl_inHigh[0], &cdl_inLow[0], &cdl_inClose[0],
                                &outBeg, &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDL3BLACKCROWS  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDL3INSIDE
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDL3INSIDE()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDL3INSIDE(startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0],
                            &cdl_inLow[0], &cdl_inClose[0], &outBeg,
                            &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDL3INSIDE  ----- */
/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDL3LINESTRIKE
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDL3LINESTRIKE()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDL3LINESTRIKE(startIdx, endIdx, &cdl_inOpen[0],
                                &cdl_inHigh[0], &cdl_inLow[0], &cdl_inClose[0],
                                &outBeg, &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDL3LINESTRIKE  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDL3OUTSIDE
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDL3OUTSIDE()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDL3OUTSIDE(startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0],
                             &cdl_inLow[0], &cdl_inClose[0], &outBeg,
                             &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDL3OUTSIDE  ----- */
/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDL3STARSINSOUTH
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDL3STARSINSOUTH()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDL3STARSINSOUTH(
        startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0], &cdl_inLow[0],
        &cdl_inClose[0], &outBeg, &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDL3STARSINSOUTH  ----- */
/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDL3WHITESOLDIERS
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDL3WHITESOLDIERS()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDL3WHITESOLDIERS(
        startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0], &cdl_inLow[0],
        &cdl_inClose[0], &outBeg, &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDL3WHITESOLDIERS  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLABANDONEDBABY
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLABANDONEDBABY()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLABANDONEDBABY(
        startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0], &cdl_inLow[0],
        &cdl_inClose[0], 0, &outBeg, &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLABANDONEDBABY  ----- */
/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLADVANCEBLOCK
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLADVANCEBLOCK()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLADVANCEBLOCK(startIdx, endIdx, &cdl_inOpen[0],
                                 &cdl_inHigh[0], &cdl_inLow[0], &cdl_inClose[0],
                                 &outBeg, &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLADVANCEBLOCK  ----- */
/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLBELTHOLD
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLBELTHOLD()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLBELTHOLD(startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0],
                             &cdl_inLow[0], &cdl_inClose[0], &outBeg,
                             &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLBELTHOLD  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLBREAKAWAY
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLBREAKAWAY()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLBREAKAWAY(startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0],
                              &cdl_inLow[0], &cdl_inClose[0], &outBeg,
                              &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLBREAKAWAY  ----- */
/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLCLOSINGMARUBOZU
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLCLOSINGMARUBOZU()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLCLOSINGMARUBOZU(
        startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0], &cdl_inLow[0],
        &cdl_inClose[0], &outBeg, &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLCLOSINGMARUBOZU  ----- */
/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLCONCEALBABYSWALL
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLCONCEALBABYSWALL()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLCONCEALBABYSWALL(
        startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0], &cdl_inLow[0],
        &cdl_inClose[0], &outBeg, &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLCONCEALBABYSWALL  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLCOUNTERATTACK
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLCOUNTERATTACK()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLCOUNTERATTACK(
        startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0], &cdl_inLow[0],
        &cdl_inClose[0], &outBeg, &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLCOUNTERATTACK  ----- */
/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLDARKCLOUDCOVER
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLDARKCLOUDCOVER()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLDARKCLOUDCOVER(
        startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0], &cdl_inLow[0],
        &cdl_inClose[0], 0, &outBeg, &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLDARKCLOUDCOVER  ----- */
/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLDOJI
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLDOJI()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLDOJI(startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0],
                         &cdl_inLow[0], &cdl_inClose[0], &outBeg, &outNbElement,
                         &outInteger[0]);
    /* for (std::size_t m = 0; m < cdl_rows; m++) { */
    /*     std::cout << outInteger[m] << ","; */
    /* } */
    /* std::cout << " : " << std::endl; */
    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLDOJI  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLDOJISTAR
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLDOJISTAR()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLDOJISTAR(startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0],
                             &cdl_inLow[0], &cdl_inClose[0], &outBeg,
                             &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLDOJISTAR  ----- */
/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLDRAGONFLYDOJI
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLDRAGONFLYDOJI()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLDRAGONFLYDOJI(
        startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0], &cdl_inLow[0],
        &cdl_inClose[0], &outBeg, &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLDRAGONFLYDOJI  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLENGULFING
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLENGULFING()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLENGULFING(startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0],
                              &cdl_inLow[0], &cdl_inClose[0], &outBeg,
                              &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLENGULFING  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLEVENINGDOJISTAR
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLEVENINGDOJISTAR()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLEVENINGDOJISTAR(
        startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0], &cdl_inLow[0],
        &cdl_inClose[0], 0, &outBeg, &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLEVENINGDOJISTAR  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLEVENINGSTAR
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLEVENINGSTAR()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLEVENINGSTAR(startIdx, endIdx, &cdl_inOpen[0],
                                &cdl_inHigh[0], &cdl_inLow[0], &cdl_inClose[0],
                                0, &outBeg, &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLEVENINGSTAR  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLGAPSIDESIDEWHITE
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLGAPSIDESIDEWHITE()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLGAPSIDESIDEWHITE(
        startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0], &cdl_inLow[0],
        &cdl_inClose[0], &outBeg, &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLGAPSIDESIDEWHITE  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLGRAVESTONEDOJI
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLGRAVESTONEDOJI()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLGRAVESTONEDOJI(
        startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0], &cdl_inLow[0],
        &cdl_inClose[0], &outBeg, &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLGRAVESTONEDOJI  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLHAMMER
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLHAMMER()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLHAMMER(startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0],
                           &cdl_inLow[0], &cdl_inClose[0], &outBeg,
                           &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLHAMMER  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLHANGINGMAN
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLHANGINGMAN()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLHANGINGMAN(startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0],
                               &cdl_inLow[0], &cdl_inClose[0], &outBeg,
                               &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLHANGINGMAN  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLHARAMI
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLHARAMI()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLHARAMI(startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0],
                           &cdl_inLow[0], &cdl_inClose[0], &outBeg,
                           &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLHARAMI  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLHARAMICROSS
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLHARAMICROSS()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLHARAMICROSS(startIdx, endIdx, &cdl_inOpen[0],
                                &cdl_inHigh[0], &cdl_inLow[0], &cdl_inClose[0],
                                &outBeg, &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLHARAMICROSS  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLHIGHWAVE
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLHIGHWAVE()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLHIGHWAVE(startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0],
                             &cdl_inLow[0], &cdl_inClose[0], &outBeg,
                             &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLHIGHWAVE  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLHIKKAKE
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLHIKKAKE()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLHIKKAKE(startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0],
                            &cdl_inLow[0], &cdl_inClose[0], &outBeg,
                            &outNbElement, &outInteger[0]);

    /* for (std::size_t m = 0; m < cdl_rows; m++) { */
    /*     std::cout << outInteger[m] << ","; */
    /* } */
    /* std::cout << " ; " << std::endl; */
    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLHIKKAKE  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLHIKKAKEMOD
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLHIKKAKEMOD()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLHIKKAKEMOD(startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0],
                               &cdl_inLow[0], &cdl_inClose[0], &outBeg,
                               &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLHIKKAKEMOD  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLHOMINGPIGEON
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLHOMINGPIGEON()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLHOMINGPIGEON(startIdx, endIdx, &cdl_inOpen[0],
                                 &cdl_inHigh[0], &cdl_inLow[0], &cdl_inClose[0],
                                 &outBeg, &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLHOMINGPIGEON  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLIDENTICAL3CROWS
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLIDENTICAL3CROWS()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLIDENTICAL3CROWS(
        startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0], &cdl_inLow[0],
        &cdl_inClose[0], &outBeg, &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLIDENTICAL3CROWS  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLINNECK
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLINNECK()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLINNECK(startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0],
                           &cdl_inLow[0], &cdl_inClose[0], &outBeg,
                           &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLINNECK  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLINVERTEDHAMMER
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLINVERTEDHAMMER()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLINVERTEDHAMMER(
        startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0], &cdl_inLow[0],
        &cdl_inClose[0], &outBeg, &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLINVERTEDHAMMER  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLKICKING
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLKICKING()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLKICKING(startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0],
                            &cdl_inLow[0], &cdl_inClose[0], &outBeg,
                            &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLKICKING  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLKICKINGBYLENGTH
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLKICKINGBYLENGTH()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLKICKINGBYLENGTH(
        startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0], &cdl_inLow[0],
        &cdl_inClose[0], &outBeg, &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLKICKINGBYLENGTH  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLLADDERBOTTOM
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLLADDERBOTTOM()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLLADDERBOTTOM(startIdx, endIdx, &cdl_inOpen[0],
                                 &cdl_inHigh[0], &cdl_inLow[0], &cdl_inClose[0],
                                 &outBeg, &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLLADDERBOTTOM  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLLONGLEGGEDDOJI
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLLONGLEGGEDDOJI()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLLONGLEGGEDDOJI(
        startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0], &cdl_inLow[0],
        &cdl_inClose[0], &outBeg, &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLLONGLEGGEDDOJI  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLLONGLINE
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLLONGLINE()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLLONGLINE(startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0],
                             &cdl_inLow[0], &cdl_inClose[0], &outBeg,
                             &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLLONGLINE  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLMARUBOZU
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLMARUBOZU()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLMARUBOZU(startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0],
                             &cdl_inLow[0], &cdl_inClose[0], &outBeg,
                             &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLMARUBOZU  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLMATCHINGLOW
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLMATCHINGLOW()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLMATCHINGLOW(startIdx, endIdx, &cdl_inOpen[0],
                                &cdl_inHigh[0], &cdl_inLow[0], &cdl_inClose[0],
                                &outBeg, &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLMATCHINGLOW  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLMATHOLD
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLMATHOLD()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLMATHOLD(startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0],
                            &cdl_inLow[0], &cdl_inClose[0], 0, &outBeg,
                            &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLMATHOLD  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLMORNINGDOJISTAR
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLMORNINGDOJISTAR()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLMORNINGDOJISTAR(
        startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0], &cdl_inLow[0],
        &cdl_inClose[0], 0, &outBeg, &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLMORNINGDOJISTAR  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLMORNINGSTAR
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLMORNINGSTAR()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLMORNINGSTAR(startIdx, endIdx, &cdl_inOpen[0],
                                &cdl_inHigh[0], &cdl_inLow[0], &cdl_inClose[0],
                                0, &outBeg, &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLMORNINGSTAR  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLONNECK
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLONNECK()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLONNECK(startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0],
                           &cdl_inLow[0], &cdl_inClose[0], &outBeg,
                           &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLONNECK  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLPIERCING
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLPIERCING()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLPIERCING(startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0],
                             &cdl_inLow[0], &cdl_inClose[0], &outBeg,
                             &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLPIERCING  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLRICKSHAWMAN
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLRICKSHAWMAN()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLRICKSHAWMAN(startIdx, endIdx, &cdl_inOpen[0],
                                &cdl_inHigh[0], &cdl_inLow[0], &cdl_inClose[0],
                                &outBeg, &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLRICKSHAWMAN  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLRISEFALL3METHODS
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLRISEFALL3METHODS()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLRISEFALL3METHODS(
        startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0], &cdl_inLow[0],
        &cdl_inClose[0], &outBeg, &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLRISEFALL3METHODS  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLSEPARATINGLINES
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLSEPARATINGLINES()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLSEPARATINGLINES(
        startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0], &cdl_inLow[0],
        &cdl_inClose[0], &outBeg, &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLSEPARATINGLINES  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLSHOOTINGSTAR
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLSHOOTINGSTAR()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLSHOOTINGSTAR(startIdx, endIdx, &cdl_inOpen[0],
                                 &cdl_inHigh[0], &cdl_inLow[0], &cdl_inClose[0],
                                 &outBeg, &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLSHOOTINGSTAR  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLSHORTLINE
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLSHORTLINE()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLSHORTLINE(startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0],
                              &cdl_inLow[0], &cdl_inClose[0], &outBeg,
                              &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLSHORTLINE  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLSPINNINGTOP
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLSPINNINGTOP()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLSPINNINGTOP(startIdx, endIdx, &cdl_inOpen[0],
                                &cdl_inHigh[0], &cdl_inLow[0], &cdl_inClose[0],
                                &outBeg, &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLSPINNINGTOP  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLSTALLEDPATTERN
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLSTALLEDPATTERN()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLSTALLEDPATTERN(
        startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0], &cdl_inLow[0],
        &cdl_inClose[0], &outBeg, &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLSTALLEDPATTERN ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLSTICKSANDWICH
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLSTICKSANDWICH()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLSTICKSANDWICH(
        startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0], &cdl_inLow[0],
        &cdl_inClose[0], &outBeg, &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLSTICKSANDWICH  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLTAKURI
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLTAKURI()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLTAKURI(startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0],
                           &cdl_inLow[0], &cdl_inClose[0], &outBeg,
                           &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLTAKURI  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLTASUKIGAP
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLTASUKIGAP()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLTASUKIGAP(startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0],
                              &cdl_inLow[0], &cdl_inClose[0], &outBeg,
                              &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLTASUKIGAP  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLTHRUSTING
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLTHRUSTING()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLTHRUSTING(startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0],
                              &cdl_inLow[0], &cdl_inClose[0], &outBeg,
                              &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLTHRUSTING  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLTRISTAR
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLTRISTAR()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLTRISTAR(startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0],
                            &cdl_inLow[0], &cdl_inClose[0], &outBeg,
                            &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLTRISTAR  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLUNIQUE3RIVER
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLUNIQUE3RIVER()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLUNIQUE3RIVER(startIdx, endIdx, &cdl_inOpen[0],
                                 &cdl_inHigh[0], &cdl_inLow[0], &cdl_inClose[0],
                                 &outBeg, &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLUNIQUE3RIVER  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLUPSIDEGAP2CROWS
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLUPSIDEGAP2CROWS()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLUPSIDEGAP2CROWS(
        startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0], &cdl_inLow[0],
        &cdl_inClose[0], &outBeg, &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLUPSIDEGAP2CROWS  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  CDLXSIDEGAP3METHODS
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e CDLXSIDEGAP3METHODS()
{
    int startIdx = 0;
    int endIdx = cdl_rows - 1;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    TA_RetCode retCode;
    int outInteger[cdl_rows] = {0};
    e2::Int_e b = 0;

    retCode = TA_CDLXSIDEGAP3METHODS(
        startIdx, endIdx, &cdl_inOpen[0], &cdl_inHigh[0], &cdl_inLow[0],
        &cdl_inClose[0], &outBeg, &outNbElement, &outInteger[0]);

    if (retCode == TA_SUCCESS && outInteger[0] != 0) {
        b = outInteger[0];
    }
    return b;
} /* -----  end of function CDLXSIDEGAP3METHODS  ----- */

// #endif

}  // namespace ETALIB

namespace e2q {
using namespace ETALIB;
/*
 * ===  FUNCTION  =============================
 *
 *         Name:  Calletalib
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
void Calletalib(std::vector<e2q::E2lFun_t>& funs)
{
    AddFunExt(funs, e2q, BBANDS, 3, "TA_BBANDS", E2L_RETURN,
              "(array_id, timeperiod,ret_id );");
    AddFunExt(funs, e2q, MA, 3, "TA_MA", E2L_RETURN,
              "(array_id, timeperiod,ret_id );");
    AddFunExt(funs, e2q, EMA, 3, "TA_EMA", E2L_RETURN,
              "(array_id, timeperiod,ret_id );");

    AddFunExt(funs, e2q, MACD, 5, "TA_MACD", E2L_RETURN,
              "(array_id, fast,slow,signal,ret_id );");

    AddFunExt(funs, e2q, CDLInit, 3, "TA_CDLInit", E2L_RETURN,
              "(cficode,timeframe, array_id);");
    AddFunExt(funs, e2q, CDL2CROWS, 0, "TA_CDL2CROWS", E2L_RETURN, "();");
    AddFunExt(funs, e2q, CDL3BLACKCROWS, 0, "TA_CDL3BLACKCROWS", E2L_RETURN,
              "();");
    AddFunExt(funs, e2q, CDL3INSIDE, 0, "TA_CDL3INSIDE", E2L_RETURN, "();");
    AddFunExt(funs, e2q, CDL3LINESTRIKE, 0, "TA_CDL3LINESTRIKE", E2L_RETURN,
              "();");
    AddFunExt(funs, e2q, CDL3OUTSIDE, 0, "TA_CDL3OUTSIDE", E2L_RETURN, "();");
    AddFunExt(funs, e2q, CDL3STARSINSOUTH, 0, "TA_CDL3STARSINSOUTH", E2L_RETURN,
              "();");
    AddFunExt(funs, e2q, CDL3WHITESOLDIERS, 0, "TA_CDL3WHITESOLDIERS",
              E2L_RETURN, "();");
    AddFunExt(funs, e2q, CDLABANDONEDBABY, 0, "TA_CDLABANDONEDBABY", E2L_RETURN,
              "();");
    AddFunExt(funs, e2q, CDLADVANCEBLOCK, 0, "TA_CDLADVANCEBLOCK", E2L_RETURN,
              "();");
    AddFunExt(funs, e2q, CDLBELTHOLD, 0, "TA_CDLBELTHOLD", E2L_RETURN, "();");
    AddFunExt(funs, e2q, CDLBREAKAWAY, 0, "TA_CDLBREAKAWAY", E2L_RETURN, "();");
    AddFunExt(funs, e2q, CDLCLOSINGMARUBOZU, 0, "TA_CDLCLOSINGMARUBOZU",
              E2L_RETURN, "();");
    AddFunExt(funs, e2q, CDLCONCEALBABYSWALL, 0, "TA_CDLCONCEALBABYSWALL",
              E2L_RETURN, "();");
    AddFunExt(funs, e2q, CDLCOUNTERATTACK, 0, "TA_CDLCOUNTERATTACK", E2L_RETURN,
              "();");
    AddFunExt(funs, e2q, CDLDARKCLOUDCOVER, 0, "TA_CDLDARKCLOUDCOVER",
              E2L_RETURN, "();");
    AddFunExt(funs, e2q, CDLDOJI, 0, "TA_CDLDOJI", E2L_RETURN, "();");
    AddFunExt(funs, e2q, CDLDOJISTAR, 0, "TA_CDLDOJISTAR", E2L_RETURN, "();");
    AddFunExt(funs, e2q, CDLDRAGONFLYDOJI, 0, "TA_CDLDRAGONFLYDOJI", E2L_RETURN,
              "();");
    AddFunExt(funs, e2q, CDLENGULFING, 0, "TA_CDLENGULFING", E2L_RETURN, "();");
    AddFunExt(funs, e2q, CDLEVENINGDOJISTAR, 0, "TA_CDLEVENINGDOJISTAR",
              E2L_RETURN, "();");
    AddFunExt(funs, e2q, CDLEVENINGSTAR, 0, "TA_CDLEVENINGSTAR", E2L_RETURN,
              "();");
    AddFunExt(funs, e2q, CDLGAPSIDESIDEWHITE, 0, "TA_CDLGAPSIDESIDEWHITE",
              E2L_RETURN, "();");
    AddFunExt(funs, e2q, CDLGRAVESTONEDOJI, 0, "TA_CDLGRAVESTONEDOJI",
              E2L_RETURN, "();");
    AddFunExt(funs, e2q, CDLHAMMER, 0, "TA_CDLHAMMER", E2L_RETURN, "();");
    AddFunExt(funs, e2q, CDLHANGINGMAN, 0, "TA_CDLHANGINGMAN", E2L_RETURN,
              "();");
    AddFunExt(funs, e2q, CDLHARAMI, 0, "TA_CDLHARAMI", E2L_RETURN, "();");

    AddFunExt(funs, e2q, CDLHARAMICROSS, 0, "TA_CDLHARAMICROSS", E2L_RETURN,
              "();");
    AddFunExt(funs, e2q, CDLHIGHWAVE, 0, "TA_CDLHIGHWAVE", E2L_RETURN, "();");
    AddFunExt(funs, e2q, CDLHIKKAKE, 0, "TA_CDLHIKKAKE", E2L_RETURN, "();");
    AddFunExt(funs, e2q, CDLHIKKAKEMOD, 0, "TA_CDLHIKKAKEMOD", E2L_RETURN,
              "();");
    AddFunExt(funs, e2q, CDLHOMINGPIGEON, 0, "TA_CDLHOMINGPIGEON", E2L_RETURN,
              "();");
    AddFunExt(funs, e2q, CDLIDENTICAL3CROWS, 0, "TA_CDLIDENTICAL3CROWS",
              E2L_RETURN, "();");
    AddFunExt(funs, e2q, CDLINNECK, 0, "TA_CDLINNECK", E2L_RETURN, "();");
    AddFunExt(funs, e2q, CDLINVERTEDHAMMER, 0, "TA_CDLINVERTEDHAMMER",
              E2L_RETURN, "();");
    AddFunExt(funs, e2q, CDLKICKING, 0, "TA_CDLKICKING", E2L_RETURN, "();");
    AddFunExt(funs, e2q, CDLKICKINGBYLENGTH, 0, "TA_CDLKICKINGBYLENGTH",
              E2L_RETURN, "();");
    AddFunExt(funs, e2q, CDLLADDERBOTTOM, 0, "TA_CDLLADDERBOTTOM", E2L_RETURN,
              "();");
    AddFunExt(funs, e2q, CDLLONGLEGGEDDOJI, 0, "TA_CDLLONGLEGGEDDOJI",
              E2L_RETURN, "();");
    AddFunExt(funs, e2q, CDLLONGLINE, 0, "TA_CDLLONGLINE", E2L_RETURN, "();");
    AddFunExt(funs, e2q, CDLMARUBOZU, 0, "TA_CDLMARUBOZU", E2L_RETURN, "();");
    AddFunExt(funs, e2q, CDLMATCHINGLOW, 0, "TA_CDLMATCHINGLOW", E2L_RETURN,
              "();");
    AddFunExt(funs, e2q, CDLMATHOLD, 0, "TA_CDLMATHOLD", E2L_RETURN, "();");
    AddFunExt(funs, e2q, CDLMORNINGDOJISTAR, 0, "TA_CDLMORNINGDOJISTAR",
              E2L_RETURN, "();");
    AddFunExt(funs, e2q, CDLMORNINGSTAR, 0, "TA_CDLMORNINGSTAR", E2L_RETURN,
              "();");
    AddFunExt(funs, e2q, CDLONNECK, 0, "TA_CDLONNECK", E2L_RETURN, "();");
    AddFunExt(funs, e2q, CDLPIERCING, 0, "TA_CDLPIERCING", E2L_RETURN, "();");
    AddFunExt(funs, e2q, CDLRICKSHAWMAN, 0, "TA_CDLRICKSHAWMAN", E2L_RETURN,
              "();");
    AddFunExt(funs, e2q, CDLRISEFALL3METHODS, 0, "TA_CDLRISEFALL3METHODS",
              E2L_RETURN, "();");
    AddFunExt(funs, e2q, CDLSEPARATINGLINES, 0, "TA_CDLSEPARATINGLINES",
              E2L_RETURN, "();");
    AddFunExt(funs, e2q, CDLSHOOTINGSTAR, 0, "TA_CDLSHOOTINGSTAR", E2L_RETURN,
              "();");
    AddFunExt(funs, e2q, CDLSHORTLINE, 0, "TA_CDLSHORTLINE", E2L_RETURN, "();");
    AddFunExt(funs, e2q, CDLSPINNINGTOP, 0, "TA_CDLSPINNINGTOP", E2L_RETURN,
              "();");
    AddFunExt(funs, e2q, CDLSTALLEDPATTERN, 0, "TA_CDLSTALLEDPATTERN",
              E2L_RETURN, "();");
    AddFunExt(funs, e2q, CDLSTICKSANDWICH, 0, "TA_CDLSTICKSANDWICH", E2L_RETURN,
              "();");
    AddFunExt(funs, e2q, CDLTAKURI, 0, "TA_CDLTAKURI", E2L_RETURN, "();");
    AddFunExt(funs, e2q, CDLTASUKIGAP, 0, "TA_CDLTASUKIGAP", E2L_RETURN, "();");
    AddFunExt(funs, e2q, CDLTHRUSTING, 0, "TA_CDLTHRUSTING", E2L_RETURN, "();");
    AddFunExt(funs, e2q, CDLTRISTAR, 0, "TA_CDLTRISTAR", E2L_RETURN, "();");
    AddFunExt(funs, e2q, CDLUNIQUE3RIVER, 0, "TA_CDLUNIQUE3RIVER", E2L_RETURN,
              "();");
    AddFunExt(funs, e2q, CDLUPSIDEGAP2CROWS, 0, "TA_CDLUPSIDEGAP2CROWS",
              E2L_RETURN, "();");
    AddFunExt(funs, e2q, CDLXSIDEGAP3METHODS, 0, "TA_CDLXSIDEGAP3METHODS",
              E2L_RETURN, "();");

} /* -----  end of function Calletalib  ----- */

}  // namespace e2q
