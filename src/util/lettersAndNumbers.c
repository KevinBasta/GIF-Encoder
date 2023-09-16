
#include <stdint.h>
#include "main.h"

#include "lettersAndNumbers.h"

// Grand9K Pixel Font
u8 zero[] =
 {0,0,0,0,0,0,0,
  0,0,1,1,1,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,1,1,0,
  0,1,0,1,0,1,0,
  0,1,1,0,0,1,0,
  0,1,0,0,0,1,0,
  0,0,1,1,1,0,0,
  0,0,0,0,0,0,0};
letterPattern zeroPattern = {7, 9, 1, zero};

u8 one[] = 
 {0,0,0,0,0,
  0,1,1,0,0,
  0,0,1,0,0,
  0,0,1,0,0,
  0,0,1,0,0,
  0,0,1,0,0,
  0,0,1,0,0,
  0,0,1,0,0,
  0,0,0,0,0};
letterPattern onePattern = {5, 9, 1, one};

u8 two[] = 
 {0,0,0,0,0,0,0,
  0,1,1,1,1,0,0,
  0,0,0,0,0,1,0,
  0,0,0,0,0,1,0,
  0,0,1,1,1,0,0,
  0,1,0,0,0,0,0,
  0,1,0,0,0,0,0,
  0,1,1,1,1,1,0,
  0,0,0,0,0,0,0};
letterPattern twoPattern = {7, 9, 1, two};

u8 three[] = 
 {0,0,0,0,0,0,
  0,1,1,1,0,0,
  0,0,0,0,1,0,
  0,0,0,0,1,0,
  0,1,1,1,0,0,
  0,0,0,0,1,0,
  0,0,0,0,1,0,
  0,1,1,1,0,0,
  0,0,0,0,0,0};
letterPattern threePattern = {6, 9, 1, three};

u8 four[] = 
 {0,0,0,0,0,0,0,
  0,0,0,0,1,1,0,
  0,0,0,1,0,1,0,
  0,0,1,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,1,1,1,1,0,
  0,0,0,0,0,1,0,
  0,0,0,0,0,1,0,
  0,0,0,0,0,0,0};
letterPattern fourPattern = {7, 9, 1, four};

u8 five[] = 
 {0,0,0,0,0,0,0,
  0,1,1,1,1,1,0,
  0,1,0,0,0,0,0,
  0,1,0,0,0,0,0,
  0,1,1,1,1,0,0,
  0,0,0,0,0,1,0,
  0,0,0,0,0,1,0,
  0,1,1,1,1,0,0,
  0,0,0,0,0,0,0};
letterPattern fivePattern = {7, 9, 1, five};

u8 six[] = 
 {0,0,0,0,0,0,0,
  0,0,1,1,1,0,0,
  0,1,0,0,0,0,0,
  0,1,0,0,0,0,0,
  0,1,1,1,1,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,0,1,1,1,0,0,
  0,0,0,0,0,0,0};
letterPattern sixPattern = {7, 9, 1, six};

u8 seven[] = 
 {0,0,0,0,0,0,0,
  0,1,1,1,1,1,0,
  0,0,0,0,0,1,0,
  0,0,0,0,1,0,0,
  0,0,0,0,1,0,0,
  0,0,0,1,0,0,0,
  0,0,0,1,0,0,0,
  0,0,0,1,0,0,0,
  0,0,0,0,0,0,0};
letterPattern sevenPattern = {7, 9, 1, seven};

u8 eight[] = 
 {0,0,0,0,0,0,0,
  0,0,1,1,1,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,0,1,1,1,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,0,1,1,1,0,0,
  0,0,0,0,0,0,0};
letterPattern eightPattern = {7, 9, 1, eight};

u8 nine[] = 
 {0,0,0,0,0,0,0,
  0,0,1,1,1,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,0,1,1,1,1,0,
  0,0,0,0,0,1,0,
  0,0,0,0,0,1,0,
  0,0,1,1,1,0,0,
  0,0,0,0,0,0,0};
