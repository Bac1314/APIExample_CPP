//
//  Agora Media Player Core SDK
//
//  Copyright (c) 2021 Agora.io. All rights reserved.

#pragma once

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <cassert>
#include <cstdint>

#include "AgoraOptional.h"

#if defined(_WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#define AGORA_MPC_CALL __cdecl
#if defined(AGORA_MPC_EXPORT)
#define AGORA_MPC_API extern "C" __declspec(dllexport)
#define AGORA_MPC_CPP_API __declspec(dllexport)
#elif defined(AGORA_MPC_IMPORT)
#define AGORA_MPC_API extern "C" __declspec(dllimport)
#define AGORA_MPC_CPP_API __declspec(dllimport)
#else
#define AGORA_MPC_API
#define AGORA_MPC_CPP_API
#endif
#elif defined(__APPLE__)
#include <TargetConditionals.h>
#define AGORA_MPC_API __attribute__((visibility("default"))) extern "C"
#define AGORA_MPC_CPP_API __attribute__((visibility("default")))
#define AGORA_MPC_CALL
#elif defined(__ANDROID__) || defined(__linux__)
#define AGORA_MPC_API extern "C" __attribute__((visibility("default")))
#define AGORA_MPC_CPP_API __attribute__((visibility("default")))
#define AGORA_MPC_CALL
#else
#define AGORA_MPC_API extern "C"
#define AGORA_MPC_CPP_API
#define AGORA_MPC_CALL
#endif

/**
 * set analyze duration for real time stream
 * @example "setPlayerOption(KEY_PLAYER_REAL_TIME_STREAM_ANALYZE_DURATION,1000000)"
 */
#define KEY_PLAYER_REAL_TIME_STREAM_ANALYZE_DURATION "analyze_duration"

/**
 * make the player to enable audio or not
 * @example  "setPlayerOption(KEY_PLAYER_ENABLE_AUDIO,0)"
 */
#define KEY_PLAYER_ENABLE_AUDIO "enable_audio"

/**
 * make the player to enable video or not
 * @example  "setPlayerOption(KEY_PLAYER_ENABLE_VIDEO,0)"
 */
#define KEY_PLAYER_ENABLE_VIDEO "enable_video"

/**
 * set the player enable to search metadata
 * @example  "setPlayerOption(KEY_PLAYER_DISABLE_SEARCH_METADATA,0)"
 */
#define KEY_PLAYER_ENABLE_SEARCH_METADATA "enable_search_metadata"

/**
 * set the player sei filter type
 * @example  "setPlayerOption(KEY_PLAYER_SEI_FILTER_TYPE,"5")"
 */
#define KEY_PLAYER_SEI_FILTER_TYPE "set_sei_filter_type"

namespace agora {

namespace mpc {

static const uint8_t kMaxCharBufferLength = 50;
/**
 * @brief The playback state.
 *
 */
enum MEDIA_PLAYER_STATE {
  /** Default state.
   */
  PLAYER_STATE_IDLE = 0,
  /** Opening the media file.
   */
  PLAYER_STATE_OPENING,
  /** The media file is opened successfully.
   */
  PLAYER_STATE_OPEN_COMPLETED,
  /** Playing the media file.
   */
  PLAYER_STATE_PLAYING,
  /** The playback is paused.
   */
  PLAYER_STATE_PAUSED,
  /** The playback is completed.
   */
  PLAYER_STATE_PLAYBACK_COMPLETED,
  /** All loops are completed.
   */
  PLAYER_STATE_PLAYBACK_ALL_LOOPS_COMPLETED,
  /** The playback is stopped.
   */
  PLAYER_STATE_STOPPED,
  /** Player pausing (internal)
   */
  PLAYER_STATE_PAUSING_INTERNAL = 50,
  /** Player stopping (internal)
   */
  PLAYER_STATE_STOPPING_INTERNAL,
  /** Player seeking state (internal)
   */
  PLAYER_STATE_SEEKING_INTERNAL,
  /** Player getting state (internal)
   */
  PLAYER_STATE_GETTING_INTERNAL,
  /** None state for state machine (internal)
   */
  PLAYER_STATE_NONE_INTERNAL,
  /** Do nothing state for state machine (internal)
   */
  PLAYER_STATE_DO_NOTHING_INTERNAL,
  /** The playback fails.
   */
  PLAYER_STATE_FAILED = 100,
};
/**
 * @brief Player error code
 *
 */
enum MEDIA_PLAYER_ERROR {
  /** No error.
   */
  PLAYER_ERROR_OK = 0,
  /** The parameter is invalid.
   */
  PLAYER_ERROR_INVALID_ARGUMENTS = -1,
  /** Internel error.
   */
  PLAYER_ERROR_FAILED = -2,
  /** No resource.
   */
  PLAYER_ERROR_NO_RESOURCE = -3,
  /** Invalid media source.
   */
  PLAYER_ERROR_INVALID_MEDIA_SOURCE = -4,
  /** The type of the media stream is unknown.
   */
  PLAYER_ERROR_UNKNOWN_STREAM_TYPE = -5,
  /** The object is not initialized.
   */
  PLAYER_ERROR_NOT_INITIALIZED = -6,
  /** The codec is not supported.
   */
  PLAYER_ERROR_CODEC_NOT_SUPPORTED = -7,
  /** Invalid renderer.
   */
  PLAYER_ERROR_VIDEO_RENDER_FAILED = -8,
  /** An error occurs in the internal state of the player.
   */
  PLAYER_ERROR_INVALID_STATE = -9,
  /** The URL of the media file cannot be found.
   */
  PLAYER_ERROR_URL_NOT_FOUND = -10,
  /** Invalid connection between the player and the Agora server.
   */
  PLAYER_ERROR_INVALID_CONNECTION_STATE = -11,
  /** The playback buffer is insufficient.
   */
  PLAYER_ERROR_SRC_BUFFER_UNDERFLOW = -12,
  /** The audio mixing file playback is interrupted.
   */
  PLAYER_ERROR_INTERRUPTED = -13,

