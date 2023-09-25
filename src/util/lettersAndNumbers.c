
#include <stdint.h>
#include "main.h"

#include "lettersAndNumbers.h"

// Grand9K Pixel Font
static u8 zero[] =
 {0,0,0,0,0,0,0,
  0,0,1,1,1,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,1,1,0,
  0,1,0,1,0,1,0,
  0,1,1,0,0,1,0,
  0,1,0,0,0,1,0,
  0,0,1,1,1,0,0,
  0,0,0,0,0,0,0};
gif_letterPattern zeroPattern = {7, 9, 1, zero};

static u8 one[] = 
 {0,0,0,0,0,
  0,1,1,0,0,
  0,0,1,0,0,
  0,0,1,0,0,
  0,0,1,0,0,
  0,0,1,0,0,
  0,0,1,0,0,
  0,0,1,0,0,
  0,0,0,0,0};
gif_letterPattern onePattern = {5, 9, 1, one};

static u8 two[] = 
 {0,0,0,0,0,0,0,
  0,1,1,1,1,0,0,
  0,0,0,0,0,1,0,
  0,0,0,0,0,1,0,
  0,0,1,1,1,0,0,
  0,1,0,0,0,0,0,
  0,1,0,0,0,0,0,
  0,1,1,1,1,1,0,
  0,0,0,0,0,0,0};
gif_letterPattern twoPattern = {7, 9, 1, two};

static u8 three[] = 
 {0,0,0,0,0,0,
  0,1,1,1,0,0,
  0,0,0,0,1,0,
  0,0,0,0,1,0,
  0,1,1,1,0,0,
  0,0,0,0,1,0,
  0,0,0,0,1,0,
  0,1,1,1,0,0,
  0,0,0,0,0,0};
gif_letterPattern threePattern = {6, 9, 1, three};

static u8 four[] = 
 {0,0,0,0,0,0,0,
  0,0,0,0,1,1,0,
  0,0,0,1,0,1,0,
  0,0,1,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,1,1,1,1,0,
  0,0,0,0,0,1,0,
  0,0,0,0,0,1,0,
  0,0,0,0,0,0,0};
gif_letterPattern fourPattern = {7, 9, 1, four};

static u8 five[] = 
 {0,0,0,0,0,0,0,
  0,1,1,1,1,1,0,
  0,1,0,0,0,0,0,
  0,1,0,0,0,0,0,
  0,1,1,1,1,0,0,
  0,0,0,0,0,1,0,
  0,0,0,0,0,1,0,
  0,1,1,1,1,0,0,
  0,0,0,0,0,0,0};
gif_letterPattern fivePattern = {7, 9, 1, five};

static u8 six[] = 
 {0,0,0,0,0,0,0,
  0,0,1,1,1,0,0,
  0,1,0,0,0,0,0,
  0,1,0,0,0,0,0,
  0,1,1,1,1,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,0,1,1,1,0,0,
  0,0,0,0,0,0,0};
gif_letterPattern sixPattern = {7, 9, 1, six};

static u8 seven[] = 
 {0,0,0,0,0,0,0,
  0,1,1,1,1,1,0,
  0,0,0,0,0,1,0,
  0,0,0,0,1,0,0,
  0,0,0,0,1,0,0,
  0,0,0,1,0,0,0,
  0,0,0,1,0,0,0,
  0,0,0,1,0,0,0,
  0,0,0,0,0,0,0};
gif_letterPattern sevenPattern = {7, 9, 1, seven};

static u8 eight[] = 
 {0,0,0,0,0,0,0,
  0,0,1,1,1,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,0,1,1,1,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,0,1,1,1,0,0,
  0,0,0,0,0,0,0};
gif_letterPattern eightPattern = {7, 9, 1, eight};

static u8 nine[] = 
 {0,0,0,0,0,0,0,
  0,0,1,1,1,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,0,1,1,1,1,0,
  0,0,0,0,0,1,0,
  0,0,0,0,0,1,0,
  0,0,1,1,1,0,0,
  0,0,0,0,0,0,0};
