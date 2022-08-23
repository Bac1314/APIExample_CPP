//
//  Agora Real-time Engagement
//
//  Copyright (c) 2021 Agora.io. All rights reserved.
//
#pragma once

#include "IAgoraMediaPlayerTypes.h"

namespace agora {
namespace mpc {

typedef enum {
  AgoraMediaPlayerCmdCreate = 0,
  AgoraMediaPlayerCmdRelease = 1,
} AgoraMediaPlayerCmd;

enum RENDER_MODE_TYPE {
  /**
   * 1: Uniformly scale the video until it fills the visible boundaries
   * (cropped). One dimension of the video may have clipped contents.
   */
  RENDER_MODE_HIDDEN = 1,
  /**
   * 2: Uniformly scale the video until one of its dimension fits the boundary
   * (zoomed to fit). Areas that are not filled due to the disparity in the
   * aspect ratio will be filled with black.
   */
  RENDER_MODE_FIT = 2
};

/**
 * This class class reports runtime events to the applications.
 */
class IMediaPlayerCoreObserver {
 public:
  virtual ~IMediaPlayerCoreObserver() {}

  /**
   * @brief Reports the playback state change.
   *
   * When the state of the playback changes, the SDK triggers this callback to report the new
   * playback state and the reason or error for the change.
   * @param state The new playback state after change. See {@link MEDIA_PLAYER_STATE
   * MEDIA_PLAYER_STATE}.
   * @param ec The player's error code. See {@link MEDIA_PLAYER_ERROR MEDIA_PLAYER_ERROR}.
   */
  virtual void onPlayerStateChanged(MEDIA_PLAYER_STATE state, MEDIA_PLAYER_ERROR ec) = 0;

  /**
   * @brief Reports current playback progress.
   *
   * The callback occurs once every one second during the playback and reports the current playback
   * progress.
   * @param position Current playback progress (second).
   */
  virtual void onPositionChanged(int64_t position) = 0;

  /**
   * @brief Reports the playback event.
   *
   * - After calling the `seek` method, the SDK triggers the callback to report the results of the
   * seek operation.
   * - After calling the `selectAudioTrack` method, the SDK triggers the callback to report that the
   * audio track changes.
   *
   * @param event The playback event. See {@link MEDIA_PLAYER_EVENT MEDIA_PLAYER_EVENT}.
   * @param elapsedTime The playback elapsed time.
   * @param message The playback message.
   */
  virtual void onPlayerEvent(MEDIA_PLAYER_EVENT event, int64_t elapsedTime, const char* message) = 0;

  /**
   * @brief Occurs when the metadata is received.
   *
   * The callback occurs when the player receives the media metadata and reports the detailed
   * information of the media metadata.
   * @param data The detailed data of the media metadata.
   * @param length The data length (bytes).
   */
  virtual void onMetaData(const void* data, int length) = 0;

  /**
   * @brief Triggered when play buffer updated, once every 1 second
   *
   * @param int cached buffer during playing, in milliseconds
   */
  virtual void onPlayBufferUpdated(int64_t playCachedBuffer) = 0;

  /**
   * @brief Triggered when the player preloadSrc
   *
   * @param event
   */
  virtual void onPreloadEvent(const char* src, PLAYER_PRELOAD_EVENT event) = 0;

  /**
   * @brief Occurs when one playback of the media file is completed.
   */
  virtual void onCompleted() = 0;

  /**
   * @brief AgoraCDN Token has expired and needs to be set up with renewAgoraCDNSrcToken(const char*
   * src).
   */
  virtual void onAgoraCDNTokenWillExpire() = 0;

  /**
   * @brief Reports current playback source bitrate changed.
   *
   * @param fromBitrate Video bitrate change from.
   * @param fromBitrateName Video bitrate name change from.
   * @param toBitrate Video bitrate change to.
   * @param toBitrateName Video bitrate name change to.
   */
  virtual void onPlaySrcBitrateChanged(int fromBitrate, const char* fromBitrateName, int toBitrate, const char* toBitrateName) = 0;