letterPattern ninePattern = {7, 9, 1, nine};

u8 space[] = 
 {0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,0,0,0,0,0};
letterPattern spacePattern = {6, 9, 0, space};

u8 underscoreMark[] = 
 {0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,1,1,1,1,1,1,0,
  0,0,0,0,0,0,0,0};
letterPattern underscorePattern = {8, 9, 0, underscoreMark};

u8 underscoreClearMark[] = 
 {0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0};
letterPattern underscoreClearPattern = {8, 9, 0, underscoreClearMark};

u8 exclamationMark[] = 
 {0,0,0,
  0,1,0,
  0,1,0,
  0,1,0,
  0,1,0,
  0,1,0,
  0,0,0,
  0,1,0,
  0,0,0};
letterPattern exclamationPattern = {3, 9, 1, exclamationMark};

u8 sharpMark[] = 
 {0,0,0,0,0,0,0,0,
  0,0,1,0,0,1,0,0,
  0,1,1,1,1,1,1,0,
  0,0,1,0,0,1,0,0,
  0,0,1,0,0,1,0,0,
  0,0,1,0,0,1,0,0,
  0,1,1,1,1,1,1,0,
  0,0,1,0,0,1,0,0,
  0,0,0,0,0,0,0,0};
letterPattern sharpPattern = {8, 9, 1, sharpMark};

u8 dollarMark[] = 
 {0,0,0,0,0,0,0,
  0,0,0,1,0,0,0,
  0,0,1,1,1,1,0,
  0,1,0,1,0,0,0,
  0,0,1,1,1,0,0,
  0,0,0,1,0,1,0,
  0,1,1,1,1,0,0,
  0,0,0,1,0,0,0,
  0,0,0,0,0,0,0};
letterPattern dollarPattern = {7, 9, 1, dollarMark};

u8 colonMark[] = 
 {0,0,0,
  0,0,0,
  0,0,0,
  0,1,0,
  0,0,0,
  0,0,0,
  0,0,0,
  0,1,0,
  0,0,0};
letterPattern colonPattern = {3, 9, 1, colonMark};

u8 semicolonMark[] = 
 {0,0,0,
  0,0,0,
  0,1,0,
  0,0,0,
  0,0,0,
  0,0,0,
  0,1,0,
  0,1,0,
  0,0,0};
letterPattern semicolonPattern = {3, 9, 0, semicolonMark};

u8 commaMark[] = 
 {0,0,0,
  0,0,0,
  0,0,0,
  0,0,0,
  0,0,0,
  0,0,0,
  0,1,0,
  0,1,0,
  0,0,0};
letterPattern commaPattern = {3, 9, 0, commaMark};

u8 dotMark[] = 
 {0,0,0,
  0,0,0,
  0,0,0,
  0,0,0,
  0,0,0,
  0,0,0,
  0,0,0,
  0,1,0,
  0,0,0};
letterPattern dotPattern = {3, 9, 1, dotMark};

u8 singleQuoteMark[] = 
 {0,0,0,
  0,1,0,
  0,1,0,
  0,0,0,
  0,0,0,
  0,0,0,
  0,0,0,
  0,0,0,
  0,0,0};
letterPattern singleQuotePattern = {3, 9, 1, singleQuoteMark};

u8 doubleQuoteMark[] = 
 {0,0,0,0,0,
  0,1,0,1,0,
  0,1,0,1,0,
  0,0,0,0,0,
  0,0,0,0,0,
  0,0,0,0,0,
  0,0,0,0,0,
  0,0,0,0,0,
  0,0,0,0,0};
letterPattern doubleQuotePattern = {5, 9, 1, doubleQuoteMark};

u8 forwardSlashMark[] = 
 {0,0,0,0,0,
  0,0,0,1,0,
  0,0,0,1,0,
  0,0,1,0,0,
  0,0,1,0,0,
  0,0,1,0,0,
  0,1,0,0,0,
  0,1,0,0,0,
  0,0,0,0,0};
