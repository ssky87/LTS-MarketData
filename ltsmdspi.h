#pragma once
#include <string>
#include <mutex>

#include "ThirdParty/inih/INIReader.h"

#include "LTS_API/SecurityFtdcL2MDUserApi.h"
#include "ringbuffer.h"

using namespace _LTS_;

class LtsMdSpi : public CSecurityFtdcL2MDUserSpi {
public:
  explicit LtsMdSpi(int mode);
  ~LtsMdSpi();

  static int whichMode(const char *s);

  void start_serve();

protected:
  // 登录
  void OnFrontConnected() override;
  void OnRspUserLogin(CSecurityFtdcUserLoginField *pUserLogin,
                      CSecurityFtdcRspInfoField *pRspInfo, int nRequestID,
                      bool bIsLast) override;
  // 订阅
  void OnRspSubL2MarketData(
      CSecurityFtdcSpecificInstrumentField *pSpecificInstrument,
      CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
  void
  OnRspSubL2Index(CSecurityFtdcSpecificInstrumentField *pSpecificInstrument,
                  CSecurityFtdcRspInfoField *pRspInfo, int nRequestID,
                  bool bIsLast) override;
  void OnRspSubL2OrderAndTrade(CSecurityFtdcRspInfoField *pRspInfo,
                               int nRequestID, bool bIsLast) override;
  // 接收
  void OnRtnL2MarketData(CSecurityFtdcL2MarketDataField *pL2MarketData) override;
  void OnRtnL2Index(CSecurityFtdcL2IndexField *pL2Index) override;
  void OnRtnL2Order(CSecurityFtdcL2OrderField *pL2Order) override;
  void OnRtnL2Trade(CSecurityFtdcL2TradeField *pL2Trade) override;

  // 掉线处理
  void OnFrontDisconnected(int reason) override;

private:
  CSecurityFtdcL2MDUserApi *api_;
  INIReader config_;
  // std::set<std::string> subscriptions_;
  RingBuffer ring_buffer_;

  int mode_;

  // requestID
  int reqID_;

  // void read_instruments_from_file_(const std::string& filePath, const
  // std::string& exchangeID);
  void subscribe_instruments_in_file_(const std::string &filePath,
                                      const std::string &exchangeID);

  // make sure body is writed after header
  std::mutex mu_;

};