gif_letterPattern ninePattern = {7, 9, 1, nine};

static u8 space[] = 
 {0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,0,0,0,0,0};
gif_letterPattern spacePattern = {6, 9, 0, space};

static u8 underscoreMark[] = 
 {0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,1,1,1,1,1,1,0,
  0,0,0,0,0,0,0,0};
gif_letterPattern underscorePattern = {8, 9, 0, underscoreMark};

static u8 dashMark[] = 
 {0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,1,1,1,1,1,1,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0};
gif_letterPattern dashPattern = {8, 9, 0, dashMark};

static u8 underscoreClearMark[] = 
 {0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0};
gif_letterPattern underscoreClearPattern = {8, 9, 0, underscoreClearMark};

static u8 exclamationMark[] = 
 {0,0,0,
  0,1,0,
  0,1,0,
  0,1,0,
  0,1,0,
  0,1,0,
  0,0,0,
  0,1,0,
  0,0,0};
gif_letterPattern exclamationPattern = {3, 9, 1, exclamationMark};

static u8 sharpMark[] = 
 {0,0,0,0,0,0,0,0,
  0,0,1,0,0,1,0,0,
  0,1,1,1,1,1,1,0,
  0,0,1,0,0,1,0,0,
  0,0,1,0,0,1,0,0,
  0,0,1,0,0,1,0,0,
  0,1,1,1,1,1,1,0,
  0,0,1,0,0,1,0,0,
  0,0,0,0,0,0,0,0};
gif_letterPattern sharpPattern = {8, 9, 1, sharpMark};

static u8 dollarMark[] = 
 {0,0,0,0,0,0,0,
  0,0,0,1,0,0,0,
  0,0,1,1,1,1,0,
  0,1,0,1,0,0,0,
  0,0,1,1,1,0,0,
  0,0,0,1,0,1,0,
  0,1,1,1,1,0,0,
  0,0,0,1,0,0,0,
  0,0,0,0,0,0,0};
gif_letterPattern dollarPattern = {7, 9, 1, dollarMark};

static u8 colonMark[] = 
 {0,0,0,
  0,0,0,
  0,0,0,
  0,1,0,
  0,0,0,
  0,0,0,
  0,0,0,
  0,1,0,
  0,0,0};
gif_letterPattern colonPattern = {3, 9, 1, colonMark};

static u8 semicolonMark[] = 
 {0,0,0,
  0,0,0,
  0,1,0,
  0,0,0,
  0,0,0,
  0,0,0,
  0,1,0,
  0,1,0,
  0,0,0};
gif_letterPattern semicolonPattern = {3, 9, 0, semicolonMark};

static u8 commaMark[] = 
 {0,0,0,
  0,0,0,
  0,0,0,
  0,0,0,
  0,0,0,
  0,0,0,
  0,1,0,
  0,1,0,
  0,0,0};
gif_letterPattern commaPattern = {3, 9, 0, commaMark};

static u8 dotMark[] = 
 {0,0,0,
  0,0,0,
  0,0,0,
  0,0,0,
  0,0,0,
  0,0,0,
  0,0,0,
  0,1,0,
  0,0,0};
gif_letterPattern dotPattern = {3, 9, 1, dotMark};

static u8 singleQuoteMark[] = 
 {0,0,0,
  0,1,0,
  0,1,0,
  0,0,0,
  0,0,0,
  0,0,0,
  0,0,0,
  0,0,0,
  0,0,0};
gif_letterPattern singleQuotePattern = {3, 9, 1, singleQuoteMark};

static u8 doubleQuoteMark[] = 
 {0,0,0,0,0,
  0,1,0,1,0,
  0,1,0,1,0,
  0,0,0,0,0,
  0,0,0,0,0,
  0,0,0,0,0,
  0,0,0,0,0,
  0,0,0,0,0,
  0,0,0,0,0};
gif_letterPattern doubleQuotePattern = {5, 9, 1, doubleQuoteMark};

