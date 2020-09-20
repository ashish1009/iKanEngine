#pragma once

static const uint32_t s_MapWidth = 212 * 2;
/*
 G : Ground
 | : Castel Brick
 o : Castel Gate
 u : castel Gate Domb
 . : Castel Domb
 l : Castel Windlow Left
 r : Castel Window Right
 S : Steps
 - : Bridge
 ! : Pipe Base
 Y : Pipe Head
 X : Bricks
 B : Bonus
 */
static const char* s_MapTiles =
"                                                                                                                                                                                                                                                                                                                                                                                                                                        "
"                                                                                                                                                                                                                                                                                                                                                                                                                                        "
"                                                                                                                                                                                                                                                                                                                                                                                                                                        "
"                                                                                                                                                                                                                                                                                                                                                                                                                               .        "
"        XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX      XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX        XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX                  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX        XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX                         |        "
"                                                                                                                                                                                                                                                                                                                                                                                                                              ...       "
"                                                                                                                                                                                                                                                                                                                                                                                                                              |u|       "
"                                                                                                                                                                                                                                                                                                                                                                                                                              |o|       "
"                                                                                                                                                                                                                                                                                                                                                                                                                            .......     "
"XXXX             B                  XXXX                             B            XBX                                                            B                                    B                               XXXXXXXX     XXXXX           B           XXXXX XX  XX XXXXX                                                                           B                                                               |u|r|u|     "
"                                                                                                                                                                                                                                                                                                                                                                                                                            |o|||o|     "
"                                                                                                                                                                                                                                                                                                                                                                                                     S                    ...........   "
"                                                                                                                                                                                                                                                                                                                                                                                                    SS                    |||||||||||   "
"                                                                                                                                                                                     SS                                                                                                                                                                                                            SSS                    |l|u|r|u|l|   "
"               XBXBX              XBXXXXBX                           S           XXXXX               B                                           S                                  SSS                            XXXX      XXXX             B    B    B             XXXX                                                                                XBXBX                                   SSSS                    |||o|||o|||   "
"                                                  Y                 SS                                                                          SS                                 SSSS                                                                                                                                           S  S                                                           SSSSS                  ............... "
"                                                  !                SSS                                                   S  S                  SSS                                SSSSS                                                                                                                      Y                   SS  SS                                                         SSSSSS                  ||||||||||||||| "
"                            Y                     !               SSSS                               Y                  SS  SS                SSSS             Y                 SSSSSS                                                                                                                      !                  SSS  SSS                    Y                                  SSSSSSS                  |u||u||u||u||u| "
"                   S        !       S  S          !              SSSSS                               !                 SSS  SSS              SSSSS             !                SSSSSSS                                                                                                                      !                 SSSS  SSSS                   !                                 SSSSSSSS                  |o||o||o||o||0| "
"GGGGGG GG GGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG--------------------GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGG GGGGGGGGGGGGGGG  GGGGGGGGGGGGG-----------------------------------------------------------------------------------------------GGGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG"
"GGGGGG GG GGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG                    GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGG GGGGGGGGGGGGGGG  GGGGGGGGGGGGG                                                                                               GGGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG"
;

