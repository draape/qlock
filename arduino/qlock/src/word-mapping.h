#ifndef WORD_MAPPING_H
#define WORD_MAPPING_H

// Norwegian Word Clock Mapping
// Grid is 11x11 = 121 LEDs total (LED strip flows as shown in image)

struct WordMap
{
  int startIndex;
  int length;

  // Helper to get all indices for this word
  void getIndices(int *indices) const
  {
    for (int i = 0; i < length; i++)
    {
      indices[i] = startIndex + i;
    }
  }
};

class WordMapping
{
public:
  WordMapping();

  // Get LED indices for specific words
  WordMap getKlokken() const { return klokken_; }
  WordMap getEr() const { return er_; }
  WordMap getKvart() const { return kvart_; }
  WordMap getPaa() const { return paa_; } // "på"
  WordMap getHalv() const { return halv_; }
  WordMap getTi() const { return ti_; } // "ti" (number/minutes)
  WordMap getOver() const { return over_; }

  // Numbers 1-12
  WordMap getEtt() const { return ett_; }       // 1
  WordMap getTo() const { return to_; }         // 2
  WordMap getTre() const { return tre_; }       // 3
  WordMap getFire() const { return fire_; }     // 4
  WordMap getFem() const { return fem_; }       // 5
  WordMap getSeks() const { return seks_; }     // 6
  WordMap getSyv() const { return syv_; }       // 7
  WordMap getAatte() const { return aatte_; }   // 8 (åtte)
  WordMap getNi() const { return ni_; }         // 9
  WordMap getTiNum() const { return ti_num_; }  // 10 (different "ti" than minutes)
  WordMap getElleve() const { return elleve_; } // 11
  WordMap getTolv() const { return tolv_; }     // 12

  // Helper function to get word by hour number (1-12)
  WordMap getHourWord(int hour) const;

  // Corner minute indicators
  int getMinuteOne() const { return minuteOne_; }     // Top left corner
  int getMinuteTwo() const { return minuteTwo_; }     // Top right corner
  int getMinuteThree() const { return minuteThree_; } // Bottom right corner
  int getMinuteFour() const { return minuteFour_; }   // Bottom left corner

  // Helper to get minute corner indices based on minute value (1-4)
  void getMinuteCorners(int minutes, int *cornerIndices, int *count) const;

private:
  // Word positions based on LED strip flow
  WordMap klokken_; // "KLOKKEN"
  WordMap er_;      // "ER"
  WordMap kvart_;   // "KVART"
  WordMap paa_;     // "PÅ"
  WordMap halv_;    // "HALV"
  WordMap ti_;      // "TI" (minutes)
  WordMap over_;    // "OVER"
  WordMap ett_;     // "ETT"
  WordMap to_;      // "TO"
  WordMap tre_;     // "TRE"
  WordMap fire_;    // "FIRE"
  WordMap fem_;     // "FEM"
  WordMap seks_;    // "SEKS"
  WordMap syv_;     // "SYV"
  WordMap aatte_;   // "ÅTTE"
  WordMap ni_;      // "NI"
  WordMap ti_num_;  // "TI" (number 10)
  WordMap elleve_;  // "ELLEVE"
  WordMap tolv_;    // "TOLV"

  // Corner minute indicators (single LEDs)
  int minuteOne_;   // Top left corner
  int minuteTwo_;   // Top right corner
  int minuteThree_; // Bottom right corner
  int minuteFour_;  // Bottom left corner
};

#endif