letterPattern forwardSlashPattern = {5, 9, 1, forwardSlashMark};

u8 backwardSlashMark[] = 
 {0,0,0,0,0,
  0,1,0,0,0,
  0,1,0,0,0,
  0,0,1,0,0,
  0,0,1,0,0,
  0,0,1,0,0,
  0,0,0,1,0,
  0,0,0,1,0,
  0,0,0,0,0};
letterPattern backwardSlashPattern = {5, 9, 1, backwardSlashMark};

u8 questionMark[] = 
 {0,0,0,0,0,0,
  0,1,1,1,0,0,
  0,0,0,0,1,0,
  0,0,0,0,1,0,
  0,0,0,1,0,0,
  0,0,1,0,0,0,
  0,0,0,0,0,0,
  0,0,1,0,0,0,
  0,0,0,0,0,0};
letterPattern questionPattern = {6, 9, 1, questionMark};

u8 percentMark[] = 
 {0,0,0,0,0,0,0,0,0,0,
  0,0,1,0,0,0,0,1,0,0,
  0,1,0,1,0,0,1,0,0,0,
  0,1,0,1,0,1,0,0,0,0,
  0,0,1,0,1,0,1,0,0,0,
  0,0,0,1,0,1,0,1,0,0,
  0,0,1,0,0,1,0,1,0,0,
  0,1,0,0,0,0,1,0,0,0,
  0,0,0,0,0,0,0,0,0,0};
letterPattern percentPattern = {10, 9, 1, percentMark};

u8 openRoundBracketMark[] = 
 {0,0,0,0,0,
  0,0,1,1,0,
  0,1,0,0,0,
  0,1,0,0,0,
  0,1,0,0,0,
  0,1,0,0,0,
  0,1,0,0,0,
  0,0,1,1,0,
  0,0,0,0,0};
letterPattern openRoundPattern = {5, 9, 1, openRoundBracketMark};

u8 closeRoundBracketMark[] = 
 {0,0,0,0,0,
  0,1,1,0,0,
  0,0,0,1,0,
  0,0,0,1,0,
  0,0,0,1,0,
  0,0,0,1,0,
  0,0,0,1,0,
  0,1,1,0,0,
  0,0,0,0,0};
letterPattern closeRoundPattern = {5, 9, 1, closeRoundBracketMark};

u8 openSquareBracketMark[] = 
 {0,0,0,0,0,
  0,1,1,1,0,
  0,1,0,0,0,
  0,1,0,0,0,
  0,1,0,0,0,
  0,1,0,0,0,
  0,1,0,0,0,
  0,1,1,1,0,
  0,0,0,0,0};
letterPattern openSquarePattern = {5, 9, 1, openSquareBracketMark};

u8 closeSquareBracketMark[] = 
 {0,0,0,0,0,
  0,1,1,1,0,
  0,0,0,1,0,
  0,0,0,1,0,
  0,0,0,1,0,
  0,0,0,1,0,
  0,0,0,1,0,
  0,1,1,1,0,
  0,0,0,0,0};
letterPattern closeSquarePattern = {5, 9, 1, closeSquareBracketMark};

u8 atMark[] = 
 {0,0,0,0,0,0,0,0,0,
  0,0,1,1,1,1,1,0,0,
  0,1,0,0,0,0,0,1,0,
  0,1,0,0,1,1,0,1,0,
  0,1,0,1,0,1,0,1,0,
  0,1,0,1,0,1,0,1,0,
  0,1,0,0,1,1,1,0,0,
  0,1,0,0,0,0,0,0,0,
  0,0,1,1,1,1,1,0,0,
  0,0,0,0,0,0,0,0,0};
letterPattern atPattern = {9, 10, 1, atMark};


u8 A[] =
 {0,0,0,0,0,0,0,
  0,0,1,1,1,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,1,1,1,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,0,0,0,0,0,0,};
letterPattern APattern = {7, 9, 1, A};

