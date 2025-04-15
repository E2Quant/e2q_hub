/*
 * =====================================================================================
 *
 *       Filename:  TraderSpi.cpp
 *
 *    Description:  TraderSpi
 *
 *        Version:  1.0
 *        Created:  2025/02/06 17时25分50秒
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
#include "TraderSpi.hpp"

#include <string>

#include "Toolkit/Norm.hpp"
#include "libs/bprinter/table_printer.h"
#include "utility/Log.hpp"

namespace E2ZTS {
// #ifdef ZTS_TRADER

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ETraderSpi::OnDisconnected
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
void ETraderSpi::OnDisconnected(uint64_t session_id, int reason)
{
    if (reason == 10200006) {
        log::info(" reason:", reason);
        return;
    }

    if (_fun_login) {
        _fun_login(session_id);
    }
} /* -----  end of function ETraderSpi::OnDisconnected  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ETraderSpi::OnOrderEvent
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
void ETraderSpi::OnOrderEvent(XTPOrderInfo *order_info, XTPRI *error_info,
                              uint64_t session_id)
{
    bprinter::TablePrinter tp(&std::cout);

    tp.AddColumn("OnOrderEvent name", 20);
    tp.AddColumn("value", 50);
    tp.PrintHeader();

#ifdef _WIN32
    SYSTEMTIME wtm;
    GetLocalTime(&wtm);
    // fout_order << "[" << wtm.wHour << ":" << wtm.wMinute << ":" <<
    // wtm.wSecond
    //            << "." << wtm.wMilliseconds << "],";
#else
    struct timeval start;
    gettimeofday(&start, 0);
    struct tm *ptr;
    time_t lt;
    lt = time(NULL);
    ptr = localtime(&lt);
    std::string cont = "[" + std::to_string(ptr->tm_hour) + ":" +
                       std::to_string(ptr->tm_min) + ":" +
                       std::to_string(ptr->tm_sec) + "." +
                       std::to_string(start.tv_usec) + "],";
    tp << "time:" << cont;

#endif  // _WIN32

    tp << "xtp_id:" << order_info->order_xtp_id;
    tp << "client_id:" << order_info->order_client_id;
    tp << "status:" << order_info->order_status;
    tp << "cancel_xtp_id:" << order_info->order_cancel_xtp_id;
    tp << "cancel_client_id:" << order_info->order_cancel_client_id;
    tp << "order_submit_status:" << order_info->order_submit_status;
    tp << "ticker:" << order_info->ticker;
    tp << "market:" << order_info->market;
    tp << "price:" << order_info->price;
    tp << "quantity:" << order_info->quantity;
    tp << "price_type:" << order_info->price_type;
    tp << "side:" << (int)(order_info->side);
    tp << "qty_traded:" << order_info->qty_traded;
    tp << "qty_left:" << order_info->qty_left;
    tp << "insert_time:" << order_info->insert_time;
    tp << "update_time:" << order_info->update_time;
    tp << "cancel_time:" << order_info->cancel_time;
    tp << "trade_amount:" << order_info->trade_amount;
    tp << "position_effect:" << (int)(order_info->position_effect);
    tp << "business_type:" << order_info->business_type;
    tp << "order_local_id:" << order_info->order_local_id;
    tp << "order_type:" << order_info->order_type;
    tp << "error_id:" << error_info->error_id;
    tp << "error_msg:" << error_info->error_msg;
    tp.PrintFooter();

    switch (order_info->order_status) {
        case XTP_ORDER_STATUS_NOTRADEQUEUEING: {
            break;
        }
        case XTP_ORDER_STATUS_ALLTRADED:
        case XTP_ORDER_STATUS_PARTTRADEDQUEUEING:
        case XTP_ORDER_STATUS_PARTTRADEDNOTQUEUEING:
        case XTP_ORDER_STATUS_CANCELED:
        case XTP_ORDER_STATUS_REJECTED: {
            if (_fun_oe) {
                _fun_oe(order_info, session_id);
            }
            break;
        }

        default:
            break;
    }

    // 检测demo是否开启乒乓报单测试，如果开启则继续后续逻辑，否则将只报一单
    if (!ping_pang_flag_) {
        return;
    }

} /* -----  end of function ETraderSpi::OnOrderEvent  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ETraderSpi::OnTradeEvent
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
void ETraderSpi::OnTradeEvent(XTPTradeReport *trade_info, uint64_t session_id)
{
    bprinter::TablePrinter tp(&std::cout);

    tp.AddColumn("OnTradeEvent name", 20);
    tp.AddColumn("value", 50);
    tp.PrintHeader();

#ifdef _WIN32
    SYSTEMTIME wtm;
    GetLocalTime(&wtm);
    // fout_trade << "[" << wtm.wHour << ":" << wtm.wMinute << ":" <<
    // wtm.wSecond
    //            << "." << wtm.wMilliseconds << "],";
#else
    struct timeval start;
    gettimeofday(&start, 0);
    struct tm *ptr;
    time_t lt;
    lt = time(NULL);
    ptr = localtime(&lt);
    std::string cont = "[" + std::to_string(ptr->tm_hour) + ":" +
                       std::to_string(ptr->tm_min) + ":" +
                       std::to_string(ptr->tm_sec) + "." +
                       std::to_string(start.tv_usec) + "],";
    tp << "time:" << cont;
#endif  // _WIN32
    tp << "xtp_id:" << trade_info->order_xtp_id;
    tp << ",client_id:" << trade_info->order_client_id;
    tp << ",ticker:" << trade_info->ticker;
    tp << ",market:" << trade_info->market;
    tp << ",price:" << trade_info->price;
    tp << ",quantity:" << trade_info->quantity;
    tp << ",side:" << (int)(trade_info->side);
    tp << ",trade_time:" << trade_info->trade_time;
    tp << ",trade_amount:" << trade_info->trade_amount;
    tp << ",position_effect:" << (int)(trade_info->position_effect);
    tp << ",business_type:" << trade_info->business_type;
    tp << ",exec_id:" << trade_info->exec_id;
    tp << ",report_index:" << trade_info->report_index;
    tp << ",order_exch_id:" << trade_info->order_exch_id;
    tp << ",trade_type:" << trade_info->trade_type;
    tp << ",branch_pbu:" << trade_info->branch_pbu;
    tp.PrintFooter();
} /* -----  end of function ETraderSpi::OnTradeEvent  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ETraderSpi::OnCancelOrderError
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
void ETraderSpi::OnCancelOrderError(XTPOrderCancelInfo *cancel_info,
                                    XTPRI *error_info, uint64_t session_id)
{
    if (save_to_file_) {
        fout_cancel_order << "cancel_order_xtp_id:"
                          << cancel_info->order_cancel_xtp_id
                          << ",order_xtp_id:" << cancel_info->order_xtp_id
                          << ",error_id:" << error_info->error_id
                          << ",msg:" << error_info->error_msg << endl;
    }

} /* -----  end of function ETraderSpi::OnCancelOrderError  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ETraderSpi::OnQueryOrderEx
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
void ETraderSpi::OnQueryOrderEx(XTPOrderInfoEx *order_info, XTPRI *error_info,
                                int request_id, bool is_last,
                                uint64_t session_id)
{
    if (error_info && error_info->error_id != 0) {
        // 查询出错
        if (error_info->error_id == 11000350) {
            // 账户没有下过订单
            cout << "------------------- No Order.-----------" << endl;
        }
        else {
            // 真正的出错
        }
        return;
    }
    bprinter::TablePrinter tp(&std::cout);

    tp.AddColumn("OnQueryOrderEx name", 20);
    tp.AddColumn("value", 50);
    tp.PrintHeader();

    tp << "xtp_id:" << order_info->order_xtp_id;
    tp << "client_id:" << order_info->order_client_id;
    tp << "status:" << order_info->order_status;
    tp << "cancel_xtp_id:" << order_info->order_cancel_xtp_id;
    tp << "cancel_client_id:" << order_info->order_cancel_client_id;
    tp << "order_submit_status:" << order_info->order_submit_status;
    tp << "ticker:" << order_info->ticker;
    tp << "market:" << order_info->market;
    tp << "price:" << order_info->price;
    tp << "quantity:" << order_info->quantity;
    tp << "price_type:" << order_info->price_type;
    tp << "side:" << (int)(order_info->side);
    tp << "qty_traded:" << order_info->qty_traded;
    tp << "qty_left:" << order_info->qty_left;
    tp << "insert_time:" << order_info->insert_time;
    tp << "update_time:" << order_info->update_time;
    tp << "cancel_time:" << order_info->cancel_time;
    tp << "trade_amount:" << order_info->trade_amount;
    tp << "position_effect:" << (int)(order_info->position_effect);
    tp << "business_type:" << order_info->business_type;
    tp << "order_local_id:" << order_info->order_local_id;
    tp << "order_type:" << order_info->order_type;
    tp << "error_id:" << order_info->order_err_t.error_id;
    tp << "error_msg:" << order_info->order_err_t.error_msg;

    tp.PrintFooter();
} /* -----  end of function ETraderSpi::OnQueryOrderEx  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ETraderSpi::OnQueryTrade
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
void ETraderSpi::OnQueryTrade(XTPQueryTradeRsp *trade_info, XTPRI *error_info,
                              int request_id, bool is_last,
                              uint64_t session_idt)
{
    if (error_info && error_info->error_id != 0) {
        // 查询出错
        if (error_info->error_id == 11000350) {
            // 账户没有成交
            cout << "------------------- No Transaction Record.-----------"
                 << endl;
        }
        else {
            // 真正的出错
        }
        return;
    }
    bprinter::TablePrinter tp(&std::cout);

    tp.AddColumn("OnQueryTrade name", 20);
    tp.AddColumn("value", 50);
    tp.PrintHeader();
    tp << "xtp_id:" << trade_info->order_xtp_id;
    tp << ",client_id:" << trade_info->order_client_id;
    tp << ",ticker:" << trade_info->ticker;
    tp << ",market:" << trade_info->market;
    tp << ",price:" << trade_info->price;
    tp << ",quantity:" << trade_info->quantity;
    tp << ",side:" << trade_info->side;
    tp << ",trade_time:" << trade_info->trade_time;
    tp << ",trade_amount:" << trade_info->trade_amount;
    tp << ",exec_id:" << trade_info->exec_id;
    tp << ",report_index:" << trade_info->report_index;
    tp << ",order_exch_id:" << trade_info->order_exch_id;
    tp << ",trade_type:" << trade_info->trade_type;
    tp << ",branch_pbu:" << trade_info->branch_pbu;
    tp.PrintFooter();

} /* -----  end of function ETraderSpi::OnQueryTrade  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ETraderSpi::OnQueryPosition
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
void ETraderSpi::OnQueryPosition(XTPQueryStkPositionRsp *investor_position,
                                 XTPRI *error_info, int request_id,
                                 bool is_last, uint64_t session_id)
{
    if (error_info && error_info->error_id != 0) {
        // 查询出错
        if (error_info->error_id == 11000350) {
            // 账户里没有持仓，不算真正的错误
            cout << "------------------- Position is empty.-----------" << endl;
        }
        else {
            // 真正的出错
        }
        return;
    }
    bprinter::TablePrinter tp(&std::cout);

    tp.AddColumn("name", 20);
    tp.AddColumn("value", 50);
    tp.PrintHeader();

    // if (save_to_file_) {
    tp << "request_id:" << request_id;
    tp << "is_last:" << is_last;
    tp << "ticker:" << investor_position->ticker;
    tp << "ticker_name:" << investor_position->ticker_name;
    tp << "total_qty:" << investor_position->total_qty;
    tp << "sellable_qty:" << investor_position->sellable_qty;
    tp << "avg_price:" << investor_position->avg_price;
    tp << "unrealized_pnl:" << investor_position->unrealized_pnl;
    tp << "position_security_type:"
       << (int)investor_position->position_security_type;
    tp << "yesterday_position:" << investor_position->yesterday_position;
    tp.PrintFooter();
    // }

} /* -----  end of function ETraderSpi::OnQueryPosition  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ETraderSpi::OnQueryAsset
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 * 请求查询资产
 * ============================================
 */
