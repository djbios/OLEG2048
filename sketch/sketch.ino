#include <string.h>
#define SIZE 4

//Hardware configuration
int joyPin1 = 0;                 // slider variable connecetd to analog pin 0
int joyPin2 = 1;                 // slider variable connecetd to analog pin 1

// Defenitions
typedef enum
{
  LEFT,
  RIGHT,
  UP,
  DOWN,
  NOPE
} cmd;
uint8_t board[SIZE][SIZE];


uint32_t score = 0;
uint8_t scheme = 0;

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

  if (len > 0) {
    r = rand() % len;
    x = list[r][0];
    y = list[r][1];
    n = (rand() % 10) / 9 + 1;
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
    render(board);
    delay(150);
    addRandom(board);
    render(board);
    if (gameEnded(board))
    {
      //TODO Game Over
    }
  }
}



void render(uint8_t board[SIZE][SIZE])
{
  Serial.println("______________________________________");
  for (int i = 0; i < SIZE; i++)
  {
    for (int j = 0; j < SIZE; j++)
      Serial.print(board[i][j]);
    Serial.print('\n');
  }
}

cmd get_joystick_command()
{
  int x = analogRead(joyPin1);
  int y = analogRead(joyPin2);
  if (x < 100)
    return UP;
  if (x > 800)
    return DOWN;
  if (y > 800)
    return RIGHT;
  if (y < 100)
    return LEFT;
  return NOPE;
    
}


void setup() {
  Serial.begin(9600);
  initBoard(board);
}


void loop() {
  cmd command = get_joystick_command();
  delay(200);
  if(command != NOPE)
    make_move(command);



}
