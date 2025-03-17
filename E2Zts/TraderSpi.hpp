/*
 * =====================================================================================
 *
 *       Filename:  TraderSpi.hpp
 *
 *    Description:  TraderSpi
 *
 *        Version:  1.0
 *        Created:  2025/02/06 17时23分39秒
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

#ifndef TRADERSPI_INC
#define TRADERSPI_INC

// #ifdef ZTS_TRADER

#include <cstdint>

#include "Toolkit/Norm.hpp"
#include "Toolkit/pack.hpp"
#include "api/xoms_api_struct.h"
#include "api/xtp_trader_api.h"

namespace E2ZTS {
using namespace XTP::API;
/*
 * ================================
 *        Class:  ETraderSpi
 *  Description:
 * ================================
 */
class ETraderSpi : public TraderSpi {
public:
    /* =============  LIFECYCLE     =================== */
    ETraderSpi()
    {
        InitOutputFile();

        save_to_file_ = false;
        ping_pang_flag_ = false;

    }; /* constructor */
    ~ETraderSpi() { CloseOutputFile(); }
    /* =============  ACCESSORS     =================== */

    /* =============  MUTATORS      =================== */
    virtual void OnDisconnected(uint64_t session_id, int reason);

    /// 报单通知
    virtual void OnOrderEvent(XTPOrderInfo *order_info, XTPRI *error_info,
                              uint64_t session_id);

    /// 成交通知
    virtual void OnTradeEvent(XTPTradeReport *trade_info, uint64_t session_id);

    virtual void OnCancelOrderError(XTPOrderCancelInfo *cancel_info,
                                    XTPRI *error_info, uint64_t session_id);

    virtual void OnQueryOrderEx(XTPOrderInfoEx *order_info, XTPRI *error_info,
                                int request_id, bool is_last,
                                uint64_t session_id);

    virtual void OnQueryTrade(XTPQueryTradeRsp *trade_info, XTPRI *error_info,
                              int request_id, bool is_last,
                              uint64_t session_idt);

    virtual void OnQueryPosition(XTPQueryStkPositionRsp *investor_position,
                                 XTPRI *error_info, int request_id,
                                 bool is_last, uint64_t session_id);

    virtual void OnQueryAsset(XTPQueryAssetRsp *trading_account,
                              XTPRI *error_info, int request_id, bool is_last,
                              uint64_t session_id);

    virtual void OnQueryStructuredFund(XTPStructuredFundInfo *fund_info,
                                       XTPRI *error_info, int request_id,
                                       bool is_last, uint64_t session_id);

    virtual void OnQueryFundTransfer(XTPFundTransferNotice *fund_transfer_info,
                                     XTPRI *error_info, int request_id,
                                     bool is_last, uint64_t session_id);

    virtual void OnFundTransfer(XTPFundTransferNotice *fund_transfer_info,
                                XTPRI *error_info, uint64_t session_id);

    virtual void OnQueryETF(XTPQueryETFBaseRsp *etf_info, XTPRI *error_info,
                            int request_id, bool is_last, uint64_t session_id);

    virtual void OnQueryETFBasket(XTPQueryETFComponentRsp *etf_component_info,
                                  XTPRI *error_info, int request_id,
                                  bool is_last, uint64_t session_id);

    virtual void OnQueryIPOInfoList(XTPQueryIPOTickerRsp *ipo_info,
                                    XTPRI *error_info, int request_id,
                                    bool is_last, uint64_t session_id);

    virtual void OnQueryIPOQuotaInfo(XTPQueryIPOQuotaRsp *quota_info,
                                     XTPRI *error_info, int request_id,
                                     bool is_last, uint64_t session_id);

    virtual void OnQueryOptionAuctionInfo(
        XTPQueryOptionAuctionInfoRsp *option_info, XTPRI *error_info,
        int request_id, bool is_last, uint64_t session_id);

    virtual void OnCreditCashRepay(XTPCrdCashRepayRsp *cash_repay_info,
                                   XTPRI *error_info, uint64_t session_id);

    virtual void OnQueryCreditCashRepayInfo(
        XTPCrdCashRepayInfo *cash_repay_info, XTPRI *error_info, int request_id,
        bool is_last, uint64_t session_id);

    virtual void OnQueryCreditFundInfo(XTPCrdFundInfo *fund_info,
                                       XTPRI *error_info, int request_id,
                                       uint64_t session_id);

    virtual void OnQueryCreditDebtInfo(XTPCrdDebtInfo *debt_info,
                                       XTPRI *error_info, int request_id,
                                       bool is_last, uint64_t session_id);

    virtual void OnQueryCreditTickerDebtInfo(XTPCrdDebtStockInfo *debt_info,
                                             XTPRI *error_info, int request_id,
                                             bool is_last, uint64_t session_id);

    virtual void OnQueryCreditAssetDebtInfo(double remain_amount,
                                            XTPRI *error_info, int request_id,
                                            uint64_t session_id);

    virtual void OnQueryCreditTickerAssignInfo(
        XTPClientQueryCrdPositionStkInfo *assign_info, XTPRI *error_info,
        int request_id, bool is_last, uint64_t session_id);

    virtual void OnQueryCreditExcessStock(
        XTPClientQueryCrdSurplusStkRspInfo *stock_info, XTPRI *error_info,
        int request_id, uint64_t session_id);

    void setUserAPI(XTP::API::TraderApi *pApi) { pUserApi = pApi; }

    inline void set_save_to_file(bool save) { save_to_file_ = save; }
    inline void set_ping_pong_test_flag(bool flag) { ping_pang_flag_ = flag; }

    void callLogin(e2q::func_type<uint64_t>);
    void callOrderEvent(e2q::func_type<XTPOrderInfo *, uint64_t>);
    /* =============  OPERATORS     =================== */

protected:
    /* =============  METHODS       =================== */

    /* =============  DATA MEMBERS  =================== */

private:
    /* =============  METHODS       =================== */
    bool IsErrorRspInfo(XTPRI *pRspInfo);

    // 初始化落盘文件
    void InitOutputFile();
    // 关闭落盘文件
    void CloseOutputFile();

    /* =============  DATA MEMBERS  =================== */

    std::ofstream fout_order;
    std::ofstream fout_trade;
    std::ofstream fout_fund;
    std::ofstream fout_position;
    std::ofstream fout_qry_order;
    std::ofstream fout_qry_trade;
    std::ofstream fout_cancel_order;
    std::ofstream fout_position_count;

    bool save_to_file_;    // 是否将回报数据落盘文件的标志
    bool ping_pang_flag_;  // 是否进行乒乓测试的标志

    XTP::API::TraderApi *pUserApi;

    XTPOrderInsertInfo *orderList{nullptr};

    e2q::func_type<uint64_t> _fun_login{nullptr};
    e2q::func_type<XTPOrderInfo *, uint64_t> _fun_oe{nullptr};
}; /* -----  end of class ETraderSpi  ----- */

}  // namespace E2ZTS
// #endif
#endif /* ----- #ifndef TRADERSPI_INC  ----- */
