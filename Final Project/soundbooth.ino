#include "Timer.h"
#include "LiquidCrystal.h"
#include "pitches.h"

// Sound Variables
int buzzer = 8;
int joystick = 10;  //button function
int *song; 
int *song_time;
// == Song 1 ==
int song1[] = {
   NOTE_E4, NOTE_C5, NOTE_B1, NOTE_F3, NOTE_C4,
   NOTE_A4, NOTE_A4, NOTE_GS5, NOTE_C5, NOTE_CS4,
   NOTE_AS4, NOTE_C5, NOTE_DS4, NOTE_CS5, NOTE_GS4,
   NOTE_C3, NOTE_E3, NOTE_DS5, NOTE_D4, NOTE_D3
 };

int song1_time[] = {
  2, 1, 2, 1, 1, 4, 8, 16, 8, 4, 4, 1, 8, 4, 2, 4, 4, 16, 4, 2
};

//== Song 2 ==

int song2[] = {
  NOTE_FS5, NOTE_D2, NOTE_DS5, NOTE_G2, NOTE_B3,
  NOTE_C2, NOTE_G5, NOTE_D6, NOTE_CS5, NOTE_AS4,
  NOTE_DS6, NOTE_D3, NOTE_CS4, NOTE_E5, NOTE_DS6,
  NOTE_E4, NOTE_B4, NOTE_F4, NOTE_E6, NOTE_DS4
};

int song2_time[] = {
  2, 2, 4, 8, 1, 8, 4, 4, 16, 8, 2, 4, 16, 8, 2, 4, 16, 4, 8, 1
};

// == Song 3 ==

int song3[] = {
  NOTE_A5, NOTE_D4, NOTE_D6, NOTE_DS3, NOTE_G4,
  NOTE_B2, NOTE_F2, NOTE_A3, NOTE_AS2, NOTE_B5,
  NOTE_C6, NOTE_C3, NOTE_GS3, NOTE_G2, NOTE_FS5,
  NOTE_AS4, NOTE_GS2, NOTE_CS3, NOTE_C3, NOTE_AS2
};

int song3_time[] = {
  1, 2, 16, 4, 16, 2, 16, 1, 1, 2, 1, 8, 2, 16, 8, 1, 16, 4, 1, 2
};


// LCD variables
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);




// Task Structure Definition
typedef struct task {
  int state;                  // Tasks current state
  unsigned long period;       // Task period
  unsigned long elapsedTime;  // Time elapsed since last task tick
  int (*TickFct)(int);        // Task tick function
} task;


const unsigned char tasksNum = 4;
task tasks[tasksNum];  // We have 4 tasks

// Task Periods

const unsigned long periodLCDOutput = 100;
const unsigned long periodJoystickInput = 100;
const unsigned long periodSoundOutput = 100;
const unsigned long periodController = 500;

// GCD
const unsigned long tasksPeriodGCD = 100;

// Task Function Definitions
int TickFct_LCDOutput(int state);
int TickFct_JoystickInput(int state);
int TickFct_SoundOutput(int state);
int TickFct_Controller(int state);

// Task Enumeration Definitions
enum LO_States { LO_init,
                 LO_MenuOptionA,
                 LO_MenuOptionB,
                 LO_MenuOptionC };
enum JI_States { JI_init,
                 JI_Sample };
enum SO_States { SO_init,
                 SO_SoundOn,
                 SO_SoundOff };
enum C_States { C_init,
                C_T1 };

void TimerISR() {  // TimerISR actually defined here
  unsigned char i;
  for (i = 0; i < tasksNum; ++i) {                  // Heart of the scheduler code
    if (tasks[i].elapsedTime >= tasks[i].period) {  // Ready
      tasks[i].state = tasks[i].TickFct(tasks[i].state);
      tasks[i].elapsedTime = 0;
    }
    tasks[i].elapsedTime += tasksPeriodGCD;
  }
}


void LCDWriteLines(String line1, String line2) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
}
int button() {
  return (!digitalRead(joystick));
}
// Task Function Definitions
struct MenuOption {
  String title;
  String option;
};

MenuOption menu[] = {
  {"Lab 7", ""},
  {"Song Selection", ""},
  {"Song ", ""},
  {"*Play", ""},
  {"*Pause", ""},
  {"Play Song ", ""},
};

// Got help from Abdel Sabbah (asabb006)
// we worked together to solve this logical issue
bool SongisPlaying = false;
bool SongisPaused = false;
int songSelect = 1;
bool SongisPicked = false;

