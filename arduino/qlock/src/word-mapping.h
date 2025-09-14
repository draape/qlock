#ifndef WORD_MAPPING_H
#define WORD_MAPPING_H

class WordMapping
{
public:
  WordMapping();

  // Get LED indices for specific words
  int *getKlokken() const { return klokken_; }
  int *getEr() const { return er_; }
  int *getFemIndicator() const { return femIndicator_; }
  int *getKvart() const { return kvart_; }
  int *getPaa1() const { return paa1_; }
  int *getPaa2() const { return paa2_; }
  int *getPaa3() const { return paa3_; }
  int *getHalv() const { return halv_; }
  int *getTi() const { return ti_; }
  int *getOver() const { return over_; }

  // Numbers 1-12
  int *getEtt() const { return ett_; }       // 1
  int *getTo() const { return to_; }         // 2
  int *getTre() const { return tre_; }       // 3
  int *getFire() const { return fire_; }     // 4
  int *getFem() const { return fem_; }       // 5
  int *getSeks() const { return seks_; }     // 6
  int *getSyv() const { return syv_; }       // 7
  int *getAatte() const { return aatte_; }   // 8 (åtte)
  int *getNi() const { return ni_; }         // 9
  int *getTiNum() const { return ti_num_; }  // 10 (different "ti" than minutes)
  int *getElleve() const { return elleve_; } // 11
  int *getTolv() const { return tolv_; }     // 12

  // Helper function to get word by hour number (1-12)
  int *getHourWord(int hour) const;

  int getMinuteOne() const { return minuteOne_; }     // Top left corner
  int getMinuteTwo() const { return minuteTwo_; }     // Top right corner
  int getMinuteThree() const { return minuteThree_; } // Bottom right corner
  int getMinuteFour() const { return minuteFour_; }   // Bottom left corner

private:
  // Word positions based on LED strip flow
  int *klokken_;      // "KLOKKEN"
  int *er_;           // "ER"
  int *femIndicator_; // "FEM"
  int *kvart_;        // "KVART"
  int *paa1_;         // "PÅ"
  int *paa2_;         // "PÅ"
  int *paa3_;         // "PÅ"
  int *halv_;         // "HALV"
  int *ti_;           // "TI" (minutes)
  int *over_;         // "OVER"
  int *ett_;          // "ETT"
  int *to_;           // "TO"
  int *tre_;          // "TRE"
  int *fire_;         // "FIRE"
  int *fem_;          // "FEM"
  int *seks_;         // "SEKS"
  int *syv_;          // "SYV"
  int *aatte_;        // "ÅTTE"
  int *ni_;           // "NI"
  int *ti_num_;       // "TI" (number 10)
  int *elleve_;       // "ELLEVE"
  int *tolv_;         // "TOLV"
  int minuteOne_;     // Top left corner
  int minuteTwo_;     // Top right corner
  int minuteThree_;   // Bottom right corner
  int minuteFour_;    // Bottom left corner
};

#endif