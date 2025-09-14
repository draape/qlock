#include "word-mapping.h"

WordMapping::WordMapping()
{
  klokken_ = new int[10]{11, 13, 32, 33, 52, 53, 72, 92, 93, -1};
  // er_ = new int[3]{92, 93, -1};

  femIndicator_ = new int[4]{10, 14, 31, -1};
  paa1_ = new int[3]{51, 54, -1};

  ti_ = new int[3]{9, 15, -1};
  paa2_ = new int[3]{35, 50, -1};

  kvart_ = new int[6]{8, 16, 29, 36, 49, -1};
  paa3_ = new int[3]{69, 76, -1};

  over_ = new int[5]{7, 17, 28, 37, -1};

  halv_ = new int[5]{6, 18, 27, 38, -1};

  ett_ = new int[4]{5, 19, 26, -1};
  to_ = new int[3]{46, 59, -1};
  tre_ = new int[4]{79, 86, 99, -1};

  fire_ = new int[5]{4, 20, 25, 40, -1};
  fem_ = new int[4]{45, 60, 65, -1};
  seks_ = new int[5]{80, 85, 100, 106, -1};

  syv_ = new int[4]{3, 21, 24, -1};
  aatte_ = new int[5]{41, 44, 61, 64, -1};
  ni_ = new int[3]{81, 84, -1};
  ti_num_ = new int[3]{101, 105, -1};

  elleve_ = new int[7]{2, 22, 23, 42, 43, 62, -1};
  tolv_ = new int[5]{82, 83, 102, 104, -1};

  // Corner minute indicators
  minuteOne_ = 12;    // Top left corner
  minuteTwo_ = 114;   // Top right corner
  minuteThree_ = 103; // Bottom right corner
  minuteFour_ = 1;    // Bottom left corner
}

int *WordMapping::getHourWord(int hour) const
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

// void WordMapping::getMinuteCorners(int minutes, int *cornerIndices, int *count) const
// {
//   *count = minutes % 5; // Minutes 1-4 show corners, 0 and 5+ show no corners

//   if (*count > 0 && *count <= 4)
//   {
//     // Light up corners in order: top-left, top-right, bottom-right, bottom-left
//     if (*count >= 1)
//       cornerIndices[0] = minuteOne_;
//     if (*count >= 2)
//       cornerIndices[1] = minuteTwo_;
//     if (*count >= 3)
//       cornerIndices[2] = minuteThree_;
//     if (*count >= 4)
//       cornerIndices[3] = minuteFour_;
//   }
// }