static u8 forwardSlashMark[] = 
 {0,0,0,0,0,
  0,0,0,1,0,
  0,0,0,1,0,
  0,0,1,0,0,
  0,0,1,0,0,
  0,0,1,0,0,
  0,1,0,0,0,
  0,1,0,0,0,
  0,0,0,0,0};
gif_letterPattern forwardSlashPattern = {5, 9, 1, forwardSlashMark};

static u8 backwardSlashMark[] = 
 {0,0,0,0,0,
  0,1,0,0,0,
  0,1,0,0,0,
  0,0,1,0,0,
  0,0,1,0,0,
  0,0,1,0,0,
  0,0,0,1,0,
  0,0,0,1,0,
  0,0,0,0,0};
gif_letterPattern backwardSlashPattern = {5, 9, 1, backwardSlashMark};

static u8 questionMark[] = 
 {0,0,0,0,0,0,
  0,1,1,1,0,0,
  0,0,0,0,1,0,
  0,0,0,0,1,0,
  0,0,0,1,0,0,
  0,0,1,0,0,0,
  0,0,0,0,0,0,
  0,0,1,0,0,0,
  0,0,0,0,0,0};
gif_letterPattern questionPattern = {6, 9, 1, questionMark};

static u8 percentMark[] = 
 {0,0,0,0,0,0,0,0,0,0,
  0,0,1,0,0,0,0,1,0,0,
  0,1,0,1,0,0,1,0,0,0,
  0,1,0,1,0,1,0,0,0,0,
  0,0,1,0,1,0,1,0,0,0,
  0,0,0,1,0,1,0,1,0,0,
  0,0,1,0,0,1,0,1,0,0,
  0,1,0,0,0,0,1,0,0,0,
  0,0,0,0,0,0,0,0,0,0};
gif_letterPattern percentPattern = {10, 9, 1, percentMark};

static u8 openRoundBracketMark[] = 
 {0,0,0,0,0,
  0,0,1,1,0,
  0,1,0,0,0,
  0,1,0,0,0,
  0,1,0,0,0,
  0,1,0,0,0,
  0,1,0,0,0,
  0,0,1,1,0,
  0,0,0,0,0};
gif_letterPattern openRoundPattern = {5, 9, 1, openRoundBracketMark};

static u8 closeRoundBracketMark[] = 
 {0,0,0,0,0,
  0,1,1,0,0,
  0,0,0,1,0,
  0,0,0,1,0,
  0,0,0,1,0,
  0,0,0,1,0,
  0,0,0,1,0,
  0,1,1,0,0,
  0,0,0,0,0};
gif_letterPattern closeRoundPattern = {5, 9, 1, closeRoundBracketMark};

static u8 openSquareBracketMark[] = 
 {0,0,0,0,0,
  0,1,1,1,0,
  0,1,0,0,0,
  0,1,0,0,0,
  0,1,0,0,0,
  0,1,0,0,0,
  0,1,0,0,0,
  0,1,1,1,0,
  0,0,0,0,0};
gif_letterPattern openSquarePattern = {5, 9, 1, openSquareBracketMark};

static u8 closeSquareBracketMark[] = 
 {0,0,0,0,0,
  0,1,1,1,0,
  0,0,0,1,0,
  0,0,0,1,0,
  0,0,0,1,0,
  0,0,0,1,0,
  0,0,0,1,0,
  0,1,1,1,0,
  0,0,0,0,0};
gif_letterPattern closeSquarePattern = {5, 9, 1, closeSquareBracketMark};

static u8 atMark[] = 
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
gif_letterPattern atPattern = {9, 10, 1, atMark};


static u8 A[] =
 {0,0,0,0,0,0,0,
  0,0,1,1,1,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,1,1,1,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,0,0,0,0,0,0,};
gif_letterPattern APattern = {7, 9, 1, A};

static u8 a[] =
 {0,0,0,0,0,0,
  0,0,1,1,0,0,
  0,0,0,0,1,0,
  0,0,1,1,1,0,
  0,1,0,0,1,0,
  0,0,1,1,1,0,
  0,0,0,0,0,0,};
