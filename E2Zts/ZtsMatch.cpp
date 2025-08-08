/*
 * =====================================================================================
 *
 *       Filename:  ZtsMatch.cpp
 *
 *    Description:  ZtsMatch
 *
 *        Version:  1.0
 *        Created:  2025/02/08 16时52分14秒
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
#include "ZtsMatch.hpp"

#include <cstddef>
#include <cstdint>
#include <string>

#include "E2L/E2LType.hpp"
#include "FileUtils.h"
#include "OMSPack/OrderBook/Order.hpp"
#include "Toolkit/GlobalConfig.hpp"
#include "Toolkit/Norm.hpp"
#include "TraderEvent.hpp"
#include "api/xtp_api_data_type.h"
#include "assembler/BaseType.hpp"
#include "utility/Log.hpp"
namespace E2ZTS {

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ZTSMatch::InitConfig
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
void ZTSMatch::InitConfig(const char *config)
{
    _futils = MALLOC(FileUtils);
    if (!_futils->init(config)) {
        log::bug("The config.json file parse error.");
        RELEASE(_futils);
    }
} /* -----  end of function ZTSMatch::InitConfig  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ZTSMatch::InitSequence
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
void ZTSMatch::InitSequence() {

} /* -----  end of function ZTSMatch::InitSequence  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ZTSMatch::sequence
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
SeqType ZTSMatch::sequence(const FIX::SessionID &sid, e2::Side side)
{
    return _TraderEve->OrderXtpId(sid);

} /* -----  end of function ZTSMatch::sequence  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ZTSMatch::matcher
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
std::vector<OrderLots> ZTSMatch::matcher(std::string symbol, e2::Int_e now,
                                         e2::Int_e price, e2::Int_e adjprice)
{
    std::vector<OrderLots> orders;

    SeqType ticket = 0;

    bool r = _TraderEve->match(orders);
    if (r == false) {
        return orders;
    }

    std::size_t idx = GlobalDBPtr->getId();

    Pgsql *gsql = GlobalDBPtr->ptr(idx);

    for (std::size_t m = 0; m < orders.size(); m++) {
        ticket = orders[m].ticket;
        if (_MapOrder.count(ticket) == 0) {
            log::bug("not exits ticket:", std::to_string(ticket));
            continue;
        }
        OrderLots one = _MapOrder.at(ticket)->Lots();
        one.isFilled = orders[m].isFilled;
        one.executedQuantity = orders[m].quantity;
        one.avgExecutedPrice = orders[m].price;

        one.lastExecutedQuantity = orders[m].quantity;
        one.lastExecutedPrice = orders[m].price;
        one.leavesQty = orders[m].leavesQty;
        one.trade_amount = orders[m].trade_amount;

        if (orders[m].isCancel) {
            if (gsql != nullptr) {
                gsql->update_table("trades");
                gsql->update_field(
                    "stat",
                    (int)e2::OrdStatus::ost_Canceled);  // OrdStatus: Canceled
                gsql->update_condition("ticket", ticket);
                gsql->update_commit();
            }
            _broker.freeMargin(one.owner, ticket, 0);
        }
        else if (one.quantId > 0) {
            _broker.SettlInst(one);
        }

        orders[m] = one;
    }
    GlobalDBPtr->release(idx);

    return orders;
} /* -----  end of function ZTSMatch::matcher  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ZTSMatch::insert
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
bool ZTSMatch::insert(OrderItem *order)
{
    XTPOrderInsertInfo xorder;
    memset(&xorder, 0, sizeof(XTPOrderInsertInfo));

    xorder.order_xtp_id = order->Pending()->getTicket();

    // XTP_MKT_SH_A
    xorder.market = XTP_MKT_SZ_A;
    std::string stdstr_ticker = order->getSymbol();

    strncpy(xorder.ticker, stdstr_ticker.c_str(), XTP_TICKER_LEN);
    xorder.business_type = XTP_BUSINESS_TYPE_CASH;

    xorder.side = XTP_SIDE_BUY;
    if (order->getSide() == e2::Side::os_Sell) {
        xorder.side = XTP_SIDE_SELL;
    }

    xorder.position_effect = XTP_POSITION_EFFECT_INIT;
    xorder.price_type = XTP_PRICE_LIMIT;
    xorder.price = NUMBERVAL(order->Pending()->getPrice());
    xorder.quantity = order->Pending()->getQuantity();

    uint64_t order_xtp_id =
        _TraderEve->InsertOrderExtra(order->getOwner(), &xorder);

    bool isOk = false;
    if (order_xtp_id == order->Pending()->getTicket() ||
        order_xtp_id == (uint64_t)order->getCloseTicket()) {
        _MapOrder.insert({order->Pending()->getTicket(), order});
        isOk = true;
    }
    else {
        _TraderEve->ErrorShow();
    }

    return isOk;
} /* -----  end of function ZTSMatch::insert  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ZTSMatch::display
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
void ZTSMatch::display() {
} /* -----  end of function ZTSMatch::display  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ZTSMatch::CheckClose
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
e2::Int_e ZTSMatch::CheckClose(SeqType ticket, const std::string &symbol,
                               e2::Int_e lots)
{
    e2::Int_e _lots = 0;
    OrderItem *order;
    auto item = _MapOrder.find(ticket);
    if (item == _MapOrder.end()) {
        log::bug("not found ticket:", ticket);
        return _lots;
    }

    order = item->second;

    if (order->Pending()->isClosed()) {
        log::bug("ticket:", ticket, " symbol:", symbol,
                 " oi ticket:", order->Pending()->getTicket(),
                 " oi sym:", order->getSymbol(),
                 " open:", order->Pending()->getOpenQuantity(),
                 "leaveqty:", order->Pending()->getLeavesQty());
        return 0;
    }

    std::size_t ctime = order->getTime();
    std::size_t now = FinFabr->_stock.at(0)[Trading::t_time];
    std::size_t startTime = now - (now % 86400);

    switch (FinFabr->_settlement) {
        case 0:
            // T+0
            break;
        case 1: {
            // T+1

            if (ctime > startTime) {
                return 0;
            }

            break;
        }
        default:
            break;
    }

    _lots = order->Pending()->CloseOrder(lots);
    return _lots;
} /* -----  end of function ZTSMatch::CheckClose  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ZTSMatch::AddBotTicket
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
int ZTSMatch::AddBotTicket(SeqType ticket, e2::OrdType ordType, e2::Side side,
                           double bot_qty, e2::Int_e symbol)
{
    // log::echo("add bot");

    return 0;
} /* -----  end of function ZTSMatch::AddBotTicket  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ZTSMatch::OrdTypePending
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
bool ZTSMatch::OrdTypePending()
{
    return false;
} /* -----  end of function ZTSMatch::OrdTypePending  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ZTSMatch::TopLevelPrice
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
void ZTSMatch::TopLevelPrice(const std::string &symbol, SeqType) {

} /* -----  end of function ZTSMatch::TopLevelPrice  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ZTSMatch::Equity
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
double ZTSMatch::Equity(const FIX::SessionID &sid, std::size_t ticket,
                        const char status)
{
    double ret = _broker.Equity(sid, ticket, status);
    return ret;
} /* -----  end of function ZTSMatch::Equity  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ZTSMatch::SettlInst
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
// void ZTSMatch::SettlInst(OrderLots &ol)
// {
//     _broker.SettlInst(ol);
// } /* -----  end of function ZTSMatch::SettlInst  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ZTSMatch::freeMargin
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
void ZTSMatch::freeMargin(const FIX::SessionID &sid, std::size_t ticket,
                          double margin)
{
    _broker.freeMargin(sid, ticket, margin);

} /* -----  end of function ZTSMatch::freeMargin  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ZTSMatch::Margin
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
bool ZTSMatch::Margin(const FIX::SessionID &sid, std::size_t ticket,
                      double margin, long qty)
{
    return _broker.Margin(sid, ticket, margin, qty);
} /* -----  end of function ZTSMatch::Margin  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ZTSMatch::CheckMargin
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
double ZTSMatch::CheckMargin(const FIX::SessionID &sid, double price, long qty)
{
    return _broker.CheckMargin(sid, price, qty);
} /* -----  end of function ZTSMatch::CheckMargin  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  ZTSMatch::traders
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *  初始化当前是哪一个 fix 账号登录的，并且初始化资金
 * ============================================
 */
double ZTSMatch::traders(const FIX::SessionID &sid, double cash)
{
    if (_futils == nullptr) {
        return 0;
    }

    if (_TraderEve == nullptr) {
        _TraderEve = MALLOC(TraderEvent, _futils);
        _TraderEve->run();
    }

    _TraderEve->login(sid, cash);

    _broker.traders(sid, cash);
    std::string cash_str = log::format("%.2f", cash);
    // log::info("sid:", sid.getTargetCompID(), " cash:", cash_str);

    return cash;
} /* -----  end of function ZTSMatch::traders  ----- */

}  // namespace E2ZTS