u8 a[] =
 {0,0,0,0,0,0,
  0,0,1,1,0,0,
  0,0,0,0,1,0,
  0,0,1,1,1,0,
  0,1,0,0,1,0,
  0,0,1,1,1,0,
  0,0,0,0,0,0,};
letterPattern aPattern = {6, 7, 1, a};

u8 B[] =
 {0,0,0,0,0,0,0,
  0,1,1,1,1,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,1,1,1,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,1,1,1,0,0,
  0,0,0,0,0,0,0,};
letterPattern BPattern = {7, 9, 1, B};

u8 b[] =
 {0,0,0,0,0,0,
  0,1,0,0,0,0,
  0,1,0,0,0,0,
  0,1,1,1,0,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,1,1,1,0,0,
  0,0,0,0,0,0};
letterPattern bPattern = {6, 9, 1, b};

u8 C[] =
 {0,0,0,0,0,0,
  0,0,1,1,1,0,
  0,1,0,0,0,0,
  0,1,0,0,0,0,
  0,1,0,0,0,0,
  0,1,0,0,0,0,
  0,1,0,0,0,0,
  0,0,1,1,1,0,
  0,0,0,0,0,0};
letterPattern CPattern = {6, 9, 1, C};

u8 c[] =
 {0,0,0,0,0,
  0,0,0,0,0,
  0,0,1,1,0,
  0,1,0,0,0,
  0,1,0,0,0,
  0,1,0,0,0,
  0,0,1,1,0,
  0,0,0,0,0,};
letterPattern cPattern = {5, 8, 1, c};


u8 D[] =
 {0,0,0,0,0,0,0,
  0,1,1,1,1,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,1,1,1,0,0,
  0,0,0,0,0,0,0};
letterPattern DPattern = {7, 9, 1, D};

u8 d[] =
 {0,0,0,0,0,0,
  0,0,0,0,1,0,
  0,0,0,0,1,0,
  0,0,1,1,1,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,0,1,1,1,0,
  0,0,0,0,0,0};
letterPattern dPattern = {6, 9, 1, d};

u8 E[] =
 {0,0,0,0,0,0,
  0,1,1,1,1,0,
  0,1,0,0,0,0,
  0,1,0,0,0,0,
  0,1,1,1,1,0,
  0,1,0,0,0,0,
  0,1,0,0,0,0,
  0,1,1,1,1,0,
  0,0,0,0,0,0};
letterPattern EPattern = {6, 9, 1, E};

u8 e[] =
 {0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,0,1,1,0,0,
  0,1,0,0,1,0,
  0,1,1,1,1,0,
  0,1,0,0,0,0,
  0,0,1,1,1,0,
  0,0,0,0,0,0};
letterPattern ePattern = {6, 8, 1, e};

u8 F[] =
 {0,0,0,0,0,0,
  0,1,1,1,1,0,
  0,1,0,0,0,0,
  0,1,0,0,0,0,
  0,1,1,1,1,0,
  0,1,0,0,0,0,
  0,1,0,0,0,0,
  0,1,0,0,0,0,
  0,0,0,0,0,0};
letterPattern FPattern = {6, 9, 1, F};

u8 f[] =
 {0,0,0,0,0,0,
  0,0,0,1,1,0,
  0,0,1,0,0,0,
  0,1,1,1,1,0,
  0,0,1,0,0,0,
  0,0,1,0,0,0,
  0,0,1,0,0,0,
  0,0,1,0,0,0,
  0,0,0,0,0,0};
letterPattern fPattern = {6, 9, 1, f};

u8 G[] =
 {0,0,0,0,0,0,0,
  0,0,1,1,1,1,0,
  0,1,0,0,0,0,0,
  0,1,0,0,0,0,0,
  0,1,0,0,1,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,0,1,1,1,1,0,
  0,0,0,0,0,0,0,};
letterPattern GPattern = {7, 9, 1, G};

u8 g[] =
 {0,0,0,0,0,0,
  0,0,1,1,1,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,0,1,1,1,0,
  0,0,0,0,1,0,
  0,1,1,1,0,0,
  0,0,0,0,0,0};