gif_letterPattern aPattern = {6, 7, 1, a};

static u8 B[] =
 {0,0,0,0,0,0,0,
  0,1,1,1,1,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,1,1,1,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,1,1,1,0,0,
  0,0,0,0,0,0,0,};
gif_letterPattern BPattern = {7, 9, 1, B};

static u8 b[] =
 {0,0,0,0,0,0,
  0,1,0,0,0,0,
  0,1,0,0,0,0,
  0,1,1,1,0,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,1,1,1,0,0,
  0,0,0,0,0,0};
gif_letterPattern bPattern = {6, 9, 1, b};

static u8 C[] =
 {0,0,0,0,0,0,
  0,0,1,1,1,0,
  0,1,0,0,0,0,
  0,1,0,0,0,0,
  0,1,0,0,0,0,
  0,1,0,0,0,0,
  0,1,0,0,0,0,
  0,0,1,1,1,0,
  0,0,0,0,0,0};
gif_letterPattern CPattern = {6, 9, 1, C};

static u8 c[] =
 {0,0,0,0,0,
  0,0,0,0,0,
  0,0,1,1,0,
  0,1,0,0,0,
  0,1,0,0,0,
  0,1,0,0,0,
  0,0,1,1,0,
  0,0,0,0,0,};
gif_letterPattern cPattern = {5, 8, 1, c};


static u8 D[] =
 {0,0,0,0,0,0,0,
  0,1,1,1,1,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,1,1,1,0,0,
  0,0,0,0,0,0,0};
gif_letterPattern DPattern = {7, 9, 1, D};

static u8 d[] =
 {0,0,0,0,0,0,
  0,0,0,0,1,0,
  0,0,0,0,1,0,
  0,0,1,1,1,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,0,1,1,1,0,
  0,0,0,0,0,0};
gif_letterPattern dPattern = {6, 9, 1, d};

static u8 E[] =
 {0,0,0,0,0,0,
  0,1,1,1,1,0,
  0,1,0,0,0,0,
  0,1,0,0,0,0,
  0,1,1,1,1,0,
  0,1,0,0,0,0,
  0,1,0,0,0,0,
  0,1,1,1,1,0,
  0,0,0,0,0,0};
gif_letterPattern EPattern = {6, 9, 1, E};

static u8 e[] =
 {0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,0,1,1,0,0,
  0,1,0,0,1,0,
  0,1,1,1,1,0,
  0,1,0,0,0,0,
  0,0,1,1,1,0,
  0,0,0,0,0,0};
gif_letterPattern ePattern = {6, 8, 1, e};

static u8 F[] =
 {0,0,0,0,0,0,
  0,1,1,1,1,0,
  0,1,0,0,0,0,
  0,1,0,0,0,0,
  0,1,1,1,1,0,
  0,1,0,0,0,0,
  0,1,0,0,0,0,
  0,1,0,0,0,0,
  0,0,0,0,0,0};
gif_letterPattern FPattern = {6, 9, 1, F};

static u8 f[] =
 {0,0,0,0,0,0,
  0,0,0,1,1,0,
  0,0,1,0,0,0,
  0,1,1,1,1,0,
  0,0,1,0,0,0,
  0,0,1,0,0,0,
  0,0,1,0,0,0,
  0,0,1,0,0,0,
  0,0,0,0,0,0};
gif_letterPattern fPattern = {6, 9, 1, f};

static u8 G[] =
 {0,0,0,0,0,0,0,
  0,0,1,1,1,1,0,
  0,1,0,0,0,0,0,
  0,1,0,0,0,0,0,
  0,1,0,0,1,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,0,1,1,1,1,0,
  0,0,0,0,0,0,0,};
gif_letterPattern GPattern = {7, 9, 1, G};

static u8 g[] =
 {0,0,0,0,0,0,
  0,0,1,1,1,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,0,1,1,1,0,
  0,0,0,0,1,0,
  0,1,1,1,0,0,
  0,0,0,0,0,0};
