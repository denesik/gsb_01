#pragma once
#ifndef thread_vane_h__
#define thread_vane_h__


#include <array>
#include <mutex>
#include <algorithm>

template<class T, size_t SIZE>
class thread_vane
{
public:
  thread_vane(std::mutex &mutex)
    : mMutex(mutex)
  {
  }


  /// Вызывается в потоке записи.
  /// Вызвать когда цикл в потоке записи завершился.
  void write_end()
  {
    {
      std::lock_guard<std::mutex>(mMutex);
      mActual = mWriteCurrent;
      mWriteCurrent = 1 + 2 - mActual - mReadCurrent;
    }
    std::copy(mElements[mActual].begin(), mElements[mActual].end(), mElements[mWriteCurrent].begin());
  }

  /// Вызывается в потоке чтения.
  /// Вызвать когда цикл в потоке чтения завершился.
  void read_end()
  {
    std::lock_guard<std::mutex>(mMutex);
    mReadCurrent = mActual;
  }

  T &read_get(size_t number)
  {
    return mData[mReadCurrent][number];
  }

  T &write_get(size_t number)
  {
    return mData[mWriteCurrent][number];
  }

  const std::array<bool, SIZE> &read_elements() const
  {
    return mElements[mReadCurrent];
  }

  /// Только в потоке записи.
  void write_copy_actual()
  {
    std::copy(mData[mActual].begin(), mData[mActual].end(), mData[mWriteCurrent].begin());
  }

  /// Только в потоке записи.
  size_t write_create()
  {
    for (size_t i = 0; i < mElements[mWriteCurrent].size(); ++i)
    {
      if (!mElements[mWriteCurrent])
      {
        mElements[mWriteCurrent] = true;
        return i;
      }
    }

    return SIZE; // TODO: exception
  }

  void write_remove(size_t number)
  {
    mElements[mWriteCurrent] = false;
  }

private:
  std::array<T, SIZE> mData[3];
  std::array<bool, SIZE> mElements[3];
  size_t mWriteCurrent = 0;
  size_t mReadCurrent = 1;
  size_t mActual = 2;
  std::mutex &mMutex;
};



#endif // thread_vane_h__
