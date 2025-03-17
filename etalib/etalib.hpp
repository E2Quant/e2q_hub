/*
 * =====================================================================================
 *
 *       Filename:  etalib.hpp
 *
 *    Description:  etalib
 *
 *        Version:  1.0
 *        Created:  2024年07月30日 11时33分55秒
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
 *
 * =====================================================================================
 */

#ifndef ETALIB_INC
#define ETALIB_INC
#include <ta-lib/ta_abstract.h>
#include <ta-lib/ta_common.h>
#include <ta-lib/ta_defs.h>
#include <ta-lib/ta_func.h>
#include <ta-lib/ta_libc.h>

#include <cstddef>
#include <vector>

#include "Toolkit/pack.hpp"
#include "assembler/BaseType.hpp"

namespace ETALIB {

// #ifdef E2_TALIB

#define cdl_rows 8
inline std::size_t cdl_time = 0;

inline double cdl_inOpen[cdl_rows] = {0};
inline double cdl_inHigh[cdl_rows] = {0};
inline double cdl_inLow[cdl_rows] = {0};
inline double cdl_inClose[cdl_rows] = {0};

// Math Operators
e2::Bool ADD();
e2::Bool DIV();
e2::Bool MAX();
e2::Bool MAXINDEX();
e2::Bool MIN();
e2::Bool MININDEX();
e2::Bool MINMAX();
e2::Bool MINMAXINDEX();
e2::Bool MULT();
e2::Bool SUB();
e2::Bool SUM();
// Math Transform
e2::Bool ACOS();
e2::Bool ASIN();
e2::Bool ATAN();
e2::Bool CEIL();
e2::Bool COS();
e2::Bool COSH();
e2::Bool EXP();
e2::Bool FLOOR();
e2::Bool LN();
e2::Bool LOG10();
e2::Bool SIN();
e2::Bool SINH();
e2::Bool SQRT();
e2::Bool TAN();
e2::Bool TANH();
// Overlap Studies
e2::Bool BBANDS(e2::Int_e array_id, e2::Int_e timeperiod, e2::Int_e);
e2::Bool HT_TRENDLINE();
e2::Bool MAVP();
e2::Bool MIDPOINT();
e2::Bool MIDPRICE();
e2::Bool SAR();
e2::Bool SAREXT();
/**
 * MATYPE
 */
e2::Bool BaseMA(e2::Int_e array_id, e2::Int_e timeperiod, e2::Int_e, TA_MAType);
e2::Bool MA(e2::Int_e array_id, e2::Int_e timeperiod, e2::Int_e);
e2::Bool SMA(e2::Int_e array_id, e2::Int_e timeperiod, e2::Int_e);    // 0
e2::Bool EMA(e2::Int_e array_id, e2::Int_e timeperiod, e2::Int_e);    // 1
e2::Bool WMA(e2::Int_e array_id, e2::Int_e timeperiod, e2::Int_e);    // 2
e2::Bool DEMA(e2::Int_e array_id, e2::Int_e timeperiod, e2::Int_e);   // 3
e2::Bool TEMA(e2::Int_e array_id, e2::Int_e timeperiod, e2::Int_e);   // 4
e2::Bool TRIMA(e2::Int_e array_id, e2::Int_e timeperiod, e2::Int_e);  // 5
e2::Bool KAMA(e2::Int_e array_id, e2::Int_e timeperiod, e2::Int_e);   // 6
e2::Bool MAMA(e2::Int_e array_id, e2::Int_e timeperiod, e2::Int_e);   // 7
e2::Bool T3(e2::Int_e array_id, e2::Int_e timeperiod, e2::Int_e);     // 8

// Volatility Indicators
e2::Bool ATR();
e2::Bool NATR();
e2::Bool TRANGE();
// Momentum Indicators
e2::Bool ADX();
e2::Bool ADXR();
e2::Bool APO();
e2::Bool AROON();
e2::Bool AROONOSC();
e2::Bool BOP();
e2::Bool CCI();
e2::Bool CMO();
e2::Bool DX();
e2::Bool MACD(e2::Int_e array_id, e2::Int_e, e2::Int_e, e2::Int_e, e2::Int_e);
e2::Bool MACDEXT();
e2::Bool MACDFIX();
e2::Bool MFI();
e2::Bool MINUS_DI();
e2::Bool MINUS_DM();
e2::Bool MOM();
e2::Bool PLUS_DI();
e2::Bool PLUS_DM();
e2::Bool PPO();
e2::Bool ROC();
e2::Bool ROCP();
e2::Bool ROCR();
e2::Bool ROCR100();
e2::Bool RSI();
e2::Bool STOCH();
e2::Bool STOCHF();
e2::Bool STOCHRSI();
e2::Bool TRIX();
e2::Bool ULTOSC();
e2::Bool WILLR();
// Cycle Indicators
e2::Bool HT_DCPERIOD();
e2::Bool HT_DCPHASE();
e2::Bool HT_PHASOR();
e2::Bool HT_SINE();
e2::Bool HT_TRENDMODE();
// Volume Indicators
e2::Bool AD();
e2::Bool ADOSC();
e2::Bool OBV();

// Pattern Recognition
// bar array id, if id == 0 default bar
e2::Bool CDLInit(e2::Int_e, e2::TimeFrames, e2::Int_e);

// outInteger is positive (1 to 100) when white (bullish),
// negative (-1 to -100) when black (bearish)
//
// BULLISH
e2::Int_e CDLHAMMER();
e2::Int_e CDLINVERTEDHAMMER();
e2::Int_e CDLBELTHOLD();
e2::Int_e CDLENGULFING();
e2::Int_e CDLHARAMI();
e2::Int_e CDLHARAMICROSS();
e2::Int_e CDLPIERCING();
e2::Int_e CDLDOJI();
e2::Int_e CDLDOJISTAR();
e2::Int_e CDLDRAGONFLYDOJI();
e2::Int_e CDLEVENINGDOJISTAR();
e2::Int_e CDLGRAVESTONEDOJI();
e2::Int_e CDLLONGLEGGEDDOJI();
e2::Int_e CDLMORNINGDOJISTAR();
// -- BULLISH MEETING LINE: --
e2::Int_e CDLHOMINGPIGEON();
e2::Int_e CDLMATCHINGLOW();
e2::Int_e CDLKICKING();
e2::Int_e CDLKICKINGBYLENGTH();
e2::Int_e CDL3WHITESOLDIERS();
e2::Int_e CDLMORNINGSTAR();
e2::Int_e CDLABANDONEDBABY();
e2::Int_e CDLTRISTAR();
//-- BULLISH DOWNSIDE GAP TWO RABBITS --
e2::Int_e CDLUNIQUE3RIVER();
e2::Int_e CDLADVANCEBLOCK();
// -- 	BULLISH TWO RABBITS --
e2::Int_e CDL3INSIDE();
e2::Int_e CDL3OUTSIDE();
e2::Int_e CDL3STARSINSOUTH();
e2::Int_e CDLSTICKSANDWICH();
e2::Int_e CDLBREAKAWAY();
e2::Int_e CDLLADDERBOTTOM();

// BEARISH
e2::Int_e CDLHANGINGMAN();
e2::Int_e CDL2CROWS();
e2::Int_e CDL3BLACKCROWS();
e2::Int_e CDL3LINESTRIKE();
e2::Int_e CDLCLOSINGMARUBOZU();
e2::Int_e CDLCONCEALBABYSWALL();
e2::Int_e CDLCOUNTERATTACK();
e2::Int_e CDLDARKCLOUDCOVER();
e2::Int_e CDLEVENINGSTAR();
e2::Int_e CDLGAPSIDESIDEWHITE();
e2::Int_e CDLHIGHWAVE();
e2::Int_e CDLHIKKAKE();
e2::Int_e CDLHIKKAKEMOD();
e2::Int_e CDLIDENTICAL3CROWS();
e2::Int_e CDLINNECK();
e2::Int_e CDLLONGLINE();
e2::Int_e CDLMARUBOZU();
e2::Int_e CDLMATHOLD();
e2::Int_e CDLONNECK();
e2::Int_e CDLRICKSHAWMAN();
e2::Int_e CDLRISEFALL3METHODS();
e2::Int_e CDLSEPARATINGLINES();
e2::Int_e CDLSHOOTINGSTAR();
e2::Int_e CDLSHORTLINE();
e2::Int_e CDLSPINNINGTOP();
e2::Int_e CDLSTALLEDPATTERN();
e2::Int_e CDLTAKURI();
e2::Int_e CDLTASUKIGAP();
e2::Int_e CDLTHRUSTING();
e2::Int_e CDLUPSIDEGAP2CROWS();
e2::Int_e CDLXSIDEGAP3METHODS();
// end

// Statistic Functions
e2::Bool BETA();
e2::Bool CORREL();
e2::Bool LINEARREG();
e2::Bool LINEARREG_ANGLE();
e2::Bool LINEARREG_INTERCEPT();
e2::Bool LINEARREG_SLOPE();
e2::Bool STDDEV();
e2::Bool TSF();
e2::Bool VAR();
// Price Transform
e2::Bool AVGPRICE();
e2::Bool MEDPRICE();
e2::Bool TYPPRICE();
e2::Bool WCLPRICE();

// #endif

}  // namespace ETALIB
   //

namespace e2q {
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
void Calletalib(std::vector<e2q::E2lFun_t>
                    &); /* -----  end of function Calletalib  ----- */
}  // namespace e2q
#endif /* ----- #ifndef ETALIB_INC  ----- */
