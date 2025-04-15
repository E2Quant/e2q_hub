/*
 * =====================================================================================
 *
 *       Filename:  TraderEvent.hpp
 *
 *    Description:  TraderEvent
 *
 *        Version:  1.0
 *        Created:  2025/02/07 11时22分54秒
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

#ifndef TRADEREVENT_INC
#define TRADEREVENT_INC
#include <cstddef>
#include <cstdint>
#include <queue>
#include <utility>

#include "FileUtils.h"
#include "OMSPack/Matcher/BaseMatcher.hpp"
#include "Toolkit/pack.hpp"
#include "api/xoms_api_struct.h"
#include "api/xtp_api_data_type.h"
#include "api/xtp_trader_api.h"
#include "zts_unit.hpp"
namespace E2ZTS {
using namespace e2q;

/*
 * ================================
 *        Class:  TraderEvent
 *  Description:
 * ================================
 */
class TraderEvent {
public:
    /* =============  LIFECYCLE     =================== */
    TraderEvent(FileUtils* futils)
    {
        _futils = std::move(futils);
        init();
    }; /* constructor */

    ~TraderEvent()
    {
        // if (orderList) {
        //     delete[] orderList;
        //     orderList = NULL;
        // }

        // if (session_arrary) {
        //     delete[] session_arrary;
        //     session_arrary = NULL;
        // }
    }
    /* =============  ACCESSORS     =================== */

    /* =============  MUTATORS      =================== */
    void run();

    void login(const FIX::SessionID&, double);

    int property(const FIX::SessionID&, ZTSTraderInfo&);

    uint64_t OrderXtpId(const FIX::SessionID&);

    // void freeMargin(const FIX::SessionID& sid, std::size_t ticket,
    //                 double margin);

    bool match(std::vector<e2q::OrderLots>& lots);

    uint64_t InsertOrderExtra(const FIX::SessionID&, XTPOrderInsertInfo* order);

    void callOrderEvent(XTPOrderInfo* order_info, uint64_t);

    void exist()
    {
        uint64_t session_id = 0;
        for (auto user : _traders) {
            session_id = user.second.session_id;
            _pUserApi->Logout(session_id);
        }
    }
    void ErrorShow();
    /* =============  OPERATORS     =================== */

protected:
    /* =============  METHODS       =================== */

    /* =============  DATA MEMBERS  =================== */

private:
    /* =============  METHODS       =================== */
    void init();

    /* =============  DATA MEMBERS  =================== */

    FileUtils* _futils{nullptr};
    XTP::API::TraderApi* _pUserApi{nullptr};
    std ::size_t account_count = 0;
    int client_id = 0;
    bool auto_save = false;
    int resume_type = 0;
    std::string filepath = "";
    std::string _account_key = "";
    bool ping_pong_test = false;
    int32_t heat_beat_interval = 0;

    std::string _trade_server_ip;
    int _trade_server_port;
    std::queue<std::pair<uint64_t, XTPOrderInfo>> _OrderQueue;

    XTP_PROTOCOL_TYPE quote_protocol = XTP_PROTOCOL_UDP;

    std::map<FIX::SessionID, ZTSTraderInfo> _traders;
}; /* -----  end of class TraderEvent  ----- */

}  // namespace E2ZTS
#endif /* ----- #ifndef TRADEREVENT_INC  ----- */
