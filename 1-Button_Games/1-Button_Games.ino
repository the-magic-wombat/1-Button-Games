#include <rgb_lcd.h>
#include <Wire.h>

byte rightArrowChar[] = { // Character for a right arrow
  B00000,
  B01000,
  B01100,
  B01110,
  B01110,
  B01100,
  B01000,
  B00000
};

byte leftArrowChar[] = { // Character for a left arrow
  B00000,
  B00010,
  B00110,
  B01110,
  B01110,
  B00110,
  B00010,
  B00000
};

byte fullChar[] = { // Character for a full rectangle
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

struct question 
{
  String text1; // 1st line
  String text2; // 2nd line
  bool right; // True or false fact
  bool used;  // Is the question used
};

const int questionCount = 14;

question questions[]  // Array of questions
{
  {
    "Snails can sleep",
    "for 3 years",
    true,
    false
  },
  {
    "Elephants can",
    "jump",
    false,
    false
  },
  {
    "Frogs cannot ",
    "vomit",
    true,
    false
  },
  {
    "Humans have uniq",
    "ue tongue prints",
    true,
    false
  },
  {
    "Apples contain",
    "around 50cal",
    false,
    false
  },
  {
    "Bees sometimes",
    "sting other bees",
    true,
    false
  },
  {
    "Dinos lived on",
    "each continent",
    true,
    false
  },
  {
    "Sharks live up",
    "to 100 years",
    false,
    false
  },
  {
    "Cheetahs dont",
    "roar",
    true,
    false
  },
  {
    "Sharks existed",
    "before trees",
    true,
    false
  },
  {
    "The queen celebr",
    "ates 2 birthdays",
    true,
    false
  },
  {
    "Slugs have four",
    "noses",
    true,
    false
  },
  {
    "Birds dont pee",
    "",
    true,
    false
  },
  {
    "Swearing ",
    "stresses people",
    true,
    false
  },
  {
    "Avocados are",
    "berries",
    true,
    false
  },
  {
    "Blood contains",
    "gold",
    true,
    false
  }
};

// Pin initialization
const int buzzerPin = 10;
const int p1ButtonPin = 7;
const int p2ButtonPin = 13;
const int confirmButtonPin = 4;

// Button state variables
int p1ButtonState = 0;
int p2ButtonState = 0;
int confirmButtonState = 0;
int p1LastButtonState = 0;
int p2LastButtonState = 0;

int currentGame = 1;
bool ongoingGame = 0;

rgb_lcd lcd;  // LCD variable
const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

// Button mash variables
const int mashMeterMin = 0;
const int mashMeterMax = 16;
const int mashMeterStart = mashMeterMax / 2;
int mashMeter = mashMeterStart;

// Quiz variables
int p1QuizPoints = 0;
int p2QuizPoints = 0;

void setup() {

  lcd.begin(16, 2);
  lcd.setRGB(colorR, colorG, colorB);

  pinMode(p1ButtonPin, INPUT);
  pinMode(p2ButtonPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  // New characters initialization
  lcd.createChar(0, leftArrowChar);
  lcd.createChar(1, rightArrowChar);
  lcd.createChar(2, fullChar);
  
  menu();
}

void loop()
{
  if (ongoingGame)
  {
    switch (currentGame) {
      case 1:
        {
          buttonMasher();
        } break;

      case 2:
        {
          reaction();
        } break;

      case 3:
        {
          trueOrFalse();
        } break;
    }
  }
  else
  {
    mashMeter = mashMeterStart; // Returns the mash meter to default
    menu();
  }
}

void menu() // Main menu
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" CHOOSE A GAME");
  confirmButtonState == 0;

  do
  {
    confirmButtonState = digitalRead(confirmButtonPin);
    p1ButtonState = digitalRead(p1ButtonPin);
    p2ButtonState = digitalRead(p2ButtonPin);

    if (p1ButtonState == HIGH)
    {
      currentGame--;
      if (currentGame < 1) {
        currentGame = 1;
      }
      delay(100);
    }
    else if (p2ButtonState == HIGH)
    {
      currentGame++;
      if (currentGame > 3) {
        currentGame = 3;
      }
      delay(100);
    }

    switch (currentGame) {
      case 1:
        {
          lcd.setCursor(0, 1);
          lcd.print("  Button Mash  "); lcd.write((byte)1);
        } break;

      case 2:
        {
          lcd.setCursor(0, 1);
          lcd.write((byte)0); lcd.print("   Reaction   "); lcd.write((byte)1);;
        } break;

      case 3:
        {
          lcd.setCursor(0, 1);
          lcd.write((byte)0); lcd.print(" True or False ");
        } break;
    }
    delay(100);
  } while (confirmButtonState == LOW);

  lcd.clear();
  ongoingGame = 1;
  introTimer();
  
  if (currentGame == 1) // Starts the mash meter
  {
    mashMeterBar();
  }
}