  /**
   * @brief Triggered when  media player information updated.
   *
   * @param info Include information of media player.
   */
  virtual void onPlayerInfoUpdated(const PlayerUpdatedInfo& info) = 0;
};

class IAgoraMediaPlayerReportSender;
/**
 * The custom data source provides a data stream input callback, and the player will continue to
 * call back this interface, requesting the user to fill in the data that needs to be played.
 */
class IMediaPlayerCustomDataProvider {
 public:
  /**
   * @brief The player requests to read the data callback, you need to fill the specified length of
   * data into the buffer
   * @param buf the buffer pointer that you need to fill data.
   * @param buf_size the bufferSize need to fill of the buffer pointer.
   * @return you need return offset value if succeed. return 0 if failed.
   */
  virtual int onReadData(unsigned char* buf, int buf_size) = 0;

  /**
   * @brief The Player seek event callback, you need to operate the corresponding stream seek
   * operation, You can refer to the definition of lseek() at
   * https://man7.org/linux/man-pages/man2/lseek.2.html
   * @param offset the value of seek offset.
   * @param whence the postion of start seeking, the directive whence as follows:
   * SEEK_SET : The file offset is set to offset bytes.
   * SEEK_CUR : The file offset is set to its current location plus offset bytes.
   * SEEK_END : The file offset is set to the size of the file plus offset bytes.
   * @return you need return offset value if succeed. return -1 if failed.
   */
  virtual int64_t onSeek(int64_t offset, int whence) = 0;

  virtual ~IMediaPlayerCustomDataProvider() {}
};

class IMediaPlayerCore {
 protected:
  virtual ~IMediaPlayerCore() {}

 public:
  /**
   * @brief Set video rendering view
   *
   * @param view view object, windows platform is HWND
   * @return int < 0 on behalf of an error, the value corresponds to one of MEDIA_PLAYER_ERROR
   */
  virtual int setView(void* view) = 0;

  /**
   * @brief Turn mute on or off
   *
   * @param mute Whether to mute on
   * @return int < 0 on behalf of an error, the value corresponds to one of MEDIA_PLAYER_ERROR
   */
  virtual int mute(bool mute) = 0;

  /**
   * @brief Get mute state
   *
   * @param[out] mute Whether is mute on
   * @return int < 0 on behalf of an error, the value corresponds to one of MEDIA_PLAYER_ERROR
   */
  virtual int getMute(bool& mute) = 0;

  /**
   * @brief Adjust playback volume
   *
   * @param volume The volume value to be adjusted
   * The volume can be adjusted from 0 to 100:
   * 0: mute;
   * @return int < 0 on behalf of an error, the value corresponds to one of MEDIA_PLAYER_ERROR
   */
  virtual int adjustPlayoutVolume(int volume) = 0;

  /**
   * @brief Get the current playback volume
   *
   * @param[out] volume
   * @return int < 0 on behalf of an error, the value corresponds to one of MEDIA_PLAYER_ERROR
   */
  virtual int getPlayoutVolume(int& volume) = 0;

  /**
   * @brief Set video display mode
   *
   * @param renderMode Video display mode
   * @return int < 0 on behalf of an error, the value corresponds to one of MEDIA_PLAYER_ERROR
   */
  virtual int setRenderMode(RENDER_MODE_TYPE renderMode) = 0;

  /**
   * Opens a media file with a specified URL.
   * @param url The URL of the media file that you want to play.
   * @return
   * - 0: Success.
   * - < 0: Failure.
   */
  virtual int open(const char* url, int64_t startPos) = 0;

  /**
   * @brief Open media file or stream with custom soucrce.
   * @param startPos Set the starting position for playback, in seconds (ms).
   * @param observer dataProvider object
   * @return
   * - 0: Success.
   * - < 0: Failure.
   */
  virtual int openWithCustomSource(int64_t startPos, IMediaPlayerCustomDataProvider* provider) = 0;

  /**
   * Plays the media file.
   * @return
   * - 0: Success.
   * - < 0: Failure.
   */
  virtual int play() = 0;

  /**
   * Pauses playing the media file.
   */
  virtual int pause() = 0;

  /**
   * Stops playing the current media file.
   */
  virtual int stop() = 0;