void ETraderSpi::OnQueryAsset(XTPQueryAssetRsp *trading_account,
                              XTPRI *error_info, int request_id, bool is_last,
                              uint64_t session_id)
{
    log::info("query asset ");

    std::string str = log::format(
        "request_id: %d,total_asset: %.2f,security_asset:%.2f ,buying_power: "
        "%.2f,fund_buy_amount: "
        "%.2f,fund_buy_fee:%.2f,fund_sell_fee:%.2f,fund_sell_amount:%.2f",
        request_id, trading_account->total_asset,
        trading_account->security_asset, trading_account->buying_power,
        trading_account->fund_buy_amount, trading_account->fund_buy_fee,
        trading_account->fund_sell_fee, trading_account->fund_sell_amount);

    log::info(str);

} /* -----  end of function ETraderSpi::OnQueryAsset  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ETraderSpi::OnQueryStructuredFund
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
void ETraderSpi::OnQueryStructuredFund(XTPStructuredFundInfo *fund_info,
                                       XTPRI *error_info, int request_id,
                                       bool is_last, uint64_t session_id)
{
} /* -----  end of function ETraderSpi::OnQueryStructuredFund  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ETraderSpi::OnQueryFundTransfer
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
void ETraderSpi::OnQueryFundTransfer(XTPFundTransferNotice *fund_transfer_info,
                                     XTPRI *error_info, int request_id,
                                     bool is_last, uint64_t session_id)
{
} /* -----  end of function ETraderSpi::OnQueryFundTransfer  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ETraderSpi::OnFundTransfer
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
void ETraderSpi::OnFundTransfer(XTPFundTransferNotice *fund_transfer_info,
                                XTPRI *error_info, uint64_t session_id)
{
} /* -----  end of function ETraderSpi::OnFundTransfer  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ETraderSpi::OnQueryETF
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
void ETraderSpi::OnQueryETF(XTPQueryETFBaseRsp *etf_info, XTPRI *error_info,
                            int request_id, bool is_last, uint64_t session_id)
{
    if (etf_info == nullptr) {
        return;
    }
    std::string str = log::format("%s", etf_info->etf);
    log::info("etf:", str);
} /* -----  end of function ETraderSpi::OnQueryETF  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ETraderSpi::OnQueryETFBasket
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
void ETraderSpi::OnQueryETFBasket(XTPQueryETFComponentRsp *etf_component_info,
                                  XTPRI *error_info, int request_id,
                                  bool is_last, uint64_t session_id)
{
} /* -----  end of function ETraderSpi::OnQueryETFBasket  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ETraderSpi::OnQueryIPOInfoList
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
void ETraderSpi::OnQueryIPOInfoList(XTPQueryIPOTickerRsp *ipo_info,
                                    XTPRI *error_info, int request_id,
                                    bool is_last, uint64_t session_id)
{
} /* -----  end of function ETraderSpi::OnQueryIPOInfoList  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ETraderSpi::OnQueryIPOQuotaInfo
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
void ETraderSpi::OnQueryIPOQuotaInfo(XTPQueryIPOQuotaRsp *quota_info,
                                     XTPRI *error_info, int request_id,
                                     bool is_last, uint64_t session_id)
{
} /* -----  end of function ETraderSpi::OnQueryIPOQuotaInfo  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ETraderSpi::OnQueryOptionAuctionInfo
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
void ETraderSpi::OnQueryOptionAuctionInfo(
    XTPQueryOptionAuctionInfoRsp *option_info, XTPRI *error_info,
    int request_id, bool is_last, uint64_t session_id)
{
} /* -----  end of function ETraderSpi::OnQueryOptionAuctionInfo  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ETraderSpi::OnCreditCashRepay
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
void ETraderSpi::OnCreditCashRepay(XTPCrdCashRepayRsp *cash_repay_info,
                                   XTPRI *error_info, uint64_t session_id)
{
} /* -----  end of function ETraderSpi::OnCreditCashRepay  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ETraderSpi::OnQueryCreditCashRepayInfo
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
void ETraderSpi::OnQueryCreditCashRepayInfo(
    XTPCrdCashRepayInfo *cash_repay_info, XTPRI *error_info, int request_id,
    bool is_last, uint64_t session_id)
{
} /* -----  end of function ETraderSpi::OnQueryCreditCashRepayInfo  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ETraderSpi::OnQueryCreditFundInfo
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
void ETraderSpi::OnQueryCreditFundInfo(XTPCrdFundInfo *fund_info,
                                       XTPRI *error_info, int request_id,
                                       uint64_t session_id)
{
} /* -----  end of function ETraderSpi::OnQueryCreditFundInfo  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ETraderSpi::OnQueryCreditDebtInfo
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
void ETraderSpi::OnQueryCreditDebtInfo(XTPCrdDebtInfo *debt_info,
                                       XTPRI *error_info, int request_id,
                                       bool is_last, uint64_t session_id)
{
} /* -----  end of function ETraderSpi::OnQueryCreditDebtInfo  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ETraderSpi::OnQueryCreditTickerDebtInfo
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
void ETraderSpi::OnQueryCreditTickerDebtInfo(XTPCrdDebtStockInfo *debt_info,
                                             XTPRI *error_info, int request_id,
                                             bool is_last, uint64_t session_id)
{
} /* -----  end of function ETraderSpi::OnQueryCreditTickerDebtInfo  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ETraderSpi::OnQueryCreditAssetDebtInfo
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
void ETraderSpi::OnQueryCreditAssetDebtInfo(double remain_amount,
                                            XTPRI *error_info, int request_id,
                                            uint64_t session_id)
{
} /* -----  end of function ETraderSpi::OnQueryCreditAssetDebtInfo  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ETraderSpi::OnQueryCreditTickerAssignInfo
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
void ETraderSpi::OnQueryCreditTickerAssignInfo(
    XTPClientQueryCrdPositionStkInfo *assign_info, XTPRI *error_info,
    int request_id, bool is_last, uint64_t session_id)
{
} /* -----  end of function ETraderSpi::OnQueryCreditTickerAssignInfo  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ETraderSpi::OnQueryCreditExcessStock
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
void ETraderSpi::OnQueryCreditExcessStock(
    XTPClientQueryCrdSurplusStkRspInfo *stock_info, XTPRI *error_info,
    int request_id, uint64_t session_id)
{
} /* -----  end of function ETraderSpi::OnQueryCreditExcessStock  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ETraderSpi::IsErrorRspInfo
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
bool ETraderSpi::IsErrorRspInfo(XTPRI *pRspInfo)
{
    // 如果ErrorID != 0, 说明收到了错误的响应
    bool bResult = ((pRspInfo) && (pRspInfo->error_id != 0));
    if (bResult)
        log::info("--->>> ErrorID=", pRspInfo->error_id,
                  ", ErrorMsg=", pRspInfo->error_msg);
    return bResult;

} /* -----  end of function ETraderSpi::IsErrorRspInfo  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ETraderSpi::InitOutputFile
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
void ETraderSpi::InitOutputFile()
{
    std::string orderName = "./log/";
    std::string tradeName = "./log/";
    std::string fundName = "./log/";
    std::string positonName = "./log/";
    std::string qryOrderName = "./log/";
    std::string qryTradeName = "./log/";
    std::string cancelOrderName = "./log/";
    std::string positionCountName = "./log/";

    std::string pre;
    stringstream ss;
#ifndef _WIN32
    struct timeval start;
    gettimeofday(&start, 0);
    ss << start.tv_sec;
#else
    SYSTEMTIME wtm;
    GetLocalTime(&wtm);
    ss << wtm.wDay << wtm.wHour << wtm.wMinute << wtm.wSecond;
#endif
    ss >> pre;

    orderName.append(pre);
    tradeName.append(pre);
    fundName.append(pre);
    positonName.append(pre);
    qryOrderName.append(pre);
    qryTradeName.append(pre);
    cancelOrderName.append(pre);
    positionCountName.append(pre);

    orderName.append("order.txt");
    tradeName.append("tradeReport.txt");
    fundName.append("fund.txt");
    positonName.append("stock_position.txt");
    qryOrderName.append("qry_order.txt");
    qryTradeName.append("qry_tradeReport.txt");
    cancelOrderName.append("cancel_order.txt");
    positionCountName.append("qry_positionCount.txt");

    fout_order.open(orderName.c_str());
    fout_trade.open(tradeName.c_str());
    fout_fund.open(fundName.c_str());
    fout_position.open(positonName.c_str());
    fout_qry_order.open(qryOrderName.c_str());
    fout_qry_trade.open(qryTradeName.c_str());
    fout_cancel_order.open(cancelOrderName.c_str());
    fout_position_count.open(positionCountName.c_str());

} /* -----  end of function ETraderSpi::InitOutputFile  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ETraderSpi::CloseOutputFile
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
void ETraderSpi::CloseOutputFile()
{
    fout_order.close();
    fout_trade.close();
    fout_fund.close();
    fout_position.close();
    fout_qry_order.close();
    fout_qry_trade.close();
    fout_cancel_order.close();
    fout_position_count.close();

} /* -----  end of function ETraderSpi::CloseOutputFile  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ETraderSpi::callLogin
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
void ETraderSpi::callLogin(e2q::func_type<uint64_t> fun)
{
    _fun_login = std::move(fun);
} /* -----  end of function ETraderSpi::callBack  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ETraderSpi::callOrderEvent
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
void ETraderSpi::callOrderEvent(e2q::func_type<XTPOrderInfo *, uint64_t> fun)
{
    _fun_oe = std::move(fun);
} /* -----  end of function ETraderSpi::callOrderEvent  ----- */

// #endif
}  // namespace E2ZTS