int TickFct_LCDOutput(int state) {
  switch (state) {
    case LO_init:
      state = LO_MenuOptionA;
      LCDWriteLines(menu[0].title, menu[1].title);
      break;
    case LO_MenuOptionA:
      menu[2].option = String(songSelect);
      LCDWriteLines(menu[2].title + menu[2].option, menu[3].title);
      if (SongisPlaying) {
        state = LO_MenuOptionB;
      }
      break;
    case LO_MenuOptionB:
      menu[5].option = String(songSelect);
      LCDWriteLines(menu[5].title + menu[5].option, menu[4].title);
      if (!SongisPlaying && SongisPaused) {
        state = LO_MenuOptionC;
      }
      break;
    case LO_MenuOptionC:
      menu[5].option = String(songSelect);
      LCDWriteLines(menu[5].title + menu[5].option, menu[3].title);
      if (SongisPlaying && !SongisPaused) {
        state = LO_MenuOptionB;
      }
      break;
  }

  return state;
}



int *songs[3] = {song1, song2, song3};
int *song_times[3] = {song1_time, song2_time, song3_time};

int lastButton = 0;

void select_song(int songIndex) {
  if (songIndex < 0 || songIndex > 2) {
    songIndex = 0;
  }
  song = songs[songIndex];
  song_time = song_times[songIndex];
}

int TickFct_JoystickInput(int state) {
  int inputX;

  switch (state) {
    case JI_init:
      songSelect = 0;
      select_song(songSelect - 1);
      state = JI_Sample;
      break;
    case JI_Sample:
      inputX = analogRead(A0);

      if (!SongisPicked && !SongisPlaying) {
          if (inputX < 200) { // joystick to the right
              songSelect = (songSelect == 0) ? 2 : songSelect - 1;
             } 
              select_song(songSelect);
      }
      // Got help from Abdel Sabbah (asabb006)
      // we worked together to solve this logical issue
      int buttonState = button();
      if (buttonState) {
        if(!lastButton){
          SongisPicked = !SongisPicked;
          SongisPlaying = SongisPicked;
          SongisPaused = !SongisPicked;
        }
      }

      lastButton = buttonState;
      break;
  }

  return state;
}


int counter = 0;
int note = 0;
int TickFct_SoundOutput(int state) {
  switch (state) {  // State Transitions
    case SO_init:
      state = SO_SoundOn;
      break;
    case SO_SoundOn:
      if (!SongisPlaying) {
        state = SO_SoundOff;
      } else {
        if (counter >= song_time[note]) {
          state = SO_SoundOff;
          note++;
          counter = 0;
          note = note % 20;
          if (note == 0) {
            SongisPicked = false;
            SongisPlaying = false;
            SongisPaused = true;
          }
        }
      }

      break;
    case SO_SoundOff:
      if (SongisPlaying) {
        state = SO_SoundOn;
      }
      break;
  }

  switch (state) {  // State Actions
    case SO_SoundOn:
      if (SongisPlaying) {
        tone(buzzer, song[note], periodSoundOutput * song_time[note]);
        counter++;
      }
      break;
    case SO_SoundOff:
      noTone(buzzer);
      break;
  }
  return state;
}

// Task 4 (Unused)
int TickFct_Controller(int state) {
  switch (state) {  // State Transitions
    case 0:
      break;
  }

  switch (state) {  // State Actions
    case 0:
      break;
  }
  return state;
}



void InitializeTasks() {
  unsigned char i = 0;
  tasks[i].state = LO_init;
  tasks[i].period = periodLCDOutput;
  tasks[i].elapsedTime = tasks[i].period;
  tasks[i].TickFct = &TickFct_LCDOutput;
  ++i;
  tasks[i].state = JI_init;
  tasks[i].period = periodJoystickInput;
  tasks[i].elapsedTime = tasks[i].period;
  tasks[i].TickFct = &TickFct_JoystickInput;
  ++i;
  tasks[i].state = SO_init;
  tasks[i].period = periodSoundOutput;
  tasks[i].elapsedTime = tasks[i].period;
  tasks[i].TickFct = &TickFct_SoundOutput;
  ++i;
  tasks[i].state = C_init;
  tasks[i].period = periodController;
  tasks[i].elapsedTime = tasks[i].period;
  tasks[i].TickFct = &TickFct_Controller;
}

void setup() {
  // put your setup code here, to run once:
  InitializeTasks();

  TimerSet(tasksPeriodGCD);
  TimerOn();
  Serial.begin(9600);
  // Initialize Outputs
  lcd.begin(16, 2);
  // Initialize Inputs
  pinMode(joystick, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Task Scheduler with Timer.h
}
