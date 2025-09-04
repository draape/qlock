#include "word-mapping.h"

WordMapping::WordMapping()
{
  // Based on the LED strip flow shown in the image
  // Grid is 11x11, LEDs flow: right→left on odd rows, left→right on even rows
  // Row 0: 0-10 (right to left)
  // Row 1: 11-21 (left to right)
  // Row 2: 22-32 (right to left)
  // etc.

  // Row 0: K L O K K E N V E R M (indices 0-10, right to left flow)
  klokken_ = {4, 6}; // KLOKKEN (positions 4-9 in reverse = LED indices 1-6)

  // Row 1: F E M H P Å S U E T S (indices 11-21, left to right flow)
  fem_ = {11, 3}; // FEM (positions 0-2 = LED indices 11-13)

  // Row 2: T I L P Å S I D O S N (indices 22-32, right to left flow)
  ti_ = {31, 2};  // TI (positions 0-1 in reverse = LED indices 31-32)
  paa_ = {27, 2}; // PÅ (positions 4-5 in reverse = LED indices 27-28)

  // Row 3: K V A R T N P Å S T O (indices 33-43, left to right flow)
  kvart_ = {33, 5}; // KVART (positions 0-4 = LED indices 33-37)

  // Row 4: O V E R X A M B P M Z (indices 44-54, right to left flow)
  over_ = {51, 4}; // OVER (positions 0-3 in reverse = LED indices 51-54)

  // Row 5: H A L V B I E G E N Z (indices 55-65, left to right flow)
  halv_ = {55, 4}; // HALV (positions 0-3 = LED indices 55-58)

  // Row 6: E T T N T O A T R E X (indices 66-76, right to left flow)
  ett_ = {74, 3}; // ETT (positions 0-2 in reverse = LED indices 74-76)
  to_ = {71, 2};  // TO (positions 5-6 in reverse = LED indices 70-71)
  tre_ = {67, 3}; // TRE (positions 8-10 in reverse = LED indices 66-68)

  // Row 7: F I R E F E M S E K S (indices 77-87, left to right flow)
  fire_ = {77, 4}; // FIRE (positions 0-3 = LED indices 77-80)
  seks_ = {84, 4}; // SEKS (positions 7-10 = LED indices 84-87)

  // Row 8: S Y V Å T T E N I T I (indices 88-98, right to left flow)
  syv_ = {96, 3};    // SYV (positions 0-2 in reverse = LED indices 96-98)
  aatte_ = {92, 4};  // ÅTTE (positions 4-7 in reverse = LED indices 91-94)
  ni_ = {89, 2};     // NI (positions 8-9 in reverse = LED indices 88-89)
  ti_num_ = {89, 2}; // TI (10) - same as NI position, different context

  // Row 9: E L E V E S T O L V (indices 99-109, left to right flow)
  elleve_ = {99, 6}; // ELLEVE (positions 0-5 = LED indices 99-104)
  tolv_ = {106, 4};  // TOLV (positions 6-9 = LED indices 106-109)

  // Corner minute indicators
  // Based on 11x11 grid with serpentine flow
  minuteOne_ = 12;    // Top left corner (start of LED strip)
  minuteTwo_ = 114;   // Top right corner (end of first row)
  minuteThree_ = 103; // Bottom right corner (end of LED strip, assuming 121 total LEDs)
  minuteFour_ = 1;    // Bottom left corner (start of last row)

  // Note: Adjust these indices based on your actual LED count and corner positions
}

WordMap WordMapping::getHourWord(int hour) const
{
  // Convert 24-hour to 12-hour and handle special cases
  hour = hour % 12;
  if (hour == 0)
    hour = 12;

  switch (hour)
  {
  case 1:
    return ett_;
  case 2:
    return to_;
  case 3:
    return tre_;
  case 4:
    return fire_;
  case 5:
    return fem_;
  case 6:
    return seks_;
  case 7:
    return syv_;
  case 8:
    return aatte_;
  case 9:
    return ni_;
  case 10:
    return ti_num_;
  case 11:
    return elleve_;
  case 12:
    return tolv_;
  default:
    return ett_; // fallback
  }
}

void WordMapping::getMinuteCorners(int minutes, int *cornerIndices, int *count) const
{
  *count = minutes % 5; // Minutes 1-4 show corners, 0 and 5+ show no corners

  if (*count > 0 && *count <= 4)
  {
    // Light up corners in order: top-left, top-right, bottom-right, bottom-left
    if (*count >= 1)
      cornerIndices[0] = minuteOne_;
    if (*count >= 2)
      cornerIndices[1] = minuteTwo_;
    if (*count >= 3)
      cornerIndices[2] = minuteThree_;
    if (*count >= 4)
      cornerIndices[3] = minuteFour_;
  }
}