letterPattern gPattern = {6, 8, 0, g};

u8 H[] =
 {0,0,0,0,0,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,1,1,1,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,0,0,0,0,0,0};
letterPattern HPattern = {7, 9, 1, H};

u8 h[] =
 {0,0,0,0,0,0,
  0,1,0,0,0,0,
  0,1,0,0,0,0,
  0,1,1,1,0,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,0,0,0,0,0};
letterPattern hPattern = {6, 9, 1, h};

u8 I[] =
 {0,0,0,
  0,1,0,
  0,1,0,
  0,1,0,
  0,1,0,
  0,1,0,
  0,1,0,
  0,1,0,
  0,0,0};
letterPattern IPattern = {3, 9, 1, I};

u8 i[] =
 {0,0,0,
  0,1,0,
  0,0,0,
  0,1,0,
  0,1,0,
  0,1,0,
  0,1,0,
  0,1,0,
  0,0,0};
letterPattern iPattern = {3, 9, 1, i};

u8 J[] =
 {0,0,0,0,0,0,0,
  0,0,0,0,0,1,0,
  0,0,0,0,0,1,0,
  0,0,0,0,0,1,0,
  0,0,0,0,0,1,0,
  0,0,0,0,0,1,0,
  0,0,0,0,0,1,0,
  0,1,1,1,1,0,0,
  0,0,0,0,0,0,0};
letterPattern JPattern = {7, 9, 1, J};

u8 j[] =
 {0,0,0,0,0,
  0,0,0,1,0,
  0,0,0,0,0,
  0,0,0,1,0,
  0,0,0,1,0,
  0,0,0,1,0,
  0,0,0,1,0,
  0,0,0,1,0,
  0,1,1,0,0,
  0,0,0,0,0};
letterPattern jPattern = {5, 10, 0, j};

u8 K[] =
 {0,0,0,0,0,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,1,0,0,
  0,1,0,1,0,0,0,
  0,1,1,0,0,0,0,
  0,1,0,1,0,0,0,
  0,1,0,0,1,0,0,
  0,1,0,0,0,1,0,
  0,0,0,0,0,0,0};
letterPattern KPattern = {7, 9, 1, K};

u8 k[] =
 {0,0,0,0,0,0,
  0,1,0,0,0,0,
  0,1,0,0,0,0,
  0,1,0,0,1,0,
  0,1,0,1,0,0,
  0,1,1,0,0,0,
  0,1,0,1,0,0,
  0,1,0,0,1,0,
  0,0,0,0,0,0};
letterPattern kPattern = {6, 9, 1, k};

u8 L[] =
 {0,0,0,0,0,0,0,
  0,1,0,0,0,0,0,
  0,1,0,0,0,0,0,
  0,1,0,0,0,0,0,
  0,1,0,0,0,0,0,
  0,1,0,0,0,0,0,
  0,1,0,0,0,0,0,
  0,1,1,1,1,1,0,
  0,0,0,0,0,0,0};
letterPattern LPattern = {7, 9, 1, L};

u8 l[] =
 {0,0,0,
  0,1,0,
  0,1,0,
  0,1,0,
  0,1,0,
  0,1,0,
  0,1,0,
  0,1,0,
  0,0,0};
letterPattern lPattern = {3, 9, 1, l};

u8 M[] =
 {0,0,0,0,0,0,0,
  0,1,0,0,0,1,0,
  0,1,1,0,1,1,0,
  0,1,0,1,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,0,0,0,0,0,0};
letterPattern MPattern = {7, 9, 1, M};

u8 m[] =
 {0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,
  0,1,1,0,1,0,0,
  0,1,0,1,0,1,0,
  0,1,0,1,0,1,0,
  0,1,0,1,0,1,0,
  0,1,0,0,0,1,0,
  0,0,0,0,0,0,0};
letterPattern mPattern = {7, 8, 1, m};