gif_letterPattern gPattern = {6, 8, 0, g};

static u8 H[] =
 {0,0,0,0,0,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,1,1,1,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,0,0,0,0,0,0};
gif_letterPattern HPattern = {7, 9, 1, H};

static u8 h[] =
 {0,0,0,0,0,0,
  0,1,0,0,0,0,
  0,1,0,0,0,0,
  0,1,1,1,0,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,0,0,0,0,0};
gif_letterPattern hPattern = {6, 9, 1, h};

static u8 I[] =
 {0,0,0,
  0,1,0,
  0,1,0,
  0,1,0,
  0,1,0,
  0,1,0,
  0,1,0,
  0,1,0,
  0,0,0};
gif_letterPattern IPattern = {3, 9, 1, I};

static u8 i[] =
 {0,0,0,
  0,1,0,
  0,0,0,
  0,1,0,
  0,1,0,
  0,1,0,
  0,1,0,
  0,1,0,
  0,0,0};
gif_letterPattern iPattern = {3, 9, 1, i};

static u8 J[] =
 {0,0,0,0,0,0,0,
  0,0,0,0,0,1,0,
  0,0,0,0,0,1,0,
  0,0,0,0,0,1,0,
  0,0,0,0,0,1,0,
  0,0,0,0,0,1,0,
  0,0,0,0,0,1,0,
  0,1,1,1,1,0,0,
  0,0,0,0,0,0,0};
gif_letterPattern JPattern = {7, 9, 1, J};

static u8 j[] =
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
gif_letterPattern jPattern = {5, 10, 0, j};

static u8 K[] =
 {0,0,0,0,0,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,1,0,0,
  0,1,0,1,0,0,0,
  0,1,1,0,0,0,0,
  0,1,0,1,0,0,0,
  0,1,0,0,1,0,0,
  0,1,0,0,0,1,0,
  0,0,0,0,0,0,0};
gif_letterPattern KPattern = {7, 9, 1, K};

static u8 k[] =
 {0,0,0,0,0,0,
  0,1,0,0,0,0,
  0,1,0,0,0,0,
  0,1,0,0,1,0,
  0,1,0,1,0,0,
  0,1,1,0,0,0,
  0,1,0,1,0,0,
  0,1,0,0,1,0,
  0,0,0,0,0,0};
gif_letterPattern kPattern = {6, 9, 1, k};

static u8 L[] =
 {0,0,0,0,0,0,0,
  0,1,0,0,0,0,0,
  0,1,0,0,0,0,0,
  0,1,0,0,0,0,0,
  0,1,0,0,0,0,0,
  0,1,0,0,0,0,0,
  0,1,0,0,0,0,0,
  0,1,1,1,1,1,0,
  0,0,0,0,0,0,0};
gif_letterPattern LPattern = {7, 9, 1, L};

static u8 l[] =
 {0,0,0,
  0,1,0,
  0,1,0,
  0,1,0,
  0,1,0,
  0,1,0,
  0,1,0,
  0,1,0,
  0,0,0};
gif_letterPattern lPattern = {3, 9, 1, l};

static u8 M[] =
 {0,0,0,0,0,0,0,
  0,1,0,0,0,1,0,
  0,1,1,0,1,1,0,
  0,1,0,1,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,0,0,0,0,0,0};
gif_letterPattern MPattern = {7, 9, 1, M};

static u8 m[] =
 {0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,
  0,1,1,0,1,0,0,
  0,1,0,1,0,1,0,
  0,1,0,1,0,1,0,
  0,1,0,1,0,1,0,
  0,1,0,0,0,1,0,
  0,0,0,0,0,0,0};
gif_letterPattern mPattern = {7, 8, 1, m};

static u8 N[] =
 {0,0,0,0,0,0,0,
  0,1,0,0,0,1,0,
  0,1,1,0,0,1,0,
  0,1,0,1,0,1,0,
  0,1,0,0,1,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,0,0,0,0,0,0};