void buttonMasher() // Game 1
{
  p1ButtonState = digitalRead(p1ButtonPin);
  p2ButtonState = digitalRead(p2ButtonPin);

  if (p1ButtonState != p1LastButtonState) {
    if (p1ButtonState == HIGH)
    {
      mashMeter--;
      lcd.clear();
      mashMeterBar();
    }
  }

  if (p2ButtonState != p2LastButtonState) {
    if (p2ButtonState == HIGH)
    {
      mashMeter++;
      lcd.clear();
      mashMeterBar();
    }
  }

  p1LastButtonState = p1ButtonState;
  p2LastButtonState = p2ButtonState;

  if (mashMeter <= mashMeterMin)
  {
    victoryScreen(1);
  }
  else if (mashMeter >= mashMeterMax)
  {
    victoryScreen(2);
  }
}

void mashMeterBar() // Mash meter generator
{
  for (int i = 0; i < mashMeter; i++)
  {
    lcd.setCursor(i, 0);
    lcd.write(2);
    lcd.setCursor(i, 1);
    lcd.write(2);
  }
}

void reaction() // Game 2
{
  int randomInterval = random(1500, 8000);

  lcd.print("Wait for signal!");

  for (int i = 0; i < randomInterval; i += 10)
  {
    p1ButtonState = digitalRead(p1ButtonPin);
    p2ButtonState = digitalRead(p2ButtonPin);
    if (p1ButtonState == HIGH)
    {
      victoryScreen(2);
      break;
    }
    else if (p2ButtonState == HIGH)
    {
      victoryScreen(1);
      break;
    }
    delay(10);
  }
  lcd.clear();
  lcd.write("      GO!!!");
  tone(buzzerPin, 1000);
  while (ongoingGame == 1)
  {
    p1ButtonState = digitalRead(p1ButtonPin);
    p2ButtonState = digitalRead(p2ButtonPin);
    if (p1ButtonState == HIGH)
    {
      noTone(buzzerPin);
      victoryScreen(1);
    }
    else if (p2ButtonState == HIGH)
    {
      noTone(buzzerPin);
      victoryScreen(2);
    }
  }
  noTone(buzzerPin);
}

void trueOrFalse()  // Game 3
{
  int questionNumber; 
  do{
    questionNumber = random(0, questionCount);
  } while (questions[questionNumber].used == true); // Checks if the questins had been used

  // Prints the question
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(questions[questionNumber].text1);
  lcd.setCursor(0, 1);
  lcd.print(questions[questionNumber].text2);
  int timer = 0;

  do {  // Checks button clicks
    
    p1ButtonState = digitalRead(p1ButtonPin);
    p2ButtonState = digitalRead(p2ButtonPin);

    if (p1ButtonState == HIGH)
    {
      if (questions[questionNumber].right == true)
      {
        p1QuizPoints++;
      }
      else if (questions[questionNumber].right == false)
      {
        p2QuizPoints++;
      }
      break;
    }

    if (p2ButtonState == HIGH)
    {
      if (questions[questionNumber].right == true)
      {
        p2QuizPoints++;
      }
      else if (questions[questionNumber].right == false)
      {
        p1QuizPoints++;
      }
      break;
    }
    
    delay(100);
    timer++;
  } while (timer <= 30);

  // Prints current points
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("     POINTS     ");
  lcd.setCursor(1, 0);
  lcd.print("P1: "); lcd.print(p1QuizPoints); lcd.print("   P2: "); lcd.print(p2QuizPoints);

  // Checks for a winner
  if(p1QuizPoints >= 4)
  {
    p1QuizPoints = 0;
    p2QuizPoints = 0;
    for(int i = 0; i < questionCount; i++)
    {
      questions[i].used = false;
    }
    victoryScreen(1);
  }
  else if(p2QuizPoints >= 4)
  {
    p1QuizPoints = 0;
    p2QuizPoints = 0;
    for(int i = 0; i < questionCount; i++)
    {
      questions[i].used = false;
    }
    victoryScreen(2);
  }

  questions[questionNumber].used = true;
  
  delay(1500);
}

void introTimer()
{
  lcd.clear();
  lcd.setCursor(7, 0);
  lcd.print("3");
  tone(buzzerPin, 650);
  delay(300);
  noTone(buzzerPin);
  delay(700);
  lcd.setCursor(7, 0);
  lcd.print("2");
  tone(buzzerPin, 650);
  delay(300);
  noTone(buzzerPin);
  delay(700);
  lcd.setCursor(7, 0);
  lcd.print("1");
  tone(buzzerPin, 650);
  delay(300);
  noTone(buzzerPin);
  delay(700);
  tone(buzzerPin, 800);
  delay(300);
  noTone(buzzerPin);
  lcd.clear();
}

void victoryScreen(int gameWinner)
{
  lcd.clear();

  if (gameWinner == 1)
  {
    lcd.print("Player 1 Wins"); //menu
  }
  else if (gameWinner == 2)
  {
    lcd.print("Player 2 Wins");
  }

  tone(buzzerPin, 500);
  delay(120);
  tone(buzzerPin, 1000);
  delay(200);
  noTone(buzzerPin);
  delay(1800);
  ongoingGame = 0;
}