u8 N[] =
 {0,0,0,0,0,0,0,
  0,1,0,0,0,1,0,
  0,1,1,0,0,1,0,
  0,1,0,1,0,1,0,
  0,1,0,0,1,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,0,0,0,0,0,0};
letterPattern NPattern = {7, 9, 1, N};

u8 n[] =
 {0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,1,1,1,0,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,0,0,0,0,0};
letterPattern nPattern = {6, 8, 1, n};

u8 O[] =
 {0,0,0,0,0,0,0,
  0,0,1,1,1,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,0,1,1,1,0,0,
  0,0,0,0,0,0,0};
letterPattern OPattern = {7, 9, 1, O};

u8 o[] =
 {0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,0,1,1,0,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,0,1,1,0,0,
  0,0,0,0,0,0};
letterPattern oPattern = {6, 8, 1, o};

u8 P[] =
 {0,0,0,0,0,0,0,
  0,1,1,1,1,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,1,1,1,0,0,
  0,1,0,0,0,0,0,
  0,1,0,0,0,0,0,
  0,0,0,0,0,0,0};
letterPattern PPattern = {7, 9, 1, P};

u8 p[] =
 {0,0,0,0,0,0,
  0,1,1,1,0,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,1,1,1,0,0,
  0,1,0,0,0,0,
  0,0,0,0,0,0};
letterPattern pPattern = {6, 8, 0, p};

u8 Q[] =
 {0,0,0,0,0,0,0,0,
  0,0,1,1,1,0,0,0,
  0,1,0,0,0,1,0,0,
  0,1,0,0,0,1,0,0,
  0,1,0,0,0,1,0,0,
  0,1,0,0,0,1,0,0,
  0,1,0,0,0,1,0,0,
  0,0,1,1,1,0,0,0,
  0,0,0,0,0,1,1,0,
  0,0,0,0,0,0,0,0,};
letterPattern QPattern = {8, 10, 0, Q};

u8 q[] =
 {0,0,0,0,0,0,
  0,0,1,1,1,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,0,1,1,1,0,
  0,0,0,0,1,0,
  0,0,0,0,0,0};
letterPattern qPattern = {6, 8, 0, q};

u8 R[] =
 {0,0,0,0,0,0,0,
  0,1,1,1,1,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,1,1,1,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,0,0,0,0,0,0,};
letterPattern RPattern = {7, 9, 1, R};

u8 r[] =
 {0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,1,0,1,1,0,
  0,1,1,0,0,0,
  0,1,0,0,0,0,
  0,1,0,0,0,0,
  0,1,0,0,0,0,
  0,0,0,0,0,0};
letterPattern rPattern = {6, 8, 1, r};

u8 S[] =
 {0,0,0,0,0,0,0,
  0,0,1,1,1,1,0,
  0,1,0,0,0,0,0,
  0,1,0,0,0,0,0,
  0,0,1,1,1,0,0,
  0,0,0,0,0,1,0,
  0,0,0,0,0,1,0,
  0,1,1,1,1,0,0,
  0,0,0,0,0,0,0,};
letterPattern SPattern = {7, 9, 1, S};

u8 s[] =
 {0,0,0,0,0,0,
  0,0,1,1,1,0,
  0,1,0,0,0,0,
  0,0,1,1,0,0,
  0,0,0,0,1,0,
  0,1,1,1,0,0,
  0,0,0,0,0,0};
letterPattern sPattern = {6, 7, 1, s};

u8 T[] =
 {0,0,0,0,0,0,0,
  0,1,1,1,1,1,0,
  0,0,0,1,0,0,0,
  0,0,0,1,0,0,0,
  0,0,0,1,0,0,0,
  0,0,0,1,0,0,0,
  0,0,0,1,0,0,0,
  0,0,0,1,0,0,0,
  0,0,0,0,0,0,0};
letterPattern TPattern = {7, 9, 1, T};