gif_letterPattern NPattern = {7, 9, 1, N};

static u8 n[] =
 {0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,1,1,1,0,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,0,0,0,0,0};
gif_letterPattern nPattern = {6, 8, 1, n};

static u8 O[] =
 {0,0,0,0,0,0,0,
  0,0,1,1,1,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,0,1,1,1,0,0,
  0,0,0,0,0,0,0};
gif_letterPattern OPattern = {7, 9, 1, O};

static u8 o[] =
 {0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,0,1,1,0,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,0,1,1,0,0,
  0,0,0,0,0,0};
gif_letterPattern oPattern = {6, 8, 1, o};

static u8 P[] =
 {0,0,0,0,0,0,0,
  0,1,1,1,1,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,1,1,1,0,0,
  0,1,0,0,0,0,0,
  0,1,0,0,0,0,0,
  0,0,0,0,0,0,0};
gif_letterPattern PPattern = {7, 9, 1, P};

static u8 p[] =
 {0,0,0,0,0,0,
  0,1,1,1,0,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,1,1,1,0,0,
  0,1,0,0,0,0,
  0,0,0,0,0,0};
gif_letterPattern pPattern = {6, 8, 0, p};

static u8 Q[] =
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
gif_letterPattern QPattern = {8, 10, 0, Q};

static u8 q[] =
 {0,0,0,0,0,0,
  0,0,1,1,1,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,0,1,1,1,0,
  0,0,0,0,1,0,
  0,0,0,0,0,0};
gif_letterPattern qPattern = {6, 8, 0, q};

static u8 R[] =
 {0,0,0,0,0,0,0,
  0,1,1,1,1,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,1,1,1,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,0,0,0,0,0,0,};
gif_letterPattern RPattern = {7, 9, 1, R};

static u8 r[] =
 {0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,1,0,1,1,0,
  0,1,1,0,0,0,
  0,1,0,0,0,0,
  0,1,0,0,0,0,
  0,1,0,0,0,0,
  0,0,0,0,0,0};
gif_letterPattern rPattern = {6, 8, 1, r};

static u8 S[] =
 {0,0,0,0,0,0,0,
  0,0,1,1,1,1,0,
  0,1,0,0,0,0,0,
  0,1,0,0,0,0,0,
  0,0,1,1,1,0,0,
  0,0,0,0,0,1,0,
  0,0,0,0,0,1,0,
  0,1,1,1,1,0,0,
  0,0,0,0,0,0,0,};
gif_letterPattern SPattern = {7, 9, 1, S};

static u8 s[] =
 {0,0,0,0,0,0,
  0,0,1,1,1,0,
  0,1,0,0,0,0,
  0,0,1,1,0,0,
  0,0,0,0,1,0,
  0,1,1,1,0,0,
  0,0,0,0,0,0};
gif_letterPattern sPattern = {6, 7, 1, s};

static u8 T[] =
 {0,0,0,0,0,0,0,
  0,1,1,1,1,1,0,
  0,0,0,1,0,0,0,
  0,0,0,1,0,0,0,
  0,0,0,1,0,0,0,
  0,0,0,1,0,0,0,
  0,0,0,1,0,0,0,
  0,0,0,1,0,0,0,
  0,0,0,0,0,0,0};
gif_letterPattern TPattern = {7, 9, 1, T};

static u8 t[] =
 {0,0,0,0,0,0,
  0,0,1,0,0,0,
  0,0,1,0,0,0,
  0,1,1,1,1,0,
  0,0,1,0,0,0,
  0,0,1,0,0,0,
  0,0,1,0,0,0,
  0,0,0,1,1,0,
  0,0,0,0,0,0};
gif_letterPattern tPattern = {6, 9, 1, t};

static u8 U[] =
 {0,0,0,0,0,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,0,1,1,1,0,0,
  0,0,0,0,0,0,0};
gif_letterPattern UPattern = {7, 9, 1, U};

