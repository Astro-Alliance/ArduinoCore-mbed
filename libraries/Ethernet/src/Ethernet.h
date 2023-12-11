/* Copyright 2018 Paul Stoffregen
 * Copyright 2020-2021 Arduino SA
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef ethernet_h_
#define ethernet_h_

#include "Arduino.h"
#include "EthernetInterface.h"
#include "SocketHelpers.h"
#include "api/IPAddress.h"
#include "netsocket/NetworkInterface.h"

enum EthernetLinkStatus { Unknown, LinkON, LinkOFF, LinkConnecting, LinkError };

enum EthernetHardwareStatus { EthernetNoHardware, EthernetMbed = 6 };

namespace arduino {

enum {  // compatibility with Arduino ::maintain()
    DHCP_CHECK_NONE = 0,
    DHCP_CHECK_RENEW_FAIL = 1,
    DHCP_CHECK_RENEW_OK = 2,
    DHCP_CHECK_REBIND_FAIL = 3,
    DHCP_CHECK_REBIND_OK = 4
};

class EthernetClass : public MbedSocketClass {
   public:
    // Initialise the Ethernet shield to use the provided MAC address and
    // gain the rest of the configuration through DHCP.
    // Returns 0 if the DHCP configuration failed, and 1 if it succeeded
    EthernetClass(EthernetInterface *_if) : eth_if(_if){};
    EthernetClass(){};

    void begin(uint8_t *mac = nullptr);
    EthernetLinkStatus linkStatus();
    EthernetHardwareStatus hardwareStatus();

    // Manual configuration
    void begin(uint8_t *mac, IPAddress ip);
    void begin(uint8_t *mac, IPAddress ip, IPAddress dns);
    void begin(uint8_t *mac, IPAddress ip, IPAddress dns, IPAddress gateway);
    void begin(uint8_t *mac, IPAddress ip, IPAddress dns, IPAddress gateway, IPAddress subnet);

    void begin(IPAddress ip) { begin(nullptr, ip); }
    void begin(IPAddress ip, IPAddress dns) { begin(nullptr, ip, dns); }
    void begin(IPAddress ip, IPAddress dns, IPAddress gateway) { begin(nullptr, ip, dns, gateway); }
    void begin(IPAddress ip, IPAddress dns, IPAddress gateway, IPAddress subnet) { begin(nullptr, ip, dns, gateway, subnet); }

    void MACAddress(uint8_t *mac_address);

    int disconnect(void);
    void end(void);

    uint8_t status();
    unsigned long getTime();

    void setDnsServerIP(const IPAddress dns_server) { _dnsServer1 = socketAddressFromIpAddress(dns_server, 0); }

    friend class EthernetClient;
    friend class EthernetServer;
    friend class EthernetUDP;

    NetworkInterface *getNetwork();

    constexpr static int maintain() { return DHCP_CHECK_NONE; }

   private:
    volatile EthernetLinkStatus _currentNetworkStatus = Unknown;
    EthernetInterface net;
    EthernetInterface *eth_if = &net;
};

}  // namespace arduino

extern arduino::EthernetClass Ethernet;

#include "EthernetClient.h"
#include "EthernetServer.h"
#include "EthernetUdp.h"

#endif