  /**
   * Resumes playing the media file.
   */
  virtual int resume() = 0;

  /**
   * Sets the current playback position of the media file.
   * @param newPos The new playback position (ms).
   * @return
   * - 0: Success.
   * - < 0: Failure.
   */
  virtual int seek(int64_t newPos) = 0;

  /**
   * Gets the duration of the media file.
   * @param duration A reference to the duration of the media file.
   * @return
   * - 0: Success.
   * - < 0: Failure.
   */
  virtual int getDuration(int64_t& duration) = 0;

  /**
   * Gets the current playback position of the media file.
   * @param currentPosition A reference to the current playback position (ms).
   * @return
   * - 0: Success.
   * - < 0: Failure.
   */
  virtual int getPlayPosition(int64_t& pos) = 0;

  virtual int getStreamCount(int64_t& count) = 0;

  virtual int getStreamInfo(int64_t index, PlayerStreamInfo* info) = 0;
  /**
   * Sets whether to loop the media file for playback.
   * @param loopCount the number of times looping the media file.
   * -  =  -1 repeat play same source media forever
   * -  >  0  repeat play same source media times
   * -  <= 0  play media source once
   * @return
   * - 0: Success.
   * - < 0: Failure.
   */
  virtual int setLoopCount(int loopCount) = 0;

  /**
   * Change playback speed
   * @param speed the enum of playback speed
   * @return
   * - 0: Success.
   * - < 0: Failure.
   */
  virtual int setPlaybackSpeed(int speed) = 0;

  /**
   * Slect playback audio track of the media file
   * @param speed the index of the audio track in meia file
   * @return
   * - 0: Success.
   * - < 0: Failure.
   */
  virtual int selectAudioTrack(int index) = 0;

  /**
   * change player option before play a file
   * @param key the key of the option param
   * @param value the value of option param
   * @return
   * - 0: Success.
   * - < 0: Failure.
   */
  virtual int setPlayerOption(const char* key, int value) = 0;

  /**
   * change player option before play a file
   * @param key the key of the option param
   * @param value the value of option param
   * @return
   * - 0: Success.
   * - < 0: Failure.
   */
  virtual int setPlayerOption(const char* key, const char* value) = 0;
  /**
   * take screenshot while playing  video
   * @param filename the filename of screenshot file
   * @return
   * - 0: Success.
   * - < 0: Failure.
   */
  virtual int takeSnapshot(const char* filename) = 0;

  /**
   * select internal subtitles in video
   * @param index the index of the internal subtitles
   * @return
   * - 0: Success.
   * - < 0: Failure.
   */
  virtual int selectInternalSubtitle(int index) = 0;

  /**
   * set an external subtitle for video
   * @param url The URL of the subtitle file that you want to load.
   * @return
   * - 0: Success.
   * - < 0: Failure.
   */
  virtual int setExternalSubtitle(const char* url) = 0;

  virtual MEDIA_PLAYER_STATE getState() = 0;

  /**
   * Registers a media player source observer.
   *
   * Once the media player source observer is registered, you can use the observer to monitor the
   * state change of the media player.
   * @param observer The pointer to the IMediaPlayerCoreObserver object.
   * @return
   * - 0: Success.
   * - < 0: Failure.
   */
  virtual int registerPlayerSourceObserver(IMediaPlayerCoreObserver* observer) = 0;

  /**
   * Releases the media player source observer.
   * @param observer The pointer to the IMediaPlayerCoreObserver object.
   * @return
   * - 0: Success.
   * - < 0: Failure.
   */
  virtual int unregisterPlayerSourceObserver(IMediaPlayerCoreObserver* observer) = 0;

  /**
   * Register the audio frame observer.
   *
   * @param observer The pointer to the IAudioFrameObserver object.
   * @return
   * - 0: Success.
   * - < 0: Failure.
   */
  virtual int registerAudioFrameObserver(mpc::IAudioFrameObserver* observer) = 0;

  /**
   * Releases the audio frame observer.
   * @param observer The pointer to the IAudioFrameObserver object.
   * @return
   * - 0: Success.
   * - < 0: Failure.
   */
  virtual int unregisterAudioFrameObserver(mpc::IAudioFrameObserver* observer) = 0;

