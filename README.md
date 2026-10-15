# Nya's Space Quest 🕹👩🏽‍🚀

An Android game for learning number identification

See Wiki: [`NUMBER_IDENTIFICATION.md`](https://github.com/elimu-ai/wiki/blob/main/numeracy-skills/NUMBER_IDENTIFICATION.md)

## Screenshots 📸

<img width="480" src="https://user-images.githubusercontent.com/15718174/26873794-641bfbfc-4b7b-11e7-95ee-82b19b661ad5.png" />

<img width="480" src="https://user-images.githubusercontent.com/15718174/26873800-67dcc0c8-4b7b-11e7-83f5-38fc8881c3ca.png" />

## Demo Videos 🎥

### Instruction

> **Level 1 - Number Identification**
> 
> This is a scroller level, simple jumps and some action to get the kid engaged with the game, in this stage the numbers are presented with the symbol, objects and audio.

<kbd>[![](https://i.ytimg.com/vi/B4pLVIHeqTc/hqdefault.jpg)](https://youtu.be/B4pLVIHeqTc)</kbd>

### Assessment

> **Level 2 - Number Identification**
>
> This is similar to the 1st level, but in each “stop” the kid has to correctly identify the number associated with the number of objects shown in the screen.

<kbd>[![](https://i.ytimg.com/vi/EkkYc0g3npE/hqdefault.jpg)](https://youtu.be/EkkYc0g3npE)</kbd>

## Installation

> [!IMPORTANT]
> Note: This app depends on the [elimu.ai Content Provider](https://github.com/elimu-ai/content-provider) to be installed.

## Build & Test 🛠

> Verified setup: Cocos2d-x 3.14.1 (C++) game, built via `proj.android-studio`
> (Gradle modules `:Nya2` = app, `:libcocos2dx` = engine) with JDK 17,
> Gradle 8.10 (wrapper), Android Gradle Plugin 8.1.4, `compileSdk 34`,
> NDK `25.2.9519653`, `minSdk 21`. First build takes ~10 min (compiles the
> Cocos native libs for 2 ABIs); incremental builds are fast.

### Prerequisites

- Android SDK with platform `android-34` and matching build-tools, plus NDK
  `25.2.9519653` (pinned via `ndkVersion` in both `build.gradle` files).
- JDK 17 and `proj.android-studio/local.properties` pointing at your SDK, e.g.
  `sdk.dir=/Users/<you>/Library/Android/sdk`.

### Build

```sh
cd proj.android-studio
./gradlew :Nya2:assembleDebug
```

APK output: `proj.android-studio/app/build/outputs/apk/debug/Nya2-debug.apk`
(~37 MB, debug-signed automatically).

Release signing uses the existing mechanism (unverified here): pass
`-PRELEASE_STORE_FILE=... -PRELEASE_STORE_PASSWORD=... -PRELEASE_KEY_ALIAS=... -PRELEASE_KEY_PASSWORD=...`
and build `:Nya2:assembleRelease`.

### Install & smoke-test on an emulator

```sh
# Boot an AVD (see GPU note below), then:
adb -s <device> install -r proj.android-studio/app/build/outputs/apk/debug/Nya2-debug.apk
adb -s <device> shell am start -n org.literacyapp.nya/org.cocos2dx.cpp.AppActivity
adb -s <device> shell pidof org.literacyapp.nya   # process alive = no crash
adb -s <device> exec-out screencap -p > nya.png   # tutorial level renders
adb -s <device> logcat -d | grep -E "FATAL EXCEPTION"  # expect no output
```

Verified: app launches to the tutorial level (astronaut, platforms, stars,
movement/jump hints), background audio plays, no `FATAL` crash. Game code
makes no Content Provider queries, so it runs standalone.

> Emulator GPU note (macOS): with host-GPU (`gfxstream`) graphics the game
> process runs but every GL draw fails with `0x0502` and the screen stays
> black. Retest with software GLES:
> `emulator -avd <name> -gpu swiftshader_indirect`, where it renders correctly
> with zero GL errors.

### What was changed to modernize the build (vs 2017 config)

- `proj.android-studio/build.gradle`: AGP `2.3.0` → `8.1.4`,
  `jcenter()` → `google()` + `mavenCentral()`.
- `proj.android-studio/app/build.gradle`: `namespace`, `compileSdk 34`,
  `ndkVersion 25.2.9519653`, `externalNativeBuild.ndkBuild` wired to
  `jni/Android.mk`, `compile` → `implementation`; ABI filters
  `armeabi-v7a` + `arm64-v8a`.
- `proj.android-studio/app/AndroidManifest.xml`: removed legacy `package=`
  attribute (namespace comes from Gradle), added `android:exported="true"`
  (required when `targetSdk` ≥ 31).
- `cocos2d/.../libcocos2dx/build.gradle`: same SDK bump, `namespace`,
  `buildFeatures { aidl true }` (restores `IGameTuningService` generation).
- `proj.android-studio/app/jni/Application.mk`: `gnustl_static` → `c++_static`
  (gnustl was removed from modern NDKs), `APP_ABI := armeabi-v7a arm64-v8a`,
  `APP_PLATFORM := android-21` (`minSdk 21` preserved).
- `proj.android-studio/app/jni/Android.mk`: `LOCAL_SRC_FILES` lists the real
  `Classes/*.cpp` (the template's `HelloWorldScene.cpp` no longer exists);
  module renamed to `MyGame` (the target Gradle's ndk-build integration
  requests).
- `cocos2d/external/unzip/ioapi.h`: skip the unconditional
  `#define __USE_FILE_OFFSET64` on Android — it hides `fgetpos`/`fsetpos`
  declarations in NDK 25 headers when targeting API < 24, which broke
  `<cstdio>` (libc++). Game assets are far below the 4 GB this flag serves.
- ABIs: `x86_64` dropped (Cocos 3.14.1 ships no `x86_64` prebuilts, so it can
  never configure); `x86` omitted for build speed — re-add both in
  `abiFilters` + `APP_ABI` if needed.

## Background Story 📎

[`Nya’s Space Quest - Background Story.pdf`](https://github.com/user-attachments/files/16119018/Nya.s.Space.Quest.-.Background.Story.pdf)

---

<p align="center">
  <img src="https://github.com/elimu-ai/webapp/blob/main/src/main/webapp/static/img/logo-text-256x78.png" />
</p>
<p align="center">
  elimu.ai - Free open-source learning software for out-of-school children 🚀✨
</p>
<p align="center">
  <a href="https://elimu.ai">Website 🌐</a>
  &nbsp;•&nbsp;
  <a href="https://github.com/elimu-ai/wiki#readme">Wiki 📃</a>
  &nbsp;•&nbsp;
  <a href="https://github.com/orgs/elimu-ai/projects?query=is%3Aopen">Projects 👩🏽‍💻</a>
  &nbsp;•&nbsp;
  <a href="https://github.com/elimu-ai/wiki/milestones">Milestones 🎯</a>
  &nbsp;•&nbsp;
  <a href="https://github.com/elimu-ai/wiki#open-source-community">Community 👋🏽</a>
  &nbsp;•&nbsp;
  <a href="https://www.drips.network/app/drip-lists/41305178594442616889778610143373288091511468151140966646158126636698">Support 💜</a>
</p>
