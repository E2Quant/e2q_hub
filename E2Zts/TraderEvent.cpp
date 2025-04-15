/*
 * =====================================================================================
 *
 *       Filename:  TraderEvent.cpp
 *
 *    Description:  TraderEvent
 *
 *        Version:  1.0
 *        Created:  2025/02/07 14时56分57秒
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
#include "TraderEvent.hpp"

#include <unistd.h>

#include <cstddef>
#include <cstdint>

#include "OMSPack/OrderBook/Order.hpp"
#include "TraderSpi.hpp"
#include "api/xoms_api_struct.h"
#include "api/xtp_api_data_type.h"
#include "utility/Log.hpp"
#include "zts_unit.hpp"
namespace E2ZTS {

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  TraderEvent::init
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
void TraderEvent::init()
{
    if (_futils == nullptr) {
        return;
    }
    // 读取交易配置
    _trade_server_ip = _futils->stdStringForKey("trade_ip");
    _trade_server_port = _futils->intForKey("trade_port");
    auto_save = _futils->boolForKey(
        "auto_save");  // 是否将回报数据落盘，此参数仅在此demo中使用，仅涉及demo回调函数中逻辑，不是必需的
    client_id = _futils->intForKey("client_id");
    account_count = (std::size_t)_futils->countForKey("account");
    resume_type = _futils->intForKey("resume_type");
    _account_key = _futils->stdStringForKey("account_key");
#ifdef _WIN32
    filepath = _futils->stdStringForKey("path");
#else
    filepath = _futils->stdStringForKey("path_linux");
#endif  // _WIN32
    ping_pong_test = _futils->boolForKey(
        "ping_pong_test");  // demo是否开启乒乓测试，默认是不开启的，如果需要开启，请在配置文件中修改，此参数仅在此demo中使用，仅涉及demo回调函数中逻辑，不是必需的

    // 读取心跳超时配置
    heat_beat_interval = _futils->intForKey("hb_interval");

} /* -----  end of function TraderEvent::init  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  TraderEvent::ErrorShow
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
void TraderEvent::ErrorShow()
{
    XTPRI* error_info = _pUserApi->GetApiLastError();
    log::bug(" login to server error, ", error_info->error_id, " : ",
             error_info->error_msg);
} /* -----  end of function TraderEvent::ErrorShow  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  TraderEvent::run
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
void TraderEvent::run()
{
    // 初始化交易类Api
    _pUserApi = XTP::API::TraderApi::CreateTraderApi(
        client_id, filepath.c_str(),
        XTP_LOG_LEVEL_DEBUG);  // 创建UserApi，log日志级别可以调整
    if (!_pUserApi) {
        std::cout
            << "Failed to create trader api, please check the input parameters."
            << std::endl;
        return;
    }
    _pUserApi->SubscribePublicTopic((XTP_TE_RESUME_TYPE)resume_type);
    _pUserApi->SetSoftwareVersion(
        "1.1.0");  // 设定此软件的开发版本号，用户自定义
    _pUserApi->SetSoftwareKey(
        _account_key
            .c_str());  // 设定用户的开发代码，在XTP申请开户时，由xtp人员提供
    _pUserApi->SetHeartBeatInterval(
        heat_beat_interval);  // 设定交易服务器超时时间，单位为秒，此为1.1.16新增接口
    ETraderSpi* pUserSpi = new ETraderSpi();
    if (!pUserSpi) {
        std::cout
            << "Failed to create trader spi, please check the input parameters."
            << std::endl;
        return;
    }

    auto fun = [this](uint64_t session_id) {
        FIX::SessionID _sid;
        bool is_connected = false;
        for (auto sid : this->_traders) {
            if (sid.second.session_id == session_id) {
                _sid = sid.first;
                is_connected = true;
                break;
            }
        }

        if (is_connected == false) {
            log::bug(" session id not found:", session_id);
            return;
        }
        // 断线后，等待3s，再重新连接

        uint64_t temp_session_id = 0;
        do {
#ifdef _WIN32
            Sleep(3000);
#else
            sleep(3);
#endif  // WIN32

            temp_session_id = this->_pUserApi->Login(
                this->_trade_server_ip.c_str(), this->_trade_server_port,
                this->_traders[_sid].user.c_str(),
                this->_traders[_sid].password.c_str(), XTP_PROTOCOL_TCP);

        } while (temp_session_id == 0);

        // 重新登录成功后更新映射关系
        this->_traders[_sid].session_id = temp_session_id;
    };  // -----  end lambda  -----

    pUserSpi->callLogin(fun);

    auto orderEvent = [this](XTPOrderInfo* order_info, uint64_t session_id) {
        this->callOrderEvent(order_info, session_id);
    };  // -----  end lambda  -----

    pUserSpi->callOrderEvent(orderEvent);

    _pUserApi->RegisterSpi(pUserSpi);  // 注册事件类
    pUserSpi->setUserAPI(_pUserApi);
    pUserSpi->set_save_to_file(auto_save);
    pUserSpi->set_ping_pong_test_flag(
        ping_pong_test);  // 设置demo是否开启乒乓报单测试

} /* -----  end of function TraderEvent::run  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  TraderEvent::login
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
void TraderEvent::login(const FIX::SessionID& sid, double cash)
{
    std::size_t idx = _traders.size();
    if (idx >= account_count) {
        log::bug(" account is empty");
        return;
    }
    if (_pUserApi == nullptr) {
        log::bug("user api is nullptr");
        return;
    }

    ZTSTraderInfo ztf;
    ztf.user = _futils->stdStringForKey("account[%d].user", idx);
    ztf.password = _futils->stdStringForKey("account[%d].password", idx);

    log::info(ztf.user, " login begin.", " idx:", idx, " count:", account_count,
              " ip:", _trade_server_ip, " port:", _trade_server_port);

    ztf.session_id =
        _pUserApi->Login(_trade_server_ip.c_str(), _trade_server_port,
                         ztf.user.c_str(), ztf.password.c_str(),
                         XTP_PROTOCOL_TCP);  // 第i个用户登录交易服务器

    if (ztf.session_id > 0) {
        // 登录成功后，建立session_id与用户之间的映射关系
        // ztf.total_cash = cash;
        _traders.insert({sid, ztf});

        _pUserApi->QueryAsset(ztf.session_id, 0);

        // int request_id = 2;
        // _pUserApi->QueryPosition(NULL, ztf.session_id, request_id);

        const char* TradDay = _pUserApi->GetTradingDay();
        std::string day = log::format(" Trader Day: %s", TradDay);
        log::info("login ok:", ztf.session_id, day);
    }
    else {
        // 登录失败，获取登录失败原因
        XTPRI* error_info = _pUserApi->GetApiLastError();
        log::bug(ztf.user, " login to server error, ", error_info->error_id,
                 " : ", error_info->error_msg);
    }

} /* -----  end of function TraderEvent::login  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  TraderEvent::property
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
int TraderEvent::property(const FIX::SessionID& sid, ZTSTraderInfo& zts)
{
    if (_traders.count(sid) == 0) {
        log::bug("property:", sid.getTargetCompID());

        return -1;
    }
    zts = _traders.at(sid);

    return 0;
} /* -----  end of function TraderEvent::property  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  TraderEvent::OrderXtpId
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
uint64_t TraderEvent::OrderXtpId(const FIX::SessionID& sid)
{
    if (_traders.count(sid) == 0) {
        log::bug("property:", sid.getTargetCompID());

        return -1;
    }
    uint64_t session_id = _traders.at(sid).session_id;

    return _pUserApi->GetANewOrderXTPID(session_id);
} /* -----  end of function TraderEvent::OrderXtpId  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  TraderEvent::match
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
bool TraderEvent::match(std::vector<e2q::OrderLots>& lots)
{
    e2q::OrderLots lot;
    while (_OrderQueue.size() > 0) {
        XTPOrderInfo info = _OrderQueue.front().second;
        lot.isFilled = true;
        lot.isCancel = false;
        lot.ticket = info.order_xtp_id;
        lot.quantity = info.quantity;
        lot.price = info.price;

        // 今成交数量，为此订单累计成交数量
        lot.avgExecutedPrice = info.qty_traded;
        lot.leavesQty = info.qty_left;
        /// 成交金额，为此订单的成交总金额
        /// lot.trade_amount = info.trade_amount;

        if (info.order_status == XTP_ORDER_STATUS_CANCELED ||
            info.order_status == XTP_ORDER_STATUS_REJECTED) {
            lot.isCancel = true;
        }

        lots.push_back(lot);
        _OrderQueue.pop();
    }
    return lots.size() > 0;
} /* -----  end of function TraderEvent::match  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  TraderEvent::InsertOrderExtra
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
uint64_t TraderEvent::InsertOrderExtra(const FIX::SessionID& sid,
                                       XTPOrderInsertInfo* order)
{
    uint64_t session_id = 0;
    uint64_t id = 0;
    if (_traders.count(sid) == 0) {
        log::bug("property:", sid.getTargetCompID());

        return -1;
    }
    session_id = _traders.at(sid).session_id;
    id = _pUserApi->InsertOrderExtra(order, session_id);
    return id;
} /* -----  end of function TraderEvent::InsertOrderExtra  ----- */

/*
 * ===  FUNCTION  =============================
 *
 *         Name:  TraderEvent::callOrderEvent
 *  ->  void *
 *  Parameters:
 *  - size_t  arg
 *  Description:
 *
 * ============================================
 */
void TraderEvent::callOrderEvent(XTPOrderInfo* order_info, uint64_t session_id)
{
    XTPOrderInfo xinfo = *order_info;

    // log::echo("status:", order_info->order_status);
    _OrderQueue.push({session_id, xinfo});

} /* -----  end of function TraderEvent::callOrderEvent  ----- */

}  // namespace E2ZTS
