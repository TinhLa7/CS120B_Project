#define unsch unsigned char
#define unssh unsigned short
#define unsl unsigned long
#define TASKS_NUM 3
#define total_ob 15
#define MENU_REFRESH_TIME 100
#define SONG_THRESHHOLD 250
#define COOLDOWN 5

#define REF_AVCC (1 <<REFS0)
#define REF_INT (1 << REFS0) | (1 << REFS1)
#define JOYSTICK_INIT 632
#define SHIFT 25

#define player_limits 16
#define player_above_limit 17
#define player_upper_limit 32
#define player_start 1

#define b1 ((~PINA) & 0x20)
#define b3 ((~PINA) & 0x10)

// Code readability
#define gameTime gTime++; mTime++;
#define score1 sTime++;  
#define enemyTime lTime
#define speedUp sTime % obMoveMult == 0 and ob_move_count > 1
#define HighScore eeprom_read_word( &ADDRESS)

#define and &&
#define or ||