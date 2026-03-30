#pragma once

#include <functional>

struct Key {
  using Callback = std::function<void(const double)>;
  // using Callback = void (*)(const double);
  // static constexpr Callback NONE = [](const double) {};

  bool on = false;
  bool justUsed = false;

  Callback justOn, holdOn, justOff;

  Key(const Callback justOn, const Callback holdOn, const Callback justOff);
  Key(const Callback holdOn, const Callback holdOff = [](const double) {});

  void react(const int action);

  void operator()(const double dt);
};