  PLAYER_ERROR_NOT_SUPPORTED = -14,

  PLAYER_ERROR_TOKEN_EXPIRED = -15,

  PLAYER_ERROR_IP_EXPIRED = -16,
};

/**
 * @brief The playback speed.
 *
 */
enum MEDIA_PLAYER_PLAYBACK_SPEED {
  /** The original playback speed.
   */
  PLAYBACK_SPEED_ORIGINAL = 100,
  /** 0.5 times the original playback speed.
   */
  PLAYBACK_SPEED_50_PERCENT = 50,
  /** 0.75 times the original playback speed.
   */
  PLAYBACK_SPEED_75_PERCENT = 75,
  /** 1.25 times the original playback speed.
   */
  PLAYBACK_SPEED_125_PERCENT = 125,
  /** 1.5 times the original playback speed.
   */
  PLAYBACK_SPEED_150_PERCENT = 150,
  /** 2.0 times the original playback.
   */
  PLAYBACK_SPEED_200_PERCENT = 200,
};

/**
 * @brief The type of the media stream.
 *
 */
enum MEDIA_STREAM_TYPE {
  /** The type is unknown.
   */
  STREAM_TYPE_UNKNOWN = 0,
  /** The video stream.
   */
  STREAM_TYPE_VIDEO = 1,
  /** The audio stream.
   */
  STREAM_TYPE_AUDIO = 2,
  /** The subtitle stream.
   */
  STREAM_TYPE_SUBTITLE = 3,
};

/**
 * @brief The playback event.
 *
 */
enum MEDIA_PLAYER_EVENT {
  /** The player begins to seek to the new playback position.
   */
  PLAYER_EVENT_SEEK_BEGIN = 0,
  /** The seek operation completes.
   */
  PLAYER_EVENT_SEEK_COMPLETE = 1,
  /** An error occurs during the seek operation.
   */
  PLAYER_EVENT_SEEK_ERROR = 2,
  /** The player publishes a video track.
   */
  PLAYER_EVENT_VIDEO_PUBLISHED = 3,
  /** The player publishes an audio track.
   */
  PLAYER_EVENT_AUDIO_PUBLISHED = 4,
  /** The player changes the audio track for playback.
   */
  PLAYER_EVENT_AUDIO_TRACK_CHANGED = 5,
  /** player buffer low
   */
  PLAYER_EVENT_BUFFER_LOW = 6,
  /** player buffer recover
   */
  PLAYER_EVENT_BUFFER_RECOVER = 7,
  /** The video or audio is interrupted
   */
  PLAYER_EVENT_FREEZE_START = 8,
  /** Interrupt at the end of the video or audio
   */
  PLAYER_EVENT_FREEZE_STOP = 9,
  /** switch source begin
   */
  PLAYER_EVENT_SWITCH_BEGIN = 10,
  /** switch source complete
   */
  PLAYER_EVENT_SWITCH_COMPLETE = 11,
  /** switch source error
   */
  PLAYER_EVENT_SWITCH_ERROR = 12,
  /** An application can render the video to less than a second
   */
  PLAYER_EVENT_FIRST_DISPLAYED = 13,
};

/**
 * @brief The play preload another source event.
 *
 */
enum PLAYER_PRELOAD_EVENT {
  /** preload source begin
   */
  PLAYER_PRELOAD_EVENT_BEGIN = 0,
  /** preload source complete
   */
  PLAYER_PRELOAD_EVENT_COMPLETE = 1,
  /** preload source error
   */
  PLAYER_PRELOAD_EVENT_ERROR = 2,
};

/**
 * @brief The information of the media stream object.
 *
 */
struct PlayerStreamInfo {
  /** The index of the media stream. */
  int streamIndex;

