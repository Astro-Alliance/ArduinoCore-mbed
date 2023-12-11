#include "Ethernet.h"

void arduino::EthernetClass::begin(uint8_t *mac) {
    if (eth_if == nullptr) return;
    eth_if->set_blocking(false);
    eth_if->connect();
}

void arduino::EthernetClass::begin(uint8_t *mac, IPAddress ip) {
    IPAddress dns = ip;
    dns[3] = 1;
    begin(mac, ip, dns);
}

void arduino::EthernetClass::begin(uint8_t *mac, IPAddress ip, IPAddress dns) {
    IPAddress gateway = ip;
    gateway[3] = 1;
    begin(mac, ip, dns, gateway);
}

void arduino::EthernetClass::begin(uint8_t *mac, IPAddress ip, IPAddress dns, IPAddress gateway) {
    begin(mac, ip, dns, gateway, IPAddress(255, 255, 255, 0));
}

void arduino::EthernetClass::begin(uint8_t *mac, IPAddress ip, IPAddress dns, IPAddress gateway, IPAddress subnet) {
    config(ip, dns, gateway, subnet);
    eth_if->set_dhcp(false);
    eth_if->set_network(_ip, _netmask, _gateway);
    eth_if->add_dns_server(_dnsServer1, nullptr);
    begin(mac);
}

void arduino::EthernetClass::end() { disconnect(); }

EthernetLinkStatus arduino::EthernetClass::linkStatus() {
    switch (eth_if->get_connection_status()) {
        case NSAPI_STATUS_GLOBAL_UP:
            return LinkON;
        case NSAPI_STATUS_DISCONNECTED:
            return LinkOFF;
        case NSAPI_STATUS_CONNECTING:
            return LinkConnecting;
        case NSAPI_STATUS_ERROR_UNSUPPORTED:
            return LinkError;
        default:
            return Unknown;
    }
}

EthernetHardwareStatus arduino::EthernetClass::hardwareStatus() { return EthernetMbed; }

int arduino::EthernetClass::disconnect() {
    eth_if->disconnect();
    return 1;
}

uint8_t arduino::EthernetClass::status() { return _currentNetworkStatus; }

NetworkInterface *arduino::EthernetClass::getNetwork() { return eth_if; }

unsigned long arduino::EthernetClass::getTime() { return 0; }

void arduino::EthernetClass::MACAddress(uint8_t *mac_address) { macAddress(mac_address); }

arduino::EthernetClass Ethernet;