static u8 u[] =
 {0,0,0,0,0,0,
  0,0,0,0,0,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,0,1,1,1,0,
  0,0,0,0,0,0};
gif_letterPattern uPattern = {6, 8, 1, u};

static u8 V[] =
 {0,0,0,0,0,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,0,1,0,1,0,0,
  0,0,0,1,0,0,0,
  0,0,0,0,0,0,0};
gif_letterPattern VPattern = {7, 9, 1, V};

static u8 v[] =
 {0,0,0,0,0,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,0,1,0,1,0,0,
  0,0,0,1,0,0,0,
  0,0,0,0,0,0,0};
gif_letterPattern vPattern = {7, 7, 1, v};

static u8 W[] =
 {0,0,0,0,0,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,1,0,1,0,
  0,1,1,0,1,1,0,
  0,1,0,0,0,1,0,
  0,0,0,0,0,0,0};
gif_letterPattern WPattern = {7, 9, 1, W};

static u8 w[] =
 {0,0,0,0,0,0,0,
  0,1,0,0,0,1,0,
  0,1,0,1,0,1,0,
  0,1,0,1,0,1,0,
  0,1,0,1,0,1,0,
  0,0,1,0,1,0,0,
  0,0,0,0,0,0,0};
gif_letterPattern wPattern = {7, 7, 1, w};

static u8 X[] =
 {0,0,0,0,0,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,0,1,0,1,0,0,
  0,0,0,1,0,0,0,
  0,0,1,0,1,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,0,0,0,0,0,0};
gif_letterPattern XPattern = {7, 9, 1, X};

static u8 x[] =
 {0,0,0,0,0,0,0,
  0,1,0,0,0,1,0,
  0,0,1,0,1,0,0,
  0,0,0,1,0,0,0,
  0,0,1,0,1,0,0,
  0,1,0,0,0,1,0,
  0,0,0,0,0,0,0};
gif_letterPattern xPattern = {7, 7, 1, x};

static u8 Y[] =
 {0,0,0,0,0,0,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,1,0,0,0,1,0,
  0,0,1,1,1,0,0,
  0,0,0,1,0,0,0,
  0,0,0,1,0,0,0,
  0,0,0,1,0,0,0,
  0,0,0,0,0,0,0};
gif_letterPattern YPattern = {7, 9, 1, Y};

static u8 y[] =
 {0,0,0,0,0,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,1,0,0,1,0,
  0,0,1,1,1,0,
  0,0,0,0,1,0,
  0,1,1,1,0,0,
  0,0,0,0,0,0};
gif_letterPattern yPattern = {6, 8, 0, y};

static u8 Z[] =
 {0,0,0,0,0,0,0,
  0,1,1,1,1,1,0,
  0,0,0,0,0,1,0,
  0,0,0,0,1,0,0,
  0,0,0,1,0,0,0,
  0,0,1,0,0,0,0,
  0,1,0,0,0,0,0,
  0,1,1,1,1,1,0,
  0,0,0,0,0,0,0};
gif_letterPattern ZPattern = {7, 9, 1, Z};

static u8 z[] =
 {0,0,0,0,0,0,0,
  0,1,1,1,1,1,0,
  0,0,0,0,1,0,0,
  0,0,0,1,0,0,0,
  0,0,1,0,0,0,0,
  0,1,1,1,1,1,0,
  0,0,0,0,0,0,0};
gif_letterPattern zPattern = {7, 7, 1, z};

static u8 empty[] = {0};
gif_letterPattern emptyPattern = {1, 1, 0, empty};


/**
 * @brief Get the appropriate struct belonging
 * to inputted letter
 */
gif_letterPattern *gif_getLetterOrNumber(char letterOrNumber) {
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
        case '-':
            return &dashPattern;
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
u32 gif_calculateWordPixelWidth(char *word, u32 wordLength) {
    u32 total = 0;
    
    for (u32 i = 0; i < wordLength; i++) {
        total += gif_getLetterOrNumber(word[i])->width;
    }

    total += gif_getLetterOrNumber(' ')->width;

    return total;
}