u8 t[] =
 {0,0,0,0,0,0,
  0,0,1,0,0,0,
  0,0,1,0,0,0,
  0,1,1,1,1,0,
  0,0,1,0,0,0,
  0,0,1,0,0,0,
  0,0,1,0,0,0,
  0,0,0,1,1,0,
  0,0,0,0,0,0};
letterPattern tPattern = {6, 9, 1, t};

u8 U[] =
 {0,0,0,0,0,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,0,1,1,1,0,0,
  0,0,0,0,0,0,0};
letterPattern UPattern = {7, 9, 1, U};

u8 u[] =
 {0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,0,1,1,1,0,
  0,0,0,0,0,0};
letterPattern uPattern = {6, 8, 1, u};

u8 V[] =
 {0,0,0,0,0,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,0,1,0,1,0,0,
  0,0,0,1,0,0,0,
  0,0,0,0,0,0,0};
letterPattern VPattern = {7, 9, 1, V};

u8 v[] =
 {0,0,0,0,0,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,0,1,0,1,0,0,
  0,0,0,1,0,0,0,
  0,0,0,0,0,0,0};
letterPattern vPattern = {7, 7, 1, v};

u8 W[] =
 {0,0,0,0,0,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,1,0,1,0,
  0,1,1,0,1,1,0,
  0,1,0,0,0,1,0,
  0,0,0,0,0,0,0};
letterPattern WPattern = {7, 9, 1, W};

u8 w[] =
 {0,0,0,0,0,0,0,
  0,1,0,0,0,1,0,
  0,1,0,1,0,1,0,
  0,1,0,1,0,1,0,
  0,1,0,1,0,1,0,
  0,0,1,0,1,0,0,
  0,0,0,0,0,0,0};
letterPattern wPattern = {7, 7, 1, w};

u8 X[] =
 {0,0,0,0,0,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,0,1,0,1,0,0,
  0,0,0,1,0,0,0,
  0,0,1,0,1,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,0,0,0,0,0,0};
letterPattern XPattern = {7, 9, 1, X};

u8 x[] =
 {0,0,0,0,0,0,0,
  0,1,0,0,0,1,0,
  0,0,1,0,1,0,0,
  0,0,0,1,0,0,0,
  0,0,1,0,1,0,0,
  0,1,0,0,0,1,0,
  0,0,0,0,0,0,0};
letterPattern xPattern = {7, 7, 1, x};

u8 Y[] =
 {0,0,0,0,0,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,0,1,1,1,0,0,
  0,0,0,1,0,0,0,
  0,0,0,1,0,0,0,
  0,0,0,1,0,0,0,
  0,0,0,0,0,0,0};
letterPattern YPattern = {7, 9, 1, Y};

u8 y[] =
 {0,0,0,0,0,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,0,1,1,1,0,
  0,0,0,0,1,0,
  0,1,1,1,0,0,
  0,0,0,0,0,0};
letterPattern yPattern = {6, 8, 0, y};

u8 Z[] =
 {0,0,0,0,0,0,0,
  0,1,1,1,1,1,0,
  0,0,0,0,0,1,0,
  0,0,0,0,1,0,0,
  0,0,0,1,0,0,0,
  0,0,1,0,0,0,0,
  0,1,0,0,0,0,0,
  0,1,1,1,1,1,0,
  0,0,0,0,0,0,0};
letterPattern ZPattern = {7, 9, 1, Z};

u8 z[] =
 {0,0,0,0,0,0,0,
  0,1,1,1,1,1,0,
  0,0,0,0,1,0,0,
  0,0,0,1,0,0,0,
  0,0,1,0,0,0,0,
  0,1,1,1,1,1,0,
  0,0,0,0,0,0,0};
letterPattern zPattern = {7, 7, 1, z};

u8 empty[] = {0};
letterPattern emptyPattern = {1, 1, 0, empty};


/**
 * @brief Get the appropriate struct belonging
 * to inputted letter
 */