  /**
   * @brief Register the player video observer
   *
   * @param observer observer object
   * @return int < 0 on behalf of an error, the value corresponds to one of MEDIA_PLAYER_ERROR
   */
  virtual int registerVideoFrameObserver(mpc::IVideoFrameObserver* observer) = 0;

  /**
   * @brief UnRegister the player video observer
   *
   * @param observer observer object
   * @return int < 0 on behalf of an error, the value corresponds to one of MEDIA_PLAYER_ERROR
   */
  virtual int unregisterVideoFrameObserver(mpc::IVideoFrameObserver* observer) = 0;

  /**
   * get sdk version and build number of player SDK.
   * @return String of the SDK version.
   */
  virtual const char* getPlayerSdkVersion() = 0;

  /**
   * Switch the media source when open a media through "open" API
   * @param src Specific src.
   * @param syncPts Live streaming must be set to false.
   * @return
   * - 0: Success.
   * - < 0: Failure.
   */
  virtual int switchSrc(const char* src, bool syncPts = true) = 0;

  /**
   * Preload a media source
   * @param src Specific src.
   * @param startPos The starting position (ms) for playback. Default value is 0.
   * @return
   * - 0: Success.
   * - < 0: Failure.
   */
  virtual int preloadSrc(const char* src, int64_t startPos) = 0;

  /**
   * Unload a preloaded media source
   * @param src Specific src.
   * @return
   * - 0: Success.
   * - < 0: Failure.
   */
  virtual int unloadSrc(const char* src) = 0;

  /**
   * Play a pre-loaded media source
   * @param src Specific src.
   * @return
   * - 0: Success.
   * - < 0: Failure.
   */
  virtual int playPreloadedSrc(const char* src) = 0;

  /**
   * Open the Agora CDN media source.
   * @param src The src of the media file that you want to play.
   * @param startPos The  playback position (ms).
   * @return
   * - 0: Success.
   * - < 0: Failure.
   */
  virtual int openWithAgoraCDNSrc(const char* src, int64_t startPos) = 0;

  /**
   * Gets the number of  Agora CDN lines.
   * @return
   * - > 0: number of CDN.
   * - <= 0: Failure.
   */
  virtual int getAgoraCDNLineCount() = 0;

  /**
   * Switch Agora CDN lines.
   * @param index Specific CDN line index.
   * @return
   * - 0: Success.
   * - < 0: Failure.
   */
  virtual int switchAgoraCDNLineByIndex(int index) = 0;

  /**
   * Gets the line of the current CDN.
   * @return
   * - >= 0: Specific line.
   * - < 0: Failure.
   */
  virtual int getCurrentAgoraCDNIndex() = 0;

  /**
   * Enable automatic CDN line switching.
   * @param enable Whether enable.
   * @return
   * - 0: Success.
   * - < 0: Failure.
   */
  virtual int enableAutoSwitchAgoraCDN(bool enable) = 0;

  /**
   * Update the CDN source token and timestamp.
   * @param token token.
   * @param ts ts.
   * @return
   * - 0: Success.
   * - < 0: Failure.
   */
  virtual int renewAgoraCDNSrcToken(const char* token, int64_t ts) = 0;

  /**
   * Switch the CDN source.
   * @param src Specific src.
   * @param syncPts Live streaming must be set to false.
   * @return
   * - 0: Success.
   * - < 0: Failure.
   */
  virtual int switchAgoraCDNSrc(const char* src, bool syncPts = false) = 0;

  /**
   * Set report sender
   * @param sender report sender
   */
  virtual void SetReportSender(IAgoraMediaPlayerReportSender* sender) = 0;

  /**
   * @brief release the object.
   *
   */
  virtual void release() = 0;
};

}  // namespace mpc
}  // namespace agora

AGORA_MPC_API int MediaPlayerCoreInit(const char* logDir, const int logSize, unsigned int filter);

AGORA_MPC_API int MediaPlayerCoreUnInit();

AGORA_MPC_API const char* getMediaPlayerCoreVer();

AGORA_MPC_API agora::mpc::IMediaPlayerCore* createMediaPlayerCore();
