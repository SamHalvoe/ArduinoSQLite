#pragma once

#include <Arduino.h>

namespace halvoe::memoryInfo
{
  // ---- memory infos about various memory regions ----
  // based on https://forum.pjrc.com/index.php?threads/memory-usage-teensy-4-1.72235/ 
  
  constexpr uint32_t getRamStart();
  constexpr uint32_t getRamSize();
  constexpr uint32_t getRamEnd();

  constexpr uint32_t getFlashStart();
  constexpr uint32_t getFlashSize();
  constexpr uint32_t getFlashEnd();

  uint32_t getCodeStart();
  uint32_t getCodeEnd();
  uint32_t getCodeInBytes();

  uint32_t getInitialisedDataStart();
  uint32_t getInitialisedDataEnd();
  uint32_t getInitialisedDataInBytes();

  uint32_t getUninitialisedDataStart();
  uint32_t getUninitialisedDataEnd();
  uint32_t getUninitialisedDataInBytes();

  uint32_t getStackPointer();
  uint32_t getAvailableStackInBytes();
  uint32_t getStackEnd();
  uint32_t getUsedStackInBytes();

  uint32_t getHeapPointer();
  uint32_t getHeapStart();
  uint32_t getHeapEnd();
  uint32_t getAvailableHeapInBytes();
  uint32_t getUsedHeapInBytes();

  uint32_t getItcmStart();
  uint32_t getItcmEnd();
  uint32_t getDtcmStart();
  uint32_t getDtcmEnd();

#if ARDUINO_TEENSY41
  uint32_t getPsramPointer();
  uint32_t getPsramStart();
  uint32_t getPsramEnd();
  uint32_t getAvailablePsramInBytes();
  uint32_t getUsedPsramInBytes();
#endif
  
  // ---- memory infos about variables, arrays and functions ----
  // based on memoryTool from https://github.com/luni64/TeensyHelpers
  
  struct ElementInfo
  {
    public:
      String m_name;
      String m_location;
      uint32_t m_size;
      uintptr_t m_startAddress;
      uintptr_t m_endAddress;
  };
  
  namespace implementation // do not use in user code
  {
    const ElementInfo getElememtInfo(const char* in_name, const void* in_begin, uint32_t in_elementSize, uint32_t in_elementCount);
  }
  
  template<typename Type>
  const ElementInfo getVariableInfo(const char* in_name, Type& in_variable)
  {
    return implementation::getElememtInfo(in_name, static_cast<void*>(&in_variable), sizeof(Type), 1);
  }
  
  template<typename Type, size_t tc_size>
  const ElementInfo getVariableInfo(const char* in_name, Type (&in_array)[tc_size])
  {
    return implementation::getElememtInfo(in_name, &in_array, sizeof(Type), tc_size);
  }
  
  template<typename Type>
  const ElementInfo getFunctionInfo(const char* in_name, Type& in_function)
  {
    return implementation::getElememtInfo(in_name, static_cast<void*>(&in_function), 0, 0);
  }
}

#define halvoe_getVariableInfo(in_element) halvoe::memoryInfo::getVariableInfo(#in_element, (in_element));
#define halvoe_getFunctionInfo(in_element) halvoe::memoryInfo::getFunctionInfo(#in_element, (in_element));