letterPattern *getLetterOrNumber(char letterOrNumber) {
    switch (letterOrNumber)
    {
        case '0':
            return &zeroPattern;
        case '1':
            return &onePattern;
        case '2':
            return &twoPattern;
        case '3':
            return &threePattern;
        case '4':
            return &fourPattern;
        case '5':
            return &fivePattern;
        case '6':
            return &sixPattern;
        case '7':
            return &sevenPattern;
        case '8':
            return &eightPattern;
        case '9':
            return &ninePattern;
        case 'A':
            return &APattern;
        case 'a':
            return &aPattern;
        case 'B':
            return &BPattern;
        case 'b':
            return &bPattern;
        case 'C':
            return &CPattern;
        case 'c':
            return &cPattern;
        case 'D':
            return &DPattern;
        case 'd':
            return &dPattern;
        case 'E':
            return &EPattern;
        case 'e':
            return &ePattern;
        case 'F':
            return &FPattern;
        case 'f':
            return &fPattern;
        case 'G':
            return &GPattern;
        case 'g':
            return &gPattern;
        case 'H':
            return &HPattern;
        case 'h':
            return &hPattern;
        case 'I':
            return &IPattern;
        case 'i':
            return &iPattern;
        case 'J':
            return &JPattern;
        case 'j':
            return &jPattern;
        case 'K':
            return &KPattern;
        case 'k':
            return &kPattern;
        case 'L':
            return &LPattern;
        case 'l':
            return &lPattern;
        case 'M':
            return &MPattern;
        case 'm':
            return &mPattern;
        case 'N':
            return &NPattern;
        case 'n':
            return &nPattern;
        case 'O':
            return &OPattern;
        case 'o':
            return &oPattern;
        case 'P':
            return &PPattern;
        case 'p':
            return &pPattern;
        case 'Q':
            return &QPattern;
        case 'q':
            return &qPattern;
        case 'R':
            return &RPattern;
        case 'r':
            return &rPattern;
        case 'S':
            return &SPattern;
        case 's':
            return &sPattern;
        case 'T':
            return &TPattern;
        case 't':
            return &tPattern;
        case 'U':
            return &UPattern;
        case 'u':
            return &uPattern;
        case 'V':
            return &VPattern;
        case 'v':
            return &vPattern;
        case 'W':
            return &WPattern;
        case 'w':
            return &wPattern;
        case 'X':
            return &XPattern;
        case 'x':
            return &xPattern;
        case 'Y':
            return &YPattern;
        case 'y':
            return &yPattern;
        case 'Z':
            return &ZPattern;
        case 'z':
            return &zPattern;
        case '!':
            return &exclamationPattern;
        case '?':
            return &questionPattern;
        case '\'':
            return &singleQuotePattern;
        case '"':
            return &doubleQuotePattern;
        case '/':
            return &forwardSlashPattern;
        case '\\':
            return &backwardSlashPattern;
        case ':':
            return &colonPattern;
        case ';':
            return &semicolonPattern;
        case '[':
            return &openSquarePattern;
        case ']':
            return &closeSquarePattern;
        case '(':
            return &openRoundPattern;
        case ')':
            return &closeRoundPattern;
        case '.':
            return &dotPattern;
        case ',':
            return &commaPattern;
        case '%':
            return &percentPattern;
        case '#':
            return &sharpPattern;
        case '$':
            return &dollarPattern;
        case '_':
            return &underscorePattern;
        case '~':
            return &underscoreClearPattern;
        case '\0':
            return &emptyPattern;
        case ' ':
            return &spacePattern;
        default:
            return &spacePattern;
    }
}

/**
 * @brief Given a string, get the pixel pattern of each letter
 * and return the sum of the width of the patterns
 * @param word        the string to get the width of
 * @param wordLength  strlen or hardcoded length of word
 * @return the total number of pixels
 */
u32 calculateWordPixelWidth(char *word, u32 wordLength) {
    u32 total = 0;
    
    for (u32 i = 0; i < wordLength; i++) {
        total += getLetterOrNumber(word[i])->width;
    }

    total += getLetterOrNumber(' ')->width;

    return total;
}