  /** The type of the media stream. See {@link MEDIA_STREAM_TYPE}. */
  MEDIA_STREAM_TYPE streamType;

  /** The codec of the media stream. */
  char codecName[kMaxCharBufferLength];

  /** The language of the media stream. */
  char language[kMaxCharBufferLength];

  /** The frame rate (fps) if the stream is video. */
  int videoFrameRate;

  /** The video bitrate (bps) if the stream is video. */
  int videoBitRate;

  /** The video width (pixel) if the stream is video. */
  int videoWidth;

  /** The video height (pixel) if the stream is video. */
  int videoHeight;

  /** The rotation angle if the steam is video. */
  int videoRotation;

  /** The sample rate if the stream is audio. */
  int audioSampleRate;

  /** The number of audio channels if the stream is audio. */
  int audioChannels;

  /** The number of bits per sample if the stream is audio. */
  int audioBitsPerSample;

  /** The total duration (second) of the media stream. */
  int64_t duration;

  PlayerStreamInfo() : streamIndex(0), streamType(STREAM_TYPE_UNKNOWN), videoFrameRate(0), videoBitRate(0), videoWidth(0), videoHeight(0), videoRotation(0), audioSampleRate(0), audioChannels(0), audioBitsPerSample(0), duration(0) {
    memset(codecName, 0, sizeof(codecName));
    memset(language, 0, sizeof(language));
  }
};

/**
 * @brief The type of the media metadata.
 *
 */
enum MEDIA_PLAYER_METADATA_TYPE {
  /** The type is unknown.
   */
  PLAYER_METADATA_TYPE_UNKNOWN = 0,
  /** The type is SEI.
   */
  PLAYER_METADATA_TYPE_SEI = 1,
};

/**
 * The detailed information of the incoming audio frame in the PCM format.
 */
struct AudioPcmFrame {
  /**
   * The buffer size of the PCM audio frame.
   */
  enum : size_t {
    // Stereo, 32 kHz, 60 ms (2 * 32 * 60)
    /**
     * The max number of the samples of the data.
     *
     * When the number of audio channel is two, the sample rate is 32 kHZ,
     * the buffer length of the data is 60 ms, the number of the samples of the data is 3840 (2 x 32
     * x 60).
     */
    kMaxDataSizeSamples = 3840,
    /** The max number of the bytes of the data. */
    kMaxDataSizeBytes = kMaxDataSizeSamples * sizeof(int16_t),
  };

  /** The timestamp (ms) of the audio frame.
   */
  uint32_t capture_timestamp;
  /** The number of samples per channel.
   */
  size_t samples_per_channel_;
  /** The sample rate (Hz) of the audio data.
   */
  int sample_rate_hz_;
  /** The channel number.
   */
  size_t num_channels_;
  /** The number of bytes per sample.
   */
  size_t bytes_per_sample;
  /** The audio frame data. */
  int16_t data_[kMaxDataSizeSamples];

  AudioPcmFrame& operator=(const AudioPcmFrame& src) {
    if (this == &src) {
      return *this;
    }

    this->capture_timestamp = src.capture_timestamp;
    this->samples_per_channel_ = src.samples_per_channel_;
    this->sample_rate_hz_ = src.sample_rate_hz_;
    this->bytes_per_sample = src.bytes_per_sample;
    this->num_channels_ = src.num_channels_;

    size_t length = src.samples_per_channel_ * src.num_channels_;
    if (length > kMaxDataSizeSamples) {
      length = kMaxDataSizeSamples;
    }

    memcpy(this->data_, src.data_, length * sizeof(int16_t));

    return *this;
  }

