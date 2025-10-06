#pragma once


#define PY_CORE_ASSERT(x, ...) { if(!(x)) { PY_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }

