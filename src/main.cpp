#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <EEPROM.h>
#define SIZE 4

//Hardware configuration
int joyPin1 = 5;                 // slider variable connecetd to analog pin 0
int joyPin2 = 6;                 // slider variable connecetd to analog pin 1
int swPin = 5;
int battPin = 4;
U8G2_SSD1327_MIDAS_128X128_1_4W_HW_SPI u8g2(U8G2_R0, 3, 4);
//U8X8_SSD1327_MIDAS_128X128_4W_HW_SPI u8x8(3, 4);
// Defenitions

#define picWidth 120
#define picHeight 25
static uint8_t picBits[] = {
  0xF8, 0xFF, 0xFF, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xFE, 0x01, 0xFE, 0xFF, 0xFF, 0x01, 0xF0, 0xFF, 0x01, 0xFC, 0x03,
  0xE0, 0xFF, 0x1F, 0xC0, 0xFF, 0x07, 0xFE, 0xFF, 0xFF, 0x01, 0xFC, 0xFF,
  0x07, 0xFC, 0x03, 0xE0, 0xFF, 0x1F, 0xE0, 0xFF, 0x1F, 0xFF, 0xFF, 0xFF,
  0x03, 0xFE, 0xFF, 0x0F, 0xFC, 0x03, 0xE0, 0xFF, 0x1F, 0xF8, 0xFF, 0x3F,
  0xFF, 0xFF, 0xFF, 0x03, 0xFF, 0xF1, 0x1F, 0xFC, 0x03, 0xE0, 0xFF, 0x1F,
  0xF8, 0x0F, 0x7F, 0xFF, 0xFF, 0xFF, 0x83, 0xFF, 0xE0, 0x1F, 0xFC, 0x03,
  0xE0, 0x1F, 0x00, 0xFC, 0x03, 0x7E, 0xFF, 0xFF, 0xFF, 0x83, 0x7F, 0xC0,
  0x3F, 0xFC, 0x03, 0xE0, 0x1F, 0x00, 0xFE, 0x01, 0x7C, 0xFF, 0xFF, 0xFF,
  0xC3, 0x3F, 0xC0, 0x3F, 0xFC, 0x03, 0xE0, 0x1F, 0x00, 0xFE, 0x01, 0x38,
  0xFF, 0xFF, 0xFF, 0xC3, 0x3F, 0x80, 0x7F, 0xFC, 0x03, 0xE0, 0x1F, 0x00,
  0xFF, 0x00, 0x08, 0xFF, 0xFF, 0xFF, 0xC3, 0x3F, 0x80, 0x7F, 0xFC, 0x03,
  0xE0, 0x1F, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xC3, 0x3F, 0x80,
  0x7F, 0xFC, 0x03, 0xE0, 0x1F, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF,
  0xE3, 0x3F, 0x80, 0x7F, 0xFC, 0x03, 0xE0, 0xFF, 0x1F, 0xFF, 0x00, 0x00,
  0xFF, 0xFF, 0xFF, 0xE3, 0x3F, 0x80, 0x7F, 0xFC, 0x03, 0xE0, 0xFF, 0x1F,
  0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xE3, 0x3F, 0x80, 0x7F, 0xFC, 0x03,
  0xE0, 0xFF, 0x1F, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xE3, 0x3F, 0x80,
  0x7F, 0xFC, 0x03, 0xE0, 0xFF, 0x1F, 0xFF, 0x00, 0xFC, 0xFF, 0xFF, 0xFF,
  0xC3, 0x3F, 0x80, 0x7F, 0xFC, 0x03, 0xE0, 0x1F, 0x00, 0xFF, 0x00, 0xFC,
  0xFF, 0xFF, 0xFF, 0xC3, 0x3F, 0x80, 0x7F, 0xFC, 0x03, 0xE0, 0x1F, 0x00,
  0xFF, 0x00, 0xFC, 0xFF, 0xFF, 0xFF, 0xC3, 0x3F, 0x80, 0x7F, 0xFC, 0x03,
  0xE0, 0x1F, 0x00, 0xFF, 0x01, 0xFC, 0xFF, 0xFF, 0xFF, 0xC3, 0x7F, 0xC0,
  0x3F, 0xFC, 0x03, 0xE0, 0x1F, 0x00, 0xFE, 0x01, 0xFC, 0xFF, 0xFF, 0xFF,
  0x83, 0x7F, 0xC0, 0x3F, 0xFC, 0x03, 0xE0, 0x1F, 0x00, 0xFE, 0x03, 0xFC,
  0xFF, 0xFF, 0xFF, 0x03, 0xFF, 0xE0, 0x1F, 0xFC, 0x03, 0xE0, 0x1F, 0x00,
  0xFC, 0x07, 0xFE, 0xFF, 0xFF, 0xFF, 0x03, 0xFF, 0xFB, 0x0F, 0xFC, 0xFF,
  0xE7, 0xFF, 0x1F, 0xF8, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0x03, 0xFE, 0xFF,
  0x07, 0xFC, 0xFF, 0xE7, 0xFF, 0x1F, 0xF0, 0xFF, 0x7F, 0xFE, 0xFF, 0xFF,
  0x01, 0xF8, 0xFF, 0x03, 0xFC, 0xFF, 0xE7, 0xFF, 0x1F, 0xE0, 0xFF, 0x3F,
  0xFC, 0xFF, 0xFF, 0x00, 0xE0, 0xFF, 0x00, 0xFC, 0xFF, 0xE7, 0xFF, 0x1F,
  0x80, 0xFF, 0x07,
};

typedef enum
{
  LEFT,
  RIGHT,
  UP,
  DOWN,
  NOPE
} cmd;
uint8_t board[SIZE][SIZE];


unsigned int score = 0;
uint8_t scheme = 0;

void writeBestScore(unsigned int bs)
{
  byte raw[2];
  (unsigned int&)raw = bs;
  for (byte i = 0; i < 2; i++) EEPROM.write(0 + i, raw[i]);
}

unsigned int readBestScore()
{
  byte raw[2];
  for (byte i = 0; i < 2; i++) raw[i] = EEPROM.read(0 + i);
  unsigned int &num = (unsigned int&)raw;
  return num;
}

int eepromOffset=10;
void saveBoard()
{
  for(int i=0;i<SIZE;i++)
  {
    for(int j=0;j<SIZE;j++)
    {
      EEPROM.write(eepromOffset+i*SIZE+j,board[i][j]);
    }
  }
}

void loadBoard()
{
   for(int i=0;i<SIZE;i++)
  {
    for(int j=0;j<SIZE;j++)
    {
      board[i][j] = EEPROM.read(eepromOffset+i*SIZE+j);
    }
  }
}

int getBattery()
{

  int  an = analogRead(battPin);
  return map(an, 700, 1020, 0, 100);
}


void render(uint8_t board[SIZE][SIZE])
{

  u8g2.firstPage();
  do {

    int height = 32;
    int weight = 32;
    char cstr[16];
    for (int i = 0; i < SIZE; i++)
    {
      for (int j = 0; j < SIZE; j++)
      {
        int now = board[i][j];
        int r =  pow(2, now) + 1;
        if (r == 3)
          r = 2;
        itoa(r, cstr, 10);
        if (now > 0)
        {
          if (now >= 7)
            u8g2.setFont(u8g2_font_logisoso16_tr);
          if (now >= 10)
            u8g2.setFont(u8g2_font_t0_14b_tr);
          if (now < 7)
            u8g2.setFont(u8g2_font_logisoso18_tr);
          u8g2.drawStr(weight * j, height + i * height, cstr);
          //delay(500);
        }
        else
        {
          u8g2.drawStr(weight * j, height + i * height, " ");
          //delay(500);
        }
        u8g2.drawStr(weight * j, height + i * height, " ");
      }
    }

  } while ( u8g2.nextPage() );

  //u8x8.sendBuffer();
}

uint8_t findTarget(uint8_t array[SIZE], uint8_t x, uint8_t stop) {
  uint8_t t;
  // if the position is already on the first, don't evaluate
  if (x == 0) {
    return x;
  }
  for (t = x - 1; t >= 0; t--) {
    if (array[t] != 0) {
      if (array[t] != array[x]) {
        // merge is not possible, take next position
        return t + 1;
      }
      return t;
    } else {
      // we should not slide further, return this one
      if (t == stop) {
        return t;
      }
    }
  }
  // we did not find a
  return x;
}
bool best_score_showed = false;
bool slideArray(uint8_t array[SIZE]) {
  bool success = false;
  uint8_t x, t, stop = 0;

  for (x = 0; x < SIZE; x++) {
    if (array[x] != 0) {
      t = findTarget(array, x, stop);
      // if target is not original position, then move or merge
      if (t != x) {
        // if target is zero, this is a move
        if (array[t] == 0) {
          array[t] = array[x];
        } else if (array[t] == array[x]) {
          // merge (increase power of two)
          array[t]++;
          // increase score
          score += (uint32_t)1 << array[t];
          if (score > readBestScore())
          {
            writeBestScore(score);
            if (!best_score_showed)
            {
              u8g2.firstPage();
              do {
                u8g2.setFont(u8g2_font_t0_22b_tr);
                u8g2.drawStr(0, 30, "New best");
                u8g2.drawStr(0, 60, "SCORE!");
              } while (u8g2.nextPage());
              delay(500);
              render(board);
              best_score_showed = true;
            }
          }

          // set stop to avoid double merge
          stop = t + 1;
        }
        array[x] = 0;
        success = true;
      }
    }
  }
  return success;
}

void rotateBoard(uint8_t board[SIZE][SIZE]) {
  uint8_t i, j, n = SIZE;
  uint8_t tmp;
  for (i = 0; i < n / 2; i++) {
    for (j = i; j < n - i - 1; j++) {
      tmp = board[i][j];
      board[i][j] = board[j][n - i - 1];
      board[j][n - i - 1] = board[n - i - 1][n - j - 1];
      board[n - i - 1][n - j - 1] = board[n - j - 1][i];
      board[n - j - 1][i] = tmp;
    }
  }
}

bool moveUp(uint8_t board[SIZE][SIZE]) {
  bool success = false;
  uint8_t x;
  for (x = 0; x < SIZE; x++) {
    success |= slideArray(board[x]);
  }
  return success;
}

bool moveLeft(uint8_t board[SIZE][SIZE]) {
  bool success;
  rotateBoard(board);
  success = moveUp(board);
  rotateBoard(board);
  rotateBoard(board);
  rotateBoard(board);
  return success;
}

bool moveDown(uint8_t board[SIZE][SIZE]) {
  bool success;
  rotateBoard(board);
  rotateBoard(board);
  success = moveUp(board);
  rotateBoard(board);
  rotateBoard(board);
  return success;
}

bool moveRight(uint8_t board[SIZE][SIZE]) {
  bool success;
  rotateBoard(board);
  rotateBoard(board);
  rotateBoard(board);
  success = moveUp(board);
  rotateBoard(board);
  return success;
}

bool findPairDown(uint8_t board[SIZE][SIZE]) {
  bool success = false;
  uint8_t x, y;
  for (x = 0; x < SIZE; x++) {
    for (y = 0; y < SIZE - 1; y++) {
      if (board[x][y] == board[x][y + 1]) return true;
    }
  }
  return success;
}

uint8_t countEmpty(uint8_t board[SIZE][SIZE]) {
  uint8_t x, y;
  uint8_t count = 0;
  for (x = 0; x < SIZE; x++) {
    for (y = 0; y < SIZE; y++) {
      if (board[x][y] == 0) {
        count++;
      }
    }
  }
  return count;
}

bool gameEnded(uint8_t board[SIZE][SIZE]) {
  bool ended = true;
  if (countEmpty(board) > 0) return false;
  if (findPairDown(board)) return false;
  rotateBoard(board);
  if (findPairDown(board)) ended = false;
  rotateBoard(board);
  rotateBoard(board);
  rotateBoard(board);
  return ended;
}

void addRandom(uint8_t board[SIZE][SIZE]) {
  static bool initialized = false;
  uint8_t x, y;
  uint8_t r, len = 0;
  uint8_t n, list[SIZE * SIZE][2];

  if (!initialized) {
    initialized = true;
  }

  for (x = 0; x < SIZE; x++) {
    for (y = 0; y < SIZE; y++) {
      if (board[x][y] == 0) {
        list[len][0] = x;
        list[len][1] = y;
        len++;
      }
    }
  }
  randomSeed(analogRead(2));
  if (len > 0) {
    r = random(len);
    x = list[r][0];
    y = list[r][1];
    n = (random(10)) / 9 + 1;
    board[x][y] = n;
  }
}

void initBoard(uint8_t board[SIZE][SIZE]) {
  uint8_t x, y;
  for (x = 0; x < SIZE; x++) {
    for (y = 0; y < SIZE; y++) {
      board[x][y] = 0;
    }
  }
  addRandom(board);
  addRandom(board);
  render(board);
  score = 0;
}



void make_move(cmd command)
{
  bool success;
  switch (command) {
    case LEFT:
      success = moveLeft(board);  break;
    case RIGHT:
      success = moveRight(board);  break;
    case UP:
      success = moveUp(board);  break;
    case DOWN:  // left arrow
      success = moveDown(board);  break;
    default: success = false;
  }
  if (success) {
    addRandom(board);
    saveBoard();
    render(board);
    if (gameEnded(board))
    {
      u8g2.firstPage();
      do {
        u8g2.setFont(u8g2_font_t0_22b_tr);
        u8g2.drawStr(16, 60, "You LOSE, mthfckr");
      } while (u8g2.nextPage());
    }
  }
}





cmd get_joystick_command()
{
  int x = analogRead(joyPin1);
  int y = analogRead(joyPin2);
  if (x < 50)
    return UP;
  if (x > 900)
    return DOWN;
  if (y > 900)
    return RIGHT;
  if (y < 50)
    return LEFT;
  return NOPE;

}


void setup() {
  Serial.begin(9600);
  analogReference(INTERNAL);
  pinMode(swPin, INPUT);
  digitalWrite(swPin, HIGH);

  u8g2.begin();
  u8g2.firstPage();
  do
  {
    u8g2.drawXBM( 0, 60, picWidth, picHeight, picBits);
  } while (u8g2.nextPage());

  delay(1000);
  initBoard(board);
  loadBoard();
  render(board);
}


void show_menu()
{
  int batt = getBattery();
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_logisoso18_tr);
    char scorestr[16];
    sprintf(scorestr, "Score=%i", score);
    u8g2.drawStr(0, 32, scorestr);
    u8g2.drawStr(2, 64, "SAVE");
    u8g2.drawStr(80, 64, "LOAD");
    u8g2.setFont(u8g2_font_t0_14b_tr);
    char bestScoreStr[16];
    sprintf(bestScoreStr, "Best score=%i", readBestScore());
    u8g2.drawStr(0, 120, bestScoreStr);
    char battStr[16];
    sprintf(battStr, "Battery %i", batt);
    u8g2.drawStr(0, 100, battStr);
  } while ( u8g2.nextPage() );
}

void menu_command(cmd command)
{
  return;
}
bool menu_mode = false;

void loop() {
  if (!digitalRead(swPin))
  {
    delay(200);
    menu_mode = !menu_mode;
    if (menu_mode)
      show_menu();
    else
      render(board);
  }
  cmd command = get_joystick_command();
  delay(100 );
  if (command != NOPE)
  {
    if (!menu_mode)
      make_move(command);
    else
      menu_command(command);
  }
}