  AudioPcmFrame() : capture_timestamp(0), samples_per_channel_(0), sample_rate_hz_(0), num_channels_(0), bytes_per_sample(2) { memset(data_, 0, sizeof(data_)); }
};

struct PlayerUpdatedInfo {
  /** player_id and device_id has value when user trigger interface of opening
   */
  mpc::Optional<const char*> player_id;
  mpc::Optional<const char*> device_id;
};

class IAudioFrameObserver {
 public:
  /**
   * Occurs when each time the player receives an audio frame.
   *
   * After registering the audio frame observer,
   * the callback occurs when each time the player receives an audio frame,
   * reporting the detailed information of the audio frame.
   * @param frame The detailed information of the audio frame. See {@link AudioPcmFrame}.
   */
  virtual void onFrame(const AudioPcmFrame* frame) = 0;
  virtual ~IAudioFrameObserver() {}
};

/**
 * Video pixel formats.
 */
enum VIDEO_PIXEL_FORMAT {
  /** Unknown pix format
   */
  VIDEO_PIXEL_UNKNOWN = 0,
  /** YUV420
   */
  VIDEO_PIXEL_I420 = 1,
  /** hardware decoding through Videotoolbox
   */
  VIDEO_PIXEL_VIDEOTOOLBOX,
  /** hardware decoding through MediaCodec
   */
  VIDEO_PIXEL_MEDIACODEC,
  /**
   * Hardware surfaces for Direct3D11.
   *
   * yBuffer contains a ID3D11Texture2D pointer, and uBuffer contains the
   * texture array index of the frame as intptr_t if the ID3D11Texture2D is
   * an array texture (or always 0 if it's a normal texture).
   */
  VIDEO_PIXEL_D3D11,
  /** HW decoding through DXVA2, yBuffer contains a LPDIRECT3DSURFACE9 pointer
   */
  VIDEO_PIXEL_DXVA2_VLD,
  /**
   *  HW acceleration through QSV, yBuffer contains a pointer to the
   *  mfxFrameSurface1 structure.
   */
  VIDEO_PIXEL_QSV,
  /**
   * HW acceleration though MMAL, yBuffer contains a pointer to the
   * MMAL_BUFFER_HEADER_T structure.
   */
  VIDEO_PIXEL_MMAL
};

/**
 * The definition of the VideoFrame struct.
 */
struct VideoFrame {
  VideoFrame() : type(VIDEO_PIXEL_UNKNOWN), width(0), height(0), yStride(0), uStride(0), vStride(0), yBuffer(NULL), uBuffer(NULL), vBuffer(NULL), rotation(0), renderTimeMs(0), avsync_type(0), metadata_buffer(NULL), metadata_size(0) {}

  /**
   * The video pixel format: #VIDEO_PIXEL_FORMAT.
   */
  VIDEO_PIXEL_FORMAT type;
  /**
   * The width of the video frame.
   */
  int width;
  /**
   * The height of the video frame.
   */
  int height;
  /**
   * The line span of Y buffer in the YUV data.
   */
  int yStride;
  /**
   * The line span of U buffer in the YUV data.
   */
  int uStride;
  /**
   * The line span of V buffer in the YUV data.
   */
  int vStride;
  /**
   * The pointer to the Y buffer in the YUV data.
   */
  uint8_t* yBuffer;
  /**
   * The pointer to the U buffer in the YUV data.
   */
  uint8_t* uBuffer;
  /**
   * The pointer to the V buffer in the YUV data.
   */
  uint8_t* vBuffer;
  /**
   * The clockwise rotation information of this frame. You can set it as 0, 90, 180 or 270.
   */
  int rotation;
  /**
   * The timestamp to render the video stream. Use this parameter for audio-video synchronization
   * when rendering the video.
   *
   * @note This parameter is for rendering the video, not capturing the video.
   */
  int64_t renderTimeMs;
  /**
   * The type of audio-video synchronization.
   */
  int avsync_type;
  /**
   * [Texture related parameter] The MetaData buffer.
   *  The default value is NULL
   */
  uint8_t* metadata_buffer;
  /**
   * [Texture related parameter] The MetaData size.
   *  The default value is 0
   */
  int metadata_size;
};

class IVideoFrameObserver {
 public:
  /**
   * Occurs each time the player receives a video frame.
   *
   * After registering the video frame observer,
   * the callback occurs each time the player receives a video frame to report the detailed
   * information of the video frame.
   * @param frame The detailed information of the video frame. See {@link VideoFrame}.
   */
  virtual void onFrame(const VideoFrame* frame) = 0;
  virtual ~IVideoFrameObserver() {}
};

}  // namespace mpc
}  // namespace agora
