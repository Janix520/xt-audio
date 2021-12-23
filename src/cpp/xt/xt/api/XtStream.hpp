#ifndef XT_API_STREAM_HPP
#define XT_API_STREAM_HPP

/** @file */
/** @cond */
#include <xt/cpp/Core.hpp>
#include <xt/cpp/Error.hpp>
#include <xt/api/Structs.hpp>
#include <xt/api/Callbacks.hpp>

#include <cstdint>
/** @endcond */

namespace Xt {

using namespace Detail;

class Stream final 
{
  XtStream* _s;
  void* const _user;
  StreamParams const _params;

  Stream(StreamParams const& params, void* user):
  _s(nullptr), _params(params), _user(user) { }

public:
  ~Stream();
  void Stop();
  void Start();
  bool IsRunning() const;
  void* GetHandle() const;
  int32_t GetFrames() const;
  bool IsInterleaved() const;
  Latency GetLatency() const;
  Format const& GetFormat() const;

/** @cond */
  friend class Device;
  friend class Service;
  friend void XT_CALLBACK 
  Detail::ForwardOnXRun(XtStream const* coreStream, int32_t index, void* user);
  friend uint32_t XT_CALLBACK 
  Detail::ForwardOnBuffer(XtStream const* coreStream, XtBuffer const* coreBuffer, void* user);
  friend void XT_CALLBACK 
  Detail::ForwardOnRunning(XtStream const* coreStream, XtBool running, uint64_t error, void* user);
/** @endcond */
};

inline void
Stream::Start() 
{ Detail::HandleError(XtStreamStart(_s)); }
inline void
Stream::Stop() 
{ Detail::HandleAssert(XtStreamStop, _s); }
inline
Stream::~Stream() 
{ Detail::HandleDestroy(XtStreamDestroy, _s); }
inline void*
Stream::GetHandle() const
{ return Detail::HandleAssert(XtStreamGetHandle(_s)); }
inline bool
Stream::IsRunning() const
{ return Detail::HandleAssert(XtStreamIsRunning(_s)); }

inline int32_t
Stream::GetFrames() const 
{
  int32_t frames;
  Detail::HandleError(XtStreamGetFrames(_s, &frames));
  return frames;
}

inline Latency
Stream::GetLatency() const
{
  Latency latency;
  auto coreLatency = reinterpret_cast<XtLatency*>(&latency);
  Detail::HandleError(XtStreamGetLatency(_s, coreLatency));
  return latency;
}

inline Format const& 
Stream::GetFormat() const
{
  auto coreFormat = Detail::HandleAssert(XtStreamGetFormat(_s));
  return *reinterpret_cast<Format const*>(coreFormat);
}

inline bool
Stream::IsInterleaved() const
{
  auto coreInterleaved = Detail::HandleAssert(XtStreamIsInterleaved(_s));
  return coreInterleaved != XtFalse;
}

} // namespace Xt
#endif // XT_API_STREAM_HPP