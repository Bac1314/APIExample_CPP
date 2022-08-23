//
//  Agora Real-time Engagement
//
//  Copyright (c) 2021 Agora.io. All rights reserved.
//
#pragma once
#include <stdint.h>
#include <stddef.h>
namespace agora {
namespace mpc {

enum class ReportItemType {
  MPKCdnConnectState = 9082,
  MPKDestroy = 9083,
  MPKInitialize = 9084,
  MPKOpen = 9085,
  MPKOpenResponse = 9086,
  MPKPause = 9087,
  MPKPlay = 9088,
  MPKPlayState = 9091,
  MPKSeek = 9092,
  MPKSeekResponse = 9093,
  MPKStop = 9094,
  MPKStopResponse = 9095,
  MPKSwitch = 9096,
  MPKNetworkState = 9098,
  MPKSwitchResponse = 9331,
  MPKLasSwitch = 9396,
};

struct IMPKEvent {
  static const int32_t kInvalidValue = -1;
  virtual const char* pack() = 0;
  virtual const char* GetSid() const = 0;
  virtual const char* GetCname() const = 0;
  virtual const char* GetIp() const = 0;

  virtual ~IMPKEvent() = default;

  ReportItemType id;
  int64_t vid = 0;
  int64_t cid = 0;
  int64_t uid = 0;
  int64_t lts = 0;
  int64_t elapse = 0;
  int64_t peer = 0;
  uint64_t event_space = 0;
};

struct FreezeTime {
  int64_t freeze_time = 0;
  int64_t detection_time = 0;
};

struct IStats {
  uint32_t vid = 0;
  uint32_t stream_id = 0;
  FreezeTime audio_200ms_freeze_time;
  FreezeTime video_200ms_freeze_time;
  FreezeTime video_500ms_freeze_time;
  FreezeTime video_600ms_freeze_time;
  int32_t audio_bitrate_avg = 0;
  int32_t video_bitrate_avg = 0;
  int32_t video_frame_rate_avg = 0;
  int32_t video_afterlost_framerate_avg = 0;
  int32_t video_jitter_delay_avg = 0;

  virtual const char* GetSid() const = 0;
  virtual ~IStats() = default;
};

class IAgoraMediaPlayerReporter {
 public:
  virtual IStats* GetStats() = 0;
  virtual void OnNetWorkChanged(int newNetworkType) = 0;

  virtual ~IAgoraMediaPlayerReporter() {}
};

class IAgoraMediaPlayerReportSender {
 public:
  virtual void InitializeReporter(IAgoraMediaPlayerReporter* reporter) = 0;
  virtual void UnInitializeReporter() = 0;
  virtual void GetSDKVersion(char* sdk_version, size_t length) = 0;
  virtual void GetInstallId(char* install_id, size_t length) = 0;
  virtual int32_t GetNetworkInfo() = 0;
  virtual void GetDeviceId(char* device_id, size_t length) = 0;
  virtual uint32_t GetVid() = 0;
  virtual void ReportMPKEvent(IMPKEvent* mpk_event) = 0;
  virtual void StartMediaPlayerCounterStats() = 0;
  virtual void StopMediaPlayerCounterStats() = 0;

  virtual ~IAgoraMediaPlayerReportSender() {}
};

}  // namespace mpc
}  // namespace agora
