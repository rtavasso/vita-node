/*
 * Copyright 2019 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef FIREBASE_APP_CLIENT_CPP_SRC_INCLUDE_FIREBASE_INTERNAL_PLATFORM_H_
#define FIREBASE_APP_CLIENT_CPP_SRC_INCLUDE_FIREBASE_INTERNAL_PLATFORM_H_

// This header serts exactly one of these FIREBASE_PLATFORM macros to 1, and the
// rest to 0:
//
// FIREBASE_PLATFORM_ANDROID
// FIREBASE_PLATFORM_IOS
// FIREBASE_PLATFORM_OSX
// FIREBASE_PLATFORM_WINDOWS
// FIREBASE_PLATFORM_LINUX
// FIREBASE_PLATFORM_UNKNOWN
//
// You can use e.g. #if FIREBASE_PLATFORM_OSX to conditionally compile code
// after including this header.
//
// It also defines some convenience macros:
//   FIREBASE_PLATFORM_DESKTOP (1 on OSX, WINDOWS, and LINUX, 0 otherwise)
//   FIREBASE_PLATFORM_MOBILE (1 on IOS and ANDROID, 0 otherwise)

#define FIREBASE_PLATFORM_ANDROID 0
#define FIREBASE_PLATFORM_IOS 0
#define FIREBASE_PLATFORM_OSX 0
#define FIREBASE_PLATFORM_WINDOWS 0
#define FIREBASE_PLATFORM_LINUX 0
#define FIREBASE_PLATFORM_UNKNOWN 0

#ifdef __APPLE__
#include "TargetConditionals.h"
#endif  // __APPLE__

#if defined(__ANDROID__)
#undef FIREBASE_PLATFORM_ANDROID
#define FIREBASE_PLATFORM_ANDROID 1
#elif defined(TARGET_OS_IOS) && TARGET_OS_IOS
#undef FIREBASE_PLATFORM_IOS
#define FIREBASE_PLATFORM_IOS 1
#elif defined(TARGET_OS_OSX) && TARGET_OS_OSX
#undef FIREBASE_PLATFORM_OSX
#define FIREBASE_PLATFORM_OSX 1
#elif defined(_WIN32)
#undef FIREBASE_PLATFORM_WINDOWS
#define FIREBASE_PLATFORM_WINDOWS 1
#elif defined(__linux__)
#undef FIREBASE_PLATFORM_LINUX
#define FIREBASE_PLATFORM_LINUX 1
#else
#undef FIREBASE_PLATFORM_UNKNOWN
#define FIREBASE_PLATFORM_UNKNOWN 1
#endif

#define FIREBASE_PLATFORM_MOBILE \
  (FIREBASE_PLATFORM_IOS || FIREBASE_PLATFORM_ANDROID)
#define FIREBASE_PLATFORM_DESKTOP                          \
  (FIREBASE_PLATFORM_LINUX || FIREBASE_PLATFORM_WINDOWS || \
   FIREBASE_PLATFORM_OSX)

#endif  // FIREBASE_APP_CLIENT_CPP_SRC_INCLUDE_FIREBASE_INTERNAL_PLATFORM_H_
