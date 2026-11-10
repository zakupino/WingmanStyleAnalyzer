Wingman Style Analyzer - প্রাথমিক VST3 প্রজেক্ট
================================================

এটি Mixed In Key Wingman 3-এর আসল সফটওয়্যার নয়; এটি নিজের তৈরি একটি ছোট
প্রাথমিক prototype। কোনো proprietary code বা asset ব্যবহার করা হয়নি।

এখন যা আছে:
- Stereo audio pass-through
- FFT ভিত্তিক সাধারণ chord estimate (Major, Minor, Diminished, Sus4)
- Plugin UI-তে chord ও note দেখানো
- Chord বদলালে triad MIDI note পাঠানোর prototype

গুরুত্বপূর্ণ সীমাবদ্ধতা:
- এটি Wingman 3-এর পূর্ণ বিকল্প নয়।
- Polyphonic/mixed audio-তে chord detection ভুল হতে পারে।
- Bassline generator, stem separation, MIDI export UI, chord progression editor
  এখনো যোগ করা হয়নি।
- DAW-তে যে audio track বিশ্লেষণ করতে চান, সেটি plugin-এর input-এ route করতে হবে।
  Plugin নিজে থেকে DAW-এর অন্য সব track শুনতে পারে না।
- VST3-compatible DAW প্রয়োজন।

Windows build:
1. Windows 10/11-এ Visual Studio 2022 (Desktop development with C++) এবং
   CMake ইনস্টল থাকতে হবে।
2. এই folder-এ PowerShell/CMD খুলুন।
3. চালান:
   cmake -S . -B build -A x64
   cmake --build build --config Release
4. JUCE source CMake প্রথমবার internet থেকে আনবে।
5. Build সফল হলে build folder-এর ভিতরে VST3 ও Standalone target তৈরি হবে।
   Exact output path CMake/JUCE version অনুযায়ী আলাদা হতে পারে।

এই ZIP-এ compiled .vst3/.dll নেই; এখানে source project আছে।
